"""GridLAB-D Geodata Distance Package

The distance package computes the haversine distance between points on a sphere where
the points are given in latitude,longitude form, with position east and positive north
orientation.

The get_location() function accepts a list of positions and computes the distances 
between those positions.  If the resolution 
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

geodata = None # set by geodata module when it loads this module, provides access to settings
elevation_data = {} # in memory holding area for image data

def load_config(file):
    global config
    fh = open(f"{configdir}/distance.cfg","r")
    if fh:
        config = json.load(f"{configdir}/distance.cfg")

def save_config(file):
    global config
    fh = open(f"{configdir}/distance.cfg","w")
    if fh:
        config = json.dump(f"{configdir}/distance.cfg", indent=4)

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
            if type(r) is float:
                lat0 = lastpos[0]
                lon0 = lastpos[1]
                for lat0
            else:
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
    for arg in args[1:]:
        pos2 = get_position(arg)
        lats.append(pos2[0])
        lons.append(pos2[1])
        dist.append(get_distance(pos1,pos2))
        pos1 = pos2
    return pandas.DataFrame(
            data={
                "latitude" : lats,
                "longitude" : lons,
                "distance" : dist}
            )
    data.index.names = ["id"]
    return data

if geodata: # if loaded by geodata module, use context info
    output = geodata.output
    warning = geodata.warning
    error = geodata.error
    load_config("system")
    load_config("user")
    load_config("local")
    if geodata.RESOLUTION:
        resolution = geodata.RESOLUTION
    else:
        resolution = config["resolution"]
    if not os.path.exists(config["cachedir"]):
        os.makedirs(config["cachedir"])

elif __name__ == '__main__':
    import unittest
    output = print
    debug = print
    warning = print
    error = print
    class TestElevation(unittest.TestCase):
        def test_distance(self):
            self.assertEqual(round(get_distance([37.415045141688054,-122.2056472090359],[37.388063971857704,-122.28844288884694]),0),7905.0)
    unittest.main()
