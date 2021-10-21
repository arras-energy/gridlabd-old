"""NOAA weather forecast tool

SYNOPSIS

Shell:
    bash$ export PYTHONPATH=/usr/local/share/gridlabd
    bash$ python3 -m noaa_forecast -p|-position=LAT,LON [-g|--glm=GLMNAME] 
        [-n|--name=OBJECTNAME] [-c|--csv=CSVNAME] [--test] [-h|--help|help]

GLM:
    #system python3 ${GLD_ETC}/nsrbd_weather.py -p|-position=LAT,LON [-g|--glm=GLMNAME] 
        [-n|--name=OBJECTNAME] [-c|--csv=CSVNAME] [--test] [-h|--help|help]
    #include "GLMNAME"

Python:
    >>> import sys
    >>> sys.path.append("/usr/local/share/gridlabd")
    >>> import nsrdb_weather as ns
    >>> data = ns.getforecast(LAT,LON)
    >>> ns.writeglm(data,GLMNAME,OBJECTNAME,CSVNAME)

DESCRIPTION

This module downloads weather forecasts from NOAA and writes GLM files.  This can be done from
the command line or using call the python API.

PARAMETERS

The module uses several parameters to control its behavior. 

    server = "https://api.weather.gov/points/{latitude},{longitude}"
    float_format="%.2f"

EXAMPLE

The following command downloads only the CSV data for a location:

    bash$ python3 /usr/local/share/gridlabd/noaa_forecast.py -p=45.62,-122.70 -c=test.csv

The following command downloads the CSV data and creates a GLM file with the data linked and weather object named:

    bash$ python3 /usr/local/share/gridlabd/noaa_forecast.py -p=45.62,-122.70 -c=test.csv -n=test -g=test.glm

"""

import sys, os, json, requests, pandas, numpy, datetime, dateutil

server = "https://api.weather.gov/points/{latitude},{longitude}"
float_format="%.2f"

def getforecast(lat,lon):
    """Get NOAA location"""
    url = server.format(latitude=lat,longitude=lon)
    location = json.loads(requests.get(url).content.decode("utf-8"))
    data = json.loads(requests.get(location["properties"]["forecast"]).content.decode("utf-8"))
    result = {
        "datetime" : [],
        "temperature" : [],
        "wind_speed" : []
    }
    for item in data["properties"]["periods"]:
        result["datetime"].append(dateutil.parser.parse(item["startTime"])+datetime.timedelta(hours=item["number"]))
        result["temperature"].append(item["temperature"])
        result["wind_speed"].append(item["windSpeed"].split()[0])
    return pandas.DataFrame(result).set_index("datetime")

def writeglm(data, glm=None, name=None, csv=None):
    """Write weather object based on NOAA forecast"""
    if glm:
        with open(glm,"w") as f:
            f.write("class weather\n{\n")
            for column in weather.columns:
                f.write(f"\tdouble {column};\n")
            f.write("}\n")
            weather.columns = list(map(lambda x:x.split('[')[0],weather.columns))
            f.write("module tape;\n")
            f.write("object forecast\n{\n")
            if name:
                f.write(f"\tname \"{name}\";\n")
            f.write("\tobject player\n\t{\n")
            f.write(f"\t\tfile \"{csv}\";\n")
            f.write(f"\t\tproperty \"{','.join(weather.columns)}\";\n")
            f.write("\t};\n")
            f.write("}\n")
        data.to_csv(csv,header=False,float_format=float_format)
    else:
        data.to_csv(csv,header=True,float_format=float_format)        

if __name__ == "__main__":
    def error(msg,code=None):
        if code != None:
            print(f"ERROR [noaa_forecast.py]: {msg}",file=sys.stderr)
            exit(code)
        else:
            raise Exception(msg)
    def syntax(code=0):
        if code == 0:
            print(__doc__)
        else:
            print(f"Syntax: {os.path.basename(sys.argv[0])} -p -position=LAT,LON [-g|--glm=GLMNAME] [-n|--name=OBJECTNAME] [-c|--csv=CSV] [--test] [-h|--help|help]")
        exit(code)
    position = None
    glm = None
    name = None
    csv = None
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
        elif token in ["-p","--position"]:
            position = value.split(",")
            if len(position) != 2:
                error("position is not a tuple")
        elif token in ["-g","--glm"]:
            glm = value
        elif token in ["-n","--name"]:
            name = value
        elif token in ["-c","--csv"]:
            csv = value
        elif token == "--test":
            position = [45.62,-122.70]
            glm = "test.glm"
            name = "test"
            writeglm(getforecast(float(position[0]),float(position[1])),glm,name,csv)
            exit(os.system(f"gridlabd {glm}"))
        else:
            error(f"option '{token}' is not valid")
    if position:
        writeglm(getforecast(float(position[0]),float(position[1])),glm,name,csv)
    else:
        syntax(1)


