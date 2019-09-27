#!/usr/local/bin/python3
"""Syntax: gridlabd_library <subcommand> <options>...
Subcommands:
  help
"""

import sys
from datetime import datetime

try:
	import gridlabd_library_config as config
except:
	import gridlabd_library_config_default as config

def output(type,msg):
	print("gridlabd_library %s %s: %s"%(datetime.now(),type,msg))

def debug(msg):
	global config
	if config.debug : output("DEBUG",msg)

def verbose(msg):
	global config
	if config.verbose : output("DEBUG",msg)

def error(msg):
	global config
	if config.error : output("DEBUG",msg)

def syntax():
	return __doc__.split('\n')[0]

def help(**kwargs):
	return __doc__

if __name__ == '__main__':
	debug("main(argc=%d,argv=%s)" % (len(sys.argv),sys.argv))
	if len(sys.argv) == 1 :
		print(syntax())
	else :
		function_name = sys.argv[1]
		print(function_name)
		function = eval(function_name)
		print(function,str(type(function)))
		if str(type(function)) == "<class 'function'>":
			function(sys.argv[2::])
		else :
			error("invalid command")