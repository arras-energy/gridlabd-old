import sys
import os
from typing import Any, Tuple

import pandas as pd
import numpy as np
import matplotlib as plt
import matplotlib.pyplot as plt
from datetime import datetime
from datetime import timedelta, tzinfo
import xarray as xr
import numpy as np
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
sys.path.append('../../geodata')
import geodata_firerisk
sys.path.append('../../converters')
import json2dfloadlocations



###########################
## Load Data Sources ######
###########################
'''
Functions to load datasource... could combine these into one function and call based on the .json or .csv
'''
def loadJsonData(path: str) -> pd.DataFrame:
    # Load and Convert JSON to matrix of load points
    data = json2dfloadlocations.convert(input_file=path, output_file='')
    return data

def loadPGEData(path: str) -> pd.DataFrame:
    # Loading Example PGE Data
    data = pd.read_csv(os.path.join(path))
    data.rename(columns={'Lat': 'lat', 'Lon': 'long',
                'FeedSplID': 'group_id','PopWtdCust':'customerCount'}, inplace=True)
    data['class'] = "overhead_line"
    data['length'] = 1
    return data

###########################
## Calculations/Edits #####
###########################


def assignGroupID_json(data: pd.DataFrame) -> pd.DataFrame:
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


def importFireRiskData(data: pd.DataFrame, fireForecastStartDateDT: datetime) -> pd.DataFrame:
    '''
    #Import Fire Risk and set up Fire Risk Timeline DataArray via USGS fire potential index
    Input: 'data' dataframe with lat, long, class fields
            class field indicates the type of object (overhead lines, underground lines, load, triplex_node, etc)
    Output: 
        'data' DataFrame with appended one day firerisk
    '''
    print("=======Importing FireRisk Data============")
    start = time.time()
    latlongs = data[['lat', 'long']]
    # fireForecastStartDateDT= datetime.today()
    fireForecastStartDate = fireForecastStartDateDT.strftime('%Y%m%d')

    # One days worth of fireRisk
    data['fireRisk'] = geodata_firerisk.apply(data=latlongs, options=dict(day=1, date=fireForecastStartDate, type='fpi'))

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
    data['fireRiskWeight'] = data['class'].map(fireRiskClassWeights).fillna(0)
    data['fireRiskWeight'] = data['fireRiskWeight'] * data['length'].fillna(1)
    data['weightedFireRisk'] = data['fireRiskWeight'] * data['fireRisk']

    end = time.time()
    print("Fire data import time: ", round(end-start,2)," sec")
    return data

def importFireRiskData7D(data: pd.DataFrame, fireForecastStartDateDT: datetime) -> tuple[pd.DataFrame, xr.Dataset]:
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
    # Loading all sevendays of fireRisk as XARRAY
    firerisk7d = np.empty(shape=len(latlongs))
    for day in range(1, 8):    
        fireForecastDate = (fireForecastStartDateDT + timedelta(days=day-1)).strftime('%Y%m%d')
        fr = geodata_firerisk.apply(data=latlongs, options=dict(day=day, date=fireForecastDate, type='fpi'))
        # add segment on if type == fpi then cut off all vals >= 248 (set to zero)
        fr = [0 if x>=248 else x for x in fr]
        # if type == lfp then cut off vals over 2000 (set to zero)
        # if type == fsp then cut off above 200 (set to zero)

        if day == 1:
            data['fireRisk'] = fr

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
    dataX: xr.Dataset = data.to_xarray()
    dataX = dataX.set_coords(['lat', 'long'])
    dataX.long.data = dataX.long.data.astype(float)
    dataX.lat.data = dataX.lat.data.astype(float)
    dataX['fireRisk7D'] = firerisk7dx
    dataX['fireRisk7DW'] = dataX.fireRisk7D * np.tile(data.fireRiskWeight, (firerisk7dx.data.shape[1], 1)).T

    end = time.time()
    print("Fire data import time: ", round(end-start,2)," sec")
    return data, dataX

