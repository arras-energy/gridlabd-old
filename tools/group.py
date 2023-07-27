# Syntax: group -i=INPUT [-o=OUTPUT] [OPTIONS ...]
"""Set group according to powerflow islanding

Options
-------

	-i|--input=INPUT 	input JSON file name
	-o|--output=OUTPUT  output file name (JSON or GLM)
	-m|--modify			output GLM modify statements instead of full modify
	-f|--force			force overwrite of existing groupid data
"""

import sys, os
import json

class GroupidException(Exception):
	pass

MODIFY = False
FORCE = True

def warning(msg):
	print(f"WARNING [groupid]: {msg}",file=sys.stderr)

def debug(msg):
	print(f"DEBUG [groupid]: {msg}",file=sys.stderr)

#
# Load model
#
with open("gridlabd.json","r") as fh:
	model = json.load(fh)

#
# Check groupid
#
if not FORCE:
	for obj,data in model['objects'].items():
		if 'groupid' in data and data['groupid']:
			raise GroupidException(f"{obj}.groupid='{data['groupid']}' (use --force to overwrite)")

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
		model['objects'][obj]['groupid'] = f"island_{groupid}"
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
	global model
	groupid = model['objects'][bus]['groupid']
	for link in nodes[bus]:
		link_data = model['objects'][link]
		if not link_data['class'] in ['switch','recloser','relay','fuse','breaker']:
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
			warning(f"node '{obj}' is not connected to network")
			del model['objects'][obj]['groupid']
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
with open("output.json","w") as fh:
	json.dump(model,fh,indent=4)

