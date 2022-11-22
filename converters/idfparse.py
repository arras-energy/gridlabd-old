"""EnergyPlus IDF parser"""
# https://bigladdersoftware.com/epx/docs/8-2/input-output-reference/group-simulation-parameters.html#group----simulation-parameters

import os
import requests

try:
	CACHEDIR = getenv("GLD_ETC") + "/energyplus"
except:
	CACHEDIR = "/tmp/energyplus"
os.makedirs(CACHEDIR,exist_ok=True)

class IDD:

	cache = {}

	def __init__(self,major,minor=0,patch=0):
		version = f"V{major}-{minor}-{patch}"
		url = f"https://raw.githubusercontent.com/NREL/EnergyPlus/develop/idd/{version}-Energy%2B.idd"
		if not url in self.cache:
			file = f"{CACHEDIR}/{version}.idd"
			if not os.path.exists(file):
				idd = requests.get(url)
				if idd.status_code != 200:
					raise Exception(f"{url} status code {idd.status_code}")
				with open(file,"wt") as fh:
					fh.write(idd.text)
				self.cache[version] = idd.text.split('\n')
			else:
				with open(file,"rt") as fh:
					self.cache[version] = fh.readlines()

		data = {}
		group = None
		object = None
		lineno = 0
		for line in self.cache[version]:
			lineno += 1
			line = line.split('!')[0]
			if not line or line[0] == '\n':
				continue

			if line[0] == '\\':
				spec = line[1:].split()
				if spec[0] == 'group':
					group = ' '.join(spec[1:])
					data[group] = {}
					object = None
					continue
				elif spec[0] and group:
					data[group][spec[0].strip()] = ' '.join(spec[1:])
					continue

			elif line[0] != ' ':

				spec = [x.strip() for x in line.split(',')]

				if len(spec) > 1 and spec[1]:
					raise Exception(f"{version}.idd@{lineno}: unexpected data in field 1 {dict(enumerate(spec))}")
				# print(spec)
				object = spec[0]
				data[group][object] = {'type':''}
				continue

			else:

				spec = [x.strip() for x in line.split(',;')]
				if not spec[0]:
					continue
				elif group:
					if object:
						if spec[0][0] == '\\': 
							# object comment
							values = spec[0].split(' ')
							tag = values[0][1:]
							if not tag in data[group][object]:
								data[group][object][tag] = []
							data[group][object][tag] = ' '.join(values[1:])
						else:
							data[group][object]['type'] += spec[0][0]
						continue
					elif spec[0] == '\\':
						# object comment
						values = spec[0].split(' ')
						tag = values[0][1:]
						if not tag in data[group]:
							data[group][tag] = []
						data[group][tag] = ' '.join(values[1:])
						continue
			raise Exception(f"{version}.idd@{lineno}: unexpected data for group '{group}' object '{object}': '{line.strip()}'")
		self.data = data

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
						idd = IDD(*(value.strip(' ,;').split('.')))
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

if __name__ == "__main__":
	import json, sys
	json.dump(IDD(9,4).data,sys.stdout,indent=4)

