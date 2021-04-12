"""GridLAB-D Geodata Distance Package

The distance package computes the haversine distance between points on a sphere where
the points are given in latitude,longitude form, with position east and positive north
orientation.

The get_location() function accepts a list of positions and computes the distances 
between those positions.  If resolution is specified, then the result is generated
in increments of distance.
"""

import os, sys
import requests
import math
import numpy as np
from PIL import Image
import pandas
import json

sharedir = os.getenv("GLD_ETC")
if not sharedir:
    sharedir = "/usr/local/share/gridlabd"

config = {
    "resolution" : None,
}

configdir = {
    "system" : sharedir + "/geodata",
    "user" : os.getenv("HOME") + "/.gridlabd/geodata",
    "local" : os.getenv("PWD"),
}

elevation_data = {} # in memory holding area for image data

def load_config(file):
    global config
    try:
        fh = open(f"{configdir[file]}/distance.cfg","r")
        config = json.load(f"{configdir[file]}/distance.cfg")
    except:
        pass

def save_config(file):
    global config
    fh = open(f"{configdir[file]}/distance.cfg","w")
    if fh:
        json.dump(config, f"{configdir[file]}/distance.cfg", indent=4)

def get_config(name):
    if name in config.keys():
        return config[name]
    else:
        raise Exception(f"configuration parameter '{name}' does not exist")

def set_config(name,value):
    if name in config.keys():
        config[name] = value
    else:
        raise Exception(f"configuration parameter '{name}' does not exist")

def get_resolution():
    geodata.output(f"resolution is {geodata.RESOLUTION}")
    if geodata.RESOLUTION:
        return geodata.RESOLUTION
    else:
        return config["resolution"]

def get_distance(pos1, pos2):
    """Compute haversine distance between two locations"""
    lat1 = pos1[0]*math.pi/180
    lat2 = pos2[0]*math.pi/180
    lon1 = pos1[1]*math.pi/180
    lon2 = pos2[1]*math.pi/180
    a = math.sin((lat2-lat1)/2)**2+math.cos(lat1)*math.cos(lat2)*math.sin((lon2-lon1)/2)**2
    return 6371e3*(2*np.arctan2(np.sqrt(a),np.sqrt(1-a)))

def get_path(args):
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

def get_position(pos):
    if type(pos) is str:
        return list(map(lambda x: float(x),pos.split(",")))
    return pos

def get_location(args):
    if len(args) < 2:
        raise Exception(f"get_location({args}) missing one or more position arguments")
    pos1 = get_position(args[0])
    lats = [pos1[0]]
    lons = [pos1[1]]
    dist = [0.0]
    resolution = get_resolution()
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

geodata = None
output = print
warning = print
error = print
verbose = print

def set_context(context):
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

    load_config("system")
    load_config("user")
    load_config("local")

if __name__ == '__main__':
    
    import unittest
    class TestElevation(unittest.TestCase):
        def test_distance(self):
            self.assertEqual(round(get_distance([37.415045141688054,-122.2056472090359],[37.388063971857704,-122.28844288884694]),0),7905.0)
    unittest.main()
