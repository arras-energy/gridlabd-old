import sys
import os
import pandas as pd
import numpy as np
import matplotlib as plt
from datetime import datetime
import xarray as xr


share = os.getenv("GLD_ETC")
if not share:
    share = "/usr/local/share/gridlabd"
if share not in sys.path:
    sys.path.append("/usr/local/share/gridlabd")
import json2dfloadlocations
import geodata_firerisk
import noaa_forecast as nf

###########################
## Load Data Sources ######
###########################
### Load and Convert JSON to matrix of load points
path= os.path.join(os.getcwd(),'example/ieee123.json')
path= os.path.join(os.getcwd(),'tools/example/ieee123.json')
# path= os.path.join(os.getcwd(),'example/Titanium_camden.json')
data = json2dfloadlocations.convert(input_file=path,output_file='')

# ### Load and Convert JSON for IEEE123_pole.json
# path2= os.path.join(os.getcwd(),'example/IEEE123_pole.json')
# data_poles = json2dfloadlocations.convert(input_file=path2,output_file='')



###########################
######## Set Options ######
###########################
resilience_Metric= 0 #1 for KWh lost, 0 for customer-hours

###########################
## Calculations/Edits #####
###########################

### Filter nodes without lat/long information (these are triplex_nodes and swing substations)
# data= data[data['class']!='triplex_node'] 

data.replace(r'^\s*$', np.nan, regex=True,inplace=True) # Replace Empty cells with Nan

### Import Fire Risk and set up Fire Risk Timeline Dataframe
latlongs= data[['lat','long']]
today= datetime.today().strftime('%Y%m%d')

#One days worth of fireRisk
fireRisk=[]
for day in range(1,2):
    fireRisk.append(geodata_firerisk.apply(data=latlongs,options=dict(day=day,date=today,type='fpi')))
data['fireRisk']= fireRisk[0]

#Loading all sevendays of fireRisk as NP array
firerisk7d=np.empty(shape=len(latlongs))
for day in range(1,8):
    fr=geodata_firerisk.apply(data=latlongs,options=dict(day=day,date=today,type='fpi'))
    firerisk7d= np.column_stack((firerisk7d, fr))
firerisk7d=firerisk7d[:,1:]
firerisk7dx= xr.DataArray(firerisk7d)
firerisk7dx['node']=data.node

# data['fireRisk']= np.random.randint(low=0, high=255, size=data.shape[0]) #TESTING-random fire risk for testing purposes

### Assign power lines to group_id Areas
loadpoints= data[data['class']=="load"]
lineAreaDict = pd.Series(loadpoints.group_id.values,index=loadpoints.node).to_dict()
lineGroupArea=data[['from','to']].applymap(lineAreaDict.get)
data['group_id'].fillna(lineGroupArea['from'].fillna(lineGroupArea['to']),inplace=True)

### Weight of fire risk for different components  and scale with length of Pole
fireRiskClassWeights = dict(
    {'overhead_line':1,
     'underground_line':0,
     'load':0,
     'substation':0,
     'triplex_meter':0,
     'triplex_node':0,
     'triplex_load':0,
     'node':0})
data['fireRiskWeight']= data['class'].map(fireRiskClassWeights)
data['fireRiskWeight']= data['fireRiskWeight'] * data.length.fillna(1)
data['weightedFireRisk']= data['fireRiskWeight'] * data['fireRisk']

firerisk7dx['fireRiskWeight'] =data['fireRiskWeight']
# firerisk7dx * data['fireRiskWeight'] 


### Sum the load within each meter group, then merge it to the corresponding load
groups = data.groupby(by='class')
df2 = groups.get_group('triplex_meter').groupby('group_id').sum()[['load']]
data=data.merge(df2,left_on='node',right_on=df2.index,how='left')
data=data[data['class']!="triplex_meter"].rename(columns={'load_y':'load'})
data=data[data['group_id']!="nodevolts"]

### Set example test loads to load points for testing purposes ####
data.loc[(data['class']=='load') & (data['group_id']=="area_A"),'load']=1
data.loc[(data['class']=='load') & (data['group_id']=="area_B"),'load']=2
data.loc[(data['class']=='load') & (data['group_id']=="area_C"),'load']=3
data.loc[(data['class']=='load') & (data['group_id']=="area_D"),'load']=4
data.loc[(data['class']=='load') & (data['group_id']=="area_E"),'load']=5

data['customerCount'] = (data['load']>0).astype(int)


