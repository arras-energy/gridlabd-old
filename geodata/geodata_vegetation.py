"""GridLAB-D Geodata Vegetation Package

The vegetation geodata package obtains the vegetation at locations.

INPUT

    latitude - the latitude field (required)

    longitude - the longitude field (required)

OUTPUT

    base - vegetation base height

    cover - vegetation cover fraction

    height - vegetation top height

OPTIONS

    units - specifies the units in which output is given (default is meters). 
    Valid units are "m", "meters", "ft", and "feet".

    precision - dictionary of output precisions

        vegetation - precision of vegetation data (default is 0 decimals).

    year - year for which data is returned (default is 2020)

CONFIGURATION

    nan_error -  Return NaN values instead of raising exceptions on bad data (default is False)
    
    cachedir - Location of local data cache (default is "/usr/local/opt/gridlabd/share/gridlabd/geodata/vegetation")
    
    repourl - Location of remote vegetation data (default is "http://geodata.gridlabd.us/vegetation")
    
    layers - Layers to collect from data repository (default is "base,cover,height")
    
    layer_units - Dictionary of layer units to use (default is {"base":"m","cover":"%","height":"m"})
    
    vegetation - Dictionary of vegetation access information.
    
        username -  Username for access credentials (usually an email, default is "")
    
        password - Password for access credentials (usually a token, default is "")
    
        provider - Data provider name (default is "cfo")
    
        usecache - Enable caching of data (default is True)
    
    maximum_image_size - Maximum size of remote data block (default is 2e9 bytes)

EXAMPLES

    The following command retrieves the vegetation someplace in the Santa Cruz Mountains:

        % gridlabd geodata merge -D vegetation 37.4,-122.3 --vegetation.username=YOUREMAIL --vegetation.password=YOURPWD
    id,latitude,longitude,base,cover,height
    0,37.4,-122.3,2.0,0.57,17.0
"""

version = 1 # specify API version

import sys, os
import requests
import json
import math, numpy
import pandas
from PIL import Image

#
# Default options can be set from the command line only
#
default_options = {
    "units" : "meters",
    "precision" : {
        "vegetation" : 0
    },
    "year" : 2020,
}

#
# Default configuration can be set using `config set` or from command line
#
default_config = {
    "nan_error" : False,
    "cachedir" : "/usr/local/opt/gridlabd/share/gridlabd/geodata/vegetation",
    "repourl" : "http://geodata.gridlabd.us/vegetation",
    "layers" : ["base","cover","height"],
    "layer_units" : {"base":"m","cover":"%","height":"m"},
    "vegetation" : {
        "username" : "",
        "password" : "",
        "provider" : "cfo",
        "usecache" : True,
    },
    "maximum_image_size" : 2000000000,
}

#
# Provider authentication
#

def cfo(username,password,usecache=True):
    import cfo 
    if "CFO_EMAIL" not in os.environ.keys() and username:
        os.environ['CFO_EMAIL'] = username
    if "CFO_PASS" not in os.environ.keys() and password:
        os.environ['CFO_PASS'] = password
    return cfo.api().authenticate(ignore_temp=(not usecache)) == 200

#
# Valid height units
#

units = {
    "m" : 1.0,
    "meters" : 1.0,
    "ft" : 3.28083888,
    "feet" : 3.28083888,
}

def process_base(data):
    return data

def process_cover(data):
    return data/100.0

def process_height(data):
    return data

layer_process = {
    "base" : process_base,
    "cover" : process_cover,
    "height" : process_height,
}

