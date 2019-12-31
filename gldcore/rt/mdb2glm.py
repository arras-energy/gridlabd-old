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
	print('  cyme           : cyme conversion');

filename_mdb = None
filename_glm = None
basename = None
input_type = None

try : 
	opts, args = getopt.getopt(sys.argv[1:],"hi:o:t",["help","ifile=","ofile=","type="])
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
		filename_mdb.append(arg)
	elif opt in ("-o", "--ofile"):
		filename_glm = arg
	elif opt in ("-t","--type"):
		output_type = arg
	else:
		raise Exception("'%s' is an invalid command line option" % opt)

modname = sys.argv[0].replace("mdb2glm.py","mdb2glm-%s.py"%output_type)
if os.path.exists(modname):

	import importlib, copy
	modspec = importlib.util.spec_from_file_location(output_type, modname)
	mod = importlib.import_module("mdb2glm-%s"%output_type)
	argv = copy.deepcopy(sys.argv)
	argv[0] = modname
	mod.main(argv)

else:

	raise Exception("type '%s' is not valid" % output_type)
