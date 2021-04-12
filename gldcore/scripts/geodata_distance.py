"""GridLAB-D Geodata Distance Package

The distance package computes the haversine distance between points on a sphere where
the points are given in latitude,longitude form, with position east and positive north
orientation.

VALIDATION

    The unit test are performed automatically when this module is main.

"""

import os, sys
import requests
import math
import numpy as np
from PIL import Image
import pandas
import json

geodata = None # this will be set by the set_context() call from geodata

def get_distance(pos1, pos2):
    """Compute haversine distance between two locations

    ARGUMENTS

        pos1, pos2 (float tuple)   Specifies the two geographic endpoints as a
                                   (latitude,longtitude) tuple
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
        raise Exception(f"get_location({args}) missing one or more position arguments")
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
            lats.append(pos2[0])
            lons.append(pos2[1])
            dist.append(d%resolution)
        else:
            lats.append(pos2[0])
            lons.append(pos2[1])
            dist.append(d)
        pos1 = pos2
    return pandas.DataFrame(
            data={
                "latitude" : lats,
                "longitude" : lons,
                "distance" : dist}
            )
    data.index.names = ["id"]
    return data

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
        distance = []
        lastpos = None
        paths.append(pandas.read_csv(file))
        for lat,lon in zip(paths[-1]["latitude"],paths[-1]["longitude"]):
            if lastpos:
                dist = get_distance(lastpos,[lat,lon])
            else:
                dist = 0.0
            lastpos = [lat,lon]
            distance.append(dist)
        paths[-1]["distance"] = distance
    return pandas.concat(paths)

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
    output = context.output

    global warning
    warning = context.warning

    global error
    error = context.error

    global verbose
    verbose = context.verbose

    context.configdata = {
        "resolution" : None,
    }
    context.load_config("distance","system")
    context.load_config("distance","user")
    context.load_config("distance","local")

if __name__ == '__main__':

    output = print
    warning = print
    error = print
    verbose = print

    import unittest
    class TestGeodata(unittest.TestCase):
        def test_distance(self):
            self.assertEqual(round(get_distance([37.415045141688054,-122.2056472090359],[37.388063971857704,-122.28844288884694]),0),7905.0)
    unittest.main()
