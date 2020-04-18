#
# main converter
#
import sys, os
from copy import copy
from collections import OrderedDict
from datetime import *
from math import *

# TODO delete when done debugging
import pprint
pprint = pprint.PrettyPrinter(indent=4).pprint

class config:
	original_data = False # set to False to leave original data comments out

# map CYME phases to GLM phases
phase_map = {
		1:'A',
		2:'B',
		3:'C',
		4:'AB',
		5:'AC',
		6:'BC',
		7:'ABC',
		}

nominal_voltage = None

def safename(name):
	"""Make an object name safe for use by GridLAB-D"""
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
	if config.original_data:
		obj['// table:'] = table_name
		obj['// keys:'] = ','.join(list(map(lambda x:str(x),data.keys())))
		if phase == None:
			obj['// values:'] = ','.join(list(map(lambda x:str(x),data.values())))
		else:
			obj[f'// values/{phase}:'] = ','.join(list(map(lambda x:str(x),data.values())))

#
# MODEL
#
class Model:
	
	def __init__(self,input_file=None):
		self.modules = {}
		self.globals = {}
		self.defines = {}
		self.objects = {}
		if input_file:
			self.load(input_file)
		self.output_file = None

	def warning(self,msg):
		print(f'WARNING [{self.input_file}]: {msg}',file=sys.stderr)

	def error(self,msg):
		print(f'ERROR [{self.input_file}]: {msg}',file=sys.stderr)

	def exception(self,msg):
		raise Exception(f'EXCEPTION [{self.input_file}]: {msg}')

	def require(self,module):
		if not module in self.modules:
			self.modules[module] = {}

	def load(self,input_file):
		import importlib
		importlib.util.spec_from_file_location('cyme', sys.argv[0].replace('mdb-cyme2glm','cyme'));
		cyme = importlib.import_module('cyme');		
		data = cyme.dataframe(input_file)
		self.input_file = input_file
		self.add_globals(data)
		self.require('powerflow')
		self.add_networks(data)
		# nodes
		self.add_capacitors(data)
		self.add_loads(data)
		self.add_nodes(data)
		# links
		self.add_overheads(data)
		self.add_sections(data)

	#
	# EXPORT
	#
	def export_glm(self,output_file=None):
		if output_file == None:
			output_file = self.input_file.replace('.mdb','.glm')
		with open(output_file,'w') as fh:
			self.export_glm_header(fh)
			self.export_glm_defines(fh)
			self.export_glm_globals(fh)
			self.export_glm_modules(fh)
			self.export_glm_objects(fh)
			self.export_glm_footer(fh)
			self.output_file = output_file

	def export_glm_header(self,fh):
		fh.write(f'// CYME {self.input_file} converted to GLM\n//\n')
		fh.write(f'// Date: {datetime.now()}\n')
		fh.write(f'// User: {os.getenv("USER")}\n')
		fh.write(f'// Workdir: {os.getenv("PWD")}\n//\n')

	def export_glm_modules(self,fh):
		fh.write('\n//\n// MODULES\n//\n')
		for mod, varlist in self.modules.items():
			fh.write(f'module {mod}\n')
			fh.write('{\n')
			for var,value in varlist.items():
				fh.write(f'\t{var} {value};\n')
			fh.write('}\n')

	def export_glm_defines(self,fh):
		fh.write('\n//\n// DEFINES\n//\n')
		for var, value in self.defines.items():
			if len(var.split('::')) == 1:
				fh.write(f'#define {var}={value}\n')

	def export_glm_globals(self,fh):
		fh.write('\n//\n// GLOBALS\n//\n')
		for var, value in self.globals.items():
			if len(var.split('::')) == 1:
				fh.write(f'#set {var}={value}\n')

	def export_glm_objects(self,fh):
		for network in self.networks:
			fh.write(f'\n//\n// NETWORK {network}\n//\n')
			for obj, props in self.objects[network].items():
				classname = props['class']
				fh.write(f'object {classname}\n')
				fh.write('{\n')
				for name, value in props.items():
					if name == 'class':
						continue
					fh.write(f'\t{name} "{value}";\n')
				fh.write('}\n')

	def export_glm_footer(self,fh):
		fh.write(f'\n// END OF FILE\n')

	#
	# SET
	#
	def set_define(self,name,value):
		self.defines[name] = value

	def set_global(self,name,value,module=None):
		if module == None:
			self.globals[name] = value
		else:
			self.modules[module][name] = value

	#
	# ADD
	#
	def add_globals(self,data):
		"""Add standard globals to model"""
		self.set_define('CYMMODELNAME',self.input_file)
		if 'CYMSCHEMAVERSION' in data.keys():
			self.set_define('CYMSCHEMAVERSION',data['CYMSCHEMAVERSION'].loc[0]['Version'])

	def add_object(self,network,obj):
		global nominal_voltage
		name = obj['name']
		if not nominal_voltage and 'nominal_voltage' in obj.keys():
			nominal_voltage = obj['nominal_voltage']
		self.objects[network][name] = obj
		self.nodes[network].pop(name,None)

	def add_networks(self,data):
		"""Add networks to model"""
		self.networks = list(reindex_to_dict(data,'CYMNETWORK',['NetworkId']).keys())
		self.set_define('CYMNETWORKS',' '.join(list(map(lambda x:str(x),self.networks))))
		for network in self.networks:
			self.objects[network] = {}
		self.headnodes = reindex_to_dict(data,'CYMHEADNODE',['NetworkId'])
		for network, headnode in self.headnodes.items():
			self.set_define(f'CYMHEADNODE_{network}',headnode['NodeId'])
		self.nodes = reindex_to_dict(data,'CYMNODE',['NetworkId','NodeId'])
		self.sections = reindex_to_dict(data,'CYMSECTION',['NetworkId','SectionId'])

	def add_nodes(self,data):
		"""Add nodes to model

		Nodes are removed by add_<type>() methods as objects are processed. 
		This call adds nodes that have not been picked up by other parts of 
		the model. These will most likely cause solver errors.
		"""
		for network, nodes in self.nodes.items():
			for name, data in copy(nodes).items():
				obj = new_object(class_name='powerflow.node',object_name=safename(name))
				comment(obj,'CYMNODE',data)
				obj['nominal_voltage'] = '${nominal_voltage}'
				obj['phases'] = 'ABC'
				self.add_object(network,obj)
			self.nodes[network] = {}

	def add_sections(self,data):
		"""Add sections to model

		Sections are removed by add_<type>() methods as objects are processed.
		This call adds nodes that have not been picked up by other parts of 
		the model. These will most likely cause solver errors.
		"""
		for network, sections in self.sections.items():
			for name, section in copy(sections).items():
				obj = new_object(class_name='powerflow.link',object_name=safename(name))
				comment(obj,'CYMSECTION',section)
				obj['nominal_voltage'] = '${nominal_voltage}'
				obj['phases'] = phase_map[section['Phase']]
				obj['from'] = safename(section['FromNodeId'])
				obj['to'] = safename(section['ToNodeId'])
				if not obj['from'] in self.objects[network].keys():
					self.warning(f'ignoring object {name} from node {obj["from"]} not found')
				elif not obj['to'] in self.objects[network].keys():
					self.warning(f'ignoring object {name} to node {obj["to"]} not found')
				else:
					self.add_object(network,obj)
			self.sections[network] = {}

	def add_overheads(self,data):
		self.warning('overheads not implemented yet')
		return

	def add_capacitors(self,data):
		"""Add capacitor bank object to model"""
		caps = reindex_to_dict(data,'CYMSHUNTCAPACITOR',['DeviceNumber'])
		if not caps:
			return
		count = 0
		for name, cap_data in caps.items():
			network = cap_data['NetworkId']
			obj = new_object(class_name='powerflow.capacitor',object_name=safename(name))
			comment(obj,'CYMSHUNTCAPACITOR',cap_data)
			obj['nominal_voltage'] = f'{cap_data["KVLN"]} kV'
			phases = phase_map[cap_data['Phase']]
			obj['phases'] = phases
			if 'A' in phases:
				obj['capacitor_A'] = f'{cap_data["KVARA"]} kVAR'
			if 'B' in phases:
				obj['capacitor_B'] = f'{cap_data["KVARB"]} kVAR'
			if 'C' in phases:
				obj['capacitor_C'] = f'{cap_data["KVARC"]} kVAR'
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
			obj['nominal_voltage'] = '${nominal_voltage}'
			phases = ''
			for phase, load in load_data.items():
				network = load['NetworkId']
				load_phase = phase_map[phase]
				load_type = load['ConsumerClassId']
				load_value1 = float(load['LoadValue1'])
				load_value2 = float(load['LoadValue2'])
				load_magnitude = sqrt(load_value1*load_value1+load_value2*load_value2)
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
					obj[f'base_power_{phase_map[phase]}'] = f'{load_magnitude} kW'
					if power_fraction > 0:
						obj[f'power_fraction_{phase_map[phase]}'] = f'{power_fraction} pu'
						obj[f'power_pf_{phase_map[phase]}'] = f'{power_factor} pu'
					if current_fraction > 0:
						obj[f'current_fraction_{phase_map[phase]}'] = f'{current_fraction} pu'
						obj[f'pcurrent_pf_{phase_map[phase]}'] = f'{current_factor} pu'
					if impedance_fraction > 0:
						obj[f'impedance_fraction_{phase_map[phase]}'] = f'{impedance_fraction} pu'
						obj[f'impedance_pf_{phase_map[phase]}'] = f'{impedance_factor} pu'
					phases += load_phase
			obj['phases'] = ''.join(sorted(set(phases)))
			self.add_object(network,obj)
			count += 1
		self.set_define('CYMLOADCOUNT',count)


def convert(input_file,output_file=None,options={}):
	model = Model(input_file)
	model.set_global('solver_method','NR',module='powerflow')
	if nominal_voltage:
		model.set_define('nominal_voltage',nominal_voltage)
	if options:
		for var, value in options.items():
			model.set_define(var,value)
	model.export_glm(output_file)

if __name__ == '__main__':
	testfile = 'autotest/IEEE-13-cyme.mdb'
	if os.path.exists(testfile):
		config.original_data = True
		convert(testfile)