def assignLoadID(data: pd.DataFrame) -> tuple[pd.DataFrame, xr.Dataset]:
    # # Reassigning the group_id of loads so that loads can be summed if this metric is to be incldued in optimization
    # dict_loadgroups = dict(zip(dataX['name'].where(
    #     dataX['class'] == 'load').data, dataX['group_id'].where(dataX['class'] == 'load').data))
    # # Create Placeholder new Group ID column
    # new_groupID = np.copy(dataX['group_id'].data)
    # for key, value in dict_loadgroups.items():              # Lookup the values from the dictionary
    #     # Match the items and create new group ID
    #     new_groupID[dataX['group_id'].data == key] = value
    # dataX['group_id'].data = new_groupID
    # # return dataX


    # Reassigning the group_id of loads so that loads can be summed if this metric is to be incldued in optimization
    dict_loadgroups = dict(zip(data['name'].where(
        data['class'] == 'load'), data['group_id'].where(data['class'] == 'load')))
    # Create Placeholder new Group ID column
    new_groupID = np.copy(data['group_id'])
    for key, value in dict_loadgroups.items():              # Lookup the values from the dictionary
        # Match the items and create new group ID
        new_groupID[data['group_id'] == key] = value
    data['group_id'] = new_groupID
    return data

def assignCustomerImpact(data: pd.DataFrame) -> pd.DataFrame:
    # check if it contains word meter then assign to customer. This is a proxy for customer count.
    data['customerCount'] = data['name'].str.contains('meter_').astype(int)
    return data

def assignCustomerImpact7D(dataX: xr.Dataset) -> xr.Dataset:
    # check if it contains word meter then assign to customer. This is a proxy for customer count.
    dataX['customerCount'] = dataX['name'].str.contains('meter_').astype(int)
    return dataX

def loadIgnitionProb(data: pd.DataFrame) -> pd.DataFrame:
    print("==== Calculating Ignition Probability Index ====")
     # load the model from disk
    filepath = os.path.join(os.getcwd(),'LogRegModel.sav')
    loaded_model = pickle.load(open(filepath, 'rb'))
    prediction = np.empty(shape=(data.wind.time_wind.shape[0]))
    for i in data.index:
        kmhr_wind= data.wind[i].values.reshape(-1,1) * 3.6 #m/s to km/hr
        result = loaded_model.predict_proba(pd.DataFrame(kmhr_wind,columns=['wind_speed']))
        prediction = np.row_stack((prediction, result[:,1]))
    prediction = prediction[1:, :]
    igPred= xr.DataArray(prediction, dims=['index', 'time_wind'])
    data['igProb'] = igPred
    data['igProbW'] = data.igProb * data.fireRiskWeight.values.reshape(-1,1)
    return data

def loadIgnitionProb7D(data: pd.DataFrame, dataX: xr.Dataset) ->  xr.Dataset:
    print("==== Calculating Ignition Probability Index ====")
     # load the model from disk
    filepath= os.path.join(os.getcwd(),'LogRegModel.sav')
    loaded_model = pickle.load(open(filepath, 'rb'))
    prediction= np.empty(shape=(dataX.wind.time_wind.shape[0]))
    for i in dataX.index:
        kmhr_wind= dataX.wind[i].values.reshape(-1,1) * 3.6 #m/s to km/hr
        result = loaded_model.predict_proba(pd.DataFrame(kmhr_wind,columns=['wind_speed']))
        prediction = np.row_stack((prediction, result[:,1]))
    prediction = prediction[1:, :]
    igPred= xr.DataArray(prediction, dims=['index', 'time_wind'])
    dataX['igProb']= igPred
    dataX['igProbW'] =dataX.igProb * data.fireRiskWeight.values.reshape(-1,1)
    return dataX

def importWeatherForecastData(data: pd.DataFrame, date_start_time: datetime):
    print("=======Importing Weather Forecast Data============")
    start = time.time()

    # nf.interpolate = 60  # interpolation time
    latlongs = data[['lat', 'long']]
    # latlongsRound=latlongs.astype(float).round(decimals=3)
    latlongs = latlongs.astype(float)

    time_Coords = nf.getforecast(
        latlongs.iloc[0,0],latlongs.iloc[0,1])['wind_speed[m/s]'].index.tz_localize(tz=None).to_pydatetime()

    Weather_forecast = np.ones(shape=len(time_Coords))
    for lat, long in latlongs.itertuples(index=False):
        if np.isnan(lat):
            new = np.zeros_like(time_Coords)
            Weather_forecast = np.row_stack((Weather_forecast, new))
        else:
            wind = nf.getforecast(lat, long)['wind_speed[m/s]']

            windspeed = wind[wind.index.day == date_start_time.day]
            windspeed = wind.values
            Weather_forecast = np.row_stack((Weather_forecast, windspeed))

    Weather_forecast = Weather_forecast[1:, :]



    
    WindX = xr.DataArray(Weather_forecast, dims=['index', 'time_wind'])
    dataX['wind'] = WindX
    dataX.wind.data = dataX.wind.data.astype(float)
    dataX['WindW'] =dataX.wind * data['fireRiskWeight'].values.reshape(-1,1)


    dataX = dataX.assign_coords({'time_wind': time_Coords.tolist()})

    end = time.time()
    print("Weather Data import time: ", round(end-start,2),' sec')
    
    dataX = loadIgnitionProb(data, dataX)
    return dataX

