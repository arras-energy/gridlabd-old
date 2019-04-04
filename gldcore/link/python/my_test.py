import csv

# create a global status variable in python
sample_count = None

# create the variable into which we will collect data
sample_data = {}

# enable a callback when the module is initialized by gridlabd at the start of a simulation
def on_init(t) :

	# get a copy the sample count from the model (should be zero)
	global sample_count
	sample_count = int(gridlabd.get_global("sample_count"))
	if not sample_count == 0 :
		gridlabd.warning("sample count is not initialized to zero")

	# setup access to the data collection variable
	global sample_data
	sample_data["t"] = []
	sample_data["x"] = []

	# successful init
	return True

def commit(obj,t) :
	
	# sample the object data
	global sample_data
	values = gridlabd.get_object(obj)
	sample_data["t"].append(t)
	sample_data["x"].append(float(values["x"]))

	# increment and save the global sample count
	global sample_count
	sample_count += 1
	gridlabd.set_global("sample_count",str(sample_count))

	# successful commit
	return gridlabd.NEVER

def on_term(t) :
	global sample_data
	import matplotlib.pyplot as plt
	import numpy as np 
	t = np.array(sample_data["t"])
	x = np.array(sample_data["x"])
	plt.figure()
	plt.plot((t-sample_data["t"][0])/3600.0,x,label="x")
	plt.xlabel('Time [h]')
	plt.ylabel('Power [MW]')
	plt.grid()
	plt.legend()
	modelname = gridlabd.get_global("modelname")
	plt.savefig(modelname.replace(".glm",".png"))