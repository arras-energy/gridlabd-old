"""EnergyPlus IDF parser"""
# https://bigladdersoftware.com/epx/docs/8-2/input-output-reference/group-simulation-parameters.html#group----simulation-parameters

import requests

IDD = None

def get_idd(major,minor=0,patch=0):
	url = f"https://raw.githubusercontent.com/NREL/EnergyPlus/develop/idd/V{major}-{minor}-{patch}-Energy%2B.idd"
	idd = requests.get(url)
	if idd.status_code != 200:
		raise Exception(f"{url} status code {idd.status_code}")
	global IDD
	IDD = idd.text

class IDF:

	def __init__(self,filename):

		with open(filename,"rt") as fh:
			self.data = {}
			section = None
			for line in fh.readlines():
				if not line or line[0] == '\n':
					continue
				if line and line[0] in [' ','\t']:
					value,name = line.split('!')
					name = name.strip('- ,;\n')
					if section == "Version" and name == "Version Identifier":
						IDD = get_idd(*(value.strip(' ,;').split('.')))
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
