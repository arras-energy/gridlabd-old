"""GridLAB-D Geodata Address Resolution Package

The address package resolves addresses and locations.  

EXAMPLES:

The following example converts an address to a latitude,longitude:

    >>> import geodata_address
    >>> geodata_address.apply(DataFrame({"address":["2575 Sand Hill Rd., Menlo Park, CA"],}))
                                  address   longitude   latitude
    0  2575 Sand Hill Rd., Menlo Park, CA -122.201176  37.415463

The following example converts a latitude,longitude to an address:

    >>> import geodata_address
    >>> geodata_address.apply(DataFrame({"latitude":[37.4205],"longitude":[-122.2046]}),{"reverse":"True"})
       latitude  longitude                                            address
    0   37.4205  -122.2046  Stanford Linear Accelerator Center National Ac...

SCRIPT

The package may be run as a script using the following command options

    unittest    Performs the unit tests of the implementation

    makeconfig  Updates the package default configuration file
"""

version = 1 # specify API version

import sys
import json
from pandas import DataFrame
from shapely.geometry import Point
from geopandas.tools import geocode, reverse_geocode

#
# Defaults
#
default_options = {
    "reverse" : False,
}

default_config = {
    "provider" : "nominatim",
    "user_agent" : "csv_user_ht",
}

#
# Implementation of address package
#
def apply(data, options=default_options, config=default_config):
    """Perform conversion between latitude,longitude and address

    ARGUMENTS:

        data (pandas.DataFrame)

            The data frame must contain either an `address` field or `latitude` and `longitude`
            fields, according to whether `options["reverse"]` is `True` or `False`.  

        options (dict)

            "reverse" (bool) specifies the direction of resolution. Forward resolves 
            `latitude,longitude` from `address`. Reverse resolves `address` from 
            `latitude,longitude`. The default is `options["reverse"] = False`, which provides
            forward resolution.

        config (dict)

            "provider" (str) specifies the providers of the address resolution algorithm.
            The default is "nominatim".

            "user_agent" (str) specifies the user agent for the address resolution algorithm.
            The default is "csv_user_ht".

    RETURNS:

        pandas.DataFrame

            The first (and only) return value is the `data` data frame with either the
            `address` or `latitude,longitude` fields updated/added.

    """

    if options["reverse"]:

        # convert lat,lon to address
        pts = list(map(lambda xy: Point(xy),list(zip(data["longitude"],data["latitude"]))))
        addr = reverse_geocode(pts,provider=config["provider"], user_agent = config["user_agent"])
        data["address"] = addr["address"]
        return data 

    else: 

        # convert address to lat,lon
        pos = geocode(data["address"],provider=config["provider"], user_agent = config["user_agent"])
        data["longitude"] = list(map(lambda p: p.x,pos["geometry"]))
        data["latitude"] = list(map(lambda p: p.y,pos["geometry"]))
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
