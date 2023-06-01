"""GridLAB-D Geodata Weather Package

The collects weather data from a region to allow object to refer to localized weather.

INPUT:

    latitude    the latitude of the location
    
    longitude   the longitude of the location

OUTPUT:

    name        the geohash code name of the location
    
    filename    the weather filename of the location


OPTIONS:

    geohash_resolution  the size of the geohash code name (default 6)
    
    key_index           the output record name (default "name")
    
    starttime           the default start time (Jan 1 of last year) 
    
    stoptime            the default stop time (Dec 31 of last year)

CONFIGURATION:

    cachedir    the path to the cache for data files (default 
                "$GLD_ETC/gridlabd/weather/nsrdb")

ENVIRONMENT:

    GLD_ETC     the path to the shared files (default "/usr/local/opt/gridlabd/<version>/share/gridlabd")

CAVEATS:

    This package can require a lot of time when accessing several years over a wide area.

SEE ALSO:

    * `gridlabd nsrbd_weather help` for details on NSRDB data source used by this package.
"""

version = 1 # specify API version

import sys
import os
import json
import datetime
import math, numpy
import pandas
import haversine

GLD_ETC = os.getenv("GLD_ETC")
if not GLD_ETC:
    GLD_ETC = "/usr/local/share/gridlabd"
if GLD_ETC not in sys.path:
    sys.path.append(GLD_ETC)
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
    cachedir = f"{GLD_ETC}/weather/nsrdb"
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
    data[options["key_index"]] = location
    data["filename"] = list(map(lambda code:f"nsrdb_{code}_{years}.csv",location))
    for code,file in zip(data[options["key_index"]],data["filename"]):
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
            result = apply(test,options=dict(geohash_resolution=6,key_index="name",starttime=starttime,stoptime=stoptime))
            pandas.set_option("display.max_rows",None)
            pandas.set_option("display.max_columns",None)
            pandas.set_option("display.max_colwidth",None)
            print(result)
            # self.assertEqual(result["wind"][1],12.0)

    if os.path.exists(f"{os.getenv('HOME')}/.nsrdb/credentials.json"):
        unittest.main()
