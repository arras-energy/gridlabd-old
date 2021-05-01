import json 
import os 
import sys, getopt
from datetime import datetime 
from feeder import *

config = {"input":"tmy3","output":"glm","type":[]}

def help():
    print('Syntax:')
    print('tmy32glm.py -i|--ifile <input-file>[,<input-file>[,...]] -o|--ofile <output-file>')
    print('  -i|--ifile     : [REQUIRED] omd input file name.')
    print('  -o|--ofile     : [REQUIRED] glm output file name.')

input_file = None
output_file = None

opts, args = getopt.getopt(sys.argv[1:],"hi:o:",["help","ifile=","ofile="])

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
    else:
        raise Exception(f"'{opt}' is an invalid command line option")

with open(output_file,"w") as glm:
    glm.write(f"#weather get {input_file}\n")
    glm.write(f"module climate;\n")
    glm.write('object climate { tmyfile "')
    glm.write(input_file)
    glm.write('"; }\n')
