import sys
import os
import pandas as pd
import numpy as np
import matplotlib as plt
import matplotlib.pyplot as plt
from datetime import datetime
from datetime import timedelta, tzinfo
import xarray as xr
from numpy import copy
import pyomo.environ as pyo
import time
import meteostat


share = os.getenv("GLD_ETC")
if not share:
    share = "/usr/local/share/gridlabd"
if share not in sys.path:
    sys.path.append("/usr/local/share/gridlabd")
import json2dfloadlocations
import geodata_firerisk
import noaa_forecast as nf
import nsrdb_weather as ns
import meteostat_weather as mw

###########################
## Load Data Sources ######
###########################
'''
Functions to load datasource... could combine these into one function and call based on the .json or .csv
'''
def loadJsonData(path):
    ### Load and Convert JSON to matrix of load points
    data = json2dfloadlocations.convert(input_file=path,output_file='')
    #pole Data
    # path2= os.path.join(os.getcwd(),'example/IEEE123_pole.json')
    # data_poles = json2dfloadlocations.convert(input_file=path2,output_file='')
    return data

def loadPGEData(path):
    ### Loading Example PGE Data
    data = pd.read_csv(os.path.join(path))
    data.rename(columns={'Lat':'lat','Lon':'long','Feeder_Nam':'group_id'},inplace=True)
    data['class'] = "overhead_line"
    data['length'] = 1
    data['customerCount'] = data.ResCust + data.ComCust + data.IndCust + data.AgrCust + data.OthCust
    return data

###########################
## Calculations/Edits #####
###########################

def assignGroupID_json(data):
    ### Assign group_id Areas to power lines based on area of "TO"
    data.replace(r'^\s*$', np.nan, regex=True,inplace=True) # Replace Empty cells with Nan
    loadpoints= data[data['class']=="load"]
    lineAreaDict = pd.Series(loadpoints.group_id.values,index=loadpoints.node).to_dict()
    lineGroupArea=data[['from','to']].applymap(lineAreaDict.get)
    data['group_id'].fillna(lineGroupArea['from'].fillna(lineGroupArea['to']),inplace=True)
    return data

def importFireRiskData(data,fireForecastStartDateDT):
    '''
    #Import Fire Risk and set up Fire Risk Timeline DataArray via USGS fire potential index
    Input: 'data' dataframe with lat, long, class fields
            class field indicates the type of object (overhead lines, underground lines, load, triplex_node, etc)
    Output: 
        'data' DataFrame with appended one day firerisk
        'dataX' DataArray with 7D forecast of firerisk appended with 'data'
    
    '''
    print("=======Importing FireRisk Data============")
    start = time.time()
    latlongs= data[['lat','long']]
    # fireForecastStartDateDT= datetime.today()
    fireForecastStartDate= fireForecastStartDateDT.strftime('%Y%m%d')

    #One days worth of fireRisk
    fireRisk=[]
    for day in range(1,2):
        fireRisk.append(geodata_firerisk.apply(data=latlongs,options=dict(day=day,date=fireForecastStartDate,type='fpi')))
    data['fireRisk']= fireRisk[0]

    ### Loading all sevendays of fireRisk as XARRAY
    firerisk7d=np.empty(shape=len(latlongs))
    for day in range(1,8):
        fireForecastDate = (fireForecastStartDateDT + timedelta(days=day-1)).strftime('%Y%m%d')
        fr=geodata_firerisk.apply(data=latlongs,options=dict(day=day,date=fireForecastDate,type='fpi'))
        #add segment on if type == fpi then cut off all vals >= 248 (set to zero)
        #if type == lfp then cut off vals over 2000 (set to zero)
        # if type == fsp then cut off above 200 (set to zero)
        firerisk7d= np.column_stack((firerisk7d, fr))
    firerisk7d=firerisk7d[:,1:]
    firerisk7dx= xr.DataArray(firerisk7d, dims=['index','time_fireRisk'])
    fireForecastCoords= pd.date_range( fireForecastStartDate, periods=firerisk7d.shape[1], freq=pd.DateOffset(days=1))
    firerisk7dx= firerisk7dx.assign_coords({'time_fireRisk':fireForecastCoords})
    firerisk7dx= firerisk7dx.resample(time_fireRisk="1h").interpolate("quadratic")

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

    ###### Converting Data to XARRAY to include more dimensions for forecasting
    ### #adding fireRiskWeight to the dxArray
    dataX =data.to_xarray()
    dataX=dataX.set_coords(['lat','long'])
    dataX.long.data= dataX.long.data.astype(float)
    dataX.lat.data= dataX.lat.data.astype(float)
    dataX['fireRisk7D']= firerisk7dx
    dataX['fireRisk7DW']= dataX.fireRisk7D * np.tile(data.fireRiskWeight, (firerisk7dx.data.shape[1],1)).T


    end= time.time()
    print("Fire data import time: ", end-start)
    return data,dataX

