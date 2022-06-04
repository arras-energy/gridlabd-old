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
import pickle

share = os.getenv("GLD_ETC")
if not share:
    share = "/usr/local/share/gridlabd"
if share not in sys.path:
    sys.path.append("/usr/local/share/gridlabd")

import meteostat_weather as mw
import nsrdb_weather as ns
import noaa_forecast as nf
import geodata_firerisk
import json2dfloadlocations



###########################
## Load Data Sources ######
###########################
'''
Functions to load datasource... could combine these into one function and call based on the .json or .csv
'''

def loadJsonData(path):
    # Load and Convert JSON to matrix of load points
    data = json2dfloadlocations.convert(input_file=path, output_file='')
    return data


def loadPGEData(path):
    # Loading Example PGE Data
    data = pd.read_csv(os.path.join(path))
    data.rename(columns={'Lat': 'lat', 'Lon': 'long',
                'FeedSplID': 'group_id','LengthWtdCust':'customerCount'}, inplace=True)
    data['class'] = "overhead_line"
    data['length'] = 1
    return data

###########################
## Calculations/Edits #####
###########################

def assignGroupID_json(data):
    # Assign group_id Areas to power lines based on area of "TO"
    # Replace Empty cells with Nan
    data.replace(r'^\s*$', np.nan, regex=True, inplace=True)
    loadpoints = data[data['class'] == "load"]
    lineAreaDict = pd.Series(loadpoints.group_id.values,
                             index=loadpoints.node).to_dict()
    lineGroupArea = data[['from', 'to']].applymap(lineAreaDict.get)
    data['group_id'].fillna(lineGroupArea['from'].fillna(
        lineGroupArea['to']), inplace=True)
    return data


def importFireRiskData(data, fireForecastStartDateDT):
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
    latlongs = data[['lat', 'long']]
    # fireForecastStartDateDT= datetime.today()
    fireForecastStartDate = fireForecastStartDateDT.strftime('%Y%m%d')

    # One days worth of fireRisk
    fireRisk = []
    for day in range(1, 2):
        fireRisk.append(geodata_firerisk.apply(data=latlongs, options=dict(
            day=day, date=fireForecastStartDate, type='fpi')))
    data['fireRisk'] = fireRisk[0]

    # Loading all sevendays of fireRisk as XARRAY
    firerisk7d = np.empty(shape=len(latlongs))
    for day in range(1, 8):
        fireForecastDate = (fireForecastStartDateDT +
                            timedelta(days=day-1)).strftime('%Y%m%d')
        fr = geodata_firerisk.apply(data=latlongs, options=dict(
            day=day, date=fireForecastDate, type='fpi'))
        # add segment on if type == fpi then cut off all vals >= 248 (set to zero)
        # if type == lfp then cut off vals over 2000 (set to zero)
        # if type == fsp then cut off above 200 (set to zero)
        firerisk7d = np.column_stack((firerisk7d, fr))
    firerisk7d = firerisk7d[:, 1:]
    firerisk7dx = xr.DataArray(firerisk7d, dims=['index', 'time_fireRisk'])
    fireForecastCoords = pd.date_range(
        fireForecastStartDate, periods=firerisk7d.shape[1], freq=pd.DateOffset(days=1))
    firerisk7dx = firerisk7dx.assign_coords(
        {'time_fireRisk': fireForecastCoords})
    firerisk7dx = firerisk7dx.resample(
        time_fireRisk="1h").interpolate("quadratic")

    # Weight of fire risk for different components  and scale with length of Pole
    fireRiskClassWeights = dict(
        {'overhead_line': 1,
         'underground_line': 0,
         'load': 0,
         'substation': 0,
         'triplex_meter': 0,
         'triplex_node': 0,
         'triplex_load': 0,
         'node': 0})
    data['fireRiskWeight'] = data['class'].map(fireRiskClassWeights)
    data['fireRiskWeight'] = data['fireRiskWeight'] * data.length.fillna(1)
    data['weightedFireRisk'] = data['fireRiskWeight'] * data['fireRisk']

    # Converting Data to XARRAY to include more dimensions for forecasting
    # adding fireRiskWeight to the dxArray
    dataX = data.to_xarray()
    dataX = dataX.set_coords(['lat', 'long'])
    dataX.long.data = dataX.long.data.astype(float)
    dataX.lat.data = dataX.lat.data.astype(float)
    dataX['fireRisk7D'] = firerisk7dx
    dataX['fireRisk7DW'] = dataX.fireRisk7D * \
        np.tile(data.fireRiskWeight, (firerisk7dx.data.shape[1], 1)).T

    end = time.time()
    print("Fire data import time: ", round(end-start,2)," sec")
    return data, dataX


