"""EnergyPlus IDF parser"""
# https://bigladdersoftware.com/epx/docs/8-2/input-output-reference/group-simulation-parameters.html#group----simulation-parameters

class IDF:

	def __init__(self,filename):

		with open(filename,"rt") as fh:
			self.data = {}
			section = None
			for line in fh.readlines():
				if not line or line[0] == '\n':
					continue
				if line and line[0] in [' ','\t']:
					value,name = line.split('!-')
					name = name.strip(' ,;\n')
					def convert(x):
						x = x.strip(' ,;')
						try: return int(x)
						except: pass
						try: return float(x)
						except: pass
						return x
					value = [convert(x) for x in value.split(',')]
					while value and value[-1] in (None,''):
						del value[-1]
					if name == 'Name':
						subsection = value[0]
						self.data[section][subsection] = {}
						continue
					if len(value) == 0:
						value = None
					elif len(value) == 1:
						value = value[0]
					if subsection:
						self.data[section][subsection][name] = value
					else:
						self.data[section][name] = value
				else:
					section = line.strip(' ,\n')
					if section not in self.data:
						self.data[section] = {}
					subsection = None

	def __getitem__(self,key):
		return self.data[key]
