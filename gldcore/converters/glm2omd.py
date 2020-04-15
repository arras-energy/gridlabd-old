import json 
import os 
import sys, getopt
from datetime import datetime 
from feeder import *

config = {"input":"glm","output":"omd","type":[]}

def help():
    print('Syntax:')
    print('glm2omd.py -i|--ifile <input-file>[,<input-file>[,...]] -o|--ofile <output-file>')
    print('  -c|--config    : [OPTIONAL] output converter configuration')
    print('  -i|--ifile     : [REQUIRED] glm input file name.')
    print('  -o|--ofile     : [REQUIRED] omd output file name.')

input_file = None
output_file = None
output_type = None

opts, args = getopt.getopt(sys.argv[1:],"hci:o:",["help","config","ifile=","ofile="])

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
    else:
        raise Exception(f"'{opt}' is an invalid command line option")

glmToOmd(input_file,output_file)