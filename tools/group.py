# Syntax: group -i=INPUT [-o=OUTPUT] [OPTIONS ...]
"""Set group according to powerflow islanding

Syntax: group [-i=INPUT] [-o=OUTPUT] [OPTIONS ...]

Options
-------

    -b|--by=GROUPER     specify grouping (default is 'island')

    -c|--cut=CLASSES    command-separated list classes of cut objects
                        (default is 'switch')

    --control=NAME      set control groupid (default "control_")

    --copy_from=CLASS.PROPERTY,... specify CLASS.PROPERTY from which groupid
                        is copied (default is `pole_mount.equipment`)

    --copy_to=CLASS.PROPERTY,... specify CLASS.PROPERTY to which groupid is
                        copied (default is `pole_mount.parent`)

    -d|--debug          enable debugging output and exception handling

    -f|--force	        force overwrite of existing groupid data

    -i|--input=INPUT    input JSON file name (default is stdin)

    -m|--modify	        output GLM modify statements instead of full modify

    -o|--output=OUTPUT  output file name (JSON or GLM, default is stdout)

    -p|--prefix=PREFIX  set the groupid prefix (default "island_")

    -q|--quiet          disable error output

    -w|--warning        disable warning output

The `group` tools identify groups of object based on a grouper method and a
cut criteria. The only grouper currently supported is the `island` grouper
with `switch` cut object criteria. Additional cut classes can be specified
using a comma-separated list of class names, e.g.,
`--cut=switch,relay,breaker`.

The input file must a GridLAB-D JSON file. You can use the
`gridlabd -o|--output FILENAME.json` option to generate a JSON file from a
GLM model.  The output file may be a JSON or GLM file, depending on the use
of the `-m|--modify` option. When `modify` is not used, the output file
format is JSON. When `modify` is used, the output file format is GLM, and
python variables defined to provide information about the grouping. The
variable `groups` contains a dictionary of all the groups created and which
objects belong to each group.  The variable `controls` contains a dictionary
of control objects and which groups are connected by that object. The
variable `swingbus` contains a dictionary of the swing buses for each island.
The variable `isolated` contains a list of groups that do not have a swing
bus. These groups are considered isolated when they are not connected to a
group that has a swing bus.

The `-f|--force` option is used to overwrite any existing `groupid` values
found in the model.

Example:

The following example generates a GLM modify file for the IEEE 123 model based
on switch islanding.

~~~
gridlabd model get IEEE/123
gridlabd -C 123.glm -o 123.json
gridlabd group -i=123.json --modify -o=groups.glm --force
~~~
"""

import sys, os
import json
import datetime

class GroupException(Exception):
	pass

MODIFY = False
FORCE = False
DEBUG = False
WARNING = False
QUIET = False
GROUPER = 'island'
PREFIX = 'island_'
CONTROL = 'control'
CUTOBJECTS = 'switch'
INPUT = "/dev/stdin"
OUTPUT = None
COPYFROM = 'pole_mount.equipment'
COPYTO = 'pole_mount.parent'

E_OK = 0
E_INVALID = 1
E_FAILED = 2
E_MISSING = 3
E_EXCEPTION = 8
E_SYNTAX = 9

def error(msg,code=None):
	"""Output error message or raise exception

	Parameters:
	  msg (str) - error message
	  code (None, int) - termination code

	If code is None, only message is displayed. If code
	is int, `exit(code)` is called.
	"""
	if not QUIET:
		print(f"ERROR [group]: {msg}",file=sys.stderr)
	if DEBUG:
		raise GroupException(msg)
	if type(code) is int:
		exit(code)
	elif not code is None:
		raise GroupException(f"{code} is an invalid error code")

def warning(msg):
	"""Output warning message"""
	if not WARNING:
		print(f"WARNING [group]: {msg}",file=sys.stderr)

def debug(msg):
	"""Output a debugging message"""
	if DEBUG:
		print(f"DEBUG [group]: {msg}",file=sys.stderr)

