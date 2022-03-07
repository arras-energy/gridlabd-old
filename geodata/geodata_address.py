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
from pandas import DataFrame, Series
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
    "timeout" : 5,
    "retries" : 5,
    "sleep" : 1,
}

#
# Implementation of address package
#
def apply(data, options=default_options, config=default_config, warning=print):
    """Perform conversion between latitude,longitude and address

    ARGUMENTS:

        data (pandas.DataFrame)

            The data frame must contain either an `address` field or `latitude` and `longitude`
            fields, according to whether `options["reverse"]` is `True` or `False`.

        options (dict)

            "reverse" (bool) specifies the direction of resolution. Forward resolves `address`
            from `latitude,longitude` from . Reverse resolves `latitude,longitude`
            `address` from. The default is `options["reverse"] = False`, which provides
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

        # convert address to lat,lon
        if not "address" in list(data.columns):
            raise Exception("reserve address resolution requires 'address' field")
        data.reset_index(inplace=True) # index is not meaningful
        for retries in range(config["retries"]):
            try:
                pos = geocode(data["address"],
                        provider = config["provider"],
                        user_agent = config["user_agent"],
                        timeout = config["timeout"],
                        )
                break
            except Exception as err:
                pos = err
            import time
            time.sleep(config["sleep"])
        if type(pos) is Exception:
            raise pos
        data["longitude"] = list(map(lambda p: p.x,pos["geometry"]))
        data["latitude"] = list(map(lambda p: p.y,pos["geometry"]))
        return data

    else:

        # convert lat,lon to address
        try:
            lats = list(map(lambda x: float(x),data["latitude"]))
            lons = list(map(lambda x: float(x),data["longitude"]))
            pos = list(map(lambda xy: Point(xy),list(zip(lons,lats))))
        except:
            pos = None
        if type(pos) == type(None):
            raise Exception("address resolution requires 'latitude' and 'longitude' fields")
        for retries in range(config["retries"]):
            try:
                addr = reverse_geocode(pos,
                        provider = config["provider"],
                        user_agent = config["user_agent"],
                        timeout = config["timeout"],
                        )
                break
            except Exception as err:
                addr = err
            import time
            time.sleep(config["sleep"])
        if type(addr) is Exception:
            raise addr
        data["address"] = Series(addr["address"],dtype="string").tolist()
        return data

#
# Perform validation tests
#
if __name__ == '__main__':

    import unittest

    class TestAddress(unittest.TestCase):

        def test_address_reverse(self):
            test = DataFrame({
                "address":["The White House"],
                })
            result = apply(test,{"reverse":True})
            self.assertEqual(round(result["latitude"][0],6),38.8977)
            self.assertEqual(round(result["longitude"][0],6),-77.036553)

        def test_address(self):
            test = DataFrame({
                "latitude" : [38.8977],
                "longitude" : [-77.036553],
                })
            result = apply(test)
            self.assertTrue(result["address"][0],"The White House")

    unittest.main()
