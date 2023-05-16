import json 
import os 
import sys, getopt
import sqlite3

filename_json = 'gridlabd.json'
filename_db = ''
try : 
	opts, args = getopt.getopt(sys.argv[1:],"hi:o:",["ifile=","ofile="])
except getopt.GetoptError:
	sys.exit(2)
# if not opts : 
# 	print('Missing command arguments')
# 	print('json2db.py -i <modelinputfile> -o <outputfile>')
# 	print('-i : [REQUIRED] json input file name.')
# 	print('-o : [OPTIONAL] text output file name.')
for opt, arg in opts:
	if opt == '-h':
		sys.exit()
	elif opt in ("-i", "--ifile"):
		filename_json = arg
	elif opt in ("-o", "--ofile"):
		filename_db = arg

# set default output filename
if filename_db == '':
	if filename_json[-5:] == ".json":
		filename_db = filename_json[:-5] + ".db"
	else: 
		filename_db = filename_json + ".db"

# load json file
with open(filename_json,"r") as f :
	model = json.load(f)
	assert(model['application']=='gridlabd')
	assert(model['version'] >= '4.2.0')

# create db
os.remove(filename_db)
db = sqlite3.connect(filename_db)

# register converters
typemap = {}
for datatype in model['types']:
	if datatype in ["double","int16","int32","int64",]:
		typemap[datatype] = datatype
	else:
		typemap[datatype] = "text"


# create tables
for classname, properties in model["classes"].items():
	if len([x for x,y in properties.items() if type(y) is dict]) > 0:
		query = f"create table {classname} (\n  "
		query += ",\n  ".join([f"'{x.replace('.','_')}' {typemap[y['type']]}" for x,y in model['header'].items() if type(y) is dict])
		query += ",\n  ".join([f"'{x.replace('.','_')}' {typemap[y['type']]}" for x,y in properties.items() if type(y) is dict])
		query += "\n  );"
		print(query)
		db.execute(query)

# for