def importWeatherForecastData7D(data: pd.DataFrame, date_start_time: datetime):
    print("=======Importing Weather Forecast Data============")
    start = time.time()

    # nf.interpolate = 60  # interpolation time
    latlongs = data[['lat', 'long']]
    # latlongsRound=latlongs.astype(float).round(decimals=3)
    latlongs = latlongs.astype(float)

    time_Coords = nf.getforecast(
        latlongs.iloc[0,0],latlongs.iloc[0,1])['wind_speed[m/s]'].index.tz_localize(tz=None).to_pydatetime()

    Weather_forecast = np.ones(shape=len(time_Coords))
    for lat, long in latlongs.itertuples(index=False):
        if np.isnan(lat):
            new = np.zeros_like(time_Coords)
            Weather_forecast = np.row_stack((Weather_forecast, new))
        else:
            wind = nf.getforecast(lat, long)['wind_speed[m/s]']

            windspeed = wind[wind.index.day == date_start_time.day]
            windspeed = wind.values
            Weather_forecast = np.row_stack((Weather_forecast, windspeed))

    Weather_forecast = Weather_forecast[1:, :]



    
    WindX = xr.DataArray(Weather_forecast, dims=['index', 'time_wind'])
    dataX['wind'] = WindX
    dataX.wind.data = dataX.wind.data.astype(float)
    dataX['WindW'] =dataX.wind * data['fireRiskWeight'].values.reshape(-1,1)


    dataX = dataX.assign_coords({'time_wind': time_Coords.tolist()})

    end = time.time()
    print("Weather Data import time: ", round(end-start,2),' sec')
    
    dataX = loadIgnitionProb7D(data, dataX)
    return dataX

def importHistoricalWeatherDataMeteostat(data: pd.DataFrame, dataX: xr.Dataset, dateDT: datetime) -> xr.Dataset:
    print("========Importing Meteostat Data=========")
    start_stopwatch = time.time()
    latlongs = data[['lat', 'long']]
    latlongs = latlongs.astype(float)

    # dateDT=  datetime(year=2020,month=10,day=15,hour=0)
    lat, long = 38.25552587, -122.348129
    start = dateDT
    end = dateDT + timedelta(days=8)

    station_info = mw.find_station(lat,long)
    station_id = station_info['id']
    weather_data: pd.DataFrame = mw.get_weather(station_id, start, end)

    time_Coords = weather_data.index
    Weather_forecast = np.ones(shape=len(time_Coords))
    stationList= []

    for lat, long in latlongs.itertuples(index=False):
        if np.isnan(lat):
            stationList.append(-1)
        else:
            new_station = mw.find_station(lat,long)
            new_station_id = new_station['id']
            stationList.append(new_station_id)

    stationSet = set(stationList)    
    stationDict={}
    for key in stationSet:
        if key == -1:
            stationDict[key]=np.zeros_like(time_Coords)
        else:
            weather_data = mw.get_weather(new_station_id, start, end)
            windspeed = weather_data['wind_speed[mph]'].values
            stationDict[key] = windspeed
    #map unique forecasts to sequence
    Weather_forecast =np.asarray([*map(stationDict.get, stationList)])

    WindX = xr.DataArray(Weather_forecast, dims=['index', 'time_wind'])
    dataX['wind'] = WindX
    dataX.wind.data = dataX.wind.data.astype(float)
    dataX['WindW'] =dataX.wind * data.fireRiskWeight.values.reshape(-1,1)
    dataX = dataX.assign_coords({'time_wind': time_Coords.tolist()})
    end_stopwatch = time.time()
    print("Weather Data import time: ", round(end_stopwatch-start_stopwatch,2)," sec")
    
    print("==== Calculating Ignition Probability Index ====")
    # load the model from disk
    filepath= os.path.join(os.getcwd(),'LogRegModel.sav')
    loaded_model = pickle.load(open(filepath, 'rb'))
    predictionDict={}
    for key in stationDict:
        kmhr_wind=stationDict[key].reshape(-1,1)/0.6213712 #mph to km/hr
        result = loaded_model.predict_proba(pd.DataFrame(kmhr_wind,columns=['wind_speed']))
        predictionDict[key]=result[:,1]
    ignitionProb =np.asarray([*map(predictionDict.get, stationList)])
    igProb= xr.DataArray(ignitionProb, dims=['index', 'time_wind'])
    dataX['igProb'] = igProb
    dataX['igProbW'] = dataX.igProb * data.fireRiskWeight.values.reshape(-1,1)

    return dataX