def grouper_island(input=None):
	"""Group object by connectivity in powerflow solution

	Parameters:
	  input (str) - input file name

	Returns:
	  dict - processed gridlabd model
	"""
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
				error(f"{obj}.groupid='{data['groupid']}' (use --force to overwrite)",E_FAILED)

	# 
	# Find swing buses and build network graph
	#
	swing_buses = []
	groupid = 1
	links = {}
	nodes = {}
	for obj,data in model['objects'].items():
		model['objects'][obj]['groupid'] = None
		if 'bustype' in data and data['bustype'] in ["SWING","SWING_PQ"]:
				swing_buses.append(obj)
				model['objects'][obj]['groupid'] = f"{PREFIX}{groupid}"
				groupid += 1
		elif 'from' in data or 'to' in data:
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
		if not bus in nodes:
			return
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
				model['objects'][link]['groupid'] = CONTROL
				debug(f"tagging control '{link}' from '{bus}' as '{CONTROL}'")
	for bus in swing_buses:
		group(bus)

	#
	# Recursively tag objects
	#
	for obj,data in model['objects'].items():
		if 'bustype' in data and data['groupid'] is None:
			if 'parent' not in data:
				model['objects'][obj]['groupid'] = f'{PREFIX}{groupid}'
				group(obj)
				warning(f"group '{data['groupid']}' does not have a swing bus")
				groupid += 1
				# del model['objects'][obj]['groupid']
			else:
				model['objects'][data['parent']]['groupid'] = model['objects'][obj]['groupid']
		elif 'from' in data and 'to' in data and data['groupid'] is None:
			if data['groupid'] == CONTROL:
				from_node = model['objects'][obj]['from']
				to_node = model['objects'][obj]['to']
				from_group = model['objects'][from_node]['groupid']
				to_group = model['objects'][to_node]['groupid']
				groupid = model['objects'][obj] = f"control_{from_group}_{to_group}"
				debug(f"tagging link {link} from {bus} as {groupid}")
			else:
				warning(f"link '{obj}' was not tagged")
				del model['objects'][obj]['groupid']

	#
	# Process linkages and children
	#
	for obj,data in model['objects'].items():

		# copy groupid from objects
		for linkage in COPYFROM.split(','):
			classname,propname = linkage.split('.')
			if data['class'] == classname and propname in data:
				if data['groupid'] is None:
					ref_name = data[propname]
					ref = model['objects'][ref_name]
					model['objects'][obj]['groupid'] = ref['groupid']
					# print(classname,'linkage from',ref_name,'to',obj,'as',ref['groupid'])
				elif data['groupid'] != ref['groupid']:
					warning(f"{classname} link from {ref_name} {ref['groupid']} differs from {obj} {data['groupid']}")

		# copy groupid to objects
		for children in COPYTO.split(','):
			classname,propname = children.split('.')
			if data['class'] == classname:
				ref_name = data[propname]
				ref = model['objects'][ref_name]
				if ref['groupid'] is None and propname in data:
					model['objects'][ref_name]['groupid'] = data['groupid']
					# print(classname,'linkage from',ref_name,'to',obj,'as',ref['groupid'])
				elif data['groupid'] != ref['groupid'] and ref['groupid'] != CONTROL:
					warning(f"{classname} link to {ref_name} {ref['groupid']} differs from {obj} {data['groupid']}")

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

	#
	# -h|--help|help
	#
	if token in ['-h','--help','help']:
		print(__doc__)
		exit(E_OK)
	
	#
	# -b|--by=GROUPER 	specify grouping (default is 'island')
	#
	elif token in ['-b','--by']:
		GROUPER = value
	
	#
	# -c|--cut=CUTOBJECTS
	#
	elif token in ['-c','--cut']:
		CUTOBJECTS = value.split(',') if ',' in value else [value]
    
    #
    # --control=NAME      set control groupid (default "control_")
    #
	elif token in ['--control']:
		CONTROL = value
	#
    # --copy_from=CLASS.PROPERTY,... specify CLASS.PROPERTY from which groupid is copied (default 
    #                     is `pole_mount.equipment`)
	#
	elif token in ['--copy_from']:
		COPYFROM = value
    
	#
    # --copy_to=CLASS.PROPERTY,... specify CLASS.PROPERTY to which groupid is copied (default 
    #                     is `pole_mount.parent`)
	#
	elif token in ['--copy_to']:
		COPYTO = value
	
	#
	# -d|--debug
	#
	elif token in ['-d','--debug']:
		DEBUG = True
	
	#
	# -f|--force			force overwrite of existing groupid data
	#
	elif token in ['-f','--force']:
		FORCE = True
	
	#
	# -i|--input=INPUT 	input JSON file name
	#
	elif token in ['-i','--input']:
		if os.path.splitext(value)[1] in [".json"]:
			INPUT = value
		else:
			error("only JSON input files are supported",E_INVALID)
	
	#
	# -m|--modify			output GLM modify statements instead of full modify
	#
	elif token in ['-m','--modify']:
		MODIFY = True
	
	#
	# -o|--output=OUTPUT  output file name (JSON or GLM)
	#
	elif token in ['-o','--output']:
		OUTPUT = value
	
	#
    # -p|--prefix=PREFIX  set the groupid prefix (default "island_")
	#
	elif token in ['--prefix']:
		PREFIX = value

	# -q|--quiet
	#
	elif token in ['-q','--quiet']:
		QUIET = True
	
	#
	# -w|--warning
	#
	elif token in ['-w','--warning']:
		WARNING = True

	#
	# invalid arg
	#
	else:
		error(f"option {arg} is invalid",E_INVALID)