#Load Weather Data
# latlongs.astype(int)
# nf.interpolate = 5 #interpolation time
latlongsRound=latlongs.astype(float).round(decimals=3)
latlongs=latlongs.astype(float)
latlongDict= dict(tuple(latlongsRound.groupby(['lat','long'])))

# forecast=[]
# for lat,long in latlongDict:
#     if np.isnan(lat):
#         pass
#     else:
#         forecast.append(nf.getforecast(lat,long)['wind_speed[m/s]'])
# forecast

forecast=[]
for lat,long in latlongs.itertuples(index=False):
    if np.isnan(lat):
        forecast.append(np.nan)
    else:
        forecast.append(nf.getforecast(lat,long)['wind_speed[m/s]'])
forecast
data['wind']=forecast


# import plotly.express as px
# fig = px.line(forecast[0])
# fig2 = px.line(forecast[40])
# fig.show()

###########################
###### Optimization ######
###########################

import pyomo.environ as pyo


areaData= data.groupby(by='group_id').sum().drop(columns={'fireRisk','fireRiskWeight','load_x','length'})
areas= areaData.index.values.tolist()
fireRiskNormalized=(areaData.weightedFireRisk)/max(areaData.weightedFireRisk)
loadNormalized=areaData.load/max(areaData.load)
customerCountNormalized = areaData.customerCount/max(areaData.customerCount)
resilienceObjective=""

if resilience_Metric == 0:
    #Use Customer-hours Resilience Metric
    resilienceObjective=customerCountNormalized
elif resilience_Metric ==1:
    #Use Load Lost Metric
    resilienceObjective= loadNormalized


dfResults = pd.DataFrame(columns= ['alpha'])
dfResults[areas]= ""

for a in range(0,101,1):

    model = pyo.ConcreteModel()
    ### Param
    alpha= a/100

    ### Var
    model.switch = pyo.Var(areas,within=pyo.NonNegativeIntegers, bounds=(-.05,1.05))

    ### Objective
    model.objective = pyo.Objective(sense=1, expr= sum( (model.switch[i] * fireRiskNormalized[i]*(1-alpha)) - (model.switch[i] * resilienceObjective[i] * alpha) for i in (model.switch.get_values())))

    ### Constraints

    ### Solver
    results= pyo.SolverFactory('cbc', executable='/usr/local/Cellar/cbc/2.10.7_1/bin/cbc').solve(model,tee=False)
    # results.write()
    # print("=================")
    # model.pprint()

    switches= [alpha]
    for i in areas:
        switches.append(model.switch[i].value)

    dfResults.loc[len(dfResults)]= switches


########################
# Plot and Prepare Results
########################
dfResults.reset_index(drop=True)
dfResults['LoadServed']= dfResults[areas].dot(areaData.load)
dfResults['fireRisk']= dfResults[areas].dot(areaData.weightedFireRisk)
dfResults['LoadServedNorm']= dfResults[areas].dot(loadNormalized)
dfResults['FireRiskNorm']= dfResults[areas].dot(fireRiskNormalized)


import seaborn as sns
import matplotlib.pyplot as plt
sns.regplot(x= dfResults['LoadServed'], y= dfResults['fireRisk'],fit_reg=False)
plt.show()

alpha_Results =0.65

#Add results to dataframe
switchResults= dfResults.loc[dfResults.alpha == alpha_Results][areas].to_dict('records')[0]
data['switchResults']= data['group_id'].map(switchResults)
data.lat =data.lat.astype(float) 
data.long =data.long.astype(float) 
lineData=data.loc[data['class'].str.contains(r'line')]


### Plot Results on map ########
import plotly.express as px

color_discrete_map = {1.0: 'rgb(255,0,0)', 0.0: 'rgb(0,255,0)'}
fig = px.scatter_mapbox(data, lat="lat", lon="long",color='switchResults', zoom=3, height=300,hover_name="node",color_discrete_map=color_discrete_map)
fig.update_layout(mapbox_style="stamen-terrain", mapbox_zoom=14, margin={"r":0,"t":0,"l":0,"b":0})

# fig2 = px.density_mapbox(data, lat='lat', lon='long', z='fireRisk', radius=10, zoom=14, mapbox_style="stamen-terrain")
# fig.add_trace(fig2.data[0])

fig.update_mapboxes(accesstoken='pk.eyJ1Ijoia3RlaHJhbmNoaSIsImEiOiJjbDJzNW5kdHMwaGJzM2pudDBsazZ5am80In0.hQfjJnhiiO1-YcJEEpN-1A') 
fig.show()
