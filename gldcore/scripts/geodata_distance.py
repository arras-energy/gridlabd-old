"""GridLAB-D Geodata Distance Package

The distance package computes the haversine distance between points on a sphere where
the points are given in latitude,longitude form, with position east and positive north
orientation.

The 'location' directive accepts a list of positions and computes the distances
between those positions.  If resolution is specified, then the result is generated
in increments of that distance.

The 'path' function accepts a list of CSV files containing latitude and
longtitude coordinates and computes the distances between consecutive positions.
If resolution is specified, then the result is generated in increments of that
distance.
"""

import os, sys
import requests
import math
import numpy as np
from PIL import Image
import pandas
import json

DATASET = "distance"

default_config = {
    "resolution" : None,
}

def get_distance(pos1, pos2):
    """Compute haversine distance between two locations

    ARGUMENTS

        pos1, pos2 (float tuple)   Specifies the two geographic endpoints as a
                                   (latitude,longtitude) tuple
    RETURNS

        float   The distance between the two points in meters.
    """
    lat1 = pos1[0]*math.pi/180
    lat2 = pos2[0]*math.pi/180
    lon1 = pos1[1]*math.pi/180
    lon2 = pos2[1]*math.pi/180
    a = math.sin((lat2-lat1)/2)**2+math.cos(lat1)*math.cos(lat2)*math.sin((lon2-lon1)/2)**2
    return 6371e3*(2*np.arctan2(np.sqrt(a),np.sqrt(1-a)))

def get_position(pos):
    """Compute the (latitude,longitude) tuple of the position given

    ARGUMENTS
        pos (str)   The position given as a comma-delimeted string
        pos (tuple) The position given as a (lat,lon) tuple

    RETURNS
        tuple   The position given as a (lat,lon) tuple
    """
    if type(pos) is str:
        return list(map(lambda x: float(x),pos.split(",")))
    return pos

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
    if len(args) < 2:
        error(f"{DATASET}.get_location({args}) missing one or more position arguments",geodata.E_SYNTAX)
    pos1 = get_position(args[0])
    lats = [pos1[0]]
    lons = [pos1[1]]
    dist = [0.0]
    resolution = geodata.get_resolution()
    for arg in args[1:]:
        pos2 = get_position(arg)
        d = get_distance(pos1,pos2)
        if type(resolution) is float:
            segs = d/resolution
            if segs > 1:
                lat = lats[-1]
                lon = lons[-1]
                dlat = (pos2[0]-pos1[0])/segs
                dlon = (pos2[1]-pos1[1])/segs
                for n in range(int(segs)):
                    lat += dlat
                    lon += dlon
                    lats.append(lat)
                    lons.append(lon)
                    dist.append(resolution)
            dist.append(d%resolution)
        else:
            dist.append(d)
        lats.append(pos2[0])
        lons.append(pos2[1])
        pos1 = pos2
    result = pandas.DataFrame(
            data={
                "latitude" : lats,
                "longitude" : lons,
                "distance" : dist}
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
            """Compute haversine distances along a path
            ARGUMENTS

                path ((lat,lon) tuple) Specifies the path along which the distance is
                                    computed

            RETURNS

                list   The cumulative distances along the path
            """
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
    class TestDataset(unittest.TestCase):
        def test_position(self):
            self.assertEqual(get_position("37,-122"),[37,-122])
        def test_distance(self):
            self.assertEqual(round(get_distance([37.415045141688054,-122.2056472090359],[37.388063971857704,-122.28844288884694]),0),7905.0)
        def test_location(self):
            context.set_resolution(None)
            self.assertEqual(round(get_location(["37,-122","37,-122.1"])["distance"][1],3),8880.421)
        def test_location_res(self):
            context.set_resolution(10.0)
            self.assertEqual(round(get_location(["37,-122","37,-122.1"])["distance"][1],3),10.0)
        def test_path(self):
            context.set_resolution(None)
            path = get_location(["37,-122","37,-122.1"])
            path.to_csv("/tmp/test.csv",index=False)
            self.assertEqual(round(get_path(["/tmp/test.csv"])["distance"][1],3),8880.421)
        def test_path_res(self):
            context.set_resolution(10.0)
            path = get_location(["37,-122","37,-122.1"])
            path.to_csv("/tmp/test.csv",index=False)
            self.assertEqual(round(get_path(["/tmp/test.csv"])["distance"][1],3),10.0)
    unittest.main()
