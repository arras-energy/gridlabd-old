""" gldcore/link/python/examples/example_2/handlers.py

This module illustrates how to implement handlers for a gridlabd model using a
'on_init' and 'commit' handlers to share collected data with the  main python
module. """
import datetime
import csv
import numpy as np 
import matplotlib.pyplot as plt

# enable a callback when the module is initialized 
def on_init(t) :
	"""The module event handler 'on_init(t)' is used to initialize the python
	module. This event handler must return 'True' for the simulation to proceed.
	Any other value will result in an error and the simulation initialization
	process will be abandoned.

	There are several 'on_*' handlers you can implement to process module-level
	events """

	# setup access to the data collection variable using a new gridlabd module
	# variable
	gridlabd.result = {}
	gridlabd.result["t"] = [] # time vector
	gridlabd.result["x"] = [] # value vector
	gridlabd.result["n"] = 0 # sample count

	# successful init
	return True

# gather the results collected by on_term
def on_term(t):
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

# handle commit events for a particular object
def commit(obj,t) :
	"""The object event handler 'commit()' can be used to collect the latest
	values from an object after a solver update.  Commit handler must return a
	timestamp, but any value that cannot be interpreted by Python as 'True' is an
	error and the simulation is abandoned.

	There are several object handlers called by object 'on_*' event handlers. Be
	careful not use module 'on_*' event handlers for object event handlers because
	the calling convention is different. """
	
	# sample the object's raw values (undecorated)
	values = gridlabd.get_object(obj)

	# save the raw value of x
	gridlabd.result["x"].append(float(values["x"]))

	# save the timestamp
	gridlabd.result["t"].append(t)

	# increment the sample count
	gridlabd.result["n"] = 0

	# successful commit
	return True
