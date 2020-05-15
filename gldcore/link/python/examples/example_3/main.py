""" gldcore/python/link/examples/example_3/main.py

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
import datetime as dt

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

data = pd.read_csv("x.csv",names=["datetime","x"],index_col="datetime")
index = np.array(list(map(lambda t:dt.datetime.strptime(t[0:-4],'%Y-%m-%d %H:%M:%S').timestamp(),data.index)))
data.index = (index-index[0])/3600
plot = data.plot(grid=True,xticks=range(0,25,5))
plot.get_figure().savefig("example.png")

