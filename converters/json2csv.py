import json 
import os 
import sys, getopt
from datetime import datetime 
import importlib, copy
from importlib import util

config = {
	"input" :	"json",
	"output" :  "csv",
	"type" : {
		"profile" : "voltage profile"
	}
}

def help():
	print(f'Syntax:')
	print(f'{config["input"]}2{config["output"]}.py -i|--ifile <input-file>[,<input-file>[,...]] -o|--ofile <output-file> -t|--type <input-type>')
	print(f'  -c|--config    : [OPTIONAL] output converter configuration')
	print(f'  -i|--ifile     : [REQUIRED] {config["input"]} input file name')
	print(f'  -o|--ofile     : [REQUIRED] {config["output"]} output file name')
	print(f'  -t|--type      : [REQUIRED] specify output type')
	print(f'Output types')
	print(f'  profile        : voltage profile');

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
		if arg in config['type'].keys():
			output_type = arg.strip()
		else:
			raise Exception(f"type '{arg}' is not recognized")
	else:
		raise Exception(f"'{opt}' is an invalid command line option")

modname = sys.argv[0].replace(f"{config['input']}2{config['output']}.py",f"{config['input']}2{config['output']}-{output_type}.py")
if os.path.exists(modname):

	modspec = util.spec_from_file_location(output_type, modname)
	mod = importlib.import_module(f"{config['input']}2{config['output']}-{output_type}")
	mod.convert(input_file=input_file,output_file=output_file)

else:

	raise Exception(f"txt2glm-{output_type}.py not found")