def importHistoricalWeatherDataMeteostat7D(data: pd.DataFrame, dataX: xr.Dataset, dateDT: datetime) -> xr.Dataset:
    print("========Importing Meteostat Data=========")
    start_stopwatch = time.time()
    latlongs = data[['lat', 'long']]
    latlongs = latlongs.astype(float)

    # dateDT=  datetime(year=2020,month=10,day=15,hour=0)
    lat, long = 38.25552587, -122.348129
    start = dateDT
    end = dateDT + timedelta(days=8)

    station_info = mw.find_station(lat,long)
    station_id = station_info['id']
    weather_data: pd.DataFrame = mw.get_weather(station_id, start, end)

    time_Coords = weather_data.index
    Weather_forecast = np.ones(shape=len(time_Coords))
    stationList= []

    for lat, long in latlongs.itertuples(index=False):
        if np.isnan(lat):
            stationList.append(-1)
        else:
            new_station = mw.find_station(lat,long)
            new_station_id = new_station['id']
            stationList.append(new_station_id)

    stationSet = set(stationList)    
    stationDict={}
    for key in stationSet:
        if key == -1:
            stationDict[key]=np.zeros_like(time_Coords)
        else:
            weather_data = mw.get_weather(new_station_id, start, end)
            windspeed = weather_data['wind_speed[mph]'].values
            stationDict[key] = windspeed
    #map unique forecasts to sequence
    Weather_forecast =np.asarray([*map(stationDict.get, stationList)])

    WindX = xr.DataArray(Weather_forecast, dims=['index', 'time_wind'])
    dataX['wind'] = WindX
    dataX.wind.data = dataX.wind.data.astype(float)
    dataX['WindW'] =dataX.wind * data.fireRiskWeight.values.reshape(-1,1)
    dataX = dataX.assign_coords({'time_wind': time_Coords.tolist()})
    end_stopwatch = time.time()
    print("Weather Data import time: ", round(end_stopwatch-start_stopwatch,2)," sec")
    
    print("==== Calculating Ignition Probability Index ====")
    # load the model from disk
    filepath= os.path.join(os.getcwd(),'LogRegModel.sav')
    loaded_model = pickle.load(open(filepath, 'rb'))
    predictionDict={}
    for key in stationDict:
        kmhr_wind=stationDict[key].reshape(-1,1)/0.6213712 #mph to km/hr
        result = loaded_model.predict_proba(pd.DataFrame(kmhr_wind,columns=['wind_speed']))
        predictionDict[key]=result[:,1]
    ignitionProb =np.asarray([*map(predictionDict.get, stationList)])
    igProb= xr.DataArray(ignitionProb, dims=['index', 'time_wind'])
    dataX['igProb'] = igProb
    dataX['igProbW'] = dataX.igProb * data.fireRiskWeight.values.reshape(-1,1)

    return dataX

