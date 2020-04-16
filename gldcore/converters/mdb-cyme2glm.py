#
# main converter
#
import sys, os
from datetime import *

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
		importlib.util.spec_from_file_location("cyme", sys.argv[0].replace("mdb-cyme2glm","cyme"));
		cyme = importlib.import_module("cyme");
		data = cyme.dataframe(input_file)
		self.input_file = input_file
		self.require("powerflow")
		self.add_loads(data)

	def save(self,output_file=None):
		if output_file == None:
			output_file = self.input_file.replace(".mdb",".glm")
		with open(output_file,"w") as fh:
			fh.write(f"// CYME {self.input_file} converted to GLM\n")
			fh.write(f"// Date: {datetime.now()}\n")
			fh.write(f"// User: {os.getenv('USER')}\n")
			fh.write(f"// Workdir: {os.getenv('PWD')}\n")

			# modules
			for mod, varlist in self.modules.items():
				fh.write(f'module {mod}\n')
				fh.write('{\n')
				for var,value in varlist.items():
					fh.write(f'\t{var} {value};\n')
				fh.write('}\n')

			for obj, props in self.objects.items():
				classname = props['class']
				fh.write(f'object {classname}\n')
				fh.write('{\n')
				fh.write(f'\tname "{obj}";\n')
				for name, value in props.items():
					if name == 'class':
						continue
					fh.write(f'\t{name} "{value}";\n')
				fh.write('}\n')

			fh.write(f"// END OF FILE\n")
			self.output_file = output_file

	def set_global(self,name,value,module=None):
		if module == None:
			self.globals[name] = value
		else:
			self.modules[module][name] = value

	def add_loads(self,data):
		if "CYMCUSTOMERLOAD" in data.keys():
			loads = data['CYMCUSTOMERLOAD'].reset_index()
			loads.set_index(['DeviceNumber','Phase'],inplace=True)
			for name in loads.index.levels[0]:
				load_data = loads.loc[(name)]
				obj = {'class':'powerflow.load'};
				# self.write('object powerflow.load {')
				# self.write(f'\tname "{name}";')
				obj["nominal_voltage"] = '120 V';
				# self.write('\tnominal_voltage 120 V;')
				phases = ''
				for phase in loads.index.levels[1]:
					load_phase = phase_map[phase]
					load = load_data.loc[(phase)]
					load_type = load['ConsumerClassId']
					load_value1 = load['LoadValue1']
					load_value2 = load['LoadValue2']
					# self.write(f'\t// phase {phase_map[phase]} load type {load_type}')
					load_map = {
							'PQ':'constant_power',
							'I':'constant_current',
							'Z':'constant_impedance',
							}
					if load_phase == "ABC":
						obj[f'{load_map[load_type]}_A'] = f'{load_value1/3}+{load_value2/3}j'
						# self.write(f'\t{load_map[load_type]}_A {load_value1/3}+{load_value2/3}j;')
						# self.write(f'\t{load_map[load_type]}_B {load_value1/3}+{load_value2/3}j;')
						# self.write(f'\t{load_map[load_type]}_C {load_value1/3}+{load_value2/3}j;')
					else:
						obj[f'{load_map[load_type]}_{load_phase}'] = f'{load_value1}+{load_value2}j'
						# self.write(f'\t{load_map[load_type]}_{load_phase} {load_value1}+{load_value2}j;')
					phases += load_phase
				obj["phases"] = ''.join(sorted(set(phases)))

				self.objects[name] = obj
				# self.write(f'\tphases {phases};')
				# self.write('}')

def convert(input_file,output_file=None):
	glm = GridlabdModel(input_file)
	# glm.set_global('solver_method','NR',module='powerflow')
	glm.save(output_file)

if __name__ == '__main__':
	testfile = "autotest/IEEE-13-cyme.mdb"
	if os.path.exists(testfile):
		convert(testfile)
