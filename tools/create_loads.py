"""Create loads

Syntax:

    $ gridlabd create_loads NETWORK_FILE.[glm,json] -o LOAD_FILE.[glm,json] [OPTIONS ...]

    Options:

      -d|--data LOAD_DATA.csv
      -t|--type LOAD_TYPE

The `create_loads` tool generates a load model for a network model.

"""

import os, sys
import json

if len(sys.argv) == 1:
    print("Syntax: gridlabd create_loads NETWORK_FILE.[glm,json] -o LOAD_FILE.[glm,json] [OPTIONS ...]",file=sys.stderr)
    exit(1)

NETWORK_FILE = sys.argv[1]


if os.path.splitext(NETWORK_FILE)[1] == ".glm":
    os.system(f"gridlabd -C '{NETWORK_FILE}' -o '{os.path.splitext(NETWORK_FILE)[0]}.json'")
    NETWORK_FILE = os.path.splitext(NETWORK_FILE)[0] + ".json"

with open(NETWORK_FILE,"r") as fh:
    data = json.load(fh)

for obj, data in data['objects'].items():
    if data['class'] == 'load':
        for key, value in data.items():
            if key.startswith('constant_') and not key.endswith('_real') and not key.endswith('_reac'):
                load = complex(value.split()[0])
                if load != 0j:
                    print(obj,key,load)