def aggregateAreaData(dataX: xr.Dataset) -> xr.Dataset:
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
    else: #for non-glm based files... taking the average here since all items equally weighted.
        areaDataX = dataX_merged.groupby(group='group_id').sum()

    areaDataX: xr.Dataset = areaDataX.assign_coords({"time": np.arange(0, areaDataX.time.shape[0])})


    maxFireRisk7DW = areaDataX.fireRisk7DW.max()

    fireRiskNormalized = areaDataX.fireRisk7DW/maxFireRisk7DW if maxFireRisk7DW != 0 else areaDataX.fireRisk7DW
    areaDataX['fireRiskNormalized'] = fireRiskNormalized

    maxWindW = areaDataX.WindW.max()


    windNormalized = areaDataX.WindW/maxWindW if maxWindW != 0 else areaDataX.WindW
    areaDataX['windNormalized'] = windNormalized


    maxIgProbW = areaDataX.igProbW.max()

    igProbNormalized = areaDataX.igProbW/ maxIgProbW if maxIgProbW != 0 else areaDataX.igProbW
    areaDataX['igProbNormalized'] = igProbNormalized


    maxCustomerCount = areaDataX.customerCount.max()

    customerCountNormalized = areaDataX.customerCount/maxCustomerCount if maxCustomerCount != 0 else areaDataX.customerCount
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
def optimizeShutoff(areaDataX: xr.Dataset, resilienceMetricOption: int, fireRiskAlpha: int, dependencies_df: pd.DataFrame, init_df: pd.DataFrame):
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
    areas = list(areaDataX.group_id.values)
    # areas.remove('switch')
    # areas = np.ndarray(areas)
    model.areas = pyo.Set(initialize=areas)
    model.timestep = pyo.Set(initialize=areaDataX.time.data)

    def init_switch(model, t, a):
        return init_df[ init_df['group_id'] == a].values[0][1]

    # Vars
    # for some reason pyo.binary didnt work for me?
    model.switch = pyo.Var(model.timestep, model.areas, within=pyo.Binary, bounds=(-.05, 1.05), initialize=init_switch)
    # model.switch.pprint()
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

    # Ignition Potential Forecast Paramater
    def init_igProb(model, t, a):
        return areaDataX.igProbNormalized.sel(group_id=a, time=t).values
    model.igProb = pyo.Param(model.timestep, model.areas, initialize=init_igProb)

    # Resilience Metric Paramater
    def init_resilienceMetric(model, t, a):
        return resilienceMetric.sel(group_id=a, time=t).values
    model.resilienceMetric = pyo.Param(model.timestep, model.areas, initialize=init_resilienceMetric)

    # # Constraints
    def init_ConnectionConstraint(model,t,a):
       
        iterable = dependencies_df[dependencies_df['group_id'].str.match(pat=f'^{a}$')].iloc[:,1:]
        expression = 0
        for header, dependencies in iterable.iteritems():
            if str(dependencies.iloc[0]) == '0'  or str(dependencies.iloc[0]) == '1' :
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
        # cost = 0
        # for t in model.timestep:
        #     for a in model.areas:
        #         value = (model.switch[t, a].value * model.fireRisk[t, a] * model.igProb[t,a] * (1-alpha_Fire)) - (model.switch[t, a].value * model.resilienceMetric[t, a] * alpha_Fire)
        #         cost += value


        cost = sum(sum((model.switch[t, a] * model.fireRisk[t, a] * model.igProb[t,a]*(1-alpha_Fire)) - (model.switch[t, a] * model.resilienceMetric[t, a] * alpha_Fire) for a in model.areas) for t in model.timestep)
        return cost
    model.objective = pyo.Objective(sense=pyo.minimize, expr=objective_rule)

    # Solver
    results = pyo.SolverFactory(
        'cbc', executable='/usr/bin/cbc').solve(model, tee=True)
    end = time.time()
    # model.solutions.store_to(results)
    # model.pprint()
    print("Model Solved in: ", round(end-start,2), " sec")
    return model, results

# ########################
# ### Run Program ########
# ########################

def optimize_psps(data: pd.DataFrame, dep_df: pd.DataFrame, init_df: pd.DataFrame, use_weather_forecast: bool, data_start_date: datetime, firerisk_alpha: int ) -> pyo.ConcreteModel:
    # Modify Data Inputs
    # data = assignGroupID_json(data)
    # data = importFireRiskData(data, data_start_date)
    data7D, dataX = importFireRiskData7D(data, data_start_date)

    # data = assignLoadID(data)
    # data = assignCustomerImpact(data)
    dataX = assignCustomerImpact7D(dataX)

    # Include weather for location
    if use_weather_forecast:
        # data = importWeatherForecastData(data, data_start_date)
        data = importWeatherForecastData7D(data, data_start_date)
    else:
        # dataX = importHistoricalWeatherDataMeteostat(data, dataX, data_start_date)
        dataX = importHistoricalWeatherDataMeteostat7D(data7D, dataX, data_start_date)

    areaDataX = aggregateAreaData(dataX)

    resilienceMetricOption = 0

    # Optimization
    model, _ = optimizeShutoff(areaDataX, resilienceMetricOption, firerisk_alpha, dep_df, init_df)


    

    # display_results(model, areaDataX, dataX)
    return model

