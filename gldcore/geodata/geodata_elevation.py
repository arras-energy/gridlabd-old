"""GridLAB-D Geodata Elevation Package

The elevation geodata package obtains the elevation of locations.

The 'location' directive accepts a list of positions and computes the elevations
at those positions.  If resolution is specified, then the result is generated
in increments of that distance.

The 'path' function accepts a list of CSV files containing latitude and
longtitude coordinates and computes the elevations at consecutive positions.
If resolution is specified, then the result is generated in increments of that
distance.
"""

version = 1 # specify API version

import sys
import json
import math, numpy
from pandas import DataFrame

#
# Defaults
#
default_options = {
    "units" : "meters",
}

default_config = {
    "precision" : 0,
    "nan_error" : False,
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
def apply(data, options=default_options, config=default_config):
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
        path = list(zip(data["longitude"],data["latitude"]))
    except:
        path = None
    if type(path) == type(None):
        raise Exception("address resolution requires 'latitude' and 'longitude' fields")
    elev = []
    for pos in path:
        try:
            elev.append(get_elevation(pos))
        except Exception as err:
            if config["nan_error"]:
                elev.append(float("nan"))
            else:
                raise
    try:
        global units
        data["elevation"] = (numpy.array(elev) * units[options["units"]]).round(config["precision"])
    except:
        raise Exception(f"unit '{options['units']}' or is not valid")
    return data

#
# Elevation data processing
#

def get_elevation(args):
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
    pos = get_position(args[0])
    n,e = get_imagedata(pos)
    row,col = get_rowcol(pos)
    elev = [e[row][col]]
    return evel

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

def get_imagedata(pos):
    """Get the image data for a location

    ARGUMENTS

        pos (float,float)   The latitude and longitude of the location

    RETURNS

        tifname (str)       The name of the image tile used
        elevation (nparray) The elevation data from the image
    """
    tifname = get_imagename(pos)
    if not tifname in elevation_data.keys():
        srcname = f"{geodata.get_config('repourl')}/{tifname}.tif"
        dstname = f"{geodata.get_config('cachedir')}/{tifname}.tif"
        if not os.path.exists(dstname):
            response = requests.get(srcname,stream=True)
            if response.status_code != 200:
                error(f"'GET {srcname}' --> HTTP error code {response.status_code}",E_NOTFOUND)
            with open(dstname,"wb") as fh:
                for chunk in response.iter_content(chunk_size=1024*1024):
                    if chunk:
                        fh.write(chunk)
        elevation_data[tifname] = np.array(Image.open(dstname))
    return tifname, elevation_data[tifname]


#
# Perform validation tests
#
if __name__ == '__main__':

    import unittest

    class TestDistance(unittest.TestCase):

        def test_distance(self):
            test = DataFrame({
                "latitude" : [37.4205,37.5205],
                "longitude" : [-122.2046,-122.3046],
                })
            result = apply(test)
            self.assertEqual(result["distance"][1],12604.0)

    unittest.main()
