"""OpenFIDO API for HiPAS GridLAB-D

Syntax: gridlabd openfido [OPTIONS] COMMAND [...]

Options:
	-v|--verbose   enables more output
	-q|--quiet     enables less output

Authentication methods:

	./github_auth.py file:
		token=<your-token>

	GITHUB_TOKEN environment variable:
		% export GITHUB_TOKEN=<your-token>

	$HOME/.github/access-token file:
		<your-token>
"""

import os, sys, pydoc, warnings
import requests, shutil, importlib

sys.path.append(".")
sys.path.append(os.getenv("HOME")+"/.gridlabd")
sys.path.append("/usr/local/share/gridlabd")

verbose = False # print more messages as work is done
quiet = False # print fewer messages as work is done
orgname = "openfido" # default repo for workflows and pipelines
branch = "main" # default branch to use when downloading workflows and pipelines
cache = "/usr/local/share/gridlabd/openfido" # additional path for downloaded modules
apiurl = "https://api.github.com"
rawurl = "https://raw.githubusercontent.com"
giturl = "https://github.com"
traceback_file = "openfido.err"
try:
	from openfido_config import *
except:
	pass

# get authorization token
try:
	import github_auth as _auth
except:
	# template class using environment variable
	class _auth:
		token = os.getenv('GITHUB_TOKEN',None)
	if not _auth.token:
		# try reading file $HOME/.github/access-token
		try:
			token_file = f"{os.getenv('HOME','')}/.github/access-token"
			_fh = open(token_file,"r")
			_auth.token = _fh.read()
		except Exception as exc:
			pass
	pass

# default streams
def _error(msg,exit=None):
	print(msg,file=sys.stderr)
	if exit:
		sys.exit(exit)
def _silent(msg,exit=None):
	if exit:
		sys.exit(exit)
default_streams = {"output":_silent, "warning":warnings.warn, "error":_error, "verbose":_silent, "quiet":_silent}
command_streams = {"output":print, "warning":warnings.warn, "error":_error, "verbose":_silent, "quiet":_silent}

#
# CONFIG FUNCTION
#
def config(options=[], stream=default_streams):
	"""Syntax: gridlabd openfido config [show|get VARIABLE|set VARIABLE VALUE]
	"""
	if len(options) == 0:
		options = ["show"]
	if options[0] == "show" and len(options) == 1:
		result = {
			"verbose" : verbose,
			"quiet" : quiet,
			"orgname" : orgname,
			"branch" : branch,
			"cache" : cache,
			"apiurl" : apiurl,
			"rawurl" : rawurl,
			"giturl" : giturl,
			"traceback_file" : "openfido.err"		
		}
		for key,value in result.items():
			if type(value) is str:
				stream["output"](f"{key}=\"{value}\"")
			else:
				stream["output"](f"{key}={value}")
		return result
	elif options[0] == "get" and len(options) == 2:
		stream["output"](getattr(sys.modules[__name__],options[1]))
	elif options[0] == "set" and len(options) in (3,4):
		result = {
			"verbose" : verbose,
			"quiet" : quiet,
			"orgname" : orgname,
			"branch" : branch,
			"cache" : cache,
			"apiurl" : apiurl,
			"rawurl" : rawurl,
			"giturl" : giturl,		
			"traceback_file" : traceback_file
		}
		if options[1] in ["-l","--local"]:
			cfgfile = "./openfido_config.py"
			options = options[1:]
		else:
			cfgfile = os.getenv("HOME")+"/.gridlabd/openfido_config.py"
		if len(options) < 3:
			raise Exception(f"missing value")
		if not options[1] in result.keys():
			raise Exception(f"'{options[1]}' is not a valid configuration variables")
		for key, value in result.items():
			if options[1] == key:
				if type(value) is str:
					result[key] = options[2]
				elif type(value) == type(eval(options[2])):
					result[key] = eval(options[2])
				else:
					raise Exception(f"'{options[2]}' is not a valid type for variable '{key}'")
		with open(cfgfile,"w") as fh:
			for key,value in result.items():
				if type(value) is str:
					print(f"{key}=\"{value}\"",file=fh)
				else:
					print(f"{key}={value}",file=fh)
		return result
	else:
		raise Exception(f"'options={options}' is not valid")
