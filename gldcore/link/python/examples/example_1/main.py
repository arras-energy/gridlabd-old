""" gldcore/python/link/example.py

This is an example of a main Python module that runs a simulation
and plots the collected data to a PNG file.  This process is completed
in three parts.

Part 1 sets up python to collect data from gridlabd.

Part 2 runs the simulation in the 'example.glm' model file. This model
loads the python module used to handle events in the simulation. It also
sets the variables used to label the plot when it is done.

Part 3 generates a plot with the data generated and collected in Part 1.
"""

import sys
assert(sys.version_info.major>2)
import numpy as np 
import matplotlib.pyplot as plt
import gridlabd

#
# Part 1 - prepare python
#

# setup the result data
gridlabd.result = {}

#
# Part 2 - run the simulation
#

# construct the command line (gridlabd hasn't started yet)
gridlabd.command('model.glm')

# start gridlabd and wait for it to complete
gridlabd.start('wait')

#
# Part 3 - plot the results
#

# gather the results collected by on_term
t = np.array(gridlabd.result["t"])
t = t - t[0]
x = np.array(gridlabd.result["x"])

# plot the result
plt.figure()
plt.plot(t/3600.0,x,label=gridlabd.graph_label)
plt.xlabel(gridlabd.graph_xlabel)
plt.ylabel(gridlabd.graph_ylabel)
plt.grid()
plt.legend()
title_name = gridlabd.graph_title
plt.title(title_name)
plt.savefig("example.png")

