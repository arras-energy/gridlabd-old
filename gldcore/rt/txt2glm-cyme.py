import json 
import os 
import sys, getopt
from datetime import datetime 

config = {"input":"txt", "output":"glm", "type":[]}

def help():
	return """txt2glm-cyme.py -i|--ifile <input-file>[,<input-file>[,...]] -o|--ofile <output-file>
    -i|--ifile     : [REQUIRED] txt input file name
    -o|--ofile     : [REQUIRED] glm output file name
    """

def convert(input_file=None, output_file=None, output_type=None):

	if not input_file:
		raise Exception(f"missing input file name")

	if not output_file:
		raise Exception(f"missing output file name")

	with open(input_file,"r") as txt:
		with open(output_file,"w") as glm:

			glm.write(f"#warning CYME conversion from {input_file} incomplete\n")

if __name__ == '__main__':
	
	opts, args = getopt.getopt(sys.argv[1:],"hi:o:t:",["help","ifile=","ofile=","type="]);

	if not opts : 
		print(help())
		sys.exit(0)
	for opt, arg in opts:
		if opt in ("-h","--help"):
			print(help())
			sys.exit(0);
		elif opt in ("-i", "--ifile"):
			input_file = arg.strip()
		elif opt in ("-o", "--ofile"):
			output_file = arg.strip()
		elif opt in ("-t", "--type"):
			output_type = arg.strip()
		else:
			raise Exception(f"'{opt}' is an invalid command line option");

	convert(input_file=input_file,output_file=output_file)