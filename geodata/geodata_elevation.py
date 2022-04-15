"""GridLAB-D Geodata Elevation Package

The elevation geodata package obtains the elevation of locations.

INPUT

    latitude - record latitude

    longitude - record longitude

OUTPUT

    elevation - record elevation

OPTIONS

    units - elevation units (default "meters"). Valid units are "m", "meters", 
    "ft", and "feet".

    precision - elevation precision (default 0 decimals)

CONFIGURATION

    nan_error - enable inserting NaN values instead of raise an exception on
    bad elevation data (default is False).

    cachedir - identifies the cache folder (default is ""/usr/local/share/gridlabd/geodata/elevation/10m")

    repourl - identifies the source URL for the elevation data (default is ""http://geodata.gridlabd.us/elevation/10m")

EXAMPLES

    % geodata merge -D elevation 37.5,-122.2 37.4,-122.3 --units=ft
    id,latitude,longitude,elevation
    0,37.5,-122.2,3.0
    1,37.4,-122.3,2100.0
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
        "elevation" : 0
    }
}

GLD_ETC = os.getenv("GLD_ETC")
if not GLD_ETC:
    GLD_ETC = "/usr/local/share/gridlabd"
default_config = {
    "nan_error" : False,
    "cachedir" : f"{GLD_ETC}/geodata/elevation/10m",
    "repourl" : "http://geodata.gridlabd.us/elevation/10m",
}

units = {
    "m" : 1.0,
    "meters" : 1.0,
    "ft" : 3.28083888,
    "feet" : 3.28083888,
}
#
# Implementation of address package
#
def apply(data, options=default_options, config=default_config, warning=print):
    """Get the elevation at the locations specified in data

    ARGUMENTS:

        data (pandas.DataFrame)

            The data frame must contain `latitude` and `longitude` fields at which
            elevations will be computed.

        options (dict)

            "units" specifies the units in which elevations are measured.  Valid units
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
        raise Exception("elevation dataset requires 'latitude' and 'longitude' fields")
    elev = []
    for pos in path:
        try:
            elev.append(get_elevation(pos,repourl=config["repourl"],cachedir=config["cachedir"]))
        except Exception as err:
            if config["nan_error"]:
                elev.append(float("nan"))
            else:
                raise
    try:
        precision = int(options["precision"]["elevation"])
    except:
        warning("elevation precision not found in options")
        precision = 0
    global units
    if "units" not in options.keys() or options["units"] not in units.keys():
        raise Exception(f"unit '{options['units']}' or is not valid")
    unit = units[options["units"]]
    data["elevation"] = (numpy.array(elev) * unit).round(precision)
    return data

#
# Elevation data processing
#

def get_elevation(pos,repourl=default_config["repourl"],cachedir=default_config["cachedir"]):
    """Compute the elevations at the locations specified

    Elevations are obtained for each entry in the args list.  If the
    resolution is set, the elevations for each step along the path at the
    specified resolution are provided.

    ARGUMENTS
        args (tuple list) List of (lat,lon) tuples.

    RETURNS
        DataFrame       Pandas dataframe containing the latitudes, longitudes,
                        and elevations.
    """
    n,e = get_imagedata(pos,repourl,cachedir)
    row,col = get_rowcol(pos)
    dx = 1.0-math.modf(abs(pos[1])*3600)[0]
    dy = 1.0-math.modf(abs(pos[0])*3600)[0]
    e00 = float(e[row][col])
    if row > 0:
        e10 = float(e[row-1][col])
    else:
        e10 = e00
    if col > 0:
        e01 = float(e[row][col-1])
    else:
        e01 = e00
    if row > 0 and col > 0:
        e11 = float(e[row-1][col-1])
    else:
        e11 = e00
    e0 = dx*e00 + (1-dx)*e01
    e1 = dx*e10 + (1-dx)*e11 

    elev = dy*e0 + (1-dy)*e1


    return [elev]

def get_rowcol(pos):
    """Find the row and column index of a pixel for a position"""
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

def get_imagename(pos):
    """Get the image name for a location

    ARGUMENTS

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
    return f"{lat}_{lon}"

elevation_data = {}

def get_imagedata(pos,repourl,cachedir):
    """Get the image data for a location

    ARGUMENTS

        pos (float,float)   The latitude and longitude of the location

    RETURNS

        tifname (str)       The name of the image tile used
        elevation (nparray) The elevation data from the image
    """
    tifname = get_imagename(pos)
    global elevation_data
    os.makedirs(cachedir,exist_ok=True)
    if not tifname in elevation_data.keys():
        srcname = f"{repourl}/{tifname}.tif"
        dstname = f"{cachedir}/{tifname}.tif"
        if not os.path.exists(dstname):
            response = requests.get(srcname,stream=True)
            if response.status_code != 200:
                raise Exception(f"'GET {srcname}' --> HTTP error code {response.status_code}")
            with open(dstname,"wb") as fh:
                for chunk in response.iter_content(chunk_size=1024*1024):
                    if chunk:
                        fh.write(chunk)
        elevation_data[tifname] = numpy.array(Image.open(dstname))
    return tifname, elevation_data[tifname]


#
# Perform validation tests
#
if __name__ == '__main__':

    import unittest

    default_config['cachedir'] = "/tmp/geodata/elevation"
    os.makedirs(default_config['cachedir'],exist_ok=True)

    class TestElevation(unittest.TestCase):

        def test_elevation_meters(self):
            test = pandas.DataFrame({
                "latitude" : [37.4205,37.5205],
                "longitude" : [-122.2046,-122.3046],
                })
            result = apply(test)
            self.assertEqual(result["elevation"][0],84.0)
            self.assertEqual(result["elevation"][1],154.0)

        def test_elevation_feet(self):
            test = pandas.DataFrame({
                "latitude" : [37.4205,37.5205],
                "longitude" : [-122.2046,-122.3046],
                })
            result = apply(test,{"units":"feet"})
            self.assertEqual(result["elevation"][0],275.0)
            self.assertEqual(result["elevation"][1],504.0)

    unittest.main()