def assignLoadID(dataX):
    # Reassigning the group_id of loads so that loads can be summed if this metric is to be incldued in optimization
    dict_loadgroups = dict(zip(dataX.node.where(
        dataX['class'] == 'load').data, dataX.group_id.where(dataX['class'] == 'load').data))
    # Create Placeholder new Group ID column
    new_groupID = copy(dataX.group_id.data)
    for key, value in dict_loadgroups.items():              # Lookup the values from the dictionary
        # Match the items and create new group ID
        new_groupID[dataX.group_id.data == key] = value
    dataX.group_id.data = new_groupID
    return dataX


def assignCustomerImpact(dataX):
    # check if it contains word meter then assign to customer. This is a proxy for customer count.
    dataX['customerCount'] = dataX['node'].str.contains('meter_').astype(int)
    return dataX


def importWeatherForecastData(data, dataX):
    print("=======Importing Weather Forecast Data============")
    start = time.time()

    nf.interpolate = 60  # interpolation time
    latlongs = data[['lat', 'long']]
    # latlongsRound=latlongs.astype(float).round(decimals=3)
    latlongs = latlongs.astype(float)
    time_Coords = nf.getforecast(
        35.38706, -119.000517)['wind_speed[m/s]'].index.tz_localize(tz=None).to_pydatetime()

    Weather_forecast = np.ones(shape=len(time_Coords))
    for lat, long in latlongs.itertuples(index=False):
        if np.isnan(lat):
            new = np.zeros_like(time_Coords)
            Weather_forecast = np.row_stack((Weather_forecast, new))
        else:
            wind = nf.getforecast(lat, long)['wind_speed[m/s]']
            windspeed = wind.values
            Weather_forecast = np.row_stack((Weather_forecast, windspeed))
    Weather_forecast = Weather_forecast[1:, :]
    WindX = xr.DataArray(Weather_forecast, dims=['index', 'time_wind'])
    dataX['wind'] = WindX
    dataX.wind.data = dataX.wind.data.astype(float)
    dataX['WindW'] =dataX.wind * data.fireRiskWeight.values.reshape(-1,1)


    dataX = dataX.assign_coords({'time_wind': time_Coords.tolist()})

    end = time.time()
    print("Weather Data import time: ", round(end-start,2),' sec')
    return dataX

def importHistoricalWeatherDataMeteostat(data, dataX, dateDT):
    print("========Importing Meteostat Data=========")
    start_stopwatch = time.time()

    latlongs = data[['lat', 'long']]
    latlongs = latlongs.astype(float)

    # dateDT=  datetime(year=2020,month=10,day=15,hour=0)
    lat, long = 38.25552587, -122.348129
    # geopoint = Point(lat,long)
    start = dateDT
    end = dateDT + timedelta(days=8)

    # station_info = mw.find_station(lat,long)
    stations = meteostat.Stations()
    stations = stations.nearby(lat, long)
    station_info = stations.fetch(1)
    station_info.reset_index(inplace=True)

    weather_data = mw.get_weather(station_info["id"][0], start, end)
    time_Coords = weather_data.index
    Weather_forecast = np.ones(shape=len(time_Coords))
    old_station = station_info
    countReuse = 0
    countNewLoopUp = 0
    timing = 0
    for lat, long in latlongs.itertuples(index=False):
        if np.isnan(lat):
            new = np.zeros_like(time_Coords)
            Weather_forecast = np.row_stack((Weather_forecast, new))
        else:
            start_loop = time.time()
            # new_station = mw.find_station(lat,long)
            stations = stations.nearby(lat, long)
            new_station = stations.fetch(1)
            new_station.reset_index(inplace=True)
            end_loop = time.time()
            if new_station.id[0] == old_station.id[0]:
                windspeed = weather_data['wind_speed[mph]'].values
                countReuse += 1
            else:
                weather_data = mw.get_weather(new_station["id"][0], start, end)
                windspeed = weather_data['wind_speed[mph]'].values
                old_station = new_station
                countNewLoopUp += 1
                print("looked up a new station", countNewLoopUp)
                print("old: ", old_station)
                print("new: ", new_station)


            Weather_forecast = np.row_stack((Weather_forecast, windspeed))
            timing += start_loop-end_loop

    print("Avg loop timing", round(timing/(countReuse+countNewLoopUp),2), " sec")

    Weather_forecast = Weather_forecast[1:, :]
    WindX = xr.DataArray(Weather_forecast, dims=['index', 'time_wind'])
    dataX['wind'] = WindX
    dataX.wind.data = dataX.wind.data.astype(float)

    dataX['WindW'] =dataX.wind * data.fireRiskWeight.values.reshape(-1,1)

    dataX = dataX.assign_coords({'time_wind': time_Coords.tolist()})

    end_stopwatch = time.time()
    print("Weather Data import time: ", round(end_stopwatch-start_stopwatch,2)," sec")
    print("reused lookUps %i , new look ups %i" % (countReuse, countNewLoopUp))
    return dataX

