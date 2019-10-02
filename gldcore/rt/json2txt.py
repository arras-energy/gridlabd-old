import json 
import os 
import sys, getopt

filename_json = ''
filename_txt = ''
try : 
	opts, args = getopt.getopt(sys.argv[1:],"hi:o:",["ifile=","ofile="])
except getopt.GetoptError:
	sys.exit(2)
if not opts : 
	print('Missing command arguments')
	print('json2txt.py -i <modelinputfile> -o <outputfile>')
	print('-i : [REQUIRED] Name of the model input file')
	print('-o : [OPTIONAL] text output file name.')
for opt, arg in opts:
	if opt == '-h':
		sys.exit()
	elif opt in ("-i", "--ifile"):
		filename_json = arg
	elif opt in ("-o", "--ofile"):
		filename_txt = arg

with open(filename_json,"r") as f :
	data = json.load(f)
	assert(data['application']=='gridlabd')
	assert(data['version'] >= '4.2.0')

with open(filename_txt, "w") as f : 
	f.write("ERROR: json to txt conversion not implemented")
