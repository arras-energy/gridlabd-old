import json 
import os 
import sys, getopt
from datetime import datetime 

def help():
	print('Syntax:')
	print('txt2glm-cyme.py -i|--ifile <input-file>[,<input-file>[,...]] -o|--ofile <output-file>')
	print('  -i|--ifile     : [REQUIRED] txt input file name.')
	print('  -o|--ofile     : [REQUIRED] glm output file name.')

filename_txt = [];
filename_glm = None;

try : 
	opts, args = getopt.getopt(sys.argv[1:],"hi:o:t",["help","ifile=","ofile=","type="]);
except getopt.GetoptError:
	sys.exit(2);
if not opts : 
	help();
	sys.exit(1);
for opt, arg in opts:
	if opt in ("-h","--help"):
		help();
		sys.exit(0);
	elif opt in ("-i", "--ifile"):
		filename_txt.append(arg.split(","))
	elif opt in ("-o", "--ofile"):
		filename_glm = arg;
	elif opt in ("-t","--type"):
		output_type = arg;
	else:
		raise Exception(f"'{opt}' is an invalid command line option");

if filename_txt == []:
	raise Exception(f"missing input file name");

if filename_glm == None:
	raise Exception(f"missing output file name");

with open(filename_glm,"w") as glm:
	for input_name in filename_txt:
		with open(input_name,"r") as txt:

			# TODO process input
			raise Exception(f"conversion of '{input_name}' to '{filename_glm}' not implemented yet");

