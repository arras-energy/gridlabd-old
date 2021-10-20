"""NSRDB weather data tool

SYNOPSIS

Shell:
    bash$ python3 nsrbd_weather.py -y|--year=YEARS -p|-position=LAT,LON [-g|--glm=GLMNAME] 
        [-n|--name=OBJECTNAME] [-c|--csv=CSVNAME] [--test] [-h|--help|help]

GLM:
    #system python3 nsrbd_weather.py -y|--year=YEARS -p|-position=LAT,LON [-g|--glm=GLMNAME] 
        [-n|--name=OBJECTNAME] [-c|--csv=CSVNAME] [--test] [-h|--help|help]
    #include "GLMNAME"

Python:
    >>> import sys
    >>> sys.path.append("/usr/local/share/gridlabd")
    >>> import nsrdb_weather as ns
    >>> data = ns.getyears(YEARS,LAT,LON)
    >>> ns.writeglm(data,GLMNAME,OBJECTNAME,CSVNAME)

DESCRIPTION

This module downloads weather data from NSRDB and writes GLM files.  This can be done from
the command line or using call the python API.

CREDENTIALS

You must obtain an API key from https://developer.nrel.gov/signup/.  Save the key
in the credentials file, which is by default `$HOME/.nsrdb/credentials.json`.  To add,
change, or delete a key, use the `addkey()` function.

EXAMPLE

    bash$ python3 nsrdb_weather.py -y=2014,2015 -p=45.62,-122.70 -g=test.glm -n=test

"""

import sys, os, json, requests, pandas, numpy, datetime

leap = True
interval = 60
utc = True
name = "HiPAS GridLAB-D".replace(" ","+")
org = "SLAC National Accelerator Laboratory".replace(" ","+")
reason = "Grid modeling".replace(" ","+")
email="gridlabd@gmail.com"
notify = False
verbose = False
server = "https://developer.nrel.gov/api/solar/nsrdb_psm3_download.csv"
cachedir = "/usr/local/share/gridlabd/weather"
attributes = 'ghi,dhi,dni,cloud_type,dew_point,air_temperature,surface_albedo,wind_speed,wind_direction,solar_zenith_angle'
credential_file = f"{os.getenv('HOME')}/.nsrdb/credentials.json"

def addkey(apikey=None):
    """Manage NSRDB API keys"""
    keys = getkeys(credential_file)
    if apikey:
        keys[email] = apikey
    elif apikey in keys.keys():
        del keys[email]
    with open(credential_file,"w") as f:
        f.write(keys)

def getkeys():
    """Get all NSRDB API keys"""
    with open(credential_file,"r") as f: 
        return json.load(f)

def getkey(email):
    """Get a single NSRDB API key"""
    return getkeys()[email]

def getyears(years,lat,lon,concat=True):
    """Get NSRDB weather data for multiple years"""
    result = {}
    for year in years:
        data = getyear(year,lat,lon)
        if result:
            for key,value in result.items():
                result[key].extend(data[key])
        else:
            result = data
    if concat:
        result["DataFrame"] = pandas.concat(result["DataFrame"])
    return result

def getyear(year,lat,lon):
    """Get NSRDB weather data for a single year"""
    api = getkey(email)
    url = f"{server}?wkt=POINT({lon}%20{lat})&names={year}&leap_day={str(leap).lower()}&interval={interval}&utc={str(utc).lower()}&full_name={name}&email={email}&affiliation={org}&mailing_list={str(notify).lower()}&reason={reason}&api_key={api}&attributes={attributes}"
    if lat > 0: lat = f"N{lat:.2f}"
    elif lat < 0: lat = f"S{-lat:.2f}"
    else: lat = "0"
    if lon > 0: lon = f"E{lon:.2f}"
    elif lon < 0: lon = f"W{-lon:.2f}"
    else: lon = "0"
    cache = f"{cachedir}/nsrbd_{lat}_{lon}_{year}.csv"
    try:
        result = pandas.read_csv(cache,nrows=1).to_dict(orient="list")
        result.update(dict(Year=[year],DataFrame=[pandas.read_csv(cache,skiprows=2)]))
    except:
        result = None
    if not result:
        with open(cache,"w") as fout:
            fout.write(requests.get(url).content.decode("utf-8"))
        result = pandas.read_csv(cache,nrows=1).to_dict(orient="list")
        result.update(dict(Year=[year],DataFrame=[pandas.read_csv(cache,skiprows=2)]))
    for data in result["DataFrame"]:
        data["datetime"] = list(map(lambda x: datetime.datetime(x[0,0],x[0,1],x[0,2],x[0,3],0,0),numpy.matrix([data.Year,data.Month,data.Day,data.Hour]).transpose()))
        data.set_index("datetime",inplace=True)
        data.drop(columns=["Year","Day","Month","Hour","Minute"],inplace=True)
        data.columns = [
            "solar_global[W/m^2]",
            "solar_horizontal[W/m^2]",
            "solar_direct[W/m^2]",
            "clouds",
            "dewpoint[degC]",
            "temperature[degC]",
            "albedo[pu]",
            "wind_speed[m/s]",
            "wind_dir[deg]",
            "solar_altitude[deg]",
            ]
    return result

def writeglm(data, glm=sys.stdout, name=None, csv=None):
    """Write weather object based on NSRDB data"""
    if not name:
        name = f"nsrdb_{data['Location ID'][0]}"
    if not csv:
        csv = f"{name}.csv"
    if type(data["DataFrame"]) is list:
        weather = pandas.concat(data["DataFrame"])
    else:
        weather = data["DataFrame"]
    with open(glm,"w") as f:
        f.write("class weather\n{\n")
        for column in weather.columns:
            f.write(f"\tdouble {column};\n")
        f.write("}\n")
        weather.columns = list(map(lambda x:x.split('[')[0],weather.columns))
        f.write("module tape;\n")
        f.write("object weather\n{\n")
        f.write(f"\tname \"{name}\";\n")
        f.write("\tobject player\n\t{\n")
        f.write(f"\t\tfile \"{csv}\";\n")
        f.write(f"\t\tproperty \"{','.join(weather.columns)}\";\n")
        f.write("\t};\n")
        f.write("}\n")
    weather.to_csv(csv,header=False)

if __name__ == "__main__":
    def error(msg,code=None):
        if code != None:
            print(f"ERROR [nsrdb_weather.py]: {msg}",file=sys.stderr)
            exit(code)
        else:
            raise Exception(msg)
    def syntax(code=0):
        if code == 0:
            print(__doc__)
        else:
            print(f"Syntax: {os.path.basename(sys.argv[0])} -y|--year=YEARS -p -position=LAT,LON [-g|--glm=GLMNAME] [-n|--name=OBJECTNAME] [-c|--csv=CSV] [--test] [-h|--help|help]")
        exit(code)
    year = None
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
        elif token in ["-y","--year"]:
            year = value.split(",")
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
            year = [2014,2015]
            position = [45.62,-122.70]
            glm = "test.glm"
            name = "test"
            writeglm(getyears(year,float(position[0]),float(position[1])),glm,name,csv)
            exit(os.system(f"gridlabd {glm}"))
        else:
            error(f"option '{token}' is not valid")
    if position and year:
        writeglm(getyears(year,float(position[0]),float(position[1])),glm,name,csv)
    else:
        syntax(1)


