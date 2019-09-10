"""
The gridlabd module is automatically installed as part of a full gridlabd install:

  host% make install

To build only the python module use the following command can be run after gridlabd is installed

  bash% export SRCDIR=$PWD
  bash% python gldcore/link/python/setup.py build


To install the python module use the following command:

  bash% export SRCDIR=$PWD
  bash% python gldcore/link/python/setup.py install

To uninstall the python module, you must save the list of installed files

  bash% export SRCDIR=$PWD
  bash% python gldcore/link/python/setup.py install --record files.txt

Then you can uninstall gridlabd's python module using the command

  bash% rm $(cat files.txt)

"""
import sys
assert(sys.version_info.major>2)

import os
srcdir = os.getenv('SRCDIR')
if not srcdir :
	raise Exception("SRCDIR environment variable was not set -- try the command 'export SRCDIR=$PWD' before running setup.py")

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
compile_options.extend(['-I%s/gldcore'%srcdir,'-Igldcore','-Igldcore/rt',"-DHAVE_CONFIG_H","-DHAVE_PYTHON"])

from distutils.core import setup, Extension
gridlabd = Extension('gridlabd', 
	include_dirs = ['gldcore/link/python','gldcore'],
	extra_compile_args = compile_options,
	libraries = ['ncurses', 'curl'],
	sources = list(map(lambda x: srcdir+'/'+x,['gldcore/link/python/python.cpp',
		'gldcore/aggregate.cpp',
		'gldcore/class.cpp',
		'gldcore/cmdarg.cpp',
		'gldcore/compare.cpp',
		'gldcore/convert.cpp',
		'gldcore/curl.cpp',
		'gldcore/daemon.cpp',
		'gldcore/debug.cpp',
		'gldcore/deltamode.cpp',
		'gldcore/enduse.cpp',
		'gldcore/environment.cpp',
		'gldcore/exception.cpp',
		'gldcore/exec.cpp',
		'gldcore/find.cpp',
		'gldcore/globals.cpp',
		'gldcore/gui.cpp',
		'gldcore/http_client.cpp',
		'gldcore/index.cpp',
		'gldcore/instance.cpp',
		'gldcore/instance_cnx.cpp',
		'gldcore/instance_slave.cpp',
		'gldcore/interpolate.cpp',
		'gldcore/job.cpp',
		'gldcore/json.cpp',
		'gldcore/kill.cpp',
		'gldcore/kml.cpp',
		'gldcore/legal.cpp',
		'gldcore/link.cpp',
		'gldcore/linkage.cpp',
		'gldcore/list.cpp',
		'gldcore/load.cpp',
		'gldcore/load_xml.cpp',
		'gldcore/load_xml_handle.cpp',
		'gldcore/loadshape.cpp',
		'gldcore/local.cpp',
		'gldcore/lock.cpp',
		'gldcore/main.cpp',
		'gldcore/match.cpp',
		'gldcore/matlab.cpp',
		'gldcore/module.cpp',
		'gldcore/object.cpp',
		'gldcore/output.cpp',
		'gldcore/property.cpp',
		'gldcore/random.cpp',
		'gldcore/realtime.cpp',
		'gldcore/sanitize.cpp',
		'gldcore/save.cpp',
		'gldcore/schedule.cpp',
		'gldcore/server.cpp',
		'gldcore/setup.cpp',
		'gldcore/stream.cpp',
		'gldcore/test.cpp',
		'gldcore/threadpool.cpp',
		'gldcore/timestamp.cpp',
		'gldcore/transform.cpp',
		'gldcore/unit.cpp',
		'gldcore/validate.cpp',
		'gldcore/version.cpp',
		# add gridlabd source code here
		])),
	)

def get_version(path=None):
	if path == None :
		path = srcdir + "/gldcore"
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
	with open(path + "/build.h") as f:
		for line in f:
			info = line.split(" ")
			if info[0] == "#define":
				if info[1] == "BUILDNUM":
					build = int(info[2])
	return '%d.%d.%d.%d' % (major,minor,patch,build)

setup (	name = 'gridlabd',
		version = get_version(),
		description = 'GridLAB-D Smart Grid Simulator',
		author = 'David P. Chassin',
		author_email = 'dchassin@stanford.edu',
		ext_modules = [gridlabd])
