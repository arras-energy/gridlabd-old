"""
To build the python module use the following command:

  python setup.py build --inplace

"""
import sys
assert(sys.version_info.major>2)

from distutils.core import setup, Extension

gridlabd = Extension('gridlabd', 
	sources = ['python.c',
		# add gridlabd source code here
		],
	)

setup (	name = 'GridLAB-D',
		version = '4.1',
		description = 'GridLAB-D Power System Simulatior',
		ext_modules = [gridlabd])
