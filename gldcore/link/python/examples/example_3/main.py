""" gldcore/python/link/examples/example_1/main.py

This is an example of a main Python module that runs a simulation and plots
the collected data to a PNG file.  This process is completed in two parts.

Part 1 runs the simulation in the 'example.glm' model file. This model loads
the python module used to handle events in the simulation. It also sets the
variables used to label the plot when it is done.

Part 2 generates a plot with the data generated and collected in Part 1. """

import sys
assert(sys.version_info.major>2)
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import gridlabd

#
# Part 1 - run the simulation
#

# construct the command line (gridlabd hasn't started yet)
gridlabd.command('model.glm')

# start gridlabd and wait for it to complete
gridlabd.start('wait')

#
# Part 2 - plot the results
#

data = pd.read_csv("x.csv",header=9,names=["datetime","x"],index_col="datetime")
print(data.index)

