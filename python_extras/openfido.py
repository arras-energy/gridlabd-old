"""OpenFIDO API for HiPAS GridLAB-D

Syntax: gridlabd-openfido [OPTIONS] COMMAND [...]

Options:
	-v|--verbose   enables more output
	-q|--quiet     enables less output
"""

import sys as sys
import pydoc as pydoc
import warnings

def _error(msg,exit=None):
	print(msg,file=sys.stderr)
	if exit:
		sys.exit(exit)
def _silent(msg,exit=None):
	if exit:
		sys.exit(exit)

default_streams = {"output":print, "warning":warnings.warn, "error":_error, "verbose":_silent, "quiet":_silent}

def help(options=[], stream=default_streams):
	"""Syntax: gridlabd openfido help [COMMAND]
	"""
	if not options:
		stream["output"]("Syntax: gridlabd openfido [OPTIONS] COMMAND [...]")
		stream["output"]("Commands:")
		for command in sorted(dir(sys.modules[__name__])):
			if callable(getattr(sys.modules[__name__],command)) and command[0] != '_':
				stream["output"](f"  {command}")
	elif not type(options) is list:
		raise Exception("help options must be a list")
	elif len(options) > 1:
		raise Exception("help is only available on one command at a time")
	elif hasattr(sys.modules[__name__],options[0]):
		call = getattr(sys.modules[__name__],options[0])
		stream["output"](pydoc.render_doc(call,renderer=pydoc.plaintext))
	else:
		raise Exception(f"help on '{options[0]}' not available or command not found")

def install(options=[], stream=default_streams):
	"""Syntax: gridlabd-openfido [OPTIONS] install WORKFLOW [REPOURL]
	"""
	raise Exception("'install' command not implemented")

def run(options=[], stream=default_streams):
	"""Syntax: gridlabd-openfido [OPTIONS] run [RUNOPTIONS] INPUTFILES OUTPUTFILES

Run options:
	-d|--dry-run	Do not create output files
	"""
	raise Exception("'run' command not implemented")