def ignitionProbModel(dataX):
    # load the model from disk
    filepath= os.path.join(os.getcwd(),'example/LogRegModel.sav')
    loaded_model = pickle.load(open(filepath, 'rb'))
    prediction= np.empty(shape=(dataX.wind.time_wind.shape[0]))
    for i in dataX.index:
        kmhr_wind=dataX.wind[i].values.reshape(-1,1)/0.6213712 #mph to km/hr
        result = loaded_model.predict_proba(pd.DataFrame(kmhr_wind,columns=['wind_speed']))
        prediction = np.row_stack((prediction, result[:,1]))
    prediction = prediction[1:, :]
    igPred= xr.DataArray(prediction, dims=['index', 'time_wind'])
    dataX['igProb']= igPred
    dataX['igProbW'] =dataX.igProb * data.fireRiskWeight.values.reshape(-1,1)
    return dataX



def aggregateAreaData(dataX):
    print("=====Aggregating for Optimization ======")
    # Combine the time axes for fireRisk forecast and the wind forecast
    fireRisk7DW_xr = dataX.fireRisk7DW.rename({'time_fireRisk': 'time'})
    windW_xr = dataX.WindW.rename({'time_wind': 'time'})
    igprob_xr = dataX.igProbW.rename({'time_wind': 'time'})

    merged_xds = xr.combine_by_coords([fireRisk7DW_xr, windW_xr, igprob_xr], coords=[
                                      'index', 'lat', 'long', 'time'], join="inner")
    dataX_merged = xr.merge(
        [dataX.drop_vars(names=['fireRisk7DW', 'WindW', 'igProbW']), merged_xds])

    # Area Data Aggregation prep for optimization. remove nodevolts groupIDs for GLM examples
    if np.any(np.isin(dataX_merged.group_id.data, "nodevolts")):
        areaDataX = dataX_merged.groupby(
            group='group_id').sum().drop_sel(group_id='nodevolts')
    else:
        areaDataX = dataX_merged.groupby(group='group_id').sum()

    areaDataX = areaDataX.assign_coords(
        {"time": np.arange(0, areaDataX.time.shape[0])})

    fireRiskNormalized = (areaDataX.fireRisk7DW)/areaDataX.fireRisk7DW.max()
    areaDataX['fireRiskNormalized'] = fireRiskNormalized

    windNormalized = (areaDataX.WindW)/areaDataX.WindW.max()
    areaDataX['windNormalized'] = windNormalized

    igProbNormalized = (areaDataX.igProb)/areaDataX.igProb.max()
    areaDataX['igProbNormalized'] = igProbNormalized

    customerCountNormalized = areaDataX.customerCount/areaDataX.customerCount.max()
    customerCountNormalized = np.tile(
        customerCountNormalized.data, (areaDataX.time.shape[0], 1))
    areaDataX['customerCountNormalized'] = xr.DataArray(
        customerCountNormalized.T, dims=['group_id', 'time'])

    # loadNormalized=areaDataX.load/areaDataX['load'].max()
    # loadNormalized= np.tile(loadNormalized.data, (areaDataX.time.shape[0],1))
    return areaDataX

