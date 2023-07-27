# Syntax: group -i=INPUT [-o=OUTPUT] [OPTIONS ...]
"""Set group according to powerflow islanding

Syntax: group -i=INPUT [-o=OUTPUT] [OPTIONS ...]

Options
-------
    -b|--by=GROUPER     specify grouping (default is 'island')
    -c|--cut=CLASSES    command-separated list classes of cut objects (default is 'switch')
    -i|--input=INPUT    input JSON file name
    -o|--output=OUTPUT  output file name (JSON or GLM)
    -m|--modify	        output GLM modify statements instead of full modify
    -f|--force	        force overwrite of existing groupid data

The `group` tools identify groups of object based on a grouper method and a
cut criteria. The only grouper currently supported is the `island` grouper
with `switch` cut object criteria. Additional cut classes can be specified
using a comma-separated list of class names, e.g., `--cut=switch,relay,breaker`.

The input file must a GridLAB-D JSON file. You can use the `gridlabd -o|--output FILENAME.json` 
optionto generate a JSON file from a GLM model.  The output file may be a JSON or GLM file, 
depending on the use of the `-m|--modify` option. When `modify` is not used, the output file
format is JSON. When `modify` is used, the output file format is GLM.

The `-f|--force` option is used to overwrite any existing `groupid` values found in the model.
"""

import sys, os
import json

class GroupException(Exception):
	pass

MODIFY = False
FORCE = True
DEBUG = False
WARNING = True
GROUPER = 'island'
CUTOBJECTS = 'switch'
INPUT = 'gridlabd.json'
OUTPUT = 'gridlabd.glm'

E_OK = 0
E_INVALID = 1
E_FAILED = 2
E_SYNTAX = 9

def error(msg,code=None):
	if not QUIET:
		print(f"ERROR [groupid]: {msg}",file=sys.stderr)
	if DEBUG:
		raise GroupException(msg)
	if type(code) is int:
		exit(code)
	elif not code is None:
		raise GroupException(f"{code} is an invalid error code")

def warning(msg):
	if WARNING:
		print(f"WARNING [groupid]: {msg}",file=sys.stderr)

def debug(msg):
	if DEBUG:
		print(f"DEBUG [groupid]: {msg}",file=sys.stderr)

def grouper_island(input=None):

	if input is None:
		input = INPUT

	#
	# Load model
	#
	with open(input,"r") as fh:
		model = json.load(fh)

	#
	# Check groupid
	#
	if not FORCE:
		for obj,data in model['objects'].items():
			if 'groupid' in data and data['groupid']:
				raise GroupException(f"{obj}.groupid='{data['groupid']}' (use --force to overwrite)")

	# 
	# Find swing buses and build network graph
	#
	swing_buses = []
	groupid = 1
	links = {}
	nodes = {}
	for obj,data in model['objects'].items():
		if 'bustype' in data and data['bustype'] == "SWING":
			swing_buses.append(obj)
			model['objects'][obj]['groupid'] = f"{GROUPER}_{groupid}"
			groupid += 1
		else:
			model['objects'][obj]['groupid'] = None
		if 'from' in data or 'to' in data:
			from_node = data['from']
			to_node = data['to']
			links[obj] = [from_node,to_node]
			if not from_node in nodes:
				nodes[from_node] = [obj]
			elif obj not in nodes[from_node]:
				nodes[from_node].append(obj)
			if not to_node in nodes:
				nodes[to_node] = [obj]
			elif obj not in nodes[to_node]:
				nodes[to_node].append(obj)

	#
	# Process each swing_bus
	#
	def group(bus):
		groupid = model['objects'][bus]['groupid']
		for link in nodes[bus]:
			link_data = model['objects'][link]
			if not link_data['class'] in CUTOBJECTS:
				debug(f"tagging link '{link}' from '{bus}' as '{groupid}'")
				model['objects'][link]['groupid'] = groupid
				for node in links[link]:
					if model['objects'][node]['groupid'] is None:
						debug(f"tagging node '{node}' from '{link}' as '{groupid}'")
						model['objects'][node]['groupid'] = groupid
						group(node)
			else:
				model['objects'][link]['groupid'] = 0
				debug(f"tagging control '{link}' from '{bus}' as '0'")
	for bus in swing_buses:
		group(bus)

	#
	# Recursively tag objects
	#
	for obj,data in model['objects'].items():
		if 'bustype' in data and data['groupid'] is None:
			if 'parent' not in data:
				model['objects'][obj]['groupid'] = f'{GROUPER}_{groupid}'
				group(obj)
				warning(f"group '{data['groupid']}' does not have a swing bus")
				groupid += 1
				# del model['objects'][obj]['groupid']
			else:
				model['objects'][data['parent']]['groupid'] = model['objects'][obj]['groupid']
		elif 'from' in data and 'to' in data and data['groupid'] is None:
			if data['groupid'] == 0:
				from_node = model['objects'][obj]['from']
				to_node = model['objects'][obj]['to']
				from_group = model['objects'][from_node]['groupid']
				to_group = model['objects'][to_node]['groupid']
				groupid = model['objects'][obj] = f"control_{from_group}_{to_group}"
				debug(f"tagging link {link} from {bus} as {groupid}")
			else:
				warning(f"link '{obj}' was not tagged")
				del model['objects'][obj]['groupid']
	return model

#
# Process argument list
#
if len(sys.argv) == 1:
	for line in __doc__.split('\n'):
		if line.startswith('Syntax: '):
			print(line)
			exit(E_SYNTAX)
for arg in sys.argv[1:]:
	token,value = (arg.split('=')[0],'='.join(arg.split('=')[1:]) if '=' in arg else None)
	if token in ['-h','--help','help']:
		print(__doc__)
		exit(E_OK)
	# -b|--by=GROUPER 	specify grouping (default is 'island')
	elif token in ['-b','--by']:
		GROUPER = value
	# -c|--cut=CUTOBJECTS
	elif token in ['-c','--cut']:
		CUTOBJECTS = value.split(',') if ',' in value else [value]
	# -i|--input=INPUT 	input JSON file name
	elif token in ['-i','--input']:
		INPUT = value
	# -o|--output=OUTPUT  output file name (JSON or GLM)
	elif token in ['-o','--output']:
		OUTPUT = value
	# -m|--modify			output GLM modify statements instead of full modify
	elif token in ['-m','--modify']:
		MODIFY = True
	# -f|--force			force overwrite of existing groupid data
	elif token in ['-f','--force']:
		FORCE = True

#
# Process commands
#
if __name__ == "__main__":

	if "grouper_"+GROUPER not in globals():
		error(f"grouper '{GROUPER}' is invalid")
		E_INVALID
	model = globals()["grouper_"+GROUPER]()
	with open(OUTPUT,"w") as fh:
		if MODIFY:
			for obj,data in model['objects'].items():
				print(f"modify {obj}.groupid '{data['groupid']}';",file=fh)
		else:
			json.dump(model,fh,indent=4)

