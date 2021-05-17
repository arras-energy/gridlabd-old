"""GridLAB-D Geodata Vegetation Package

The vegetation geodata package obtains the vegetation at locations.
"""

version = 1 # specify API version

import sys, os
import requests
import json
import math, numpy
import pandas
from PIL import Image

#
# Defaults
#
default_options = {
    "units" : "meters",
    "precision" : {
        "vegetation" : 0
    },
    "year" : 2020,
}

default_config = {
    "nan_error" : False,
    "cachedir" : "/usr/local/share/gridlabd/geodata/vegetation",
    "repourl" : "http://geodata.gridlabd.us/vegetation",
}

valid_layers = ["cover","density","base"]

units = {
    "m" : 1.0,
    "meters" : 1.0,
    "ft" : 3.28083888,
    "feet" : 3.28083888,
}

def apply(data, options=default_options, config=default_config, warning=print):
    """Get the vegetation at the locations specified in data

    ARGUMENTS:

        data (pandas.DataFrame)

            The data frame must contain `latitude` and `longitude` fields at which
            vegetation will be computed.

        options (dict)

            "units" specifies the units in which vegetation are measured.  Valid units
            are ["meters","m"] and ["feet","ft"].

        config (dict)

            There are no configuration options

    RETURNS:

        pandas.DataFrame

            The first (and only) return value is the `data` data frame with either the
            `distance` fields updated/added for consecutive fields.

    """

    # convert lat,lon to address
    try:
        path = list(zip(data["latitude"],data["longitude"]))
    except:
        path = None
    if type(path) == type(None):
        raise Exception("vegetation dataset requires 'latitude' and 'longitude' fields")
    elev = []
    for pos in path:
        try:
            elev.append(get_vegetation(pos,repourl=config["repourl"],cachedir=config["cachedir"]))
        except Exception as err:
            if config["nan_error"]:
                elev.append(float("nan"))
            else:
                raise
    try:
        precision = int(options["precision"]["vegetation"])
    except:
        warning("vegetation precision not found in options")
        precision = 0
    global units
    if "units" not in options.keys() or options["units"] not in units.keys():
        raise Exception(f"unit '{options['units']}' or is not valid")
    unit = units[options["units"]]
    data["vegetation"] = (numpy.array(elev) * unit).round(precision)
    return data

def get_vegetation(pos,repourl=default_config["repourl"],cachedir=default_config["cachedir"]):
    """Compute the vegetation at the locations specified

    Elevations are obtained for each entry in the args list.  If the
    resolution is set, the vegetation for each step along the path at the
    specified resolution are provided.

    ARGUMENTS
        args (tuple list) List of (lat,lon) tuples.

    RETURNS
        layer_data (dict)   Layer vegetation data
    """
    result = {}
    for layer in valid_layers:
        n,e = get_imagedata(layer,pos,repourl,cachedir)
        row,col = get_rowcol(pos)
        result[layer] = [e[row][col]]

    return result

def get_rowcol(pos):
    row = 3600-int(math.modf(abs(pos[0]))[0]*3600)
    col = 3600-int(math.modf(abs(pos[1]))[0]*3600)
    return row, col

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

def get_imagename(layer,pos):
    """Get the image name for a location

    ARGUMENTS

        layer (str)         The vegetation data layer, e.g., "cover", "height", "base"
        pos (float,float)   The latitude and longitude of the location

    RETURNS

        tifname (str)       The name of the image tile used
    """
    lat = pos[0]
    lon = pos[1]
    if lat < 0:
        lat = f"{-math.floor(lat)}S"
    elif lat > 0:
        lat = f"{math.floor(lat)}N"
    else:
        lat = "0"
    if lon < 0:
        lon = f"{-math.floor(lon)}W"
    elif lon > 0:
        lon = f"{math.floor(lon)}E"
    else:
        lon = "0"
    return f"{layer}/{lat}_{lon}"

vegetation_data = {}

def get_imagedata(layer,pos,repourl,cachedir,year=default_options['year']):
    """Get the image data for a location

    ARGUMENTS

        layer (str)         The vegetation layer desired, i.e., "cover", "height", "base"
        pos (float,float)   The latitude and longitude of the location

    RETURNS

        tifname (str)       The name of the image tile used
        vegetation (nparray) The vegetation data from the image
    """
    tifname = get_imagename(layer,pos)
    global vegetation_data
    os.makedirs(cachedir,exist_ok=True)
    if not tifname in vegetation_data.keys():
        srcname = f"{repourl}/{year}/{tifname}.tif"
        dstname = f"{cachedir}/{year}/{tifname}.tif"
        if not os.path.exists(dstname):
            response = requests.get(srcname,stream=True)
            if response.status_code != 200:
                raise Exception(f"'GET {srcname}' --> HTTP error code {response.status_code}")
            with open(dstname,"wb") as fh:
                for chunk in response.iter_content(chunk_size=1024*1024):
                    if chunk:
                        fh.write(chunk)
        vegetation_data[tifname] = numpy.array(Image.open(dstname))
    return tifname, vegetation_data[tifname]


#
# Perform validation tests
#
if __name__ == '__main__':

    import unittest

    class TestElevation(unittest.TestCase):

        def test_vegetation_meters(self):
            test = pandas.DataFrame({
                "latitude" : [37.4205,37.5205],
                "longitude" : [-122.2046,-122.3046],
                })
            result = apply(test)
            self.assertEqual(result["vegetation"][0],85.0)
            self.assertEqual(result["vegetation"][1],157.0)

        def test_vegetation_feet(self):
            test = pandas.DataFrame({
                "latitude" : [37.4205,37.5205],
                "longitude" : [-122.2046,-122.3046],
                })
            result = apply(test,{"units":"feet"})
            self.assertEqual(result["vegetation"][0],279.0)
            self.assertEqual(result["vegetation"][1],515.0)

    unittest.main()
