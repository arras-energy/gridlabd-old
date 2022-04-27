from http.server import executable
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

### filter nodes without lat/long information (these are triplex_nodes and swing substations)
data= data[data['lat']!=''] 
data.replace(r'^\s*$', np.nan, regex=True,inplace=True)

### Look up fire risk values at all points  ##### uncomment for final implementation
# latlongs= data[['lat','long']]
# firerisk= geodata_firerisk.apply(data=latlongs)
# data['fireRisk']= firerisk

###testing purposes###
data['fireRisk']= np.random.randint(low=0, high=255, size=data.shape[0]) #random fire risk for testing purposes
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

### Set
import pyomo.environ as pyo
model = pyo.ConcreteModel()

### Param
areaData= data.groupby(by='group_id').sum().drop(columns={'fireRisk','fireRiskWeight','load_x','length'})
alpha= 0.5
areas= areaData.index.values.tolist()

model.demand = pyo.Param(areaData['load'])
model.fireRisk = pyo.Param(areaData['weightedFireRisk'])

### Var
model.switch = pyo.Var(areas,within=pyo.NonNegativeIntegers, bounds=(0,1))

### Objective
model.objective = pyo.Objective(expr= sum(model.switch[i] * areaData.load[i] * alpha + model.switch[i] * areaData.weightedFireRisk[i]*(1-alpha) for i in (model.switch.get_values())))
### Constraints

### Solver
results= pyo.SolverFactory('cbc', executable='/usr/local/Cellar/cbc/2.10.7_1/bin/cbc').solve(model)
results.write()

#TO-DO FOR LATER:
#transform the points to region with higher fire risk
#Split up line objects to pole objects then sum risk over those
#weight the loads with critical loads