#
# HELP FUNCTION
#
def help(options=[], stream=default_streams):
	"""Syntax: gridlabd openfido help [COMMAND]
	"""
	if not options:
		text = pydoc.render_doc(sys.modules[__name__],renderer=pydoc.plaintext).split("\n")
		for line in text:
			stream["output"](line)
	elif not type(options) is list:
		raise Exception("help options must be a list")
	elif len(options) > 1:
		raise Exception("help is only available on one command at a time")
	elif hasattr(sys.modules[__name__],options[0]):
		call = getattr(sys.modules[__name__],options[0])
		text = pydoc.render_doc(call,renderer=pydoc.plaintext).split("\n")[3:]
		for line in text:
			stream["output"](line[4:])
		return text
	else:
		raise Exception(f"help on '{options[0]}' not available or command not found")

#
# LIST FUNCTION
#
def index(options=[], stream=default_streams):
	"""Syntax: gridlabd openfido index [PATTERN]
	"""
	headers = {}
	if _auth.token:
		headers = {"Authorization": f"token {_auth.token.strip()}"}
	else:
		stream["verbose"]("using unauthenticated access")
	url = f"{apiurl}/orgs/{orgname}/repos"
	data = requests.get(url,headers=headers,params={}).json()
	if not data:
		raise Exception(f"unable to reach repo list for org '{orgname}' at {apiurl}")
	elif not type(data) is list:
		raise Exception(f"API error for org '{orgname}' at {url}: response ({type(data)}) = {data}")
	if _auth.token:
		stream["verbose"]("access token ok")
	repos = dict(zip(list(map(lambda r:r['name'],data)),data))
	if len(options) > 0:
		result = []
		for option in options:
			for repo in list(repos.keys()):
				if repo in result:
					continue
				pos = repo.find(option[option[0]=='^':])
				if pos < 0:
					continue
				if option[0] != '^' or pos == 0:
					result.append(repo)
	else:
		result = list(repos.keys())
	for name in sorted(result):
		stream["output"](name)
	return result

#
# INFO FUNCTION
#
def info(options=[], stream=default_streams):
	"""Syntax: gridlabd openfido info NAME
	"""
	if len(options) == 0:
		raise Exception("product name is required")
	elif len(options) > 1:
		raise Exception("only one product name is allowed")
	name = options[0]
	path = f"{cache}/{name}/__init__.py"
	if os.path.exists(path):
		stream["verbose"](f"examining {path}")
		spec = importlib.util.spec_from_file_location(name,path)
		module = importlib.util.module_from_spec(spec)
		spec.loader.exec_module(module)
		text = pydoc.render_doc(module,renderer=pydoc.plaintext).split("\n")
		for line in text:
			stream["output"](line)
		return text


#
# INSTALL FUNCTION
#
def install(options=[], stream=default_streams):
	"""Syntax: gridlabd openfido [OPTIONS] install [-d|--dryrun] NAME ...
	"""
	headers = {}
	if _auth.token:
		headers = {"Authorization": f"token {_auth.token.strip()}"}
	else:
		stream["verbose"]("using unauthenticated access")
	url = f"{apiurl}/orgs/{orgname}/repos"
	data = requests.get(url,headers=headers,params={}).json()
	if not data:
		raise Exception(f"unable to reach repo list for org '{orgname}' at {apiurl}")
	elif not type(data) is list:
		raise Exception(f"API error for org '{orgname}' at {url}: response ({type(data)}) = {data}")
	if _auth.token:
		stream["verbose"]("access token ok")
	repos = dict(zip(list(map(lambda r:r['name'],data)),data))
	dryrun = os.system
	failed = []
	done = []
	for option in options:
		if option[0] == '-':
			if option in ['-d','--dry-run']:
				dryrun = stream["output"]
			else:
				raise Exception(f"option '{option}' is invalid")

	for name in options:
		if name[0] == '-':
			continue
		elif not name in repos.keys():
			stream["error"](f"'{name}' not found in openfido repository")
			failed.append(name)
		else:
			repo = repos[name]
			url = f"{rawurl}/{orgname}/{name}/{branch}/openfido.json"
			data = requests.get(url)
			try:
				manifest = data.json()
			except:
				manifest = None
			if not manifest:
				stream["error"](f"manifest read failed: url={url}, status_code={data.status_code}, headers={data.headers}, body=[{data.text}]") 
			if not "application" in manifest.keys() or manifest["application"] != "openfido":
				stream["error"](f"tool '{name}' is not an openfido application")
				failed.append(name)
			elif not "valid" in manifest.keys() or not manifest["valid"]:
				stream["warning"](f"tool '{name}' is not valid")
				failed.append(name)
			else:
				if not "version" in manifest.keys():
					stream["warning"](f"tool '{name}' has no version")
				if not "tooltype" in manifest.keys() or manifest["tooltype"] not in ("pipeline","workflow"):
					stream["warning"](f"tool '{name}' type is missing or invalid")
				stream["verbose"](f"{name}: {manifest['tooltype']} version {manifest['version']} is valid")
				source = f"{giturl}/{orgname}/{name}"
				target = f"{cache}/{name}"
				if os.path.exists(target):
					stream["warning"](f"'{name}' is already installed")
					done.append(name)
				elif os.system(f"git clone -q {source} {target} -b {branch}") != 0:
					stream["error"](f"unable to clone '{name}' into openfido cache '{cache}'")
					failed.append(name)
				else:
					stream["verbose"](f"'{name}' cloned ok")
					done.append(name)
				# TODO: implement installation
				done.append(name)
	return {"ok":len(done), "errors":len(failed), "done":done, "failed": failed}

