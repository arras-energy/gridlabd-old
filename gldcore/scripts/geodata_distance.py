"""GridLAB-D Geodata Distance Package

The distance package computes the haversine distance between points on a sphere where
the points are given in latitude,longitude form, with position east and positive north
orientation.

The get_location() function accepts a list of positions and computes the distances
between those positions.  If resolution is specified, then the result is generated
in increments of that distance.

The get_path() function accepts a list of CSV files containing latitude and
longtitude coordinates and computes the distances between consecutive positions.
If resolution is specified, then the result is generated in increments of that
distance.

The set_context() function is used to link the dataset package to the geodata
module implementation.  This provides access to the options and the message
routing functions output(), verbose(), warning(), and error().

CONFIGURATION

    The configuration file is always named "distance.cfg". The configuration
    files are loaded in the following order, if found: system, user, local.

    System configurations are stored by default in the gridlabd share folder,
    which is normally "/usr/local/share/gridlabd", and is always specified by
    the GLD_ETC environment variable when it is set by the gridlabd script.

    User configurations are stored by default in the "$HOME/.gridlabd/geodata"
    folder.

    Local configurations are stored by default in the current folder.

DEVELOPERS

    The unit test are performed automatically when this module is main.

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
    """Read the specified configuration file

    ARGUMENTS
       file (str)   Specifies which configuration file to load. Valid values are
                    "system", "user", "local".
    RETURNS
       varies       The value of the configuration parameter
"""
    global config
    try:
        fh = open(f"{configdir[file]}/distance.cfg","r")
        config.update(json.load(f"{configdir[file]}/distance.cfg"))
    except:
        pass

def save_config(file):
    """Save the specified configuration file

    ARGUMENTS
       file (str)   Specifies which configuration file to load. Valid values are
                    "system", "user", "local".

    RETURNS
        None
    """
    global config
    fh = open(f"{configdir[file]}/distance.cfg","w")
    if fh:
        json.dump(config, f"{configdir[file]}/distance.cfg", indent=4)

def get_config(name):
    """Get a configuration PARAMETER

    ARGUMENTS

        name (str)   Specifies the name of the configuration parameter to get.
    """
    if name in config.keys():
        return config[name]
    else:
        raise Exception(f"configuration parameter '{name}' does not exist")

def set_config(name,value):
    """Set a configuration parameter

    ARGUMENTS

        name (str)       Specifies the name of the configuration parameter to
                         set.

        value (varies)   Specifies the value of the configuration parameter.
    """
    if name in config.keys():
        config[name] = value
    else:
        raise Exception(f"configuration parameter '{name}' does not exist")

def get_resolution():
    """Get the current distance resolution, if any

    RETURNS

        None     the resolution is not specified

        float    the resolution in meters
    """
    if geodata.RESOLUTION:
        return geodata.RESOLUTION
    else:
        return config["resolution"]

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

    load_config("system")
    load_config("user")
    load_config("local")

if __name__ == '__main__':

    import unittest
    class TestElevation(unittest.TestCase):
        def test_distance(self):
            self.assertEqual(round(get_distance([37.415045141688054,-122.2056472090359],[37.388063971857704,-122.28844288884694]),0),7905.0)
    unittest.main()
