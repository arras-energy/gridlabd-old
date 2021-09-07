def on_init(t):
	global object_list
	object_list = gridlabd.get("objects")
	gridlabd.output(f"{len(object_list)} objects found")
	global result
	result = {}
	return True

def on_precommit(t): # (1) reset accumulators
	global y
	global n
	y = 0.0
	n = 0
	return gridlabd.NEVER

def sync(obj,t):
	global y
	global n
	y += float(gridlabd.get_value(obj,"x"))
	n += 1
	return gridlabd.NEVER

def on_commit(t): # (2) update accumulators
	if n > 0:
		import datetime
		result[datetime.datetime.fromtimestamp(t)] = y/n
	return gridlabd.NEVER

def on_term(t): # (3) generate CSV output
	import pandas
	data = pandas.DataFrame(
		data = result.values(), 
		index = result.keys(), 
		columns = ["average"])
	data.index.name = "datetime"
	data.to_csv("average_2.csv")