import os, sys
import pandas
import geopandas
import fiona
import pickle
# import requests
# import math
import numpy as np
# from PIL import Image
# import json

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

DATASET = "utility"

default_config = {
    "repourl" : "http://geodata.gridlabd.us/utility",
    "resolution" : 10.0,
    "cachedir" : "/usr/local/share/gridlabd/geodata/utility",
}

kmlfile = None
kmldata = None
csvfile = None
csvdata = None

TODO = 199 # utility id to use until get_utility works ok

def read_kml():
    global kmldata
    if type(kmldata) == type(None):
        if not os.path.exists(kmlfile):
            verbose("Downloading kml")
            geopandas.io.file.fiona.drvsupport.supported_drivers['KML'] = 'rw'
            kmldata = geopandas.read_file(geodata.get_config("repourl")+"/geometry.kml", driver="KML")
            verbose("Caching kml")
            with open(kmlfile, "wb") as fh:
                pickle.dump(kmldata,fh)
        else:
            verbose("Reading kml")
            with open(kmlfile, "rb") as fh:
                kmldata = pickle.load(fh)
    return kmldata

def read_csv():
    global csvdata
    if type(csvdata) == type(None):
        if not os.path.exists(csvfile):
            verbose("Downloading csv")
            csvdata = pandas.read_csv(geodata.get_config("repourl")+"/information.csv")
            verbose("Caching csv")
            csvdata.to_csv(csvfile)
        else:
            verbose("Reading csv")
            csvdata = pandas.read_csv(csvfile)
    return csvdata

def get_information(id,field=None):
    data = read_csv()
    if field:
        return data[field][id]
    else:
        return data.iloc[id]

def get_geometry(id):
    data = read_csv()
    geom = read_kml()
    return geom["geometry"][id]

def get_utility(pos):
    return TODO

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
    """Obtain the utility information for the locations specified

    If the resolution is set, the information for each step along the path at the
    specified resolution are provided.

    ARGUMENTS
        args (tuple list)   List of (lat,lon) tuples.

    RETURNS
        DataFrame       Pandas dataframe containing the utility information for
                        the position given.
    """
    if not args:
        error(f"{DATASET}.get_location({args}) missing one or more position arguments",geodata.E_SYNTAX)
    info = {"latitude":[],"longitude":[],"utility":[]}
    resolution = geodata.get_resolution()
    pos0 = None
    for arg in args:
        pos = get_position(arg)
        id = get_utility(pos)
        data = get_information(id)
        if pos0:
            d = get_distance(pos0,pos)
        else:
            d = 0
        if type(resolution) is float:
            segs = d/resolution
            if segs > 1:
                lat = info["latitude"][-1]
                lon = info["longitude"][-1]
                dlat = (pos0[0]-pos[0])/segs
                dlon = (pos0[1]-pos[1])/segs
                for n in range(int(segs)):
                    lat += dlat
                    lon += dlon
                    info["latitude"].append(lat)
                    info["longitude"].append(lon)
                    info.update(data)
        info["latitude"].append(pos[0])
        info["longitude"].append(pos[1])
        info["utility"] = data["NAME"]
        info.update(data)
        pos0 = pos
    result = pandas.DataFrame(info)
    result["id"] = geodata.distance(list(zip(info["latitude"],info["longitude"])))
    result.set_index("id")
    return result

def get_path(args):
    """Obain utility information along paths specified in CSV files

    If the resolution is set, the coordinates for each step along the path at the
    specified resolution are provided.

    ARGUMENTS
        args (str list)   List of CSV files containing paths. The CSV files must
                          include columns named "latitude" and "longitude".

    RETURNS
        DataFrame         Pandas dataframe containing the concatenated contents
                          of the CSV files with the information columns added.
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

# local geodata class to support testing and offline usage
class geodata:
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
    def get_config(x):
        return default_config[x]
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

# set online geodata context
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

    global kmlfile
    kmlfile = geodata.get_config("cachedir") + "/geometry.kml"

    global csvfile
    csvfile = geodata.get_config("cachedir") + "/information.csv"

    global kmldata
    kmldata = read_kml()

    global csvdata
    csvdata = read_csv()

# perform validation tests
if __name__ == '__main__':

    import unittest
    import numpy
    IDPRECISION = 3
    kmlfile = "/usr/local/share/gridlabd/geodata/utility/geometry.kml"
    csvfile = "/usr/local/share/gridlabd/geodata/utility/information.csv"
    os.makedirs("/usr/local/share/gridlabd/geodata/utility",exist_ok=True)

    class TestDataset(unittest.TestCase):
        def test_utility_geometry(self):
            self.assertEqual(numpy.round(get_geometry(199).area,2),17.74)
        def test_utility_name(self):
            self.assertEqual(get_information(199,"NAME"),"PACIFIC GAS & ELECTRIC CO.")
        def test_utility_information(self):
            self.assertEqual(get_information(199)["NAME"],"PACIFIC GAS & ELECTRIC CO.")
    unittest.main()
