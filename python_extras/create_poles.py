"""Create poles for a GridLAB-D model

SYNTAX

Shell:

    bash$ gridlabd create_poles INPUTFILE OPTIONS ...

GLM:

    #gridlabd create_poles INPUTFILE OPTIONS ...

Python:

    >>> import create_poles
    >>> create_poles.main(INPUTFILE,OPTIONS ...)

Output options:

  --include_network                 include the input network in the output
                                    GLM file
  
  --output=GLMNAME                  set the output GLM file name
                                    (default is /dev/stdout)
  
  --format={GLM,JSON}               specify the output format (default is
                                    GLM)

Pole options:

  --ignore_length                   ignore the line length when computing pole
                                    locations
  
  --ignore_location                 ignore node latitude/longitude when
                                    computer pole locations
  
  --pole_type=CONFIGURATION_NAME    set the pole type to use
  
  --spacing=FEET                    set the pole spacing in feet on overhead
                                    power lines

Weather options:

  --weather=NAME                    use named weather object
  
  --location=LAT,LON                specify the weather location
  
  --year=YEAR                       specify the weather year
  
  --timezone=TZSPEC                 specify the timezone (overrides default
                                    based on location)
  
  --include_weather                 obtain weather data for the year specified
                                    or realtime forecast

DESCRIPTION

The `create_poles` subcommand automatically generates a pole model for a
network model and mounts the overhead lines and equipment to the newly
created poles.  The output is written to `/dev/stdout` unless the
`--output=GLMNAME` option is given.

The `--pole_type=CONFIGURATION_NAME` and `--spacing=FEET` options are
required.  Configuration names may be obtained from the
`pole_configuration.glm` library (see [[/Subcommand/Library]] for details on
using libraries.

Some network models include latitude and longitude information.  When this
information is present, the line length information checked.  If there is a
discrepancy between these, a warning is printed and the latitude/longitude
information is used.  The `--ignore_length` option will suppress this
warning. The `--ignore_location` warning will cause the model to use the line
length data instead.

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

WEATHER

If `--include_weather` is specified, then the weather forecast data is linked
based on location, if any, and the clock is automatically set based on the
weather window.  If the `--weather_name` option is provided, all poles
created will use the specified weather object instead of using downloaded
weather, and the clock will not set.  If the `--year` option is specified,
then the historical weather data for that year and location is used, and the
clock is set to run the entire year.  Without the `--year` specification, a
realtime weather forecast is used, and the clock is set to the forecast
window. By default the timezone is determined from the location, unless the
`--timezone=TZSPEC` option is used to override it. If `--ignore_location` is
specified, then the local system timezone specification is used.

SEE ALSO

  - Module powerflow pole and pole_mount
  - Subcommand nsrdb_weather and noaa_forecast
"""

import sys, os, json, datetime
from haversine import haversine, Unit
import nsrdb_weather, noaa_forecast

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
        output = (lambda x: print(x,file=sys.stdout))
    else:
        output = (lambda x: print(x,file=sys.stderr))
    if code == None:
        output(__doc__)
    else:
        output(f"Syntax: gridlabd create_poles INPUTFILE OPTIONS ...")
    if type(code) is int:
        exit(code)
    elif code != None:
        raise ValueError(f"error code '{code}' is not valid")

spacing = None
pole_type = None
weather_name = None
location = None
year = None
timezone = None
weather_locations = []

def get_timezone():
    """Get local timezone based on how datetime works"""
    tzlist = [
        "EST+5EDT",
        "CST+6CDT",
        "MST+7MDT",
        "PST+8PDT",
        "AST+9ADT",
        "HST+10HDT",
        ]
    now = datetime.datetime.now()
    tz = datetime.datetime.now(datetime.timezone.utc).astimezone().tzinfo.tzname(now)
    for tzspec in tzlist:
        if tz in tzspec:
            return tzspec
    return tz

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
        pole["weather"] = "weather@" + nsrdb_weather.geohash(*location)

    # try pole location, if any
    elif "latitude" in pole.keys() and "longitude" in pole.keys():
        lat = float(pole["latitude"].split()[0])
        lon = float(pole["longitude"].split()[0])
        if (lat,lon) not in weather_locations:
            weather_locations.append((lat,lon))
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
    global timezone
    global weather_locations
    ignore_length = False
    ignore_location = False
    include_network = False
    include_weather = False
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
        elif opt == "include_weather":
            include_weather = True
        elif opt == "output":
            outputfile = value
            output = open(outputfile,"wt")
        elif opt == "pole_type":
            pole_type = value
        elif opt == "weather":
            weather_name = value
        elif opt == "location":
            location = list(map(lambda x:float(x),value.split(",")))
            if not location in weather_locations:
                weather_locations.append(location)
        elif opt == "year":
            year = int(value)
        elif opt == "format":
            output_format = value
        elif opt == "timezone":
            timezone = value
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

    # add weather data and player
    weather_data = None
    weather
    if include_weather:

        # download NSRDB weather
        if not weather_locations:

            warning("cannot include whether because the model does not contain location information")

        elif year:

            for latlon in weather_locations:
                weather_name = "weather@" + nsrdb_weather.geohash(*latlon)
                weather_data = nsrdb_weather.getyears(year.split(","),*latlon)
                nsrdb_weather.writeglm(weather_data,glm=output,name=weather_name,csv=weather_name+".csv")

        # download NOAA forecast
        else:

            for latlon in weather_locations:
                weather_name = "weather@" + nsrdb_weather.geohash(*latlon)
                weather_data = noaa_forecast.getforecast(*latlon)
                noaa_forecast.writeglm(weather_data,glm=output,name=weather_name,csv=weather_name+".csv")

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
        
        # generate GLM clock
        if year:
            if not timezone:
                if not ignore_location:
                    # TODO: get timezone from location service
                    warning("location-based timezone is not implemented, timezone statically set to PST+8PDT")
                    timezone = get_timezone()
                else:
                    timezone = get_timezone()
            starttime = datetime.datetime(year,1,1,0,0,0).strftime("%Y-%m-%d %H:%M:%S")
            stoptime = datetime.datetime(year+1,1,1,0,0,0).strftime("%Y-%m-%d %H:%M:%S")
            print("clock",file=output)
            print("{",file=output)
            print(f"  timezone \"{timezone}\";",file=output)
            print(f"  starttime \"{starttime} {timezone[0:3]}\";",file=output)
            print(f"  stoptime \"{stoptime} {timezone[0:3]}\";",file=output)
            print("}",file=output)

        # generate GLM pole data
        for name,data in poles.items():
            write_object("pole",name,data,output)

        # generate weather and player

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
