"""NOAA weather forecast tool

SYNOPSIS

Shell:
    bash$ gridlabd noaa_forecast -p|-position=LAT,LON [-i|--interpolate=TIME|METHOD]
        [-g|--glm=GLMNAME] [-n|--name=OBJECTNAME] [-c|--csv=CSVNAME] [--test] [-h|--help|help]

GLM:
    #python -m noaa_forecast -p|-position=LAT,LON [-i|--interpolate=TIME|METHOD]
        [-g|--glm=GLMNAME] [-n|--name=OBJECTNAME] [-c|--csv=CSVNAME] [--test] [-h|--help|help]
    #include "GLMNAME"

Python:
    bash$ gridlabd python
    >>> import noaa_forecast as nf
    >>> nf.getforecast(37.5,-122.3)
                                   temperature[degF]  wind_speed[m/s]  wind_dir[deg]
    2021-10-21 14:00:00-07:00          68.000000         4.470400     202.500000
    2021-10-21 15:00:00-07:00          65.327601         4.275738     201.983153
    2021-10-21 16:00:00-07:00          62.990829         4.099336     201.673044
    ...                                      ...              ...            ...
    2021-10-28 06:00:00-07:00          57.284721         2.770949     315.450517
    2021-10-28 07:00:00-07:00          55.246212         2.946331     315.403079
    2021-10-28 08:00:00-07:00          53.000000         3.129280     315.000000

    [163 rows x 3 columns]


DESCRIPTION

This module downloads weather forecasts from NOAA and writes GLM files.  This can be done from
the command line or using call the python API.

Interpolation is usually necessary because the data samples received from NOAA span several hours.
The default interval is 60 minutes, but can be set to any integer value in minutes. The sampling
method is by default quadratic.  Other interpolation methods supported include 

  - linear

    Ignore the index and treat the values as equally spaced. This is the only method 
    supported on MultiIndexes.

  - time

    Works on daily and higher resolution data to interpolate given length of interval.

  - index, values

    Use the actual numerical values of the index.

  - pad

    Fill in NaNs using existing values.

  - nearest, zero, slinear, quadratic, cubic, spline, barycentric, polynomial

    Passed to scipy.interpolate.interp1d. These methods use the numerical values of the index. 
    Both ‘polynomial’ and ‘spline’ require that you also specify an order (int), e.g. 
    df.interpolate(method='polynomial', order=5).

  - krogh, piecewise_polynomial, spline, pchip, akima, cubicspline

    Wrappers around the SciPy interpolation methods of similar names. See Notes.

  - from_derivatives

    Refers to scipy.interpolate.BPoly.from_derivatives which replaces ‘piecewise_polynomial’ 
    interpolation method in scipy 0.18.

PARAMETERS

The module uses several parameters to control its behavior. 

    server = "https://api.weather.gov/points/{latitude},{longitude}" # NOAA location server (provides forecast URL)
    user_agent = "(gridlabd.us, gridlabd@gmail.com)" # default user agent to report to NOAA
    date_format = "%Y-%m-%d %H:%M:%S"
    float_format="%.1f" # float format to use 
    interpolate_time = 60
    interpolate_method = 'quadratic'

The parameters can be changed before obtained the forecast.

    >>> import noaa_forecast as nf
    >>> nf.interpolate = 5
    >>> nf.getforecast(37.5,-122.3)

EXAMPLE

The following command downloads only the CSV data for a location:

    bash$ gridlabd noaa_forecast -p=45.62,-122.70 -c=test.csv

The following command downloads the CSV data and creates a GLM file with the data linked and weather object named:

    bash$ gridlabd noaa_forecast -p=45.62,-122.70 -c=test.csv -n=test -g=test.glm

SEE ALSO

* [https://www.weather.gov/documentation/services-web-api]
"""

import sys, os, json, requests, pandas, numpy, datetime, dateutil

server = "https://api.weather.gov/points/{latitude},{longitude}"
user_agent = "(gridlabd.us, gridlabd@gmail.com)"
interpolate_time = 60
interpolate_method = 'quadratic'
float_format = "%.1f"
date_format = "%Y-%m-%d %H:%M:%S"

