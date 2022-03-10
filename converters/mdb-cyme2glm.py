"""CYME 'mdb' to GridLAB-D 'glm' converter


"""
import sys, os
from copy import copy
from collections import OrderedDict
import datetime as dt
import math

# TODO delete uses of print and pprint when done debugging
import pprint
pprint = pprint.PrettyPrinter(indent=4).pprint

class config:
	nominal_voltage_name = 'CYMNOMINALVOLTAGE'
	default_nominal_voltage = "2401.778 V"
	annotated_model = False # set to False to omit tagged comments
	issue_tag = '// issue:'
	table_tag = '// table:'
	field_tag = '// field:'
	value_tag = '// value:'
	phase_tag = '// phase_%s:'
	swing_tag = '// swing:'
	solver_method = None

# map CYME phases to GLM phases
cyme_phase_map = {
		1:'A',
		2:'B',
		3:'C',
		4:'AB',
		5:'AC',
		6:'BC',
		7:'ABC',
		}

# nominal voltage tracking
nominal_voltage = None

def safename(name):
	"""Make an object name safe for use by GridLAB-D"""
	name = str(name)
	if name[0] >= '0' and name[0] <= '9':
		return '_'+name
	else:
		return name

def reindex_to_dict(df,name,index,orient='index'):
	"""Reindex a multi-index dataframe into a nested dict"""
	if type(df) is dict:
		keys = df.keys()
		data = df[name]
	else:
		keys = df.index.unique()
		data = df.loc[(name)]
	if name in keys:
		data = data.reset_index().set_index(index[0])
		if len(index) == 1:
			return data.to_dict(orient=orient)
		else:
			result = {}
			for key in data.index.unique():
				result[key] = reindex_to_dict(data,key,index[1:],orient=orient)
			return result
	else:
		return {}

def new_object(class_name,object_name):
	"""Construct a new object"""
	obj = OrderedDict()
	obj['class'] = class_name
	obj['name'] = object_name
	return obj

def comment(obj,table_name,data,phase=None):
	"""Add the table data as a comment in an object"""
	if config.annotated_model:
		obj[config.table_tag] = table_name
		obj[config.field_tag] = ','.join(list(map(lambda x:str(x),data.keys())))
		if phase == None:
			obj[config.value_tag] = ','.join(list(map(lambda x:str(x),data.values())))
		else:
			obj[config.phase_tag%(phase)] = ','.join(list(map(lambda x:str(x),data.values())))

