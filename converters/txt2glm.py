import json 
import os 
import sys, getopt
from datetime import datetime 
import importlib, copy
from importlib import util

config = {"input":"txt","output":"glm","type":["cyme"]}

def help():
	print('Syntax:')
	print('txt2glm.py -i|--ifile <input-file>[,<input-file>[,...]] -o|--ofile <output-file> -t|--type <input-type>')
	print('  -c|--config    : [OPTIONAL] output converter configuration')
	print('  -i|--ifile     : [REQUIRED] txt input file name.')
	print('  -o|--ofile     : [REQUIRED] glm output file name.')
	print('  -t|--type      : [REQUIRED] specify input type')
	print('Input types')
	print('  cyme           : cyme conversion');

input_file = None
output_file = None
output_type = None

opts, args = getopt.getopt(sys.argv[1:],"hci:o:t:",["help","config","ifile=","ofile=","type="])

if not opts : 
	help()
	sys.exit(1)
for opt, arg in opts:
	if opt in ("-h","--help"):
		help()
		sys.exit(0)
	elif opt in ("-c","--config"):
		print(json.dumps(config))
		sys.exit(0)
	elif opt in ("-i", "--ifile"):
		input_file = arg.strip()
	elif opt in ("-o", "--ofile"):
		output_file = arg.strip()
	elif opt in ("-t","--type"):
		output_type = arg.strip()
	else:
		raise Exception(f"'{opt}' is an invalid command line option")

modname = sys.argv[0].replace("txt2glm.py",f"txt2glm-{output_type}.py")
if os.path.exists(modname):

	modspec = util.spec_from_file_location(output_type, modname)
	mod = importlib.import_module(f"txt2glm-{output_type}")
	mod.convert(input_file=input_file,output_file=output_file)

else:

	raise Exception(f"txt2glm-{output_type}.py not found")
