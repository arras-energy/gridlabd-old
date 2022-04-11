import time 
import json
import datetime


class GridlabdModel :
	def __init__(self,jsonfile):
		fh = open(jsonfile)
		self.model = json.load(fh)
		assert(self.model["application"]=='gridlabd')
		assert(self.model["version"]>='4.0.0')

if __name__ == '__main__' :
	model = GridlabdModel('../test_clock.json')
	global_stop = model.model["globals"]["stoptime"]["value"]
	object_clock = {}
	for i,value in model.model["objects"].items() :
		object_clock = value["clock"]
		if global_stop == object_clock:
			#print("success", object_clock)
			exit(0)
		else: 
			print("fail", object_clock)
			exit(1)
			break
