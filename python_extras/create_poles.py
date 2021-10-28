"""Create poles for a GridLAB-D model

SYNTAX

Shell:

    bash$ gridlabd create_poles INPUTFILE [OPTIONS ...]

GLM:

    #gridlabd create_poles INPUTFILE [OPTIONS ...]

Python:

    >>> import create_poles
    >>> create_poles.main(v[,OPTIONS ...])
"""

import sys, os, json

def error(msg,code=None):
    print(f"ERROR [create_poles]: {msg}",file=sys.stderr)
    if code == None:
        return
    if type(code) is int:
        exit(code)
    raise Exception(f"exit code '{code}' is not valid")

def warning(msg):
    print(f"WARNING [create_poles]: {msg}",file=sys.stderr)

def syntax(code=None):
    if code == 0:
        output = sys.stdout
    else:
        output = sys.stderr
    print(f"Syntax: gridlabd create_poles INPUTFILE [OPTIONS ...]",file=output)
    if type(code) is int:
        exit(code)
    raise Exception(f"error code '{code}' is not valid")

def main(inputfile,**options):
    if inputfile.endswith(".glm"):
        glmfile = inputfile
        jsonfile = inputfile.replace(".glm",".json")
        code = os.system(f"gridlabd -C {glmfile} -o {jsonfile}")
        if code != 0:
            error(f"unable to compile '{glmfile}' into {jsonfile} (error code {code})",2)
    elif inputfile.endswith(".json"):
        jsonfile = inputfile
    else:
        error(f"main(inputfile='{inputfile}',options={options}): inputfile type not recognized",1)

    with open(jsonfile,"r") as f:
        model = json.load(f)

    objects = model["objects"]
    for name,data in objects.items():
        if data["class"] in ["overhead_line"]:
            fromname = data["from"]
            fromdata = objects[fromname]
            toname = data["to"]
            todata = objects[toname]
            if "latitude" not in fromdata.keys() or "longitude" not in fromdata.keys() or "latitude" not in todata.keys() or "longitude" not in todata.keys():
                warning(f"overhead_line '{name}' is missing location of either '{fromname}' or '{toname}' ")

if __name__ == "__main__":
    if len(sys.argv) == 1:
        syntax(1)
    glmfile = None
    options = {}
    for arg in sys.argv[1:]:
        args = arg.split("=")
        if type(args) is list and len(args) > 1:
            token = args[0]
            value = args[1]
        elif type(args) is list:
            token = args[0]
            value = None
        else:
            token = args
            value = None
        if token in ["-h","--help","help"]:
            syntax()
        elif token.endswith(".glm") or token.endswith(".json"):
            glmfile = token
        elif token.startswith("--"):
            options[token[2:]] = value
        else:
            error(f"'{arg}' is not valid",1)
    if glmfile:
        main(glmfile,**options)
