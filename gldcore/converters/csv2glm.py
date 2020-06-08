import json 
import os 
import sys, getopt
from datetime import datetime 
import importlib, copy

config = {"input":"csv","output":"glm","from":["ami","scada","onpoint-weather"],"type":["ceus","rbsa","climate"]}

def help():
    print('Syntax:')
    print(f'{config["input"]}2{config["output"]}.py -i|--ifile <input-file>[,<input-file>[,...]] -o|--ofile <output-file> [options ...]')
    print(f'  -c|--config    : [OPTIONAL] display converter configuration')
    print(f'  -i|--ifile     : [REQUIRED] {config["input"]} input file name')
    print(f'  -o|--ofile     : [REQUIRED] {config["output"]} output file name')
    print(f'  -f|--from      : [REQUIRED] input {config["input"]} data type')
    print(f'  -t|--type      : [REQUIRED] output {config["output"]} data type')
    print(f'  -p|--property  : [OPTIONAL] property option')

def error(msg):
    print(f'ERROR    [{config["input"]}2{config["output"]}]: {msg}')
    sys.exit(1)

input_file = None
input_type = None
output_file = None
output_type = None
options = {}

opts, args = getopt.getopt(sys.argv[1:],"hci:o:f:t:p:",["help","config","ifile=","ofile=","from=","type=","property="])

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
    error("missing input file name")
elif output_file == None:
    error("missing output file name")
elif input_type == None:
    error("missing input data type")
elif output_type == None:
    error("missing output data type")

modname = sys.argv[0].replace(f'{config["input"]}2{config["output"]}.py',f'{config["input"]}-{input_type}2{config["output"]}-{output_type}.py')
if os.path.exists(modname):
    modspec = importlib.util.spec_from_file_location(output_type, f"{modname}.py")
    mod = importlib.import_module(f'{config["input"]}-{input_type}2{config["output"]}-{output_type}')
    mod.convert(input_file,output_file,options)
else:
    error(f"{modname} not found")
