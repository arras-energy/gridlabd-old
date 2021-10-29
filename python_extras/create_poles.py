"""Create poles for a GridLAB-D model

SYNTAX

Shell:

    bash$ gridlabd create_poles INPUTFILE OPTIONS ...

GLM:

    #gridlabd create_poles INPUTFILE OPTIONS ...

Python:

    >>> import create_poles
    >>> create_poles.main(INPUTFILE,OPTIONS ...)

DESCRIPTION

The `create_poles` subcommand automatically generates a pole model for a network model and
mounts the overhead lines and equipment to the newly created poles.  The output is written to
`/dev/stdout` unless the `--output=GLMNAME` option is given.

The `--pole_type=CONFIGURATION_NAME` and `--spacing=FEET` options are required.  Configuration names
may be obtained from the `pole_configuration.glm` library (see [[/Subcommand/Library]] for details
on using libraries.

Some network models include latitude and longitude information.  When this information is present,
the line length information checked.  If there is a discrepancy between these, a warning is printed
and the latitude/longitude information is used.  The `--ignore_length` option will suppress this
warning. The `--ignore_location` warning will cause the model to use the line length data instead.

The `--include_network` adds a `#include "FILENAME"` directive in the output to ensure that the 
resulting GLM file contains all the objects required to run the simulation, e.g.,

  $ gridlabd create_poles example.glm --output=model.glm --spacing=100 --pole_type=WOOD-EC-45/4 --weather=example
  $ gridlabd example.glm model.glm

Alternative, when the input is a GLM, the two GLM files can be used together in a single command, e.g.,

  $ gridlabd create_poles example.glm --output=model.glm  --spacing=100 --pole_type=WOOD-EC-45/4 --include_network --weather=example
  $ gridlabd model.glm

The python usage requires the options be provided as a keyword arguments where the leading `--` is
omitted, e.g., the command 

  $ gridlabd create_poles example.glm --output=model.glm  --spacing=100 --pole_type=WOOD-EC-45/4 --include_network  --weather=example

is equivalent to

  >>> gridlabd python
  >>> import create_poles
  >>> create_poles.main('example.glm',output='model.glm',spacing=100,pole_type='WOOD-EC-45/4',include_network=True,weather=example)

SEE ALSO

  - Module powerflow pole and pole_mount
  - Subcommand nsrdb_weather and noaa_forecast
"""

import sys, os, json, datetime
from haversine import haversine, Unit
import nsrdb_weather

def error(msg,code=None):
    """Display error message and exit with code"""
    print(f"ERROR [create_poles]: {msg}",file=sys.stderr)
    if code == None:
        return
    if type(code) is int:
        exit(code)
    raise Exception(f"exit code '{code}' is not valid")

def warning(msg):
    """Display a warning message"""
    print(f"WARNING [create_poles]: {msg}",file=sys.stderr)

def syntax(code=None):
    """Display syntax/help message

    Parameters:

        - code (int or None)  Output to stdout if evaluates to False, otherwise output to stderr. 
                              If None, output full help, otherwise output only syntax line.
                              If integer, exit with code

    Exceptions:

        - ValueError          Type of code is not int or None
    """
    if not code:
        output = sys.stdout
    else:
        output = sys.stderr
    print(f"Syntax: gridlabd create_poles INPUTFILE [OPTIONS ...]",file=output)
    if code == None:
        print("Output options:",file=output)
        print("  --include_network                 include the input network in the output GLM file",file=output)
        print("  --output=GLMNAME                  set the output GLM file name (default is /dev/stdout)",file=output)
        print("  --format={GLM,JSON}               specify the output format (default is GLM)",file=output)
        print("Pole options:",file=output)
        print("  --ignore_length                   ignore the line length when computing pole locations",file=output)
        print("  --ignore_location                 ignore node latitude/longitude when computer pole locations",file=output)
        print("  --pole_type=CONFIGURATION_NAME    set the pole type to use",file=output)
        print("  --spacing=FEET                    set the pole spacing in feet on overhead power lines",file=output)
        print("Weather options:",file=output)
        print("  --weather=NAME                    use named weather object",file=output)
        print("  --location=LAT,LON                specify the weather location",file=output)
        print("  --year=YEAR                       specify the weather year (default is forecasted)",file=output)
    if type(code) is int:
        exit(code)
    elif code != None:
        raise ValueError(f"error code '{code}' is not valid")