def assignLoadID(dataX):
    #Reassigning the group_id of loads so that loads can be summed if this metric is to be incldued in optimization
    dict_loadgroups = dict(zip(dataX.node.where(dataX['class']=='load').data, dataX.group_id.where(dataX['class']=='load').data))
    new_groupID = copy(dataX.group_id.data)                 # Create Placeholder new Group ID column
    for key, value in dict_loadgroups.items():              # Lookup the values from the dictionary
        new_groupID[dataX.group_id.data==key] = value       # Match the items and create new group ID
    dataX.group_id.data= new_groupID
    return dataX

def assignCustomerImpact(dataX):
    #check if it contains word meter then assign to customer. This is a proxy for customer count.
    dataX['customerCount'] = dataX['node'].str.contains('meter_').astype(int)
    return dataX

def importWeatherForecastData(data,dataX):
    print("=======Importing Weather Forecast Data============")
    start=time.time()

    nf.interpolate = 60 #interpolation time
    latlongs= data[['lat','long']]
    # latlongsRound=latlongs.astype(float).round(decimals=3)
    latlongs=latlongs.astype(float)
    # latlongDict= dict(tuple(latlongsRound.groupby(['lat','long'])))

    time_Coords= nf.getforecast(35.38706,-119.000517)['wind_speed[m/s]'].index.tz_localize(tz=None).to_pydatetime()
    Weather_forecast= np.ones(shape=len(time_Coords))
    # forecastWeather= xr.DataArray(latlongs)
    for lat,long in latlongs.itertuples(index=False):
        if np.isnan(lat):
            new=np.zeros_like(time_Coords)
            Weather_forecast= np.row_stack((Weather_forecast, new))
        else:
            wind=nf.getforecast(lat,long)['wind_speed[m/s]']
            windspeed= wind.values  
            Weather_forecast= np.row_stack((Weather_forecast, windspeed))
    Weather_forecast= Weather_forecast[1:,:]
    WindX= xr.DataArray(Weather_forecast,dims=['index','time_wind'])
    dataX['wind']= WindX
    dataX.wind.data= dataX.wind.data.astype(float)
    dataX= dataX.assign_coords({'time_wind':time_Coords.tolist()})

    end= time.time()
    print("Weather Data import time: ", end-start)
    return dataX

def importHistoricalWeatherDataNSRDB(data,dataX,dateDT):
    print("=======Importing Historical Weather Data============")
    start_stopwatch=time.time()

    latlongs= data[['lat','long']]
    latlongs=latlongs.astype(float)

    # dateDT=  datetime(year=2020,month=10,day=15,hour=0)
    lat,long= 38.25552587,-122.348129

    time_Coords= pd.concat(ns.getyear(dateDT.year,lat,long)['DataFrame'])['wind_speed[m/s]']
    mask = (time_Coords.index > dateDT) & (time_Coords.index <= (dateDT + timedelta(days=7)))
    time_Coords= time_Coords.loc[mask]

    Weather_forecast= np.ones(shape=len(time_Coords))
    for lat,long in latlongs.itertuples(index=False):
        if np.isnan(lat):
            new=np.zeros_like(time_Coords)
            Weather_forecast= np.row_stack((Weather_forecast, new))
        else:
            wind= pd.concat(ns.getyear(dateDT.year,lat,long)['DataFrame'])['wind_speed[m/s]']
            mask = (wind.index > dateDT) & (wind.index <= (dateDT + timedelta(days=7)))
            wind= wind.loc[mask]
            windspeed= wind.values  
            Weather_forecast= np.row_stack((Weather_forecast, windspeed))
    Weather_forecast= Weather_forecast[1:,:]
    WindX= xr.DataArray(Weather_forecast,dims=['index','time_wind'])
    dataX['wind']= WindX
    dataX.wind.data= dataX.wind.data.astype(float)
    dataX= dataX.assign_coords({'time_wind':time_Coords.tolist()})

    end_stopwatch= time.time()
    print("Weather Data import time: ", end_stopwatch-start_stopwatch)
    return dataX