def apply(data, options=default_options, config=default_config, warning=lambda x:print(x,file=sys.stderr)):
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

    if not config["vegetation"]["username"] or not config["vegetation"]["password"]:
        warning(f"'vegetation.username' not specified, only static {options['year']} data is available to unregistered users (see {config['repourl']} to register)")
    elif config["vegetation"]["provider"] and not eval(config["vegetation"]["provider"])(username=config["vegetation"]["username"],password=config["vegetation"]["password"],usecache=config["vegetation"]["usecache"]):
        warning(f"vegetation data provider authentication failed")

    # convert lat,lon to address
    try:
        path = list(zip(data["latitude"],data["longitude"]))
    except:
        path = None
    if type(path) == type(None):
        raise Exception("vegetation dataset requires 'latitude' and 'longitude' fields")
    try:
        precision = int(options["precision"]["vegetation"])
    except:
        warning("vegetation precision not found in options")
        precision = 0
    global units
    if "units" not in options.keys() or options["units"] not in units.keys():
        raise Exception(f"unit '{options['units']}' or is not valid")
    unit = float(units[options["units"]])
    result = {}
    for pos in path:
        if math.isnan(pos[0]) or math.isnan(pos[1]):
            warning(f"field of 'latitude' or 'longitude' is missing, use 'nan' for vegetation data.")
            values={'base': [numpy.nan], 'cover': [numpy.nan], 'height': [numpy.nan]}
            for key, value in values.items():
                if config["layer_units"][key] == "m":
                    value = list(map(lambda x:round(float(x)*unit,int(precision)),value))
                elif config["layer_units"][key] == "%":
                    value = list(map(lambda x:round(float(x),int(precision+2)),value))
                if key in result.keys():
                    result[key].extend(value)
                else:
                    result[key] = value
        else:
            try:
                values = get_vegetation(pos,lambda x:warning(x),repourl=config["repourl"],cachedir=config["cachedir"],layers=config["layers"])
                for key, value in values.items():
                    if config["layer_units"][key] == "m":
                        value = list(map(lambda x:round(float(x)*unit,int(precision)),value))
                    elif config["layer_units"][key] == "%":
                        value = list(map(lambda x:round(float(x),int(precision+2)),value))
                    if key in result.keys():
                        result[key].extend(value)
                    else:
                        result[key] = value
            except Exception as err:
                if config["nan_error"]:
                    elev.append(float("nan"))
                else:
                    raise
    for key, values in result.items():
        data[key] = values
    return data

def get_vegetation(pos,warning=lambda x:print(x,file=sys.stderr),
        repourl = default_config["repourl"],
        cachedir = default_config["cachedir"],
        layers = default_config["layers"],
        year = default_options["year"]):
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
    for layer in layers:
        name,data = get_imagedata(layer,pos,repourl,cachedir,warning,year)
        data = layer_process[layer](data)
        row,col = get_rowcol(pos,data)

        result[layer] = [data[row][col]]
    return result

def get_rowcol(pos,data):
    """Get the row and column location in a 1 degree^2 image tile"""
    height = len(data)
    width = len(data[0])
    row = height-math.floor(math.modf(abs(pos[0]))[0]*height)-1
    col = width-math.floor(math.modf(abs(pos[1]))[0]*width)-1
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

    lat = pos[0]*10
    lon = pos[1]*10

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

def get_imagedata(layer,pos,repourl,cachedir,warning=lambda x:print(x,file=sys.stderr),year=default_options['year'],maximum_image_size=default_config['maximum_image_size']):

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
    os.makedirs(f"{cachedir}/{year}/{layer}",exist_ok=True)
    if not tifname in vegetation_data.keys():
        srcname = f"{repourl}/{year}/{tifname}.tif"
        dstname = f"{cachedir}/{year}/{tifname}.tif"

        if not os.path.exists(dstname):
            response = requests.get(srcname,stream=True)
            if response.status_code != 200:
                warning(f"'GET {srcname}' --> HTTP error code {response.status_code}, use 'nan' for vegetation data.")
                im = Image.fromarray(numpy.full((1, 1), numpy.nan))
                im.save(dstname)
            else:
                with open(dstname,"wb") as fh:
                    for chunk in response.iter_content(chunk_size=1024*1024):
                        if chunk:
                            fh.write(chunk)

        Image.MAX_IMAGE_PIXELS = maximum_image_size
        vegetation_data[tifname] = numpy.array(Image.open(dstname))
    return tifname, vegetation_data[tifname]


#
# Perform validation tests
#
if __name__ == '__main__':

    import unittest

    default_config['cachedir'] = "/tmp/geodata/vegetation"
    os.makedirs(default_config['cachedir'],exist_ok=True)

    class TestVegetation(unittest.TestCase):

        def test_vegetation_meters(self):
            if default_config["vegetation"]["username"]:
                test = pandas.DataFrame({
                    "latitude" : [37.4205,37.5205],
                    "longitude" : [-122.2046,-122.3046],
                    })

                default_options.update({"units":"meters"})
                result = apply(test)
                self.assertEqual(result["cover"][0],0.05)
                self.assertEqual(result["height"][0],0.0)
                self.assertEqual(result["base"][0],0.0)

        def test_vegetation_feet(self):
            if default_config["vegetation"]["username"]:
                test = pandas.DataFrame({
                    "latitude" : [37.4205,37.5205],
                    "longitude" : [-122.2046,-122.3046],
                    })

                default_options.update({"units":"feet"})
                result = apply(test,default_options)

                self.assertEqual(result["cover"][0],0.05)
                self.assertEqual(result["height"][0],0.0)
                self.assertEqual(result["base"][0],0.0)

    unittest.main()
