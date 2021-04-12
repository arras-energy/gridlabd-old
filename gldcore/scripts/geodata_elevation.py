"""GridLAB-D Geodata Elevation Package

This geodata package obtains 'elevation' data for the input data based on the 
latitude and longitude columns found.
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
    "repourl" : "http://elevation.gridlabd.us/10m",
    "resolution" : 10.0,
    "cachedir" : f"{sharedir}/geodata/elevation/10m",
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
    fh = open(f"{configdir}/elevation.cfg","r")
    if fh:
        config = json.load(f"{configdir}/elevation.cfg")

def save_config(file):
    global config
    fh = open(f"{configdir}/elevation.cfg","w")
    if fh:
        config = json.dump(f"{configdir}/elevation.cfg", indent=4)

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

def get_elevation(pos):
    name = get_name(pos)
    if name not in elevation_data.keys():
        get_data(pos)
    raise Exception("not implemented yet")

def get_path(args):
    paths = []
    for file in args:
        data = None
        last = None
        elevation = []
        distance = []
        lastpos = None
        paths.append(pandas.read_csv(file))
        for lat,lon in zip(paths[-1]["latitude"],paths[-1]["longitude"]):
            if last != [int(lat),int(lon)]:
                name,data = get_data([lat,lon])
                print(f"loading image {name}")
                last = [int(lat),int(lon)]
            col = 3600 - int(math.modf(abs(lon))[0]*3600)
            row = 3600 - int(math.modf(abs(lat))[0]*3600)
            elev = data[row][col]
            if lastpos:
                dist = get_distance(lastpos,[lat,lon])
            else:
                dist = 0.0
            lastpos = [lat,lon]
            elevation.append(elev)
            distance.append(dist)
            # print(f"({lat},{lon}) --> {name}[{row}][{col}], elev={elev}, dist={dist}")
        paths[-1]["elevation"] = elevation
        # path["distance"] = distance
        # print(path["distance"].sum())
    return pandas.concat(paths)

def get_location(args):
    for pos in args:
        pos = list(map(lambda x: float(x),pos.split(",")))
        name,data = get_data(pos)
        lat = pos[0]
        lon = pos[1]
        x = int(math.modf(lon)[0]*3600)
        y = int(math.modf(lat)[0]*3600)
        data = pandas.DataFrame(
            data={
                "latitude" : [lat],
                "longitude" : [lon],
                "elevation" : [data[y][x]]},
            )
        data.index.names = ["id"]
    return data

def get_name(pos):
    lat = pos[0]
    lon = pos[1]
    if lat < 0:
        lat = f"{-math.ceil(lat)}S"
    elif lat > 0:
        lat = f"{math.floor(lat)}N"
    else:
        lat = "0"
    if lon < 0:
        lon = f"{-math.ceil(lon)}W"
    elif lon > 0:
        lon = f"{math.floor(lon)}E"
    else:
        lon = "0"
    return f"{lat}_{lon}"

def get_data(pos):
    tifname = get_name(pos)
    if not tifname in elevation_data.keys():
        srcname = f"{config['repourl']}/{tifname}.tif"
        dstname = f"{config['cachedir']}/{tifname}.tif"
        if not os.path.exists(dstname):
            response = requests.get(srcname,stream=True)
            if response.status_code != 200:
                return tifname, f"'GET {srcname}'' --> error code {response.status_code}"
            with open(dstname,"wb") as fh:
                for chunk in response.iter_content(chunk_size=1024*1024):
                    if chunk:
                        fh.write(chunk)
        elevation_data[tifname] = np.array(Image.open(dstname))
    return tifname, elevation_data[tifname]

if geodata: # if loaded by geodata module, use context info
    output = geodata.output
    warning = geodata.warning
    error = geodata.error
    load_config("system")
    load_config("user")
    load_config("local")
    if not os.path.exists(config["cachedir"]):
        os.makedirs(config["cachedir"])

elif __name__ == '__main__':
    import unittest
    output = print
    debug = print
    warning = print
    error = print
    class TestElevation(unittest.TestCase):
        def test_location(self):
            self.assertEqual(get_location([[37.415055930821744, -122.20564599810152]])["elevation"][0],100.0) # TODO get the actual elevation at this point
        def test_distance(self):
            self.assertEqual(round(get_distance([37.415045141688054,-122.2056472090359],[37.388063971857704,-122.28844288884694]),0),7905.0)
        def test_path_min(self):
            data = get_path(["path_example.csv"])["elevation"]
            self.assertEqual(data.min(),100.0) # TODO get the actual minimum elevation along this path
        def test_path_max(self):
            data = get_path(["path_example.csv"])["elevation"]
            self.assertEqual(data.max(),600.0) # TODO get the actual minimum elevation along this path
    unittest.main()