def importHistoricalWeatherDataMeteostat(data,dataX,dateDT):
    print("=======Importing Meteostat Weather Data============")
    start_stopwatch=time.time()

    latlongs= data[['lat','long']]
    latlongs=latlongs.astype(float)

    dateDT=  datetime(year=2020,month=10,day=15,hour=0)
    lat,long= 38.25552587,-122.348129
    # geopoint = Point(lat,long)
    start= dateDT
    end= dateDT +timedelta(days=8)

    # station_info = mw.find_station(lat,long)
    stations = meteostat.Stations()
    stations = stations.nearby(lat,long)
    station_info = stations.fetch(1)
    station_info.reset_index(inplace=True)
    old_station = station_info


    data = mw.get_weather(station_info["id"],start,end)
    time_Coords = data.index
    Weather_forecast= np.ones(shape=len(time_Coords))
    old_station = station_info
    countReuse=0
    countNewLoopUp =0
    timing=0
    for lat,long in latlongs.itertuples(index=False):
        if np.isnan(lat):
            new=np.zeros_like(time_Coords)
            Weather_forecast= np.row_stack((Weather_forecast, new))
        else:
            start_loop = time.time()
            # new_station = mw.find_station(lat,long)

            stations = meteostat.Stations()
            stations = stations.nearby(lat,long)
            new_station = stations.fetch(1)

            end_loop = time.time()
            if new_station == old_station:
                windspeed =  data['wind_speed[mph]'].values
                countReuse+=1
                # print("looking up an old point")
            else:
                data = mw.get_weather(new_station["id"],start,end)
                windspeed = data['wind_speed[mph]'].values
                old_station = new_station
                countNewLoopUp+=1
                # print("looking up a new point")
            # wind = Hourly(geopoint, start, end)
            # wind = pd.DataFrame(wind.fetch())['wspd']
            # windspeed= wind.values  
            Weather_forecast= np.row_stack((Weather_forecast, windspeed))
            timing += start_loop-end_loop

    print("Avg loop timing",timing/(countReuse+countNewLoopUp))
    Weather_forecast= Weather_forecast[1:,:]
    WindX= xr.DataArray(Weather_forecast,dims=['index','time_wind'])
    dataX['wind']= WindX
    dataX.wind.data= dataX.wind.data.astype(float)
    dataX= dataX.assign_coords({'time_wind':time_Coords.tolist()})

    end_stopwatch= time.time()
    print("Weather Data import time: ", end_stopwatch-start_stopwatch)
    print("reused lookUps %i , new look ups %i" % (countReuse,countNewLoopUp))
    return dataX

