import json 
import os 
import sys, getopt
import sqlite3

def error(msg,code=None):
	print(f"ERROR [json2db]: {msg}",file=sys.stderr)

def warning(msg):
	print(f"WARNING [json2db]: {msg}",file=sys.stderr)

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
if os.path.exists(filename_db):
	os.remove(filename_db)
db = sqlite3.connect(filename_db)

# register converters
typemap = {}
for datatype in model['types']:
	if datatype in ["double","int16","int32","int64",]:
		typemap[datatype] = datatype
	else:
		typemap[datatype] = "text"

# create gridlabd tables
db.execute("""CREATE TABLE gridlabd_modules (
	name text,
	major int,
	minor int
	);""")
for name,info in model['modules'].items():
	db.execute(f"""INSERT INTO gridlabd_modules (name,major,minor) VALUES ('{name}','{info['major']}','{info['minor']}');""")

db.execute("""CREATE TABLE gridlabd_globals (
	name text,
	type text,
	access text,
	value text);""")
for name,info in model['globals'].items():
	db.execute(f"""INSERT INTO gridlabd_globals (name,type,access,value) VALUES ('{name}','{info['type']}','{info['access']}','{info['value']}')""")

# db.execute("""CREATE TABLE gridlabd_schedule ()""")
if "schedule" in model and model["schedule"]:
	warning("schedules not exported")

# db.execute("""CREATE TABLE gridlabd_filter ()""")
if "filter" in model and model["filter"]:
	warning("filters not exported")

# create object tables
def get_properties(model,classname):
	result = get_properties(model,model["classes"][classname]["parent"]) if "parent" in model["classes"][classname] else []
	properties = {}
	for name,data in list(model["classes"][classname].items()):
		if name.lower() in [x.lower() for x in properties]:
			warning(f"'{classname}.{name}' is a case-insensitive duplicate name and cannot be converted to SQL")
		else:
			properties[name] = data
	result.extend([f"'{x.replace('.','_')}' {typemap[y['type']]}" for x,y in properties.items() if type(y) is dict])
	return result

for classname, properties in model["classes"].items():
	if len([x for x,y in properties.items() if type(y) is dict]) > 0:
		query = f"create table {classname} (\n  "
		query += ",\n  ".join([f"'{x.replace('.','_')}' {typemap[y['type']]}" for x,y in model['header'].items()]) + ",\n  "
		query += ",\n  ".join(get_properties(model,classname))
		query += "\n  );"
		# print(query)
		db.execute(query)

for name, properties in model["objects"].items():
	classname = properties["class"]
	query = f"""INSERT INTO {classname} ("{'","'.join(properties.keys()).replace('.','_')}") VALUES ("{'","'.join(properties.values())}");"""
	# print(query)
	db.execute(query)

db.commit()