###########################
###### Optimization ######
###########################


def optimizeShutoff(areaDataX, resilienceMetricOption, fireRiskAlpha,dependencies_df):
    print("=======Optimizing==========")
    start = time.time()

    # resilienceObjective=""
    if resilienceMetricOption == 0:
        # Use Customer-hours Resilience Metric
        resilienceMetric = areaDataX.customerCountNormalized
    elif resilienceMetricOption == 1:
        pass  # resilienceMetric= loadNormalized             #Use Load Lost Metric
        # Need to implement

    # dfResults = pd.DataFrame(columns= ['alpha'])
    # dfResults[areas]= ""
    # for a in range(0,101,1):

    model = pyo.ConcreteModel()
    # Sets
    areas = areaDataX.group_id.values
    model.areas = pyo.Set(initialize=areas)
    model.timestep = pyo.Set(initialize=areaDataX.time.data)

    # Vars
    # for some reason pyo.binary didnt work for me?
    model.switch = pyo.Var(model.timestep, model.areas, within=pyo.Binary, bounds=(-.05, 1.05))

    # Params
    alpha_Fire = fireRiskAlpha/100

    # Fire Risk Forecast Paramater
    def init_fireRisk(model, t, a):
        return areaDataX.fireRiskNormalized.sel(group_id=a, time=t).values
    model.fireRisk = pyo.Param(
        model.timestep, model.areas, initialize=init_fireRisk)

    # Wind Forecast Paramater
    def init_wind(model, t, a):
        return areaDataX.windNormalized.sel(group_id=a, time=t).values
    model.wind = pyo.Param(model.timestep, model.areas, initialize=init_wind)

    # # Ignition Potential Forecast Paramater
    # def init_igProb(model, t, a):
    #     return areaDataX.igProbW.sel(group_id=a, time=t).values
    # model.igProb = pyo.Param(model.timestep, model.areas, initialize=init_igProb)

    # Resilience Metric Paramater
    def init_resilienceMetric(model, t, a):
        return resilienceMetric.sel(group_id=a, time=t).values
    model.resilienceMetric = pyo.Param(model.timestep, model.areas, initialize=init_resilienceMetric)

    # # Constraints
    def init_ConnectionConstraint(model,t,a):
        iterable = dependencies_df[dependencies_df.group_id.str.contains(a)].iloc[:,1:]
        expression = 0
        for header, dependencies in iterable.iteritems():
            # if 'area' in str(dependencies.iloc[0]):
            #     expression = expression + model.switch[t,dependencies.iloc[0]]
            # else:
            #     expression = expression + int(dependencies.iloc[0])

            if dependencies.iloc[0] == 0  or dependencies.iloc[0]==1 :
                expression = expression + int(dependencies.iloc[0])
            else:
                expression = expression + model.switch[t,dependencies.iloc[0]]

        # print(a, " <= ", expression,'  ^^^^^    ')
        return (model.switch[t,a] <= expression)
    model.connection_constraint = pyo.Constraint(model.timestep, model.areas, rule= init_ConnectionConstraint)

    # def init_igProbConstraint(model,t,a):
    #     model.igProb[t,a] <= .1
    #     return
    # model.igProb_constraint = pyo.Constraint(model.timestep,model.areas,rule=init_igProbConstraint)
    # Objective
    def objective_rule(model):
        return sum(sum((model.switch[t, a] * model.fireRisk[t, a]*(1-alpha_Fire)) - (model.switch[t, a] * model.resilienceMetric[t, a] * alpha_Fire) for a in model.areas) for t in model.timestep)
    model.objective = pyo.Objective(sense=pyo.minimize, expr=objective_rule)

    # Solver
    results = pyo.SolverFactory(
        'cbc', executable='/usr/local/Cellar/cbc/2.10.7_1/bin/cbc').solve(model, tee=False)
    # results.write()
    end = time.time()
    print("Model Solved in: ", round(end-start,2), " sec")
    # model.pprint()
    return model, results



########################
### Run Program ########
########################

