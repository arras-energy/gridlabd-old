import os 
import csv
import datetime
import re
import math
import cmath

data = {}
nodes = ["Timestamp"]
lastnodes = []
timestamp = None
timezone = "UTC"
with open('output/volt_dump.csv', 'r') as dumpfile:
	print("Reading volt_dump...")
	reader = csv.reader(dumpfile)
	for row in reader:
		if row[0].startswith("#") :
			tpos = row[0].find(" at ")
			if tpos > 0 :
				timestamp = row[0][tpos+4:tpos+27]
				timestamp = datetime.datetime.strptime(timestamp,"%Y-%m-%d %H:%M:%S %Z")
				data[timestamp] = []
				timezone = row[0][tpos+24:tpos+27]
			header = []
		elif not header :
			header = row
			assert(header==['node_name', 'voltA_real', 'voltA_imag', 'voltB_real', 'voltB_imag', 'voltC_real', 'voltC_imag'])
			if lastnodes :
				assert(lastnodes==nodes)
			elif nodes :
				lastnodes = nodes
		else :
			try :
				node = row[0]
				A = complex(float(row[1]),float(row[2]))
				B = complex(float(row[3]),float(row[4]))
				C = complex(float(row[5]),float(row[6]))
				if not node+"_A" in nodes :
					nodes.extend([node+"_A",node+"_B",node+"_C"])
				data[timestamp].extend([A,B,C])
			except :
				print("ERROR: ignored row '%s'" % row)

with open('output/voltages.csv','w') as voltages:
	print("Writing voltages...")
	writer = csv.writer(voltages)
	writer.writerow(nodes)
	for key in sorted(data.keys()) :
		row = [key.strftime("%Y-%m-%dT%H:%M:%S%z")]
		for value in data[key]:
			row.append("%g%+gd" % (abs(value),(cmath.phase(value))*180/3.1415926))
			#row.append("%g%+gd" % (value.real,value.imag))
		writer.writerow(row)

headers = ["Timestamp"]
data = {}
timestamp_common = []
timestamp_current = []
re_complex = re.compile("([+-][0-9]*\\.?[0-9]+|[+-][0-9]+.[0-9]+[eE][0-9]+)([+-][0-9]*\\.?[0-9]+|[+-][0-9]+.[0-9]+[eE][0-9]+)([ijdr])")
def to_complex(s) :
	r = re.split(re_complex,s)
	if type(r) is list and len(r) > 4 :
		if r[3] == 'd' :
			m = float(r[1])
			a = float(r[2])*3.1415926/180.0
			return complex(m*math.cos(a),m*math.sin(a))
		elif r[3] == 'r' :
			m = float(r[1])
			a = float(r[2])
			return complex(m*math.cos(a),m*math.sin(a))
	try :
		return complex(s)
	except :
		raise Exception("complex('%s') is not valid" % s)
for filename in os.listdir("output") :
	if filename.startswith("power_dump_") :
		with open("output/"+filename,"r") as dumpfile :
			print("Timestamp Read %s..." % filename)
			reader = csv.reader(dumpfile)
 			for row in reader:
 				if '#' not in row[0][0] : 
 					timestamp_current.append(datetime.datetime.strptime(row[0],"%Y-%m-%d %H:%M:%S %Z"))
			if not timestamp_common : 
				timestamp_common = timestamp_current[:]
				continue
 		timestamp_common = set(timestamp_common) & set(timestamp_current)
 		timestamp_current = []

for filename in os.listdir("output") :
	if filename.startswith("power_dump_") :
		with open("output/"+filename,"r") as dumpfile :
			print("Data Read %s..." % filename)
 			reader = csv.reader(dumpfile)
 			for row in reader :
 				if '#' not in row[0][0] :
 					timestamp = datetime.datetime.strptime(row[0],"%Y-%m-%d %H:%M:%S %Z")
					if not timestamp in data.keys() and timestamp in list(timestamp_common) : 
						data[timestamp] = []
 					if timestamp in list(timestamp_common) : 
 							data[timestamp].extend(list(map(lambda x:to_complex(x),row[1:])))
 					else : 
 						print("Timestamp '%s' unique to '%s'..." % (timestamp, filename))
				if row[0][0] == '#' :
					if row[0]=="# timestamp" :
						headers.extend(row[1:])
					continue


with open("output/powers.csv","w") as powers:
	print("Writing powers...")
	writer = csv.writer(powers)
	writer.writerow(headers)
	for key,values in data.items() :
		data = [key.strftime("%Y-%m-%dT%H:%M:%S%z")]
		for value in values:
			#data.append("%g%+gj" % (value.real,value.imag))
			data.append("%g%+gd" % (abs(value),(cmath.phase(value))*180/3.1415926))
		writer.writerow(data)
