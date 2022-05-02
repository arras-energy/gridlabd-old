import sys
import os
import pandas as pd
import numpy as np

share = os.getenv("GLD_ETC")
if not share:
    share = "/usr/local/share/gridlabd"
if share not in sys.path:
    sys.path.append("/usr/local/share/gridlabd")
import json2dfloadlocations
import geodata_firerisk

### Load and Convert JSON to matrix of load points
path= os.path.join(os.getcwd(),'example/ieee123.json')
data = json2dfloadlocations.convert(input_file=path,output_file='')

### Load and Convert JSON for IEEE123_pole.json
path2= os.path.join(os.getcwd(),'example/IEEE123_pole.json')
datapole = json2dfloadlocations.convert(input_file=path2,output_file='')

### filter nodes without lat/long information (these are triplex_nodes and swing substations)
data= data[data['lat']!=''] 
data.replace(r'^\s*$', np.nan, regex=True,inplace=True)

### Look up fire risk values at all points  ##### uncomment for final implementation
latlongs= data[['lat','long']]
firerisk= geodata_firerisk.apply(data=latlongs)
data['fireRisk']= firerisk

###testing purposes###
# data['fireRisk']= np.random.randint(low=0, high=255, size=data.shape[0]) #random fire risk for testing purposes
# data.to_csv(os.path.join(os.getcwd(),'testlatlong.csv')) 
# data= pd.read_csv(os.path.join(os.getcwd(),'testlatlong.csv')).drop(columns='Unnamed: 0')
#######################

### Assign power lines to group_id Areas
loadpoints= data[data['class']=="load"]
lineAreaDict = pd.Series(loadpoints.group_id.values,index=loadpoints.node).to_dict()
lineGroupArea=data[['from','to']].applymap(lineAreaDict.get)
data['group_id'].fillna(lineGroupArea['from'].fillna(lineGroupArea['to']),inplace=True)

### Weight of fire risk for different components
fireRiskClassWeights = dict(
    {'overhead_line':1,
     'underground_line':0,
     'load':0,
     'substation':0,
     'triplex_meter':0,
     'node':0})
data['fireRiskWeight']= data['class'].map(fireRiskClassWeights)
data['weightedFireRisk']= data['fireRiskWeight'] * data['fireRisk']

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
###########################

import pyomo.environ as pyo

areaData= data.groupby(by='group_id').sum().drop(columns={'fireRisk','fireRiskWeight','load_x','length'})
areas= areaData.index.values.tolist()
fireRiskNormalized=(areaData.weightedFireRisk)/max(areaData.weightedFireRisk)
loadNormalized=areaData.load/max(areaData.load)

dfResults = pd.DataFrame(columns= ['alpha'])
dfResults[areas]= ""

for a in range(0,101,1):

    model = pyo.ConcreteModel()
    ### Param
    alpha= a/100

    ### Var
    model.switch = pyo.Var(areas,within=pyo.NonNegativeIntegers, bounds=(-.05,1.5))

    ### Objective
    model.objective = pyo.Objective(sense=1, expr= sum( (model.switch[i] * fireRiskNormalized[i]*(1-alpha)) - (model.switch[i] * loadNormalized[i] * alpha) for i in (model.switch.get_values())))

    ### Constraints

    ### Solver
    results= pyo.SolverFactory('cbc', executable='/usr/local/Cellar/cbc/2.10.7_1/bin/cbc').solve(model)
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
dfResults['FireRisk']= dfResults[areas].dot(areaData.weightedFireRisk)
dfResults['LoadServedNorm']= dfResults[areas].dot(loadNormalized)
dfResults['FireRiskNorm']= dfResults[areas].dot(fireRiskNormalized)


import seaborn as sns
import matplotlib.pyplot as plt
sns.regplot(x= dfResults['LoadServed'], y= dfResults['FireRisk'],fit_reg=False)
plt.show()


#TO-DO FOR LATER:
#transform the points to region with higher fire risk
#Split up line objects to pole objects then sum risk over those
#weight the loads with critical loads

# https://github.com/Pyomo/pyomo/issues/2241