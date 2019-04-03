"""
To build the python module use the following command:

  python gldcore/link/python/setup.py build

To install the python module use the following command:

  python gldcore/link/python/setup.py install

"""
import sys
assert(sys.version_info.major>2)

import os
if not os.path.exists('gldcore/build.h') :
	raise Exception("python module must be built from project folder after the main build is completed (gldcore/build.h is missing)")
srcdir = os.getenv('SRCDIR')

try:
	from compile_options import *
except:
	import os
	try :
		compile_options = os.getenv("CFLAGS").split(" ")
	except :
		compile_options = None
	if not compile_options :
		compile_options=['-O2','-g']
if not srcdir :
	raise Exception("SRCDIR environment variable was not set -- try the command 'export SRCDIR=$PWD' before running setup.py")
compile_options.extend(['-I'+srcdir+'/gldcore',"-DHAVE_CONFIG_H","-DHAVE_PYTHON"])

from distutils.core import setup, Extension
gridlabd = Extension('gridlabd', 
	include_dirs = ['gldcore/link/python','gldcore'],
	extra_compile_args = compile_options,
	libraries = ['ncurses'],
	sources = list(map(lambda x: srcdir+'/'+x,['gldcore/link/python/python.cpp',
		'gldcore/aggregate.cpp',
		'gldcore/class.cpp',
		'gldcore/cmdarg.cpp',
		'gldcore/compare.cpp',
		'gldcore/convert.cpp',
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
		'gldcore/load.c',
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
		'gldcore/test_framework.cpp',
		'gldcore/threadpool.cpp',
		'gldcore/timestamp.cpp',
		'gldcore/transform.cpp',
		'gldcore/unit.cpp',
		'gldcore/validate.cpp',
		'gldcore/version.cpp',
		# add gridlabd source code here
		])),
	)

setup (	name = 'GridLAB-D',
		version = '4.2',
		description = 'GridLAB-D Power System Simulator',
		author = 'David P. Chassin',
		author_email = 'dchassin@stanford.edu',
		ext_modules = [gridlabd])
