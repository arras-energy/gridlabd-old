import json 
import os 
import sys, getopt
from datetime import datetime 
import csv

def convert(input_file,output_file=None, options={}):

	if output_file == '':
		if input_file[-5:] == ".json":
			output_file = input_file[:-5] + ".csv" 
		else: 
			output_file = input_file + ".csv"

	with open(input_file,"r") as f :
		data = json.load(f)
		assert(data['application']=='gridlabd')
		assert(data['version'] >= '4.2.0')
	
	def find(objects,property,value):
		result = []
		for name,values in objects.items():
			if property in values.keys() and values[property] == value:
				result.append(name)
		return result

	def get_string(values,prop):
		return values[prop]

	def get_complex(values,prop):
		return complex(get_string(values,prop).split(" ")[0].replace('i','j'))

	def get_real(values,prop):
		return get_complex(values,prop).real

	def get_voltages(values):
		ph = get_string(values,"phases")
		vn = abs(get_complex(values,"nominal_voltage"))
		result = []
		try:
			va = abs(get_complex(values,"voltage_A"))/vn
		except:
			va = None
		try:
			vb = abs(get_complex(values,"voltage_B"))/vn
		except:
			vb = None
		try:
			vc = abs(get_complex(values,"voltage_C"))/vn
		except:
			vc = None
		return ph,vn,va,vb,vc

	def profile(writer,objects,root,pos=0):
		fromdata = objects[root]
		ph0,vn0,va0,vb0,vc0 = get_voltages(fromdata)
		writer.writerow([root,pos,ph0,vn0,va0,vb0,vc0])
		for link in find(objects,"from",root):
			linkdata = objects[link]
			linktype = "-"
			if "length" in linkdata.keys():
				linklen = get_real(linkdata,"length")/5280
			else:
				linklen = 0.0
			if not "line" in get_string(linkdata,"class"):
				linktype = "--o"
			if "to" in linkdata.keys():
				to = linkdata["to"]
				todata = objects[to]
				profile(writer,objects,to,pos+linklen)

	with open(output_file,"w") as csvfile:
		csvwriter = csv.writer(csvfile)
		csvwriter.writerow(["node","distance","phases","nominal_voltage","phase_A_voltage","phase_B_voltage","phase_C_voltage"])
		for obj in find(objects=data["objects"],property="bustype",value="SWING"):
			profile(csvwriter,objects=data["objects"],root=obj)
