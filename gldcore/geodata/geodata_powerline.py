"""GridLAB-D Geodata Powerline Package

The powerline package computes geographic information about powerlines, such as
line sag and line sway.

The 'location' directive accepts a list of pole locations and computes the line
properties at those locations, which are nominally trivial. If resolution is
specified, then the line properties are generated in increments of that distance
between the specified pole locations.

The 'path' function accepts a list of CSV files containing latitude and
longtitude for the pole locations. If resolution is specified, then the result
is generated in increments of that distance.

To compute "linesag" and "linesway", the following parameters must be provided
either on the command line or as columns in the CSV file.

  "cabletype"   This property is required to obtain the cable's physical

  "windspeed"   Optional. If omitted, the windspeed is assumed to be 0.
  "winddir"     Optional. Required if windspeed is given.
  "temperature" Optional. If omitted, the temperature is assumed to be 10C.
  "powerflow"   Optional. If omitted, the powerflow is assumed to be 0MW.
  "elevation"   Optional. If omitted, the pole elevations are assumed to be 0.

The values of "linesag" are given with respect to the first pole elevation in a
pole pair vertically in meters. The values of "linesway" are given with respect
to the line from pole to pole, horizontally in meters.  If the "linesway" is
non-zero, the "linesag" will be computed according to the formula

  linesag(linesway)^2 = linesag(0)^2 - linesway^2

"""

version = 1

import os, sys
import requests
import math
import numpy as np
from PIL import Image
import pandas
import json

TODO=float('nan')

default_options = {
}

default_config = {
}

def linesag(**kwargs):
    return TODO

def linesway(**kwargs):
    return TODO

def apply(data,options=default_options,config=default_config):
    result = pandas.DataFrame(data)
    result["linesag"] = linesag(data)
    result["linesway"] = linesway(data)
    return result

# perform validation tests
if __name__ == '__main__':

    import unittest

    class TestPowerline(unittest.TestCase):

        def test_linesag(self):
            self.assertEqual(linesag(TODO),TODO)

        def test_linesway(self):
            self.assertEqual(linesway(TODO),TODO)

    unittest.main()
