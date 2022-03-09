import json 
import os 
import sys, getopt
import zipfile as zipf

config = {"input":"json","output":"zip"}

method = zipf.ZIP_STORED
# options:
# ZIP_STORED (default)
# ZIP_DEFLATED (requires zlib)
# ZIP_BZIP2 (requires bz2)
# ZIP_LZMA (requires lzma)

def help():
	return """json2zip.py -i <input-file> -o <output-file>
    -c|--config              output converter configuration data
    -h|--help                output this help
    -i|--ifile <filename>    [REQUIRED] JSON input file
    -o|--ofile <filename>    [OPTIONAL] ZIP output file name
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
	if input_name[-5:] != ".json":
		raise Exception(f"input file '{input_name}' is not a JSON file")
	output_name = input_name[0:-5] + ".zip"
	path = os.path.dirname(os.path.abspath(input_name))
	os.chdir(path)
	dirs = os.listdir(".")
	if dirs:
		zip = zipf.ZipFile(output_name,"w")
		for file in dirs:
			if file != output_name:
				zip.write(file)
	
if __name__ == '__main__':
	main()