def display_results(model: pyo.ConcreteModel, areaDataX: xr.Dataset, dataX: xr.Dataset) -> None:
    ##############
    # Plot Results
    ##############
    results = []
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
    plot_results = areaDataX.results.plot.line(x="time",alpha=0.4,lw=3,add_legend=False)
    plt.title("Napa County Switch Status")
    plt.xlim((50,140))
    plt.xlabel("Hours")
    plt.show()

    areaDataX.results.sum(dim='group_id').plot.line(x='time')
    plt.title("Sum of Switch Status")
    plt.xlabel("Hours")
    plt.show()

    areaDataX.fireRiskNormalized.plot.line(x="time",alpha=0.7,lw=4,add_legend=False)
    plt.title("Normalized Fire Risk")
    # plt.xlim((50,140))
    plt.xlabel("Hours")
    plt.show()

    areaDataX.igProbNormalized.plot.line(x="time",alpha=0.7,lw=4,add_legend=False)
    plt.title("Normalized Ignition Probability")
    plt.xlabel("Hours")
    plt.show()

    areaDataX.customerCountNormalized.plot.line(x='time',alpha=0.7,lw=4,add_legend=False)
    plt.title("Normalized Customer Hours")
    plt.xlabel("Hours")
    plt.show()

    ### Plot Results on map ########
    #Add results to dataframe
    dataX = dataX.reset_coords()
    df_groupid = dataX[['group_id','lat','long']].to_dataframe()
    df_results = areaDataX[['results','igProbNormalized','windNormalized','fireRiskNormalized','customerCountNormalized','WindW','fireRisk7DW','igProbW']].to_dataframe().reset_index()
    resultsTimeSeries = pd.merge(df_results,df_groupid,how='left',on='group_id')
    windTimeSeries = dataX[['group_id','lat','long','igProbW','WindW']].to_dataframe().reset_index().rename(columns={'time_wind':'time'})
    # resultsTimeSeries.to_csv('/Users/kamrantehranchi/Documents/GradSchool/Research/PSPS_Optimization_EREproject/Data/results.csv')

    import plotly.express as px
    def animate_map(time_col,df,col,scaleUL):
        fig = px.scatter_mapbox(df,
                lat="lat" ,
                lon="long",
                hover_name="group_id",
                color=col,
                animation_frame=time_col,
                mapbox_style='stamen-terrain',
                category_orders={
                time_col:list(np.sort(df[time_col].values))
                },                  
                zoom=9,width=700, height=700,
                range_color=[0,scaleUL])
        fig.update_layout(
            margin=dict(l=20, r=20, t=20, b=10),
            paper_bgcolor="LightSteelBlue",
            )
        fig.show()

    plotdf= resultsTimeSeries.iloc[::5, :]
    animate_map(time_col='time',df=plotdf,col='results',scaleUL= 1)
    animate_map(time_col='time',df=plotdf,col='igProbNormalized',scaleUL= 1)
    animate_map(time_col='time',df=plotdf,col='fireRiskNormalized',scaleUL= 1)

    # plotdf= windTimeSeries.iloc[::20, :]
    # animate_map(time_col='time',df=plotdf,col='WindW')





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
# # dataX= ignitionProbModel(dataX)

# areaDataX = aggregateAreaData(dataX)
# # Run optimization
# resilienceMetricOption = 0  # 1 for KWh lost, 0 for customer-hours
# fireRiskAlpha = 40  # higher number means you are more willing to accept fire risk
# model, results = optimizeShutoff(
#     areaDataX, resilienceMetricOption, fireRiskAlpha, IEEEdep)
# # END

# #Routine for Historical Event Data
# pathNapa= os.path.join(os.getcwd(), 'example/NapaSubfeederPoints-30m_CustCount.csv')
# NapadepPath = os.path.join(os.getcwd(), 'example/NapaSubfeederInterconnections.csv')
# Napadep = pd.read_csv(NapadepPath)
# Napadep.fillna(0,inplace=True)

# dataStartDate = datetime(year=2021,month=10,day=15,hour=0)
# #Load Data
# data = loadPGEData(pathNapa)
# # data = data.head(5000)

# #Modify Data
# data, dataX = importFireRiskData(data,dataStartDate)
# dataX = importHistoricalWeatherDataMeteostat(data,dataX,dataStartDate)
# areaDataX = aggregateAreaData(dataX)

# #Run optimization
# resilienceMetricOption = 0 #1 for KWh lost (not implemented), 0 for customer-hours
# fireRiskAlpha = 60 #higher number means you are more willing to accept fire risk (0 - 100) 
# model, results= optimizeShutoff(areaDataX, resilienceMetricOption,fireRiskAlpha,Napadep)
# #END

