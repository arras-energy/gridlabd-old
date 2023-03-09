import json 
import os 
import sys, getopt
from datetime import datetime 

config = {"input":"tmy3","output":"glm","type":[]}

def help():
    print('Syntax:')
    print('tmy32glm.py -i|--ifile <input-file>[,<input-file>[,...]] -o|--ofile <output-file> -p|--property <name>=<value>')
    print('  -i|--ifile     : [REQUIRED] omd input file name.')
    print('  -o|--ofile     : [REQUIRED] glm output file name.')
    print('  -p|--property  : [OPTIONAL] specify additional GLM object properties')

input_file = None
output_file = None
properties = {}

opts, args = getopt.getopt(sys.argv[1:],"hi:o:p:",["help","ifile=","ofile=","property="])

if not opts : 
    help()
    sys.exit(1)
for opt, arg in opts:
    if opt in ("-h","--help"):
        help()
        sys.exit(0)
    elif opt in ("-i", "--ifile"):
        input_file = arg.strip()
    elif opt in ("-o", "--ofile"):
        output_file = arg.strip()
    elif opt in ("-p", "--property"):
        spec = arg.split("=")
        properties[spec[0]] = spec[1]
    else:
        raise Exception(f"'{opt}' is an invalid command line option")

with open(output_file,"w") as glm:
    glm.write(f"#weather get {input_file}\n")
    glm.write("module climate;\n")
    glm.write("object climate\n")
    glm.write("{\n")
    glm.write(f"\ttmyfile \"{input_file}\";\n")
    for name, value in properties.items():
        glm.write(f"\t{name} \"{value}\";\n")
    glm.write("}\n")
