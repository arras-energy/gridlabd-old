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
}

units = {
    "m" : 1.0,
    "meters" : 1.0,
    "km" : 1e-3,
    "kilometers" : 1e-3,
    "mi" : 0.000621371,
    "miles" : 0.000621371,
    "yards" : 1.09361296,
    "yd" : 1.09361296,
    "ft" : 3.28083888,
    "feet" : 3.28083888,
}
#
# Implementation of address package
#
def apply(data, options=default_options, config=default_config):
    """Perform conversion between latitude,longitude and address

    ARGUMENTS:

        data (pandas.DataFrame)

            The data frame must contain `latitude` and `longitude` fields between which
            distances will be computed.  

        options (dict)

            "units" specifies the units in which distances are measured.  Valid units
            are ["meters","m"], ["kilometers","km"], ["feet","ft"], ["yards","yd", 
            and ["miles","mi"].

        config (dict)

            There are no configuration options

    RETURNS:

        pandas.DataFrame

            The first (and only) return value is the `data` data frame with either the
            `address` or `latitude,longitude` fields updated/added.

    """

    # convert lat,lon to address
    try:
        path = list(zip(data["longitude"],data["latitude"]))
    except:
        path = None
    if type(path) == type(None):
        raise Exception("address resolution requires 'latitude' and 'longitude' fields")
    if len(path) == 0:
        dist = []
    else:
        dist = [0.0]
        pos1 = path[0]
        for pos2 in path[1:]:
            lat1 = pos1[0]*math.pi/180
            lat2 = pos2[0]*math.pi/180
            lon1 = pos1[1]*math.pi/180
            lon2 = pos2[1]*math.pi/180
            a = math.sin((lat2-lat1)/2)**2+math.cos(lat1)*math.cos(lat2)*math.sin((lon2-lon1)/2)**2
            dist.append(6371e3*(2*numpy.arctan2(numpy.sqrt(a),numpy.sqrt(1-a))))
        try:
            global units
            data["distance"] = (numpy.array(dist) * units[options["units"]]).round(config["precision"])
        except:
            raise Exception(f"unit '{options['units']}' is not valid")
    return data 

#
# Perform validation tests
#
if __name__ == '__main__':

    if len(sys.argv) == 1 or sys.argv[1] in ["-h","--help","help"]:
        
        print(f"Syntax: {sys.argv[0].split('/')[-1]} [unittest|makeconfig]")

    elif sys.argv[1] in ["unittest"]:

        import unittest

        class TestAddress(unittest.TestCase):

            def test_address(self):
                test = DataFrame({
                    "address":["2575 Sand Hill Rd., Menlo Park, CA 94025, USA"],
                    })
                result = apply(test)
                self.assertEqual(round(result["latitude"][0],6),37.420457)
                self.assertEqual(round(result["longitude"][0],6),-122.204568)

            def test_reverse(self):
                options = default_options
                options["reverse"] = True
                test = DataFrame({
                    "latitude" : [37.4205],
                    "longitude" : [-122.2046],
                    })
                result = apply(test, options)
                self.assertEqual(result["address"][0],"Stanford Linear Accelerator Center National Accelerator Laboratory, Sand Hill Road, Menlo Park, San Mateo County, California, 94028, United States")

        unittest.main()

    elif sys.argv[1] in ["makeconfig"]:

        with open(sys.argv[0].replace(".py",".cfg"),"w") as fh: 
            json.dump(default_config,fh,indent=4)

    else:

        raise Exception(f"'{sys.argv[0]}' is an invalid command option")
