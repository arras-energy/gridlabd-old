"""GridLAB-D Geodata Address Package

The address package computers the location for an address or list of addresses.

The 'location' directive accepts a list of addresses and computes the location
at those positions.  If resolution is specified, then the result is generated
in increments of that distance between each address

The 'path' function accepts a list of CSV files containing addresses and
computes the locations at each address, or between each address if resolution
is specified.
"""

import os, sys
import requests
import math
import numpy as np
from PIL import Image
import pandas
import json
import geopandas as gpd
from shapely.geometry import Point
from geopandas.tools import geocode

DATASET = "address"

default_config = {
    "resolution" : None,
    "provider" : "nominatim",
    "user_agent" : "csv_user_ht",
}

def get_position(addr):
    """Compute the (latitude,longitude) tuple of the address given

    ARGUMENTS
        addr (str)   The address given as a comma delimited string

    RETURNS
        tuple   The position given as a (lat,lon) tuple
    """
    try:
        pos = geocode([addr],provider=default_config["provider"],user_agent=default_config["user_agent"])
        if len(pos)==0:
            raise Exception("address not found")
        pt = pos.values[0][0]
        return [pt.y,pt.x]
    except Exception as err:
        geodata.error(f"[{addr}]: {err}")
        return None

def get_location(args):
    """Compute the distances between the locations specified

    Distances are computed from the previous entry in the args list.  If the
    resolution is set, the coordinates for each step along the path at the
    specified resolution are provided.  The last step is always the remaining
    distance, i.e., the total distance modulo the resolution.

    ARGUMENTS
        args (tuple list) List of (lat,lon) tuples.

    RETURNS
        DataFrame       Pandas dataframe containing the latitudes, longitudes,
                        and distances.
    """
    resolution = geodata.get_resolution()
    pos0 = None
    lats = []
    lons = []
    addr = []
    for arg in args:
        arg = arg.replace("+"," ")
        pos = get_position(arg)
        if pos:
            if pos0 and type(resolution) is float:
                segs = d/resolution
                if segs > 1:
                    lat = lats[-1]
                    lon = lons[-1]
                    dlat = (pos[0]-pos0[0])/segs
                    dlon = (pos[1]-pos0[1])/segs
                    for n in range(int(segs)):
                        lat += dlat
                        lon += dlon
                        lats.append(lat)
                        lons.append(lon)
                        addr.append("")
            lats.append(pos[0])
            lons.append(pos[1])
            addr.append(arg)
            pos0 = pos
    result = pandas.DataFrame(
            data={
                "latitude" : lats,
                "longitude" : lons,
                "address" : addr}
            )
    result["id"] = geodata.distance(list(zip(lats,lons)))
    result.set_index("id")
    return result

def get_path(args):
    """Compute the distances along paths specified in CSV files

    Distances are computed from the previous row of valid CSV data found.  If
    the resolution is set, the coordinates for each step along the path at the
    specified resolution are provided.  The last step is always the remaining
    distance, i.e., the total distance modulo the resolution.

    ARGUMENTS
        args (str list)   List of CSV files containing paths. The CSV files must
                          include columns named "latitude" and "longitude".

    RETURNS
        DataFrame         Pandas dataframe containing the concatenated contents
                          of the CSV files with the "distance" column added.
    """
    paths = []
    for file in args:
        data = None
        last = None
        elevation = []
        distance = []
        lastpos = None
        paths.append(pandas.read_csv(file))
    path = pandas.concat(paths)
    lats = path["latitude"].to_list()
    lons = path["longitude"].to_list()
    pos = list(zip(lats,lons))
    path["id"] = geodata.distance(pos)
    locs = get_location(pos).set_index(["id","latitude","longitude"])
    return path.set_index(["id","latitude","longitude"]).join(locs,how=geodata.PATHJOIN,rsuffix="_loc").reset_index()

geodata = None # this will be set by the set_context() call from geodata
def set_context(context):
    """Sets the geodata context

    This function must be called to bind the message, configuration, and options
    specified by the geodata command.

    ARGUMENTS
        context (module)   Sets the geodata module context.

    RETURNS
        None
    """
    global geodata
    geodata = context

    global output
    output = geodata.output

    global warning
    warning = geodata.warning

    global error
    error = geodata.error

    global verbose
    verbose = geodata.verbose

    geodata.set_config(default_config)
    geodata.load_config(DATASET,"system")
    geodata.load_config(DATASET,"user")
    geodata.load_config(DATASET,"local")

# perform validation tests
if __name__ == '__main__':
    import unittest
    import numpy
    IDPRECISION = 3
    class context:
        """Fake geodata context to support unit testing"""
        resolution = None
        PATHJOIN = "outer"
        def get_resolution():
            return context.resolution
        def set_resolution(x):
            context.resolution = x
        output = lambda x: print(f"OUTPUT [unittest]: {x}")
        warning = lambda x: print(f"WARNING [unittest]: {x}")
        error = lambda x: print(f"ERROR [unittest]: {x}")
        verbose = lambda x: print(f"VERBOSE [unittest]: {x}")
        def distance(path):
            if len(path) == 0:
                return []
            dist = [0.0]
            pos1 = path[0]
            for pos2 in path[1:]:
                lat1 = pos1[0]*math.pi/180
                lat2 = pos2[0]*math.pi/180
                lon1 = pos1[1]*math.pi/180
                lon2 = pos2[1]*math.pi/180
                a = math.sin((lat2-lat1)/2)**2+math.cos(lat1)*math.cos(lat2)*math.sin((lon2-lon1)/2)**2
                dist.append(round(6371e3*(2*numpy.arctan2(numpy.sqrt(a),numpy.sqrt(1-a))),IDPRECISION))
            return dist
    geodata = context
    class TestAddress(unittest.TestCase):
        def test_address(self):
            self.assertEqual(get_position("2575 Sand Hill Rd., Menlo Park, CA 94025, USA"),[37.42045725,-122.20456827880435])
    unittest.main()