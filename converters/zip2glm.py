import json 
import os 
import sys, getopt
import zipfile as zipf

config = {"input":"zip","output":"glm"}

def help():
	return """zip2glm.py -i <input-file> -o <output-file>
    -c|--config              output converter configuration data
    -h|--help                output this help
    -i|--ifile <filename>    [REQUIRED] ZIP input file
    -o|--ofile <filename>    [OPTIONAL] GLM output file name
"""

def main():
	input_name = None
	output_name = None
	try : 
		opts, args = getopt.getopt(sys.argv[1:],"chi:o:",["config","help","ifile=","ofile="])
	except getopt.GetoptError:
		sys.exit(2)
	if not opts : 
		print('Missing command arguments')
		sys.exit(2)
	for opt, arg in opts:
		if opt in ("-c","--config"):
			print(config)
			sys.exit(0)
		elif opt in ("-h","--help"):
			print(help())
			sys.exit(0)
		elif opt in ("-i", "--ifile"):
			input_name = arg
		elif opt in ("-o", "--ofile"):
			output_name = arg
		else:
			raise Exception("option '{opt}={arg}' is not valid");

	if input_name == None:
		raise Exception("input filename not specified")
	zip = zipf.ZipFile(input_name,mode="r");
	if output_name == None:
		output_name = input_name[0:-4] + ".glm"
	print(output_name)
	print(zip.namelist())
	if not output_name in zip.namelist():
		raise Exception(f"output filename '{output_name}' not in '{input_name}'")
	zip.extractall(path=".")

if __name__ == '__main__':
	main()