def aggregateAreaData(dataX):
    ### Combine the time axes for fireRisk forecast and the wind forecast
    fireRisk7DW_xr = dataX.fireRisk7DW.rename({'time_fireRisk':'time'})
    wind_xr = dataX.wind.rename({'time_wind':'time'})
    merged_xds = xr.combine_by_coords([fireRisk7DW_xr, wind_xr], coords=['index','lat', 'long', 'time'], join="inner")
    dataX_merged= xr.merge([dataX.drop_vars(names=['fireRisk7DW','wind']), merged_xds])
    time_Coords= dataX_merged.time

    #Area Data Aggregation prep for optimization. remove nodevolts groupIDs for GLM examples
    if np.any(np.isin(dataX_merged.group_id.data,"nodevolts")):
        areaDataX = dataX_merged.groupby(group='group_id').sum().drop_sel(group_id = 'nodevolts')
    else:
        areaDataX = dataX_merged.groupby(group='group_id').sum()

    areaDataX = areaDataX.assign_coords({"time": np.arange(0,areaDataX.time.shape[0])})

    fireRiskNormalized=(areaDataX.fireRisk7DW)/areaDataX.fireRisk7DW.max()
    areaDataX['fireRiskNormalized']=fireRiskNormalized

    windNormalized=(areaDataX.wind)/areaDataX.wind.max()
    areaDataX['windNormalized']=windNormalized

    customerCountNormalized = areaDataX.customerCount/areaDataX.customerCount.max()
    customerCountNormalized= np.tile(customerCountNormalized.data, (areaDataX.time.shape[0],1))
    areaDataX['customerCountNormalized'] = xr.DataArray(customerCountNormalized.T, dims=['group_id','time'])

    # loadNormalized=areaDataX.load/areaDataX['load'].max()
    # loadNormalized= np.tile(loadNormalized.data, (areaDataX.time.shape[0],1))
    return areaDataX

###########################
###### Optimization ######
###########################
def optimizeShutoff(areaDataX,resilienceMetricOption,fireRiskAlpha):
    print("=======Optimizing==========")
    start= time.time()

    # resilienceObjective=""
    if resilienceMetricOption == 0:
        resilienceMetric=areaDataX.customerCountNormalized     #Use Customer-hours Resilience Metric
    elif resilienceMetricOption ==1:
        pass# resilienceMetric= loadNormalized             #Use Load Lost Metric
        #Need to implement

    # dfResults = pd.DataFrame(columns= ['alpha'])
    # dfResults[areas]= ""
    # for a in range(0,101,1):

    model = pyo.ConcreteModel()
    ### Sets
    areas= areaDataX.group_id.values
    model.areas = pyo.Set(initialize = areas)
    model.timestep = pyo.Set(initialize = areaDataX.time.data)

    ### Vars
    model.switch = pyo.Var(model.timestep,model.areas, within=pyo.Binary, bounds=(-.05,1.05)) #for some reason pyo.binary didnt work for me?
    # model.switch = pyo.Var(areas,within=pyo.NonNegativeIntegers, bounds=(-.05,1.05))

    ### Params
    alpha_Fire= fireRiskAlpha/100

    #Fire Risk Forecast Paramater
    def init_fireRisk(model, i,j):
        return areaDataX.fireRiskNormalized.sel(group_id=j,time=i).values
    model.fireRisk = pyo.Param(model.timestep,model.areas,initialize=init_fireRisk)

    #Wind Forecast Paramater
    def init_wind(model, i,j):
        return areaDataX.windNormalized.sel(group_id=j,time=i).values
    model.wind = pyo.Param(model.timestep,model.areas,initialize=init_wind)

    #Resilience Metric Paramater
    def init_resilienceMetric(model, i,j):
        return resilienceMetric.sel(group_id=j,time=i).values
    model.resilienceMetric = pyo.Param(model.timestep,model.areas,initialize=init_resilienceMetric)

    ### Constraints

    ### Objective
    def objective_rule(model):
        return sum(sum( (model.switch[t,a] * model.fireRisk[t,a]*(1-alpha_Fire)) - (model.switch[t,a] * model.resilienceMetric[t,a] * alpha_Fire) for a in model.areas) for t in model.timestep)
    model.objective = pyo.Objective(sense=pyo.minimize, expr= objective_rule )

    ### Solver
    results= pyo.SolverFactory('cbc', executable='/usr/local/Cellar/cbc/2.10.7_1/bin/cbc').solve(model,tee=False)
    # results.write()
    end= time.time()
    print("Model Solved in: ",end-start)
    # model.pprint()
    return model, results


########################
### Run Program ########
########################

#Routine for IEEE123 Data
IEEEpath= os.path.join(os.getcwd(),'example/ieee123.json')
# dataStartDate = datetime.today() +timedelta(days=-1)
dataStartDate = datetime(year=2021,month=10,day=15,hour=0)