#
# Process commands
#
if __name__ == "__main__":

	EXITCODE = E_OK
	#
	# check for existence of grouper function
	#
	if "grouper_"+GROUPER not in globals():
		error(f"grouper '{GROUPER}' is invalid",E_INVALID)

	#
	# check for consistent use of modify and output file extension
	#
	if OUTPUT is None:
		OUTPUT = "/dev/stdout"
	elif MODIFY and os.path.splitext(OUTPUT)[1] != ".glm":
		error(f"modify output must be GLM",E_INVALID)
	elif not MODIFY and os.path.splitext(OUTPUT)[1] != ".json":
		error(f"model output must be JSON",E_INVALID)

	try:

		#
		# call grouper
		#
		model = globals()["grouper_"+GROUPER]()

		#
		# generate output
		#
		with open(OUTPUT,"w") as fh:
			if MODIFY:
				print(f"// generated by {' '.join(sys.argv)} at {datetime.datetime.now()}",file=fh)

				# generate python variables
				groups = {}
				controls = {}
				swingbus = {}
				isolated = []
				for obj,data in model['objects'].items():
					groupid = data['groupid']
					if groupid:
						if groupid.startswith(PREFIX):
							if not data['groupid'] in groups:
								groups[groupid] = [obj]
							else:
								groups[groupid].append(obj)
							if not groupid in isolated and not groupid in swingbus:
								isolated.append(groupid)
							if 'bustype' in data and data['bustype'] in ['SWING','SWING_PG']:
								if not obj in swingbus:
									swingbus[groupid] = [obj]
								else:
									swingbus[groupid].append(obj)
								if groupid in isolated:
									isolated.remove(groupid)
						elif groupid == CONTROL and 'from' in data and 'to' in data:
							controls[obj] = [model['objects'][data[x]]['groupid'] for x in ['from','to']]
				print(f"""#begin python
groups = {groups}
controls = {controls}
swingbus = {swingbus}
isolated = {isolated}
#end
""",file=fh)
				# output modify directives
				for obj,data in model['objects'].items():
					groupid = data['groupid']
					if groupid:
						print(f"modify {obj}.groupid '{groupid}';",file=fh)
			else:
				json.dump(model,fh,indent=4)

	except SystemExit as err:

		EXITCODE = err

	except:

		if DEBUG:
			raise
		
		e_type,e_value,e_trace = sys.exc_info()
		error(f"{e_type.__name__} {e_value} at line {e_trace.tb_lineno}",E_EXCEPTION)

	exit(EXITCODE)
