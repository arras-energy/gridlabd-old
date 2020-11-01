"""OpenFIDO API for HiPAS GridLAB-D

Syntax: gridlabd-openfido [OPTIONS] COMMAND [...]

Options:
	-v|--verbose   enables more output
	-q|--quiet     enables less output
"""

# using leading _ hides modules and globals from main API
import sys as _sys
import pydoc as _pydoc

def _perror(msg,**kwargs):
    print(f'ERROR [openfido]: {msg}',file=_sys.stderr,flush=True)

def _pwarning(msg,**kwargs):
    print(f'WARNING [openfido]: {msg}',file=_sys.stderr,flush=True)

def _poutput(msg,**kwargs):
    print(msg,file=_sys.stdout,flush=True)

_streams = {"output":_poutput,"warning":_pwarning,"error":_perror}

def help(options=[],streams=_streams):
	"""Syntax: gridlabd openfido help [COMMAND]
	"""
	if not options:
		streams["output"]("Syntax: gridlabd openfido [OPTIONS] COMMAND [...]")
	elif not type(options) is list:
		raise Exception("help options must be a list")
	elif len(options) > 1:
		raise Exception("help is only available on one command at a time")
	elif hasattr(_sys.modules[__name__],options[0]):
		call = getattr(_sys.modules[__name__],options[0])
		streams["output"](_pydoc.render_doc(call,renderer=_pydoc.plaintext))
	else:
		raise Exception(f"help on '{options[0]}' not available or command not found")

def install(options=[],streams=_streams):
	"""Syntax: gridlabd-openfido [OPTIONS] install WORKFLOW [REPOURL]
	"""
	raise Exception("'install' command not implemented")

def run(options=[],streams=_streams):
	"""Syntax: gridlabd-openfido [OPTIONS] run [RUNOPTIONS] INPUTFILES OUTPUTFILES

Run options:
	-d|--dry-run	Do not create output files
	"""
	raise Exception("'run' command not implemented")
