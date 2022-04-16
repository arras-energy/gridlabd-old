
"""GridLAB-D Geodata Fire Danger Package

This package gathers data on the 7 day wildfire forecast risk from the USGS Fire Danger Map

INPUT:

    latitude    the latitude of the location
    
    longitude   the longitude of the location

    day of forecast 1-7
    date of forecast (cooresponds to day 1)
    type of forecast:  fpi - Fire Potential Index, lfp - Large Fire Probability, fsp - Fire Spread Probability

OUTPUT:

    fireDanger  Firedanger Value for all listed lat,longs


OPTIONS:

"""

# testdir='/Users/kamrantehranchi/Documents/GradSchool/Research/WildfireRiskData'
# #Find fire danger file
# dir='/usr/local/opt/gridlabd/4.3.1-220412-develop_add_fire_danger_tool/share/gridlabd/usgs/firedanger'
# fileName= 'fpi_20220415_1.tif'
# filePath= os.path.join(dir,fileName)

import sys
import os
from click import option
import pandas
import rasterio as rio
from rasterio.plot import show
from pyproj import Transformer
from datetime import datetime

today= datetime.today().strftime('%Y%m%d')

share = os.getenv("GLD_ETC")
if not share:
    share = "/usr/local/share/gridlabd"
if share not in sys.path:
    sys.path.append("/usr/local/share/gridlabd")
import fire_danger

#
# Defaults
#
default_options = dict(
    day= "1",
    date= datetime.today().strftime('%Y%m%d'),
    type ="fpi",
    )

default_config = dict(
    cachedir = f"{share}/usgs/firedanger"
    )

#
# Implementation of fireRisk package
#
def apply(data, options=default_options, config=default_config, warning=print):
    """Gather fire danger data for various locations
    ARGUMENTS:

        data (pandas.DataFrame)

            The data frame must contain `latitude` and `longitude` fields for which the firedanger 
                will be calculated

        options (dict)

            days ahead of forecast date
            date of forecast
            type of forecast

        config (dict)

            There are no configuration options

    RETURNS:

        pandas.DataFrame

            The specified fire danger value for the geocodes provided.

    """
    CACHEDIR = "/usr/local/opt/gridlabd/current/share/gridlabd/usgs/firedanger"
    USGSURL = "https://edcintl.cr.usgs.gov/downloads/sciweb1/shared/firedanger/download-tool/source_rasters/w{TYPE}-forecast-{DAYAHEAD}/emodis-w{TYPE}-forecast-{DAYAHEAD}_data_{DATE}_{DATE}.zip"
    
    filePath= fire_danger.get_data(options['day'],options['date'],options['type'],USGSURL,CACHEDIR)
    with rio.open(filePath) as usgsMap:
        transformer= Transformer.from_crs("epsg:4326",usgsMap.crs)
        coords= [transformer.transform(x, y) for x,y in data.itertuples(index=False)]
        coordsRC= []
        band= usgsMap.read(1)
        for x, y in coords:            
            row,col= usgsMap.index(x,y)
            coordsRC.append(band[row,col])

    # rio.plot.show(dataset, title = "firerisk")
    return coordsRC

#
# Perform validation tests
#
if __name__ == '__main__':

    import unittest

    class TestFireRisk(unittest.TestCase):

        def test_firerisk():
            testData = pandas.DataFrame({
                "lat":[34.8020,36.636],
                "long":[-104.1632, -121.9298],
                })
            testOptions=dict(day="5",date=today,type="fpi")
            result = apply(data=testData,options=testOptions)
            print(result) 
        test_firerisk()

    # if os.path.exists("~/.nsrdb/credentials.json"):
    #     unittest.main()
