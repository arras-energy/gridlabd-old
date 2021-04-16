"""GridLAB-D Geodata Powerline Package

The powerline package computes geographic information about powerlines, such as
line sag and line sway.

The 'location' directive accepts a list of pole locations and computes the line
properties at those locations, which are nominally trivial. If resolution is
specified, then the line properties are generated in increments of that distance
between the specified pole locations.

The 'path' function accepts a list of CSV files containing latitude and
longtitude for the pole locations. If resolution is specified, then the result
is generated in increments of that distance.

To compute "linesag" and "linesway", the following parameters must be provided
either on the command line or as columns in the CSV file.

  "cabletype"   This property is required to obtain the cable's physical

  "windspeed"   Optional. If omitted, the windspeed is assumed to be 0.
  "winddir"     Optional. Required if windspeed is given.
  "temperature" Optional. If omitted, the temperature is assumed to be 10C.
  "powerflow"   Optional. If omitted, the powerflow is assumed to be 0MW.
  "elevation"   Optional. If omitted, the pole elevations are assumed to be 0.

The values of "linesag" are given with respect to the first pole elevation in a
pole pair vertically in meters. The values of "linesway" are given with respect
to the line from pole to pole, horizontally in meters.  If the "linesway" is
non-zero, the "linesag" will be computed according to the formula

  linesag(linesway)^2 = linesag(0)^2 - linesway^2

"""

import os, sys
import requests
import math
import numpy as np
from PIL import Image
import pandas
import json

DATASET = "powerline"

TODO=1.23

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
    """Compute the properties of a powerline between the locations specified

    The properties are computed for consecutive entries in the args list.  If
    the resolution is set, the coordinates for each step along the path at the
    specified resolution are provided.

    ARGUMENTS
        args (tuple list) List of (lat,lon) tuples.

    RETURNS
        DataFrame       Pandas dataframe containing the latitudes, longitudes,
                        and powerline properties.
    """
    if len(args) < 2:
        error(f"{DATASET}.get_location({args}) missing one or more position arguments",geodata.E_SYNTAX)
    pos1 = get_position(args[0])
    lats = [pos1[0]]
    lons = [pos1[1]]
    dist = [0.0]
    sag = [0.0]
    sway = [0.0]
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
                    sag.append(TODO)
                    sway.append(TODO)
            dist.append(d%resolution)
        else:
            lats.append(pos2[0])
            lons.append(pos2[1])
            sag.append(TODO)
            sway.append(TODO)
            dist.append(d)
        pos1 = pos2
    result = pandas.DataFrame(
            data={
                "latitude" : lats,
                "longitude" : lons,
                "linesag" : sag,
                "linesway" : sway,
                "distance" : dist,
                }
            )
    result["id"] = geodata.distance(list(zip(lats,lons)))
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
    pos = zip(lats,lons)
    locs = get_location(list(pos)).set_index(["latitude","longitude"])
    return path.set_index(["latitude","longitude"]).join(locs,how=geodata.PATHJOIN).reset_index()

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
    class context:
        """Fake geodata context to support unit testing"""
        resolution = None
        def get_resolution():
            return context.resolution
        def set_resolution(x):
            context.resolution = x
        output = lambda x: print(f"OUTPUT [unittest]: {x}")
        warning = lambda x: print(f"WARNING [unittest]: {x}")
        error = lambda x: print(f"ERROR [unittest]: {x}")
        verbose = lambda x: print(f"VERBOSE [unittest]: {x}")
    geodata = context
    class TestPowerline(unittest.TestCase):
        def test_position(self):
            self.assertEqual(get_position("37,-122"),[37,-122])
        def test_distance(self):
            self.assertEqual(round(get_distance([37.415045141688054,-122.2056472090359],[37.388063971857704,-122.28844288884694]),0),7905.0)
        def test_location(self):
            context.set_resolution(None)
            self.assertEqual(round(get_location(["37,-122","37,-122.1"])["linesag"][1],3),TODO)
            self.assertEqual(round(get_location(["37,-122","37,-122.1"])["linesway"][1],3),TODO)
        def test_location_res(self):
            context.set_resolution(10.0)
            self.assertEqual(round(get_location(["37,-122","37,-122.1"])["linesag"][1],3),TODO)
            self.assertEqual(round(get_location(["37,-122","37,-122.1"])["linesway"][1],3),TODO)
        def test_path(self):
            context.set_resolution(None)
            path = get_location(["37,-122","37,-122.1"])
            path.to_csv("/tmp/test.csv",index=False)
            self.assertEqual(round(get_path(["/tmp/test.csv"])["linesag"][1],3),TODO)
            self.assertEqual(round(get_path(["/tmp/test.csv"])["linesway"][1],3),TODO)
        def test_path_res(self):
            context.set_resolution(10.0)
            path = get_location(["37,-122","37,-122.1"])
            path.to_csv("/tmp/test.csv",index=False)
            self.assertEqual(round(get_path(["/tmp/test.csv"])["linesag"][1],3),TODO)
            self.assertEqual(round(get_path(["/tmp/test.csv"])["linesag"][1],3),TODO)
    unittest.main()