def getforecast(lat,lon):
    """Get NOAA location"""
    url = server.format(latitude=lat,longitude=lon)
    headers = {'User-agent' : user_agent}
    location = json.loads(requests.get(url,headers=headers).content.decode("utf-8"))

    data = json.loads(requests.get(location["properties"]["forecastHourly"],headers=headers).content.decode("utf-8"))
    result = {
        "datetime" : [],
        "temperature[degF]" : [],
        "wind_speed[m/s]" : [],
        "wind_dir[deg]" : [],
    }
    if not "properties" in data.keys():
        raise Exception(f"data does not contain required properties information (data={data})")
    if not "periods" in data["properties"]:
        raise Exception(f"data does not contain required period information (data={data})")
    for item in data["properties"]["periods"]:
        result["datetime"].append(dateutil.parser.parse(item["startTime"])+datetime.timedelta(hours=item["number"]))
        result["temperature[degF]"].append(float(item["temperature"]))
        result["wind_speed[m/s]"].append(float(item["windSpeed"].split()[0])*0.44704)
        result["wind_dir[deg]"].append(dict(
            N=0, NNE=22.5, NE=45, ENE=67.5, E=90, ESE=112.5, SE=135, SSE=157.5,
            S=180, SSW=202.5, SW=225, WSW=247.5, W=270, WNW=292.5, NW=315, NNW=337.5,
            )[item["windDirection"]])
    df = pandas.DataFrame(result).set_index("datetime")
    if interpolate_time:
        starttime = df.index.min()
        stoptime = df.index.max()
        if starttime.tzinfo.utcoffset != stoptime.tzinfo.utcoffset:
            dt = stoptime - starttime
            stoptime = starttime + dt
        daterange = pandas.DataFrame(index=pandas.date_range(starttime,stoptime,freq=f"{interpolate_time}min"))
        df = df.join(daterange,how="outer").interpolate(interpolate_method)
    df.index.name = "datetime"
    return df

def writeglm(data, glm=sys.stdout, name=None, csv=None,download_now=True):
    """Write weather object based on NOAA forecast"""
    if glm:
        if csv == 'auto' or csv == None:
            if type(glm) is str:
                csv = glm.replace(".glm",".csv")
            else:
                raise Exception("csv name is required when glm is not a filename")
        if type(glm) is str:
            glm = open(glm,"w")
        glm.write("class forecast\n{\n")
        for column in data.columns:
            glm.write(f"\tdouble {column};\n")
        glm.write("}\n")
        data.columns = list(map(lambda x:x.split('[')[0],data.columns))
        glm.write("module tape;\n")
        glm.write("#define NOAA_FORECAST_TIMEZONE=${SHELL gridlabd timezone local}\n")
        glm.write(f"#define NOAA_FORECAST_STARTTIME={data.index.min().isoformat('T')}\n")
        glm.write(f"#define NOAA_FORECAST_STOPTIME={data.index.max().isoformat('T')}\n")
        glm.write("object forecast\n{\n")
        if name:
            glm.write(f"\tname \"{name}\";\n")
        glm.write("\tobject player\n\t{\n")
        glm.write(f"\t\tfile \"{csv}\";\n")
        glm.write(f"\t\tproperty \"{','.join(data.columns)}\";\n")
        glm.write("\t};\n")
        glm.write("}\n")
        if download_now:
            data.to_csv(csv,header=False,float_format=float_format,date_format=date_format)
    else:
        if csv == None:
            csv = "/dev/stdout"
        elif csv == 'auto':
            raise Exception("csv cannot be automatically named if GLM is not specified")
        data.to_csv(csv,header=True,float_format=float_format,date_format=date_format)

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
            print(f"Syntax: {os.path.basename(sys.argv[0]).replace('.py','')} -p -position=LAT,LON [-i|--interpolate=TIME|METHOD] [-g|--glm=GLMNAME] [-n|--name=OBJECTNAME] [-c|--csv=CSV] [--test] [-h|--help|help]")
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
        elif token in ["-i","--interpolate"]:
            try:
                interpolate_time = int(value)
            except:
                if value:
                    interpolate_method = value
                else:
                    interpolate_time = None
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


