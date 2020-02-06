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

	sections = get_sections(input_file)
	print(sections)

	with open(output_file,"w") as glm:

		glm.write(f"#warning CYME conversion from {input_file} incomplete\n")

def get_sections(input_file):
	sections = {}
	with open(input_file,"r") as txt:
		text = txt.read().split("\n\n")
		for section in text:
			lines = section.splitlines()
			heading = lines[0]
			if heading[0] != "[" or heading[-1] != "]":
				raise Exception(f"{heading} is an invalid section heading")
			heading_name = heading[1:-1]
			if len(lines) > 1:
				if lines[1][0:6] == "FORMAT":
					# CSV data
					header = lines[1].split("=")[1].split(",")
					name = header[0]
					tags = header[1:]
					values = {}
					for line in lines[2:]:
						values.zip(header,line.split()[1])
					data["values"] = values
					sections[heading_name] = data
				else:
					# tuples
					sections[heading_name] = []
			else:
				sections[heading_name] = []
	return sections

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
	