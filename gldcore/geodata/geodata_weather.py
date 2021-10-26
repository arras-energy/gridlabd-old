"""GridLAB-D Geodata Weather Package

The collects weather data from a region to allow object to refer to localized weather.

Note: This package can require a lot of time when accessing several years over a wide area.
"""

version = 1 # specify API version

import sys
import os
import json
import datetime
import math, numpy
import pandas
import haversine

share = os.getenv("GLD_ETC")
if not share:
    share = "/usr/local/share/gridlabd"
if share not in sys.path:
    sys.path.append("/usr/local/share/gridlabd")
import nsrdb_weather

#
# Defaults
#
default_options = dict(
    geohash_resolution = 6,
    key_index = "name",
    starttime = datetime.datetime(datetime.date.today().year-1,1,1,0,0,0).strftime("%Y"),
    stoptime = datetime.datetime(datetime.date.today().year-1,12,31,23,0,0).strftime("%Y"),
    )

default_config = dict(
    cachedir = f"{share}/weather/nsrdb"
    )

#
# Implementation of weather package
#
def apply(data, options=default_options, config=default_config, warning=print):
    """Gather weather data for various locations

    ARGUMENTS:

        data (pandas.DataFrame)

            The data frame must contain `latitude` and `longitude` fields between which
            distances will be computed.

        options (dict)

            There are no options

        config (dict)

            There are no configuration options

    RETURNS:

        pandas.DataFrame

            The specified weather value for the geocodes provided.

    """
    nsrdb_weather.geocode_precision = options["geohash_resolution"]
    startyear = int(options["starttime"])
    stopyear = int(options["stoptime"])
    if startyear == stopyear:
        years = f"{startyear}"
    else:
        years = f"{startyear}-{stopyear}"
    location = list(map(lambda row:nsrdb_weather.geohash(float(row['latitude']),float(row['longitude']),nsrdb_weather.geocode_precision),data.to_dict('records')))
    data["name"] = location
    data["filename"] = list(map(lambda code:f"nsrdb_{code}_{years}.csv",location))
    for code,file in zip(data["name"],data["filename"]):
        if not os.path.exists(file):
            lat,lon = nsrdb_weather.geocode(code)
            years = list(range(startyear,stopyear+1))
            weather = nsrdb_weather.getyears(years,lat,lon)
            weather['DataFrame'].to_csv(file)
    return data

#
# Perform validation tests
#
if __name__ == '__main__':

    import unittest

    class TestWeather(unittest.TestCase):

        def test_weather(self):
            test = pandas.DataFrame({
                "latitude" : [37.4205,37.5205],
                "longitude" : [-122.2046,-122.3046],
                })
            starttime = "2020"
            stoptime = "2020"
            result = apply(test,options=dict(geohash_resolution=6,starttime=starttime,stoptime=stoptime))
            pandas.set_option("display.max_rows",None)
            pandas.set_option("display.max_columns",None)
            pandas.set_option("display.max_colwidth",None)
            print(result)
            # self.assertEqual(result["wind"][1],12.0)

    unittest.main()
