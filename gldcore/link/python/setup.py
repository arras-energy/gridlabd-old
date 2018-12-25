"""
To build the python module use the following command:

  python setup.py build

To install the python module use the following command:

  python setup.py install

"""
import sys
assert(sys.version_info.major>2)

from distutils.core import setup, Extension

import os
if not os.path.exists('build.h') :
	raise Exception("python module must be built from gldcore")

gridlabd = Extension('gridlabd', 
	include_dirs = ['link/python','.'],
	libraries = ['boost_python37'],
	sources = ['link/python/python.cpp',
		'aggregate.c',
		'class.c',
		'cmdarg.c',
		'compare.c',
		'debug.c',
		'deltamode.c',
		'enduse.c',
		'environment.c',
		'exception.c',
		'exec.c',
		'find.c',
		'globals.c',
		'gui.c',
		'http_client.c',
		'index.c',
		'instance.c',
		'instance_cnx.c',
		'instance_slave.c',
		'interpolate.c',
		'json.c',
		'kill.c',
		'kml.c',
		'legal.c',
		'linkage.c',
		'list.c',
		'load.c',
		'loadshape.c',
		'local.c',
		'main.c',
		'match.c',
		'matlab.c',
		'module.c',
		'object.c',
		'output.c',
		'property.c',
		'random.c',
		'realtime.c',
		'save.c',
		'schedule.c',
		'server.c',
		'test.c',
		'threadpool.c',
		'timestamp.c',
		'transform.c',
		'unit.c',
		'version.c',
		'convert.cpp',
		'daemon.cpp',
		'job.cpp',
		'link.cpp',
		'load_xml.cpp',
		'load_xml_handle.cpp',
		'lock.cpp',
		'sanitize.cpp',
		'setup.cpp',
		'stream.cpp',
		'test_framework.cpp',
		'validate.cpp',
		# add gridlabd source code here
		],
	)

setup (	name = 'GridLAB-D',
		version = '4.0',
		description = 'GridLAB-D Power System Simulator',
		author = 'David P. Chassin',
		author_email = 'dchassin@stanford.edu',
		ext_modules = [gridlabd])
