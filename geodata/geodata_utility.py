"""GridLAB-D Geodata Utility Package

The utility package obtains information about utilities and their service
territories.  The information is collected from Homeland Infrastructure
Foundation-Level Data (HIFLD) website.  See
https://hifld-geoplatform.opendata.arcgis.com/datasets/electric-retail-service-territories.
to details.

The 'location' directive accepts a list of positions and obtains utility data at
those positions.

The 'path' function accepts a list of CSV files containing latitude and
longtitude coordinates and obtain utility information at those positions.

Note, the utility package does not support the resolution option.

The current fields generated by the utility dataset include the following:

    OBJECTID
    ID
    NAME
    ADDRESS
    CITY
    STATE
    ZIP
    TELEPHONE
    TYPE
    COUNTRY
    NAICS_CODE
    NAICS_DESC
    SOURCE
    SOURCEDATE
    VAL_METHOD
    VAL_DATE
    WEBSITE
    REGULATED
    CNTRL_AREA
    PLAN_AREA
    HOLDING_CO
    SUMMR_PEAK
    WINTR_PEAK
    SUMMER_CAP
    WINTER_CAP
    NET_GEN
    PURCHASED
    NET_EX
    RETAIL_MWH
    WSALE_MWH
    TOTAL_MWH
    TRANS_MWH
    CUSTOMERS
    YEAR
    SHAPE_Length
    SHAPE_Area
"""

version = 1

import os, sys
import math
import pandas
import geopandas
import fiona
import pickle
import numpy as np
from shapely.geometry import Point
from haversine import haversine_vector

GLD_ETC = os.getenv("GLD_ETC")

CACHEDIR = os.getenv("GLD_ETC")
if not CACHEDIR:
    CACHEDIR = "{GLD_ETC}/gridlabd"

REPOURL = os.getenv("REPOURL")
if not REPOURL:
    REPOURL = "http://geodata.gridlabd.us"

#
# Defaults
#
default_options = {
    "fields" : "NAME",
    "geometry" : False,
}

default_config = {
    "repourl" : f"{REPOURL}/utility",
    "kmlrepo" : f"{REPOURL}/utility/geometry.kml",
    "csvrepo" : f"{REPOURL}/utility/information.csv",
    "cachedir" : f"{CACHEDIR}/geodata/utility",
    "kmlfile" : f"{CACHEDIR}/geometry.kml",
    "csvfile" : f"{CACHEDIR}/information.csv",
}

#
# Internal data
#
kmldata = None
csvdata = None
CONFIG = default_config
OPTIONS = default_options

def read_kml():
    """Read KML data from cache, and download from repo if necessary"""
    global kmldata
    global CONFIG
    if type(kmldata) == type(None):
        if not os.path.exists(CONFIG["kmlfile"]):
            fiona.drvsupport.supported_drivers['KML'] = 'rw'
            kmldata = geopandas.read_file(CONFIG["kmlrepo"], driver="KML")
            os.makedirs(CONFIG["cachedir"],exist_ok=True)
            with open(CONFIG["kmlfile"], "wb") as fh:
                pickle.dump(kmldata,fh)
        else:
            with open(CONFIG["kmlfile"], "rb") as fh:
                kmldata = pickle.load(fh)
    return kmldata

def read_csv():
    """Read CSV data from cache, and download from repo if necessary"""
    global csvdata
    global CONFIG
    if type(csvdata) == type(None):
        if not os.path.exists(CONFIG["csvfile"]):
            csvdata = pandas.read_csv(CONFIG["csvrepo"],
                na_values=["-999999","NOT AVAILABLE"])
            os.makedirs(CONFIG["cachedir"],exist_ok=True)
            csvdata.to_csv(CONFIG["csvfile"])
        else:
            csvdata = pandas.read_csv(CONFIG["csvfile"])
    return csvdata

def get_information(id,field=None):
    """Get the utility information dataframe"""
    data = read_csv()
    if field:
        return data[field][id]
    else:
        return data.iloc[id]

def get_geometry(id):
    """Get the utility territory geometry"""
    geom = read_kml()
    result = geom["geometry"][id]
    # print(f"get_geometry(id={id.__repr__()}) --> {result}")
    # result.plot()
    return result

def get_utility(pos):
    """Returns the utility id(s) at the given position(s)

    ARGUMENT

        pos (tuple)   (latitude,longitude) of position

        pos (list)    list of (latitude,longitude) positions

    RETURN

        int           utility id if pos is a tuple

        list          list of utility ids if pos is a list of tuples
    """
    kml = read_kml()
    if type(pos[0]) in (list,tuple): # vector of positions
        return list(map(lambda x: list(kml[kml.contains(Point(x[1],x[0]))==True].index),pos))
    else: # singleton
        return list(kml[kml.contains(Point(pos[1],pos[0]))].index)

def get_position(pos):
    """Compute the (latitude,longitude) tuple of the position given

    ARGUMENT

        pos (str)   The position given as a comma-delimeted string

        pos (tuple) The position given as a (lat,lon) tuple

    RETURN

        tuple   The position given as a (lat,lon) tuple
    """
    if type(pos) is str:
        return list(map(lambda x: float(x),pos.split(",")))
    return pos

def apply(data, options=default_options, config=default_config, warning=print):
    """Obtain the utility information for the locations specified"""

    global CONFIG
    CONFIG = config

    global OPTIONS
    OPTIONS = options

    csv_keys = read_csv().keys()
    info = {"latitude":[],"longitude":[]}
    if not type(options["fields"]) is str:
        raise Exception(f"configured fields '{CONFIG['fields']}' is not valid")
    options["fields"] = options["fields"].split(",")
    for field in options["fields"]:
        if field not in csv_keys:
            raise Exception(f"field '{field}' not found in repo '{CONFIG['csvrepo']}'")
        info[field] = []
    if "latitude" not in data.columns or "longitude" not in data.columns:
        raise Exception("data must include 'latitude' and 'longitude' columns")
    if options["geometry"]:
        info["geometry"] = []
    for index, row in data.iterrows():
        pos = get_position((row["latitude"],row["longitude"]))
        id = get_utility(pos)
        values = get_information(id)
        info["latitude"].append(pos[0])
        info["longitude"].append(pos[1])
        for key in options["fields"]:
            info[key].extend(values[key].to_list())
        if options["geometry"]:
            info["geometry"].extend(get_geometry(id).to_list())
    result = pandas.DataFrame(info)
    return result

# perform validation tests
if __name__ == '__main__':

    import unittest

    default_config['cachedir'] = "/tmp/geodata/utility"
    os.makedirs(default_config['cachedir'],exist_ok=True)

    class TestDataset(unittest.TestCase):

        def test_utility_geometry(self):
            self.assertEqual(round(get_geometry(199).area,2),17.74)

        def test_utility_name(self):
            self.assertEqual(get_information(199,"NAME"),"PACIFIC GAS & ELECTRIC CO.")

        def test_utility_information(self):
            self.assertEqual(get_information(199)["NAME"],"PACIFIC GAS & ELECTRIC CO.")

    unittest.main()
