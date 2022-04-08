"""
The gridlabd module is automatically installed as part of a full gridlabd install:

  host% make install

To build only the python module use the following command can be run after gridlabd is installed

  bash% export SRCDIR=$PWD
  bash% python python/setup.py build


To install the python module use the following command:

  bash% export SRCDIR=$PWD
  bash% python python/setup.py install

To uninstall the python module, you must save the list of installed files

  bash% export SRCDIR=$PWD
  bash% python python/setup.py install --record files.txt

Then you can uninstall gridlabd's python module using the command

  bash% rm $(cat files.txt)

"""
import sys
assert(sys.version_info.major>2)

import os
srcdir = os.getenv('SRCDIR')
if not srcdir :
	srcdir = os.path.realpath(sys.argv[0]).replace("/python/setup.py","")
else:
	srcdir = os.path.realpath(srcdir)

try:
	from compile_options import *
except:
	import os
	try :
		compile_options = os.getenv("CFLAGS").split(" ")
	except :
		compile_options = None
if not compile_options :
	compile_options=['-Wall','-O3','-g']
compile_options.extend(['-I%s/source'%srcdir,'-Isource','-Isource/rt',"-fPIC","-DHAVE_CONFIG_H","-DHAVE_PYTHON"])

from distutils.core import setup, Extension
gridlabd = Extension('gridlabd', 
	include_dirs = list(map(lambda x: srcdir+'/'+x,['python','source'])),
	extra_compile_args = compile_options,
	libraries = ['ncurses', 'curl'],
	sources = list(map(lambda x: srcdir+'/'+x,['python/python.cpp',
		'source/aggregate.cpp',
		'source/class.cpp',
		'source/cmdarg.cpp',
		'source/compare.cpp',
		'source/convert.cpp',
		'source/curl.cpp',
		'source/daemon.cpp',
		'source/debug.cpp',
		'source/deltamode.cpp',
		'source/enduse.cpp',
		'source/environment.cpp',
		'source/exception.cpp',
		'source/exec.cpp',
		'source/find.cpp',
		'source/globals.cpp',
		'source/gui.cpp',
		'source/http_client.cpp',
		'source/index.cpp',
		'source/instance.cpp',
		'source/instance_cnx.cpp',
		'source/instance_slave.cpp',
		'source/interpolate.cpp',
		'source/job.cpp',
		'source/json.cpp',
		'source/kill.cpp',
		'source/kml.cpp',
		'source/legal.cpp',
		'source/link.cpp',
		'source/linkage.cpp',
		'source/list.cpp',
		'source/load.cpp',
		'source/loadshape.cpp',
		'source/local.cpp',
		'source/lock.cpp',
		'source/main.cpp',
		'source/match.cpp',
		'source/matlab.cpp',
		'source/module.cpp',
		'source/object.cpp',
		'source/output.cpp',
		'source/property.cpp',
		'source/python_embed.cpp',
		'source/python_property.cpp',
		'source/random.cpp',
		'source/realtime.cpp',
		'source/sanitize.cpp',
		'source/save.cpp',
		'source/schedule.cpp',
		'source/server.cpp',
		'source/setup.cpp',
		'source/stream.cpp',
		'source/test.cpp',
		'source/threadpool.cpp',
		'source/timestamp.cpp',
		'source/transform.cpp',
		'source/unit.cpp',
		'source/validate.cpp',
		'source/version.cpp',
		# add gridlabd source code here
		])),
	)

def get_version(path=None):
	if path == None :
		path = srcdir + "/source"
	major = None
	minor = None
	patch = None
	build = None
	with open(path + "/version.h") as f:
		for line in f:
			info = line.split(" ")
			if info[0] == "#define":
				if info[1] == "REV_MAJOR":
					major = int(info[2])
				elif info[1] == "REV_MINOR":
					minor = int(info[2])
				elif info[1] == "REV_PATCH":
					patch = int(info[2])
	try:
		with open(path + "/build.h") as f:
			for line in f:
				info = line.split(" ")
				if info[0] == "#define":
					if info[1] == "BUILDNUM":
						build = int(info[2])
					elif info[1] == "BRANCH":
						branch = info[2].replace('"','')

		from hashlib import blake2b
		h = blake2b(digest_size=8)
		h.update(branch.encode())
		return '%d.%d.%d.%d.%d' % (major,minor,patch,build,int(h.hexdigest(),16))
	except:
		return '0.0.0'

if len(sys.argv) > 1 and sys.argv[1] == "--version":
	print(get_version())
else:
	setup (	
		name = 'gridlabd',
		version = get_version(),
		description = 'HiPAS GridLAB-D',
		author = 'SLAC Gismo',
		author_email = 'gridlabd@gmail.com',
		ext_modules = [gridlabd],
		url = "https://www.gridlabd.us/")
