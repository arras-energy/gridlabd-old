""" gldcore/link/python/example_module.py

This module illustrates how to collect data from a gridlabd simulation
using a 'commit' handler and share the collected data with the main
python module.
"""
import datetime
import csv

# enable a callback when the module is initialized by gridlabd at the start of a simulation
def on_init(t) :
	"""The module event handler 'on_init(t)' is used to initialize the python module. This
	event handler must return 'True' for the simulation to proceed. Any other value will
	result in an error and the simulation initialization process will be abandoned.

	There are several 'on_*' handlers you can implement to process module-level events
	"""

	# setup access to the data collection variable using a new gridlabd module variable
	gridlabd.result = {}
	gridlabd.result["t"] = [] # time vector
	gridlabd.result["x"] = [] # value vector
	gridlabd.result["n"] = 0 # sample count

	# successful init
	return True

def commit(obj,t) :
	"""The object event handler 'commit()' can be used to collect the latest values from
	an object after a solver update.  Commit handler must return a timestamp, but any value
	that cannot be interpreted by Python as 'True' is an error and the simulation is abandoned.

	There are several object handlers called by object 'on_*' event handlers. Be careful not
	use module 'on_*' event handlers for object event handlers because the calling convention
	is different.
	"""
	
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
