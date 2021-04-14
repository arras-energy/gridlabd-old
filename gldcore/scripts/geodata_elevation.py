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

DATASET = "elevation"

import os, sys
import requests
import math
import numpy as np
from PIL import Image
import pandas
import json

elevation_data = {} # in memory holding area for image data

def get_path(args):
    """Compute the elevations along a path specified in the CSV files

    Elevations are obtained for each CSV file in the args list.  If the
    resolution is set, the elevations for each step along the path at the
    specified resolution are provided.

    Multiple CSV files are concatenated. The elevations are added to the
    columns found in the CSV file. If the elevation column is already
    present, the previous data is overwritten.

    ARGUMENTS
        args (str list) List of CSV files to read

    RETURNS
        DataFrame       Pandas dataframe containing the latitudes, longitudes,
                        and elevations.
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
    pos = [list(a) for a in zip(lats,lons)]
    path["id"] = geodata.distance(pos)
    path.set_index(["id","latitude","longitude"],inplace=True)

    locs = get_location(pos)
    lats = locs["latitude"].to_list()
    lons = locs["longitude"].to_list()
    pos = [list(a) for a in zip(lats,lons)]
    locs["id"] = geodata.distance(pos)
    locs.set_index(["id","latitude","longitude"],inplace=True)

    return path.join(locs,how=geodata.PATHJOIN,sort=True).reset_index()

def get_location(args):
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
    resolution = geodata.get_resolution()
    verbose(f"resolution = {resolution} meters")
    if len(args) < 2:
        pos = get_position(args[0])
        row,col = get_rowcol((pos[0],pos[1]))
        lats = [pos[0]]
        lons = [pos[1]]
        n,e = get_data(pos)
        elev = [e[row][col]]
        if type(resolution) is float:
            warning("resolution is ignored when using a single location")
    else:
        pos1 = get_position(args[0])
        n,e = get_data(pos1)
        row,col = get_rowcol(pos1)
        lats = [pos1[0]]
        lons = [pos1[1]]
        elev = [e[row][col]]
        for arg in args[1:]:
            pos2 = get_position(arg)
            d = get_distance(pos1,pos2)
            if type(resolution) is float:
                segs = d/resolution
                if segs > 1:
                    lat = lats[-1]
                    lon = lons[-1]
                    dlat = (pos2[0]-pos1[0])/segs
                    dlon = (pos2[1]-pos1[1])/segs
                    for n in range(int(segs)):
                        lat += dlat
                        lon += dlon
                        lats.append(lat)
                        lons.append(lon)
                        n,e = get_data((lat,lon))
                        row,col = get_rowcol((lat,lon))
                        elev.append(e[row][col])
            lats.append(pos2[0])
            lons.append(pos2[1])
            n,e = get_data(pos2)
            row,col = get_rowcol(pos2)

            # can't interpolate at lower edges of images (rare/small impact)
            if row == 0 or col == 0:
                z = e[row][col]
            # 2-d interpolation
            else:
                dy = math.modf(abs(pos2[0]))[0]*3600 - (3600-row)
                dx = math.modf(abs(pos2[1]))[0]*3600 - (3600-col)
                z00 = e[row][col]
                z01 = e[row-1][col]
                z10 = e[row][col-1]
                z11 = e[row-1][col-1]
                z0 = dx*(z10-z00) + z00
                z1 = dx*(z11-z01) + z01
                z = dy*(z1-z0) + z0
                # print(f"{pos2} -> {row},{col} -> {z00} {z01} {z10} {z11} -> {dx} {dy} -> {round(z)}")
            elev.append(round(z))
            pos1 = pos2
    result = pandas.DataFrame(
            data={
                "latitude" : lats,
                "longitude" : lons,
                "elevation" : elev}
            )
    result.index.names = ["id"]
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

def get_name(pos):
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

def get_data(pos):
    """Get the image data for a location

    ARGUMENTS

        pos (float,float)   The latitude and longitude of the location

    RETURNS

        tifname (str)       The name of the image tile used
        elevation (nparray) The elevation data from the image
    """
    tifname = get_name(pos)
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

geodata = None # this will be set by the set_context() call from geodata
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

    geodata.set_config({
        "repourl" : f"http://geodata.gridlabd.us/{DATASET}/10m",
        "resolution" : None,
        "cachedir" : f"{geodata.PKGDATA}/geodata/{DATASET}/10m",
    })
    geodata.load_config(DATASET,"system")
    geodata.load_config(DATASET,"user")
    geodata.load_config(DATASET,"local")
    os.makedirs(geodata.get_config("cachedir"),exist_ok=True)

if __name__ == '__main__':

    import unittest
    class TestElevation(unittest.TestCase):
        def test_distance(self):
            self.assertEqual(round(get_distance([37.415045141688054,-122.2056472090359],[37.388063971857704,-122.28844288884694]),0),7905.0)
    unittest.main()