spacing = None
pole_type = None
weather_name = None
location = None
year = None

def get_pole(model,name):
    """Find (and possibly create) specified pole in the model"""
    global pole_type
    if name not in model["objects"]:
        
        # create pole object
        model["objects"][name] = {
            "configuration":pole_type,
        }
    pole = model["objects"][name]

    # add weather link
    if weather_name:

        # link weather based on name given
        pole["weather"] = weather_name

    # try location, if specified
    elif location:

        # link weather based on location
        lat,lon = list(map(lambda x:float(x),location.split(",")))
        pole["weather"] = "weather@" + nsrdb_weather.geohash(lat,lon)

    # try pole location, if any
    elif "latitude" in pole.keys() and "longitude" in pole.keys():
        lat = float(pole["latitude"].split()[0])
        lon = float(pole["longitude"].split()[0])
        pole["weather"] = "weather@" + nsrdb_weather.geohash(lat,lon)

    # no weather
    else:
        warning(f"unable to identify weather for pole '{name}'")

    return model["objects"][name]

def mount_line(model,pole,line,position):
    """Connect line to pole"""
    global spacing
    poledata = get_pole(model,pole)
    poledata[position] = {"class":"pole_mount","equipment":line,"pole_spacing":f"{spacing} ft"}
    return poledata

def write_object(otype,name,data,output,indent_level=0):
    """Write object data in GLM"""
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
    """Main pole creation function"""
    # options
    global spacing
    global pole_type
    global location
    global weather_name
    global year
    ignore_length = False
    ignore_location = False
    include_network = False
    outputfile = "/dev/stdout"
    output_format = "GLM"
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
        elif opt == "weather":
            weather_name = value
        elif opt == "location":
            location = value
        elif opt == "year":
            year = value
        elif opt == "format":
            output_format = value
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

        # overhead line
        if data["class"] in ["overhead_line"]:
            length = float(data["length"].split()[0])
            fromname = data["from"]
            toname = data["to"]
            fromdata = objects[fromname]
            todata = objects[toname]

            # try to use lat/lon
            if not ignore_location:

                # lat/lon found
                if "latitude" in fromdata.keys() and \
                    "longitude" in fromdata.keys() and \
                    "latitude"  in todata.keys() and \
                    "longitude" in todata.keys():

                    # length of line based on lat/lon
                    dist = haversine([float(fromdata["latitude"]),float(fromdata["longitude"])],
                        [float(todata["latitude"]),float(todata["longitude"])],Unit.FEET)
                
                    # check length
                    if dist != length:

                        # if ignore line length
                        if ignore_length:

                            # use length based on lat/lon
                            length = dist

                        # don't ignore length
                        else:

                            # print a warning
                            warning(f"overhead_line '{name}' length '{float(data['length'].split()[0])}' does not match distance {length} from '{fromname}' to '{toname}' ")

            # place first pole
            poles[f"pole_{fromname}"] = mount_line(model,f"pole_{fromname}",name,f"mount_{name}_{fromname}")

            # place intermediate poles
            for position in range(int(spacing),int(length),int(spacing)):
                poles[f"pole_{name}_{position}"] = mount_line(model,f"pole_{name}_{position}",name,f"mount_{name}_{position}")

            # place last pole
            poles[f"pole_{toname}"] = mount_line(model,f"pole_{toname}",name,f"mount_{name}_{toname}")

    # write JSON output
    if outputfile.endswith(".json") or output_format == "JSON":
        json.dump(model,output,indent=4)

    # write GLM output
    elif outputfile.endswith(".glm") or output_format == "GLM":

        # generate GLM data from model
        print(f"// automatically generated model from command `{' '.join(sys.argv)}` on {datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S %z')}",file=output)
        if include_network:
            print(f"#include \"{inputfile}\"",file=output)
        print("#library get pole_configuration.glm",file=output)
        print("#include \"${GLD_ETC}/library/${country}/${region}/${organization}/pole_configuration.glm\"",file=output)
        for name,data in poles.items():
            write_object("pole",name,data,output)

    else:
        error(f"output format '{output_format}' is not valid",1)

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
