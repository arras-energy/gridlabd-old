import os 
import csv
import datetime

data = {}
nodes = []
lastnodes = []
timestamp = None
timezone = "UTC"
with open('output/volt_dump.csv', 'r') as dumpfile:
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
	writer = csv.writer(voltages)
	writer.writerow(nodes)
	for key,values in data.items() :
		data = [key.strftime("%Y-%m-%dT%H:%M:%S%z")]
		for value in values:
			data.append("%g%+gj" % (value.real,value.imag))
		writer.writerow(data)
