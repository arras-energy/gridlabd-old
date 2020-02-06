import json 
import os 
import sys, getopt
from datetime import datetime 

def help():
	print('Syntax:')
	print('mdb2glm.py -i|--ifile <input-file> -o|--ofile <output-file> -t|--type <input-type>')
	print('  -i|--ifile     : [REQUIRED] mdb input file name.')
	print('  -o|--ofile     : [REQUIRED] glm output file name.')
	print('  -t|--type      : [REQUIRED] specify input type')
	print('Input types')
	print('  cyme           : cyme input');

input_name = None
output_name = None
input_type = None

try : 
	opts, args = getopt.getopt(sys.argv[1:],"hi:o:t:",["help","ifile=","ofile=","type="])
except getopt.GetoptError:
	sys.exit(2)
if not opts : 
	help()
	sys.exit(1)
for opt, arg in opts:
	if opt in ("-h","--help"):
		help()
		sys.exit(0)
	elif opt in ("-i", "--ifile"):
		input_name = arg.strip();
	elif opt in ("-o", "--ofile"):
		output_name = arg.strip();
	elif opt in ("-t","--type"):
		input_type = arg.strip();
	else:
		raise Exception(f"'{opt}' is an invalid command line option")

modname = sys.argv[0].replace("mdb2glm.py",f"mdb2glm-{input_type}.py");
if os.path.exists(modname):

	import importlib, copy
	importlib.util.spec_from_file_location(input_type, modname);
	mod = importlib.import_module(f"mdb2glm-{input_type}");
	argv = copy.deepcopy(sys.argv)
	argv[0] = modname
	mod.convert(input_name,output_name)

else:

	raise Exception(f"type '{input_type}' is not valid -- {modname} not found");
