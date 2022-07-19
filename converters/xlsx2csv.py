import json 
import os 
import sys, getopt
from datetime import datetime 
import importlib, copy
from importlib import util

config = {
    "input" : "xlsx",
    "output" : "csv",
    "from" : ["workbook"],
    "type" : ["table"],
    }

def help():
    print('Syntax:')
    print(f'{config["input"]}2{config["output"]}.py -i|--ifile <input-file>[,<input-file>[,...]] -o|--ofile <output-file> [options ...]')
    print(f'  -c|--config    : [OPTIONAL] display converter configuration')
    print(f'  -i|--ifile     : [REQUIRED] {config["input"]} input file name')
    print(f'  -o|--ofile     : [REQUIRED] {config["output"]} output file name')
    print(f'  -f|--from      : [REQUIRED] input {config["input"]} data type')
    print(f'  -t|--type      : [REQUIRED] output {config["output"]} data type')
    print(f'  -r|--read      : [OPTIONAL] set converter read option')
    print(f'  -w|--write     : [OPTIONAL] set converter write option')

def error(msg):
    print(f'ERROR    [{config["input"]}2{config["output"]}]: {msg}')
    sys.exit(1)

input_file = None
input_type = None
output_file = None
output_type = None
options = {}

opts, args = getopt.getopt(sys.argv[1:],"hci:o:f:t:r:w:",["help","config","ifile=","ofile=","from=","type=","read=","write="])

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
        input_type = arg.strip()
    elif opt in ("-t","--type"):
        output_type = arg.strip()
    elif opt in ("-r","--read"):
        spec = arg.split("=")
        if len(spec) == 1:
            options[f"read.{arg}"] = True
        elif len(spec) == 2:
            options[f"read.{spec[0]}"] = spec[1]
        else:
            options[f"read.{spec[0]}"] = "=".join(spec[1:])
    elif opt in ("-w","--write"):
        spec = arg.split("=")
        if len(spec) == 1:
            options[f"write.{arg}"] = True
        elif len(spec) == 2:
            options[f"write.{spec[0]}"] = spec[1]
        else:
            options[f"write.{spec[0]}"] = "=".join(spec[1:])
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
    modspec = util.spec_from_file_location(output_type, f"{modname}.py")
    mod = importlib.import_module(f'{config["input"]}-{input_type}2{config["output"]}-{output_type}')
    mod.convert(input_file,output_file,options)
else:
    error(f"{modname} not found")
