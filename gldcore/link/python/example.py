""" gldcore/python/link/example.py

This is an example of a main Python module that runs a simulation
and plots the collected data to a PNG file.  This process is completed
in two parts.

Part 1 runs the simulation in the 'example.glm' model file. This model
loads the python module used to handle events in the simulation. It also
sets the variables used to label the plot when it is done.

Part 2 generates a plot with the data generated and collected in Part 1.
"""

import sys
assert(sys.version_info.major>2)
import numpy as np 
import matplotlib.pyplot as plt
import gridlabd

#
# Part 1 - run the simulation
#

# construct the command line (gridlabd hasn't started yet)
gridlabd.command('example.glm')

# start gridlabd and wait for it to complete
gridlabd.start('wait')

# send the final model state to a file
gridlabd.save('done.json');

#
# Part 2 - plot the results
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
modelname = gridlabd.get_global("modelname")
title_name = gridlabd.graph_title
plt.title(title_name)
plt.savefig(modelname.replace(".glm",".png"))


