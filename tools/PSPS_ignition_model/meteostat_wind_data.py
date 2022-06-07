'''
This tool is used to pull both absence and precsence wind data given the PGE ignition vegetation ignition files

I could have used the gridlab d meteostat package to accomplish this but I had already written much of this prior to that packages completion.
'''

from dataclasses import replace
import pandas as pd
from datetime import datetime, timedelta
from meteostat import Hourly, Point, Stations
import numpy as np

#Load Data
PGE = pd.read_csv('Data/PGE_fire_incident_reportsAll.csv')
PGE_vegFire = PGE[PGE.material_at_origin.str.contains('Vegetation') | PGE.contributing_factor.str.contains('High Winds') | PGE.contributing_factor.str.contains('Weather')]
PGE_vegFire['datetime'] = pd.to_datetime(PGE_vegFire[['year','month','day','hour','minute']])

absence=1 #option for loading uniform random distribution of wind data at each location prior to event 
#Import windspeeds
wind_speed_vector = []
wind_speed_max_vector = []
abs_wind_speeds_vector = np.array([0])

def load_windData(lat,long,start,end):
    stations = Stations()
    stations = stations.nearby(lat,long)
    station = stations.fetch(3)
    data = Hourly(station, start, end)
    data = data.fetch()
    return data

for i in PGE_vegFire.index:
    lat = PGE_vegFire['latitude'][i]
    long = PGE_vegFire['longitude'][i]

    if absence==1:
        start = PGE_vegFire['datetime'][i]+ timedelta(days=-20)
        end = PGE_vegFire['datetime'][i] + timedelta(days=-7)
    else:
        start = PGE_vegFire['datetime'][i]+ timedelta(hours=-5)
        end = PGE_vegFire['datetime'][i]

    data =load_windData(lat,long,start,end)

    if len(data['wspd'].values) == 0:
        wind_speed = 'no data'
    elif absence==1:
        data= data.loc[data.index.get_level_values(0)[0]] #filter out the first returned station data
        if len(data.wspd.dropna())>=10:
            abs_wind_speeds= data.wspd.dropna().sample(n=20,replace=True)
        else:
            abs_wind_speeds= np.nan
        wind_speed = np.nan
        wind_speed_max = np.nan

    else:
        data= data.loc[data.index.get_level_values(0)[0]] #filter out the first returned station data
        wind_speed = data.wspd.last('1h')[0]
        wind_speed_max = data.wspd.max()
        abs_wind_speeds= np.nan

    wind_speed_vector.append(wind_speed)
    wind_speed_max_vector.append(wind_speed_max)
    abs_wind_speeds_vector =np.append(abs_wind_speeds_vector,abs_wind_speeds)


PGE_vegFire['wind_speed'] = wind_speed_vector
PGE_vegFire['wind_speed_max'] = wind_speed_max_vector
# PGE_vegFire.to_csv('Data/PGE_VegFire_windData.csv')

pd.DataFrame(abs_wind_speeds_vector).to_csv('Data/abs_windData.csv')


