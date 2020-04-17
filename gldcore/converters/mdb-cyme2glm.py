#
# main converter
#
import sys, os
from datetime import *
from math import *

# TODO delete when done debugging
import pprint
pprint = pprint.PrettyPrinter(indent=4).pprint

# map CYME phases to GLM phases
phase_map = {
		1:"A",
		2:"B",
		3:"C",
		4:"AB",
		5:"AC",
		6:"BC",
		7:"ABC",
		}

def reindex_to_dict(df,name,index,orient='index'):
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

class GridlabdModel:
	
	def __init__(self,input_file=None):
		self.modules = {}
		self.globals = {}
		self.objects = {}
		if input_file:
			self.load(input_file)
		self.output_file = None

	def require(self,module):
		if not module in self.modules:
			self.modules[module] = {}
			# self.write('module powerflow {')
			# self.write('\tsolver_method NR;')
			# self.write('}')

	def load(self,input_file):
		import importlib
		importlib.util.spec_from_file_location('cyme', sys.argv[0].replace('mdb-cyme2glm','cyme'));
		cyme = importlib.import_module('cyme');		
		data = cyme.dataframe(input_file)
		self.input_file = input_file
		self.add_networks(data)
		self.require('powerflow')
		self.add_loads(data)

	def save(self,output_file=None):
		if output_file == None:
			output_file = self.input_file.replace(".mdb",".glm")
		with open(output_file,"w") as fh:
			fh.write(f"// CYME {self.input_file} converted to GLM\n")
			fh.write(f"// Date: {datetime.now()}\n")
			fh.write(f"// User: {os.getenv('USER')}\n")
			fh.write(f"// Workdir: {os.getenv('PWD')}\n")
			self.save_modules(fh)
			self.save_object(fh)
			fh.write(f"// END OF FILE\n")
			self.output_file = output_file

	def save_modules(self,fh):
		for mod, varlist in self.modules.items():
			fh.write(f'module {mod}\n')
			fh.write('{\n')
			for var,value in varlist.items():
				fh.write(f'\t{var} {value};\n')
			fh.write('}\n')

	def save_object(self,fh):
		for network in self.networks:
			fh.write(f'//\n// NETWORK {network}\n//\n')
			for obj, props in self.objects[network].items():
				classname = props['class']
				fh.write(f'object {classname}\n')
				fh.write('{\n')
				fh.write(f'\tname "{obj}";\n')
				for name, value in props.items():
					if name == 'class':
						continue
					fh.write(f'\t{name} "{value}";\n')
				fh.write('}\n')


	def set_global(self,name,value,module=None):
		if module == None:
			self.globals[name] = value
		else:
			self.modules[module][name] = value

	def add_networks(self,data):
		self.networks = list(reindex_to_dict(data,'CYMNETWORK',['NetworkId']).keys())
		for network in self.networks:
			self.objects[network] = {}

	def add_loads(self,data):
		load_classes = reindex_to_dict(data,'CYMCONSUMERCLASS',['ConsumerClassId'])
		loads = reindex_to_dict(data,'CYMCUSTOMERLOAD',['DeviceNumber','Phase'])
		if loads:
			for name, load_data in loads.items():
				obj = {'class':'powerflow.load'};
				obj["nominal_voltage"] = '120 V';
				phases = ''
				for phase, load in load_data.items():
					network = load['NetworkId']
					load_phase = phase_map[phase]
					load_type = load['ConsumerClassId']
					load_value1 = float(load['LoadValue1'])
					load_value2 = float(load['LoadValue2'])
					load_magnitude = sqrt(load_value1*load_value1+load_value2*load_value2)
					load_class = load_classes[load_type]
					power_fraction = float(load_class["ConstantPower"])/100
					current_fraction = float(load_class["ConstantCurrent"])/100
					impedance_fraction = float(load_class["ConstantImpedance"])/100
					power_factor = float(load_class["PowerFactor"])/100
					ZP = float(load_class["ConstantImpedanceZ_P"])/100
					ZQ = float(load_class["ConstantImpedanceZ_Q"])/100
					IP = float(load_class["ConstantCurrentI_P"])/100
					IQ = float(load_class["ConstantCurrentI_Q"])/100
					PP = float(load_class["ConstantPowerP_P"])/100
					PQ = float(load_class["ConstantPowerP_Q"])/100
					if load_phase == 'ABC': # balanced load
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
				self.objects[network][name] = obj

def convert(input_file,output_file=None,options=None):
	glm = GridlabdModel(input_file)
	# glm.set_global('solver_method','NR',module='powerflow')
	glm.save(output_file)

if __name__ == '__main__':
	testfile = "autotest/IEEE-13-cyme.mdb"
	if os.path.exists(testfile):
		convert(testfile)
