import json 
import os 
import sys, getopt
from datetime import datetime 
import importlib, copy

config = {"input":"csv","output":"glm","from":["ami","scada"],"type":["ceus","rbsa"]}

def help():
    print('Syntax:')
    print('omd2glm.py -i|--ifile <input-file>[,<input-file>[,...]] -o|--ofile <output-file>')
    print('  -c|--config    : [OPTIONAL] output converter configuration')
    print('  -i|--ifile     : [REQUIRED] omd input file name.')
    print('  -o|--ofile     : [REQUIRED] glm output file name.')
    print('  -f|--from      : [REQUIRED] input csv file type.')
    print('  -t|--type      : [REQUIRED] output glm object type.')

def error(msg):
    print(f"ERROR    [csv2glm]: {msg}")
    sys.exit(1)

input_file = None
input_type = None
output_file = None
output_type = None
options = {}

opts, args = getopt.getopt(sys.argv[1:],"hci:o:f:t:p:",["help","config","ifile=","ofile=","type=","from=","property="])

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
    elif opt in ("-f","--from"):
        input_type = arg.strip();
    elif opt in ("-t","--type"):
        output_type = arg.strip();
    elif opt in ("-p","--property"):
        prop = arg.split("=")
        options[prop[0]] = prop[1]
    else:
        error(f"{opt}={arg} is not a valid option");

if input_file == None:
    error("missing input file")
elif output_file == None:
    error("missing output file")
elif input_type == None:
    error("missing input type")
elif output_type == None:
    error("missing output type")

modname = sys.argv[0].replace("csv2glm.py",f"csv-{input_type}2glm-{output_type}.py")
if os.path.exists(modname):
    modspec = importlib.util.spec_from_file_location(output_type, f"{modname}.py")
    mod = importlib.import_module(f"csv-{input_type}2glm-{output_type}")
    mod.convert(input_file,output_file,options)
else:
    error(f"{modname} not found")
