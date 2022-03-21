import json
import os 
import sys, getopt
from datetime import datetime 
import importlib, copy
from importlib import util

config = {
    "input" : "mdb",
    "output" : "glm",
    "type" : ["cyme","table"],
    "format" : ["player","object"],
    "options" : {},
}

def help():
    print('Syntax:')
    print('mdb2glm.py -i|--ifile <input-file> -o|--ofile <output-file> -t|--type <input-type> -f|--format <output-type> -p <name>=<value> ...')
    print('  -c|--config    : [OPTIONAL] print converter configuration')
    print('  -i|--ifile     : [REQUIRED] mdb input file name.')
    print('  -o|--ofile     : [REQUIRED] glm output file name.')
    print('  -t|--type      : [REQUIRED] specify input type')
    print('  -f|--format    : [OPTIONAL] specify output format')
    print('  -p|--property  : [OPTIONAL] specify a converter option/property')
    print('Input types')
    print('  cyme           : cyme input');

input_name = None
output_name = None
input_type = None
output_type = ""
options = {}
try : 
    opts, args = getopt.getopt(sys.argv[1:],"hi:o:t:cf:p:",["help","ifile=","ofile=","type=","config","format","property"])
except getopt.GetoptError:
    print("ERROR [mdb2glm]: command line options not valid")
    sys.exit(2)

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
        input_name = arg.strip()
    elif opt in ("-o", "--ofile"):
        output_name = arg.strip()
    elif opt in ("-t","--type"):
        if arg in config["type"]:
            input_type = arg
        else:
            print(f"ERROR [mdb2glm]: '{arg}'' is not a valid input data type")
            sys.exit(1)
    elif opt in ("-f","--format"):
        if arg in config["format"]:
            output_type = arg
        else:
            print(f"ERROR [mdb2glm]: '{arg}'' is not a valid output format")
            sys.exit(1)
    elif opt in ("-p","--property"):
        spec = arg.split("=")
        if len(spec) == 1:
            options[spec[0]] = True
        else:
            options[spec[0]] = "=".join(spec[1:])

if input_type is None:
    print("ERROR [mdb2glm]: conversion type not specified (-t <type> option is missing)")
    sys.exit(1)
if output_type:
    output_type = "-" + output_type 

modname = sys.argv[0].replace("mdb2glm.py",f"mdb-{input_type}2glm{output_type}.py")
if os.path.exists(modname):
    util.spec_from_file_location(input_type, modname)
    mod = importlib.import_module(f"mdb-{input_type}2glm{output_type}")
    argv = copy.deepcopy(sys.argv)
    argv[0] = modname
    try:
        mod.convert(input_name,output_name,options)
    except:
        import traceback
        print(f"ERROR [mdb2glm]: {traceback.print_exc()}")
        sys.exit(3)
else:
    print(f"ERROR [mdb2glm]: type '{input_type}' is not valid -- {modname} not found");
    sys.exit(2)