# # Routine for IEEE123 Data
# IEEEpath = os.path.join(os.getcwd(), 'example/ieee123.json')
# IEEEpolespath = os.path.join(os.getcwd(), 'example/IEEE123_pole.json')
# IEEEdepPath = os.path.join(os.getcwd(), 'example/ieeeDependencies.csv')
# IEEEdep = pd.read_csv(IEEEdepPath)


# dataStartDate = datetime.today() + timedelta(days=-1)
# # dataStartDate = datetime(year=2021,month=10,day=15,hour=0)

# # Load Data
# data = loadJsonData(IEEEpath)
# # dataPoles= loadJsonData(IEEEpolespath)

# # Modify Data
# data = assignGroupID_json(data)
# data, dataX = importFireRiskData(data, dataStartDate)
# dataX = assignLoadID(dataX)
# dataX = assignCustomerImpact(dataX)
# dataX = importWeatherForecastData(data, dataX)
# # dataX = importHistoricalWeatherDataMeteostat(data,dataX,dataStartDate)
# dataX= ignitionProbModel(dataX)

# areaDataX = aggregateAreaData(dataX)
# # Run optimization
# resilienceMetricOption = 0  # 1 for KWh lost, 0 for customer-hours
# fireRiskAlpha = 40  # higher number means you are more willing to accept fire risk
# model, results = optimizeShutoff(
#     areaDataX, resilienceMetricOption, fireRiskAlpha, IEEEdep)
# # END

#Routine for Historical Event Data
# pathPGE = '/Users/kamrantehranchi/Documents/GradSchool/Research/PSPS_Optimization_EREproject/Data/NapaFeeders/NapaFeederPoints-30m-NoSimplifyWGS84.csv'
pathNapa= os.path.join(os.getcwd(), 'example/NapaSubfeederPoints-30m.csv')
pathNapaCustCount= os.path.join(os.getcwd(), 'example/NapaSubfeederCustomers.csv')
napa=pd.read_csv(pathNapa)
napaCustCount= pd.read_csv(pathNapaCustCount)
napaCustCount= napaCustCount[['FeedSplID','LengthWtdCust']]
napa= pd.merge(left=napa,right=napaCustCount,on='FeedSplID',how='left')
pathNapa = os.path.join(os.getcwd(), 'example/NapaSubfeederPoints-30m_CustCount.csv')
napa.to_csv(pathNapa)


NapadepPath = os.path.join(os.getcwd(), 'example/NapaSubfeederInterconnections.csv')
Napadep = pd.read_csv(NapadepPath)
Napadep.fillna(0,inplace=True)

dataStartDate = datetime(year=2021,month=10,day=15,hour=0)
#Load Data
data = loadPGEData(pathNapa)
data = data.head(2000)

#Modify Data
data, dataX = importFireRiskData(data,dataStartDate)
dataX = importHistoricalWeatherDataMeteostat(data,dataX,dataStartDate)


dataX= ignitionProbModel(dataX)
areaDataX = aggregateAreaData(dataX)

#Run optimization
resilienceMetricOption= 0 #1 for KWh lost, 0 for customer-hours
fireRiskAlpha=40 #higher number means you are more willing to accept fire risk
model, results= optimizeShutoff(areaDataX, resilienceMetricOption,fireRiskAlpha,Napadep)
##END

######


##############
# Plot Results
##############

# Get Results
results = []
# outputVariables_list =  [model.switch[timestep, group_id].value for timestep in model.timestep for group_id in model.areas]
for timestep in model.timestep:
    results_t = []
    for group_id in model.areas:
        results_t.append(model.switch[timestep, group_id].value)
    results.append(results_t)
areaDataX['results'] = xr.DataArray(results, dims=['time', 'group_id'])

# plot_results = areaDataX.results.plot.line(x="time", col="group_id")
from cycler import cycler
plt.rc('axes', prop_cycle=(cycler('color', ['r', 'g', 'b', 'y', 'c', 'k']) +
                           cycler('linestyle', ['-', '--', ':', '-.', '-', '--'])))
plot_results = areaDataX.results.plot.line(x="time",alpha=0.7,lw=4)
plt.show()

areaDataX.fireRiskNormalized.plot.line(x="time",alpha=0.7,lw=4)
plt.show()

areaDataX.customerCountNormalized.plot.line(x='time',alpha=0.7,lw=4)
plt.show()

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
