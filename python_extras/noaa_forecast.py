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

    server = "https://api.weather.gov/points/{latitude},{longitude}" # NOAA location server (provides forecast URL)
    user_agent = "(gridlabd.us, gridlabd@gmail.com)" # default user agent to report to NOAA
    date_format = "%Y-%m-%d %H:%M:%S"
    float_format="%.1f" # float format to use 

EXAMPLE

The following command downloads only the CSV data for a location:

    bash$ python3 /usr/local/share/gridlabd/noaa_forecast.py -p=45.62,-122.70 -c=test.csv

The following command downloads the CSV data and creates a GLM file with the data linked and weather object named:

    bash$ python3 /usr/local/share/gridlabd/noaa_forecast.py -p=45.62,-122.70 -c=test.csv -n=test -g=test.glm

SEE ALSO

* [https://www.weather.gov/documentation/services-web-api]
"""

import sys, os, json, requests, pandas, numpy, datetime, dateutil

server = "https://api.weather.gov/points/{latitude},{longitude}"
user_agent = "(gridlabd.us, gridlabd@gmail.com)"
interpolate = None
float_format = "%.1f"
date_format = "%Y-%m-%d %H:%M:%S"

def getforecast(lat,lon):
    """Get NOAA location"""
    url = server.format(latitude=lat,longitude=lon)
    headers = {'User-agent' : user_agent}
    location = json.loads(requests.get(url,headers=headers).content.decode("utf-8"))
    data = json.loads(requests.get(location["properties"]["forecast"],headers=headers).content.decode("utf-8"))
    result = {
        "datetime" : [],
        "temperature[degF]" : [],
        "wind_speed[m/s]" : [],
        "wind_dir[deg]" : [],
    }
    for item in data["properties"]["periods"]:
        result["datetime"].append(dateutil.parser.parse(item["startTime"])+datetime.timedelta(hours=item["number"]))
        result["temperature[degF]"].append(float(item["temperature"]))
        result["wind_speed[m/s]"].append(float(item["windSpeed"].split()[0])*0.44704)
        result["wind_dir[deg]"].append(dict(
            N=0, NNE=22.5, NE=45, ENE=67.5, E=90, ESE=112.5, SE=135, SSE=157.5,
            S=180, SSW=202.5, SW=225, WSW=247.5, W=270, WNW=292.5, NW=315, NNW=337.5,
            )[item["windDirection"]])
    return pandas.DataFrame(result).set_index("datetime")

def writeglm(data, glm=None, name=None, csv=None):
    """Write weather object based on NOAA forecast"""
    if glm:
        if csv == None:
            csv = glm.replace(".glm",".csv")
        with open(glm,"w") as f:
            f.write("class forecast\n{\n")
            for column in data.columns:
                f.write(f"\tdouble {column};\n")
            f.write("}\n")
            data.columns = list(map(lambda x:x.split('[')[0],data.columns))
            f.write("module tape;\n")
            f.write("object forecast\n{\n")
            if name:
                f.write(f"\tname \"{name}\";\n")
            f.write("\tobject player\n\t{\n")
            f.write(f"\t\tfile \"{csv}\";\n")
            f.write(f"\t\tproperty \"{','.join(data.columns)}\";\n")
            f.write("\t};\n")
            f.write("}\n")
        data.to_csv(csv,header=False,float_format=float_format,date_format=date_format)
    elif csv:
        data.to_csv(csv,header=True,float_format=float_format,date_format=date_format)
    else:
        data.to_csv("/dev/stdout",header=True,float_format=float_format,date_format=date_format)

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