#
# UPDATE FUNCTION
#
def update(options=[], stream=default_streams):
	"""Syntax: gridlabd openfido [OPTIONS] update [-d|--dryrun] NAME ...
	"""
	dryrun = os.system
	done = []
	failed = []
	for option in options:
		if option[0] == '-':
			if option in ['-d','--dry-run']:
				dryrun = stream["output"]
			else:
				raise Exception(f"option '{option}' is invalid")
	for name in options:
		if name[0] != '-':
			if os.path.exists(f"{cache}/{name}"):
				stream["verbose"](f"updating {cache}/{name}")
				dryrun(f"cd {cache}/{name} && git pull")
				done.append(name)
			else:
				stream["warning"](f"'{name}' not found")
				failed.append(name)
	return {"ok":len(done), "errors":len(failed), "done":done, "failed": failed}

#
# REMOVE FUNCTION
#
def remove(options=[], stream=default_streams):
	"""Syntax: gridlabd openfido [OPTIONS] remove [-d|--dryrun] NAME ...
	"""
	dryrun = os.system # shutil.rmtree # too chicken to enable it now
	done = []
	failed = []
	for option in options:
		if option[0] == '-':
			if option in ['-d','--dry-run']:
				dryrun = stream["output"]
			else:
				raise Exception(f"option '{option}' is invalid")
	for name in options:
		if name[0] != '-':
			if cache[0] != '/':
				stream["error"](f"too chicken to remove a folder without an absolute path")
				failed.append(name)
			elif os.path.exists(f"{cache}/{name}/.git"):
				stream["verbose"](f"removing {cache}/{name}")
				dryrun(f"rm -rf '{cache}/{name}'")
				done.append(name)
			else:
				stream["warning"](f"'{name}' not found or not an openfido product")
				failed.append(name)
	return {"ok":len(done), "errors":len(failed), "done":done, "failed": failed}

#
# RUN FUNCTION
#
def run(options=[], stream=default_streams):
	"""Syntax: gridlabd openfido [OPTIONS] run NAME [OPTIONS ...] INPUTFILES [OUTPUTFILES]
	"""
	if not options:
		raise Exception("missing package name")
	name = options[0]
	path = f"{cache}/{name}"
	if not os.path.exists(f"{path}/openfido.json"):
		raise Exception(f"'{cache}/{name}' not found")
	sys.path.append(f"{cache}/{name}")
	if not os.path.exists(f"{path}/__init__.py"):
		raise Exception(f"'{path}/__init__.py' not found")
	spec = importlib.util.spec_from_file_location(name,f"{path}/__init__.py")
	module = importlib.util.module_from_spec(spec)
	spec.loader.exec_module(module)
	if not hasattr(module,"main") or not callable(module.main):
		raise Exception(f"'{name}/__init__.py' missing callable main")
	inputs = []
	outputs = []
	flags = []
	for n in range(1,len(options)):
		if options[n][0] == '-': 
			flags.append(options[n])
		elif not inputs:
			inputs = options[n].split(',')
		elif not outputs:
			outputs = options[n].split(',')
		else:
			raise Exception(f"option {options[n]} unexpected")
	return module.main(inputs=inputs,outputs=outputs,options=flags)