# #Load Data
# data= loadJsonData(IEEEpath)
# #Modify Data
# data = assignGroupID_json(data) #do not need for historical GIS based analysis data
# data, dataX = importFireRiskData(data,dataStartDate)

# dataX = assignLoadID(dataX) #do not need for historical GIS based analysis data
# dataX = assignCustomerImpact(dataX) #do not need for historical GIS based analysis data
# # dataX = importWeatherForecastData(data,dataX) #do not need for historical GIS based analysis data
# dataX = importHistoricalWeatherDataMeteostat(data,dataX,dataStartDate)
# areaDataX = aggregateAreaData(dataX)
# #Run optimization
# resilienceMetricOption= 0 #1 for KWh lost, 0 for customer-hours
# fireRiskAlpha=40 #higher number means you are more willing to accept fire risk
# model, results= optimizeShutoff(areaDataX, resilienceMetricOption,fireRiskAlpha)
# ###END



#Routine for Historical Event Data
# pathPGE = '/Users/kamrantehranchi/Documents/GradSchool/Research/PSPS_Optimization_EREproject/Data/NapaFeeders/NapaFeederPoints-30m-NoSimplify.csv'
pathPGE = '/Users/kamrantehranchi/Documents/GradSchool/Research/PSPS_Optimization_EREproject/Data/NapaFeeders/NapaFeederPoints-30m-NoSimplifyWGS84.csv'
dataStartDate = datetime(year=2021,month=10,day=15,hour=0)
#Load Data
data = loadPGEData(pathPGE)

data = data.head(20)
#Modify Data
data, dataX = importFireRiskData(data,dataStartDate)
dataX = importHistoricalWeatherDataMeteostat(data,dataX,dataStartDate)

areaDataX = aggregateAreaData(dataX)

#Run optimization
resilienceMetricOption= 0 #1 for KWh lost, 0 for customer-hours
fireRiskAlpha=40 #higher number means you are more willing to accept fire risk
model, results= optimizeShutoff(areaDataX, resilienceMetricOption,fireRiskAlpha)
##END

######


##############
# Plot Results
##############

#Get Results
results= []
# outputVariables_list =  [model.switch[timestep, group_id].value for timestep in model.timestep for group_id in model.areas]
for timestep in model.timestep:
    results_t =[]
    for group_id in model.areas:
        results_t.append(model.switch[timestep,group_id].value)
    results.append(results_t)
areaDataX['results'] = xr.DataArray(results,dims=['time','group_id'])

# plot_results = areaDataX.results.plot.line(x="time", col="group_id")
plot_results = areaDataX.results.plot.line(x="time")
plt.show()

areaDataX.fireRiskNormalized.plot.line(x="time")
plt.show()

areaDataX.customerCountNormalized.plot.line(x='time')
plt.show()



#create plot that shows the intersected weighting of firerisk and customer count

# #some plots
# dataX_merged.plot.scatter(x="long", y="lat",hue='wind')
# dataX_merged.wind.plot()
# dataX_merged.fireRisk7DW.plot()
# dataX_merged.groupby(group='group_id').sum()

# ### Plot Results on map ########
# import plotly.express as px

# color_discrete_map = {1.0: 'rgb(255,0,0)', 0.0: 'rgb(0,255,0)'}
# fig = px.scatter_mapbox(data, lat="lat", lon="long",color='switchResults', zoom=3, height=300,hover_name="node",color_discrete_map=color_discrete_map)
# fig.update_layout(mapbox_style="stamen-terrain", mapbox_zoom=14, margin={"r":0,"t":0,"l":0,"b":0})

# # fig2 = px.density_mapbox(data, lat='lat', lon='long', z='fireRisk', radius=10, zoom=14, mapbox_style="stamen-terrain")
# # fig.add_trace(fig2.data[0])

# fig.update_mapboxes(accesstoken='pk.eyJ1Ijoia3RlaHJhbmNoaSIsImEiOiJjbDJzNW5kdHMwaGJzM2pudDBsazZ5am80In0.hQfjJnhiiO1-YcJEEpN-1A') 
# fig.show()