#
# MODEL
#
class Model:
	
	def __init__(self,input_file=None):
		"""Initialize a Model

		Parameters:
			input_file (str)	the MDB file name to load (default None)
		"""
		self.modules = {}
		self.globals = {}
		self.defines = {}
		self.objects = {}
		self.connect = {}
		self.issues = {}
		if input_file:
			self.load(input_file)
		self.output_file = None

	def warning(self,msg):
		"""Display a warning message in GridLAB-D style"""
		print(f'WARNING [{self.input_file}]: {msg}',file=sys.stderr)

	def error(self,msg):
		"""Display an error message in GridLAB-D style"""
		print(f'ERROR [{self.input_file}]: {msg}',file=sys.stderr)

	def exception(self,msg):
		"""Raise an exception in GridLAB-D style"""
		raise Exception(f'EXCEPTION [{self.input_file}]: {msg}')

	def require(self,module):
		"""Require a GridLAB-D module"""
		if not module in self.modules:
			self.modules[module] = {}

	def load(self,input_file):
		"""Load CYME model"""
		import importlib
		importlib.util.spec_from_file_location('cyme', sys.argv[0].replace('mdb-cyme2glm','cyme'));
		cyme = importlib.import_module('cyme');		
		data = cyme.dataframe(input_file)
		self.input_file = input_file
		self.set_define('CYMANNOTATEDMODEL',str(config.annotated_model).upper())
		self.set_version(data)
		self.add_globals(data)
		self.require('powerflow')
		self.add_networks(data)
		# nodes
		self.add_capacitors(data)
		self.add_loads(data)
		self.add_nodes(data)
		# links
		self.add_lines(data)
		self.add_links(data)
		# set solver method
		self.set_issuelist(data)
		self.set_solvermethod(data)

	#
	# SET
	#
	def set_version(self,data):
		if 'CYMSCHEMAVERSION' in data.keys():
			self.version = data['CYMSCHEMAVERSION'].loc[0]['Version']
			self.set_define('CYMSCHEMAVERSION',self.version)
		else:
			self.warning("unknown schema version")

	def set_define(self,name,value):
		"""Define a GLM global (must not be defined)"""
		self.defines[name] = value

	def set_global(self,name,value,module=None):
		"""Set a GLM global (must already be defined)"""
		if module == None:
			self.globals[name] = value
		else:
			self.modules[module][name] = value

	def set_issuelist(self,data):
		"""Set the CYMISSUES global from the issues found list"""
		self.set_define('CYMISSUES',' '.join(list(self.issues.keys())))

	def set_solvermethod(self,data):
		"""Set the solver method based on the graph structure

		If the solver method is not already specified and the network model 
		contains at least one loop, the solver method is set to 'NR' otherwise 
		it is set to 'FBS'."""
		if not config.solver_method:
			config.solver_method = 'FBS'
			def set_swing(model,network,obj_name,swingbus=None):
				"""Sets the swing bus and counts the number of loops below"""
				if swingbus == None:
					swingbus = obj_name
				if not obj_name in model.objects[network]:
					model.warning(f"graph of network {network} references invalid object {obj_name}")
					return 0
				obj = model.objects[network][obj_name]
				result = 0
				if not config.swing_tag in obj.keys():
					obj[config.swing_tag] = swingbus
				elif not obj[config.swing_tag] == swingbus:
					model.error(f"object {obj_name} swing bus conflict, trying to set swing to '{swingbus}' when it's already set to '{obj[config.swing_tag]}'")
				else:
					result = 1
				for node in list(model.graph[obj_name]):
					result += set_swing(model,network,node,swingbus)
				return result
			for network in self.networks:
				self.graph = self.graph_connect(network)
				swing_bus = safename(self.headnodes[network]['NodeId'])
				swing_obj = self.objects[network][swing_bus]['name']
				if set_swing(self,network,swing_obj) > 0:
					config.solver_method = 'NR'
		self.set_global('solver_method',config.solver_method,'powerflow')

	#
	# GRAPH
	#

	def graph_connect(self,network):
		"""Graph the connections in a network

		Returns the dict of nodes that are connected to other nodes
		"""
		if not network in self.connect.keys():

			# first time requires construction of connectivity graph
			objects = self.objects[network]
			graph = {}
			for name, obj in objects.items():
				if obj['class'][0:10] == 'powerflow.' and 'from' in obj.keys() and 'to' in obj.keys():
					from_name = obj['from']
					to_name = obj['to']
					# print(f'Processing object {name}: {from_name} --> {to_name}')
					if from_name in objects.keys() and objects[to_name]['class'][0:10] == 'powerflow.':
						# if to_name in self.issues.keys():
						# 	self.warning(f'object {name} from node {from_name} has an issue: {self.issues[to_name]}')
						if not from_name in graph.keys():
							graph[from_name] = set((to_name))
						else:
							graph[from_name].add(to_name)
					else:
						self.warning(f'object {name} from node {from_name} is not a valid powerflow node')
					if to_name in objects.keys() and objects[from_name]['class'][0:10] == 'powerflow.':
						# if from_name in self.issues.keys():
						# 	self.warning(f'object {name} from node {to_name} has an issue: {self.issues[from_name]}')
						if not to_name in graph.keys():
							graph[to_name] = set((from_name))
						else:
							graph[to_name].add(from_name)
					else:
						self.warning(f'object {name} to node {to_name} is not a valid powerflow node')
			self.connect[network] = graph
			# pprint(self.connect[network])
		return self.connect[network]

	#
	# ADD
	#
	def add_globals(self,data):
		"""Add standard globals to model"""
		self.set_define('CYMMODELNAME',self.input_file)

	def add_object(self,network,obj):
		"""Add object to model"""
		global nominal_voltage
		name = obj['name']
		if not nominal_voltage and 'nominal_voltage' in obj.keys():
			nominal_voltage = obj['nominal_voltage']
		self.objects[network][name] = obj
		if name in self.nodes[network].keys():
			self.nodes[network].pop(name,None)
		if name in self.links[network].keys():
			self.links[network].pop(name,None)

	def add_networks(self,data):
		"""Add networks to model"""
		self.networks = list(reindex_to_dict(data,'CYMNETWORK',['NetworkId']).keys())
		self.set_define('CYMNETWORKS',' '.join(list(map(lambda x:str(x),self.networks))))
		for network in self.networks:
			self.objects[network] = {}
		self.headnodes = reindex_to_dict(data,'CYMHEADNODE',['NetworkId'])
		for network, headnode in self.headnodes.items():
			self.set_define(f'CYMHEADNODE_{network}',safename(headnode['NodeId']))
		self.nodes = reindex_to_dict(data,'CYMNODE',['NetworkId','NodeId'])
		self.links = reindex_to_dict(data,'CYMSECTION',['NetworkId','SectionId'])

	def add_nodes(self,data):
		"""Add nodes to model

		Nodes are removed by add_<type>() methods as objects are processed. 
		This call tracks nodes that have not been picked up by other parts of 
		the model and reports them as 'unprocessed node' in the issue list. 
		These are most likely to cause solver errors or incorrect solutions.
		"""
		for network, nodes in self.nodes.items():
			for name, data in copy(nodes).items():
				obj = new_object(class_name='powerflow.node',object_name=safename(name))
				comment(obj,'CYMNODE',data)
				obj['nominal_voltage'] = '${%s}'%(config.nominal_voltage_name)
				obj['phases'] = 'ABC'
				self.add_issue(obj,'unprocessed node')
				self.add_object(network,obj)
			self.nodes[network] = {}

	def add_issue(self,obj,issue):
		"""Add an issue to the model's issue list

		Parameters:
			obj (str)	object name
			issue (str)	issue description

		If 'config.annotated_model' is True, the issue will also be
		added to the object's issue list
		"""
		if config.annotated_model:
			if config.issue_tag not in obj.keys():
				obj[config.issue_tag] = []
			obj[config.issue_tag].append(issue)
		self.issues[obj['name']] = issue
		self.warning(f"object {obj['name']} -- {issue}")

	def add_links(self,data):
		"""Add links to model

		Sections are removed by add_<type>() methods as objects are processed.
		This call adds nodes that have not been picked up by other parts of 
		the model. These will most likely cause solver errors.
		"""
		for network, links in self.links.items():
			for name, section in copy(links).items():
				if safename(section['ToNodeId']) == name:
					if name in self.objects.keys():
						# special case for links that refer to a node by the same name
						obj[name][parent] = safename(section['FromNodeId'])
					else:
						# special for link that should be a node
						obj = new_object(class_name='powerflow.node',object_name=safename(section['FromNodeId']))
						comment(obj,'CYMSECTION',section)
						obj['nominal_voltage'] = '${%s}'%(config.nominal_voltage_name)
						obj['phases'] = cyme_phase_map[section['Phase']]
						self.add_object(network,obj)
				else:
					obj = new_object(class_name='powerflow.link',object_name=safename(name))
					comment(obj,'CYMSECTION',section)
					obj['nominal_voltage'] = '${%s}'%(config.nominal_voltage_name)
					obj['phases'] = cyme_phase_map[section['Phase']]
					obj['from'] = safename(section['FromNodeId'])
					obj['to'] = safename(section['ToNodeId'])
					if not obj['from'] in self.objects[network].keys():
						self.warning(f'ignoring object {name} from node {obj["from"]} not found')
					elif not obj['to'] in self.objects[network].keys():
						self.warning(f'ignoring object {name} to node {obj["to"]} not found')
					else:
						self.add_object(network,obj)
			self.links[network] = {}

	def add_lines(self,data):
		"""Add overhead lines to model"""
		self.warning('overheads not fully implemented yet')
		sections = reindex_to_dict(data,'CYMSECTION',['SectionId'])
		count = 0
		for name, line_data in sections.items():
			network = line_data['NetworkId']			
			obj = new_object(class_name='powerflow.overhead_line',object_name=safename(name))
			comment(obj,'CYMSECTION',line_data)
			obj['nominal_voltage'] = '${%s}'%(config.nominal_voltage_name)
			obj['phases'] = cyme_phase_map[line_data['Phase']]
			obj['from'] = safename(line_data['FromNodeId'])
			obj['to'] = safename(line_data['ToNodeId'])
			self.add_object(network,obj)
			count += 1
		self.set_define('CYMSECTION',count)

	def add_capacitors(self,data):
		"""Add capacitors to model"""
		caps = reindex_to_dict(data,'CYMSHUNTCAPACITOR',['DeviceNumber'])
		if not caps:
			return
		count = 0
		for name, cap_data in caps.items():
			network = cap_data['NetworkId']
			obj = new_object(class_name='powerflow.capacitor',object_name=safename(name))
			comment(obj,'CYMSHUNTCAPACITOR',cap_data)
			obj['nominal_voltage'] = f'{cap_data["KVLN"]} kV'
			if 'Phases' in cap_data.keys():
				phases = cyme_phase_map[cap_data['Phase']]
				obj['phases'] = phases
			else:
				phases = []
			if 'A' in phases or cap_data['KVARA'] != '0':
				obj['capacitor_A'] = f'{cap_data["KVARA"]} kVA'
			if 'B' in phases or cap_data['KVARB'] != '0':
				obj['capacitor_B'] = f'{cap_data["KVARB"]} kVA'
			if 'C' in phases or cap_data['KVARC'] != '0':
				obj['capacitor_C'] = f'{cap_data["KVARC"]} kVA'
			obj['phases_connected'] = phases
			self.add_object(network,obj)
			count += 1
		self.set_define('CYMCAPACITORCOUNT',count)

	def add_loads(self,data):
		"""Add load objects to model"""
		load_classes = reindex_to_dict(data,'CYMCONSUMERCLASS',['ConsumerClassId'])
		loads = reindex_to_dict(data,'CYMCUSTOMERLOAD',['DeviceNumber','Phase'])
		if not loads:
			return
		count = 0
		for name, load_data in loads.items():
			obj = new_object(class_name='powerflow.load',object_name=safename(name))
			obj['nominal_voltage'] = '${%s}'%(config.nominal_voltage_name)
			phases = ''
			for phase, load in load_data.items():
				network = load['NetworkId']
				load_phase = cyme_phase_map[phase]
				load_type = load['ConsumerClassId']
				load_value1 = float(load['LoadValue1'])
				load_value2 = float(load['LoadValue2'])
				load_magnitude = math.sqrt(load_value1*load_value1+load_value2*load_value2)
				load_class = load_classes[load_type]
				power_fraction = float(load_class['ConstantPower'])/100
				current_fraction = float(load_class['ConstantCurrent'])/100
				impedance_fraction = float(load_class['ConstantImpedance'])/100
				power_factor = float(load_class['PowerFactor'])/100
				ZP = float(load_class['ConstantImpedanceZ_P'])/100
				ZQ = float(load_class['ConstantImpedanceZ_Q'])/100
				IP = float(load_class['ConstantCurrentI_P'])/100
				IQ = float(load_class['ConstantCurrentI_Q'])/100
				PP = float(load_class['ConstantPowerP_P'])/100
				PQ = float(load_class['ConstantPowerP_Q'])/100
				if load_phase == 'ABC': # balanced load
					comment(obj,'CYMCUSTOMERLOAD',load,phase=load_phase)
					obj['base_power_A'] = f'{load_magnitude/3} kW'
					obj['base_power_B'] = f'{load_magnitude/3} kW'
					obj['base_power_C'] = f'{load_magnitude/3} kW'
					if power_fraction > 0:
						obj['power_fraction_A'] = f'{power_fraction} pu'
						obj['power_fraction_B'] = f'{power_fraction} pu'
						obj['power_fraction_C'] = f'{power_fraction} pu'
						obj['power_pf_A'] = f'{power_factor} pu'
						obj['power_pf_B'] = f'{power_factor} pu'
						obj['power_pf_C'] = f'{power_factor} pu'
					if current_faction > 0:
						obj['current_fraction_A'] = f'{current_fraction} pu'
						obj['current_fraction_B'] = f'{current_fraction} pu'
						obj['current_fraction_C'] = f'{current_fraction} pu'
						obj['current_pf_A'] = f'{power_factor} pu'
						obj['current_pf_B'] = f'{power_factor} pu'
						obj['current_pf_C'] = f'{power_factor} pu'
					if impedance_fraction > 0:
						obj['impedance_fraction_A'] = f'{impedance_fraction} pu'
						obj['impedance_fraction_B'] = f'{impedance_fraction} pu'
						obj['impedance_fraction_C'] = f'{impedance_fraction} pu'
						obj['impedance_pf_A'] = f'{power_factor} pu'
						obj['impedance_pf_B'] = f'{power_factor} pu'
						obj['impedance_pf_C'] = f'{power_factor} pu'
					phases += load_phase
				elif load_magnitude > 0: # unbalanced non-trivial load
					comment(obj,'CYMCUSTOMERLOAD',load,phase=load_phase)
					obj[f'base_power_{cyme_phase_map[phase]}'] = f'{load_magnitude} kW'
					if power_fraction > 0:
						obj[f'power_fraction_{cyme_phase_map[phase]}'] = f'{power_fraction} pu'
						obj[f'power_pf_{cyme_phase_map[phase]}'] = f'{power_factor} pu'
					if current_fraction > 0:
						obj[f'current_fraction_{cyme_phase_map[phase]}'] = f'{current_fraction} pu'
						obj[f'pcurrent_pf_{cyme_phase_map[phase]}'] = f'{current_factor} pu'
					if impedance_fraction > 0:
						obj[f'impedance_fraction_{cyme_phase_map[phase]}'] = f'{impedance_fraction} pu'
						obj[f'impedance_pf_{cyme_phase_map[phase]}'] = f'{impedance_factor} pu'
					phases += load_phase
			obj['phases'] = ''.join(sorted(set(phases)))
			self.add_object(network,obj)
			count += 1
		self.set_define('CYMLOADCOUNT',count)

	#
	# EXPORT
	#
	def export_glm(self,output_file=None):
		"""Export model to GLM"""
		if output_file == None:
			output_file = self.input_file.replace('.mdb','.glm')
		self.output_file = output_file
		with open(output_file,'w') as fh:
			self.export_glm_header(fh)
			self.export_glm_defines(fh)
			self.export_glm_globals(fh)
			self.export_glm_modules(fh)
			self.export_glm_objects(fh)
			self.export_glm_footer(fh)

	def export_glm_header(self,fh):
		"""Export GLM header section"""
		modify_file = self.output_file.replace(".glm","-modify.glm")
		fh.write(f'// CYME {self.input_file} converted to GLM\n//\n')
		fh.write(f'// Date: {dt.datetime.now()}\n')
		fh.write(f'// User: {os.getenv("USER")}\n')
		fh.write(f'// Workdir: {os.getenv("PWD")}\n//\n\n')

	def export_glm_modules(self,fh):
		"""Export GLM modules section"""
		fh.write('\n//\n// MODULES\n//\n')
		for mod, varlist in self.modules.items():
			fh.write(f'module {mod}\n')
			fh.write('{\n')
			for var,value in varlist.items():
				fh.write(f'\t{var} {value};\n')
			fh.write('}\n')

	def export_glm_defines(self,fh):
		"""Export GLM defines section"""
		fh.write('\n//\n// DEFINES\n//\n')
		for var, value in self.defines.items():
			if len(var.split('::')) == 1:
				fh.write(f'#define {var}={value}\n')

	def export_glm_globals(self,fh):
		"""Export GLM globals section"""
		fh.write('\n//\n// GLOBALS\n//\n')
		for var, value in self.globals.items():
			if len(var.split('::')) == 1:
				fh.write(f'#set {var}={value}\n')

	def export_glm_objects(self,fh):
		"""Export GLM objects section"""
		for network in self.networks:
			fh.write(f'\n//\n// NETWORK {network}\n//\n')
			headnode = self.headnodes[network]
			for obj, props in self.objects[network].items():
				if obj == safename(headnode['NodeId']):
					props['bustype'] = 'SWING'
				classname = props['class']
				fh.write(f'object {classname}\n')
				fh.write('{\n')
				for name, value in props.items():
					if name == 'class':
						continue
					fh.write(f'\t{name} "{value}";\n')
					if name == 'name':
						fh.write(f'\tgroupid "{network}";\n')
				fh.write('}\n')

	def export_glm_footer(self,fh):
		"""Export GLM footer section

		This section include an attempt to load a file names
		${modelname}-modify.glm if it exists.  This file can be
		used to apply modifications to the model if desired.
		"""
		fh.write('\n#ifexist "${modelname/.glm/-modify.glm}"\n')
		fh.write('#include "${modelname/.glm/-modify.glm}"\n')
		fh.write('#endif\n')
		fh.write(f'\n// END OF FILE\n')


def convert(input_file,output_file=None,options={}):
	"""Convert a CYME MDB file to GLM

	Parameters:
		input_file (str)	input MDB file name
		output_file (str)	output GLM file name
		options (dict)		options to define as globals in model
	"""
	model = Model(input_file)
	if nominal_voltage:
		model.set_define(config.nominal_voltage_name,nominal_voltage)
	else:
		model.set_define(config.nominal_voltage_name,config.default_nominal_voltage)
	if options:
		for var, value in options.items():
			model.set_define(var,value)
	model.export_glm(output_file)

if __name__ == '__main__':
	testfile = 'autotest/IEEE-123-cyme.mdb'
	if os.path.exists(testfile):
		config.annotated_model = True
		convert(testfile)
