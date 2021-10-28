"""Create poles for a GridLAB-D model

SYNTAX

Shell:

    bash$ gridlabd create_poles INPUTFILE [OPTIONS ...]

GLM:

    #gridlabd create_poles INPUTFILE [OPTIONS ...]

Python:

    >>> import create_poles
    >>> create_poles.main(v[,OPTIONS ...])

DESCRIPTION

EXAMPLE

The following example creates poles spaced 100 feet apart in the IEEE 13 bus model

    $ gridlabd create_poles python_extras/example/ieee13.glm

SEE ALSO

    - Module powerflow pole and pole_mount
"""

import sys, os, json, datetime
from haversine import haversine, Unit

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
    if not code:
        output = sys.stdout
    else:
        output = sys.stderr
    print(f"Syntax: gridlabd create_poles INPUTFILE [OPTIONS ...]",file=output)
    if code == None:
        print("Options:",file=output)
        print("  --ignore_length                   ignore the line length when computing pole locations")
        print("  --ignore_location                 ignore node latitude/longitude when computer pole locations")
        print("  --include_network                 include the input network in the output GLM file")
        print("  --output=GLMNAME                  set the output GLM file name (default is /dev/stdout)")
        print("  --pole_type=CONFIGURATION_NAME    set the pole type to use")
        print("  --spacing=NUMBER                  set the pole spacing in feet on overhead power lines")
    if type(code) is int:
        exit(code)
    elif code != None:
        raise Exception(f"error code '{code}' is not valid")

spacing = None
pole_type = None

def get_pole(model,name):
    global pole_type
    if name not in model["objects"]:
        # print(f"adding pole {name}")
        model["objects"][name] = {
            "configuration":pole_type,
        }
    return model["objects"][name]

def mount_line(model,pole,line,position):
    global spacing
    poledata = get_pole(model,pole)
    poledata[position] = {"class":"pole_mount","equipment":line,"pole_spacing":f"{spacing} ft"}
    return poledata

def write_object(otype,name,data,output,indent_level=0):
    indent = '    '*indent_level
    print(indent+f"object {otype}",file=output)
    print(indent+"{",file=output)
    print(indent+f"    name \"{name}\";",file=output)
    for item,value in data.items():
        if item in ["class"]:
            continue
        if type(value) is dict:
            if "class" in value.keys():
                write_object(value["class"],item,value,output,indent_level+1)
        else:
            print(indent+f"    {item} \"{value}\";",file=output)
    print(indent+"}"+(';'*min(indent_level,1)),file=output)

def main(inputfile,**options):

    # options
    global spacing
    global pole_type
    ignore_length = False
    ignore_location = False
    include_network = False
    outputfile = "/dev/stdout"
    output = sys.stdout
    for opt,value in options.items():
        if opt == "spacing":
            spacing = float(value)
        elif opt == "ignore_length":
            ignore_length = True
        elif opt == "ignore_location":
            ignore_location = True
        elif opt == "include_network":
            include_network = True
        elif opt == "output":
            outputfile = value
            output = open(outputfile,"wt")
        elif opt == "pole_type":
            pole_type = value
        else:
            raise Exception(f"options '{opt}={value}' is not valid")    
    if spacing == None:
        raise Exception("option for spacing is required")
    if pole_type == None:
        raise Exception("option for pole_type is required")

    # input
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

    # process overhead lines in model
    objects = model["objects"]
    poles = {}
    for name in list(objects.keys()):
        data = model["objects"][name]
        if data["class"] in ["overhead_line"]:
            fromname = data["from"]
            toname = data["to"]
            if not ignore_location:
                fromdata = objects[fromname]
                todata = objects[toname]
                if "latitude" in fromdata.keys() and \
                    "longitude" in fromdata.keys() and \
                    "latitude"  in todata.keys() and \
                    "longitude" in todata.keys():
                    length = haversine([float(fromdata["latitude"]),float(fromdata["longitude"])],
                        [float(todata["latitude"]),float(todata["longitude"])],Unit.FEET)
                else:
                    length = None
                if not ignore_length and length != float(data["length"].split()[0]):
                    warning(f"overhead_line '{name}' length '{float(data['length'].split()[0])}' does not match distance {length} from '{fromname}' to '{toname}' ")
            elif not ignore_length:
                length = float(data["length"].split()[0])
            else:
                length = None
                warning(f"overhead_line '{name}' has no length information")
            poles[f"pole_{fromname}"] = mount_line(model,f"pole_{fromname}",name,f"mount_{name}_{fromname}")
            for position in range(int(spacing),int(length),int(spacing)):
                poles[f"pole_{name}_{position}"] = mount_line(model,f"pole_{name}_{position}",name,f"mount_{name}_{position}")
            poles[f"pole_{toname}"] = mount_line(model,f"pole_{toname}",name,f"mount_{name}_{toname}")
    print(f"// automatically generated model from command `{' '.join(sys.argv)}` on {datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S %z')}",file=output)
    if include_network:
        print(f"#include \"{inputfile}\"",file=output)
    print("#library get pole_configuration.glm",file=output)
    print("#include \"${GLD_ETC}/library/${country}/${region}/${organization}/pole_configuration.glm\"",file=output)
    for name,data in poles.items():
        write_object("pole",name,data,output)
    return

if __name__ == "__main__":
    if len(sys.argv) == 1:
        syntax(1)
    glmfile = None
    options = {}
    debug = False
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
        elif token in ["--debug"]:
            debug = not debug
        elif token.endswith(".glm") or token.endswith(".json"):
            glmfile = token
        elif token.startswith("--"):
            options[token[2:]] = value
        else:
            error(f"'{arg}' is not valid",1)
    if glmfile:
        try:
            main(glmfile,**options)
        except Exception as msg:
            if debug:
                raise
            else:
                error(msg,1)
