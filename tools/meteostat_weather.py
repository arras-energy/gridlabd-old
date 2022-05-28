"""Meteostat weather access

SYNOPSIS

Shell:

    $ python3 meteostat.py [OPTIONS] ...

GLM:

    #python3 meteostat.py [OPTIONS] ...

Options:

  -h|--help|help

  -c|--csv=CSVNAME
  
  -h|--glm=GLMNAME
  
  -n|--name=OBJNAME
  
  -p|--position=LAT,LON

DESCRIPTION

TODO

"""

import os, sys, math
import requests
import json, gzip
import haversine
import pandas
import datetime

VERBOSE = False
DEBUG = False
QUIET = False
WARNING = True

E_OK = 0 # no error
E_MISSING = 1 # missing argument
E_INVALID = 2 # invalid data
E_EXCEPTION = 9 # exception caught

BASENAME = os.path.splitext(os.path.basename(sys.argv[0]))[0]
URL_STATIONS = "https://bulk.meteostat.net/v2/stations/full.json.gz"
URL_HOURLY = "https://bulk.meteostat.net/v2/hourly/{station}.csv.gz"

LOCATION = None
OBJECT_NAME = None
OUTPUT_NAME = None
CSV_NAME = None

DATA_COLUMNS = [
  'date', #    The date string (format: YYYY-MM-DD)    String
  'time', #    The hour (UTC)  Integer
  'temperature[degC]', #    The air temperature in °C   Float
  'dewpoint[degC]', #    The dew point in °C Float
  'humidity[%]', #    The relative humidity in percent (%)    Integer
  'rainfall[mm]', #    The one hour precipitation total in mm  Float
  'snowdepth[mm]', #    The snow depth in mm    Integer
  'wind_direction[deg]', #    The wind direction in degrees (°)   Integer
  'wind_speed[km/h]', #    The average wind speed in km/h  Float
  'wind_gusts[km/h]', #    The peak wind gust in km/h  Float
  'air_pressure[hPa]', #    The sea-level air pressure in hPa   Float
  'sunshine_duration[m]', #    The one hour sunshine total in minutes (m)  Integer
  'condition_code', #    The weather condition code  Integer
]

try:
    CACHE_DIR = getenv("GLD_ETC") + "/weather/meteostat"
except:
    CACHE_DIR = "/tmp/meteostat"
os.makedirs(CACHE_DIR,exist_ok=True)
STATIONS_FILE = CACHE_DIR + "/stations.json.gz"
STATIONS = None

class MeteostatError(Exception):
    pass

def error(msg,code=None):
    if not QUIET:
        print(f"ERROR [{BASENAME}]: {msg} (code {code})", file=sys.stderr)
    if DEBUG:
        raise MeteostatError(msg)
    if type(code) == int:
        exit(code)

def warning(msg):
    if WARNING:
        print(f"WARNING [{BASENAME}]: {msg}", file=sys.stderr)

def verbose(msg):
    if VERBOSE:
        print(f"VERBOSE [{BASENAME}]: {msg}", file=sys.stderr)

def get_stations(refresh=True):

    if not os.path.exists(STATIONS_FILE) or refresh:
        reply = requests.get(URL_STATIONS)
        if reply.status_code != 200:
            error(f"{URL_STATIONS} error {reply.status_code}",E_INVALID)
        with open(STATIONS_FILE,"wb") as fh:
            fh.write(reply.content)
    with gzip.open(STATIONS_FILE,"r") as fh:
        STATIONS = json.load(fh)
        return STATIONS
    error(f"{STATIONS_FILE} is not a valid jzon gz file",E_INVALID)

def find_station(lat,lon):
    best_distance = None
    best_station = None
    for station in get_stations():
        location = (station["location"]["latitude"],station["location"]["longitude"])
        distance = haversine.haversine(location,(lat,lon))
        if not best_station or distance < best_distance:
            best_distance = distance
            best_station = station
    verbose(f"find_station(lat={lat},lon={lon}) --> {best_station['id']}")
    return best_station

def change_column(data,from_name,to_name,transformation):
    data[to_name] = data[from_name].transform(transformation)
    return data.drop(from_name,axis=1)

def get_weather(station,start=None,stop=None):
    station_file = f"{CACHE_DIR}/{station}.csv.gz"
    if not os.path.exists(station_file):
        url = URL_HOURLY.format(station=station)
        reply = requests.get(url)
        if reply.status_code != 200:
            error(f"{url} error {reply.status_code}",E_INVALID)
        with open(station_file,"wb") as fh:
            fh.write(reply.content)
    data = pandas.read_csv(station_file,names=DATA_COLUMNS,index_col=0,parse_dates=[DATA_COLUMNS[0:2]]).sort_index()

    # convert to gridlabd weather format/units
    data = change_column(data,"temperature[degC]","temperature[degF]",lambda x:round(x*9/5+32,1))
    data = change_column(data,"dewpoint[degC]","dewpoint[degF]",lambda x:round(x*9/5+32,1))
    data = change_column(data,"rainfall[mm]","rainfall[in/h]",lambda x:round(x/25.4,2))
    data = change_column(data,"snowdepth[mm]","snowdepth[in]",lambda x:round(x/25.4,2))
    data = change_column(data,"wind_speed[km/h]","wind_speed[mph]",lambda x:round(x*0.6213712,1))
    data = change_column(data,"wind_gusts[km/h]","wind_gusts[mph]",lambda x:round(x*0.6213712,1))
    data = change_column(data,"air_pressure[hPa]","pressure[mbar]",lambda x:round(x,1))
    data = change_column(data,"wind_direction[deg]","wind_dir[deg]",lambda x:round(x,1))
    verbose(f"get_weather(station='{station}',start={start},stop={stop}) --> {len(data)} rows")
    return data

try:

    for arg in sys.argv[1:]:
        spec = arg.split("=")
        if len(spec) == 1:
            value = True
        elif len(spec) == 2:
            value = spec[1]
        else:
            value = spec[1:]
        tag = spec[0]
        if tag in ["-h","--help","help"]:
            print(__doc__)
            exit(E_OK)
        elif tag in ["-g","--glm"]:
            OUTPUT_NAME=value
        elif tag in ["-p","--position"]:
            LOCATION = list(map(lambda x:float(x),value.split(",")))
            if len(LOCATION) != 2:
                error(f"position '{LOCATION}' is invalid",E_INVALID)
        else:
            error(f"option '{arg}' is invalid",E_INVALID)


    if not LOCATION:
        error("location not specified",E_MISSING)

    station_info = find_station(*LOCATION)

    data = get_weather(station_info["id"])

    if not OUTPUT_NAME:

        print(data.to_csv(),end='')

    elif os.path.splitext(OUTPUT_NAME)[1] == ".csv":

        data.to_csv(OUTPUT_NAME)

    elif os.path.splitext(OUTPUT_NAME)[1] == ".glm":

        if not OBJECT_NAME:
            OBJECT_NAME = f"meteostat_{station_info['id']}"

        if not CSV_NAME:
            CSV_NAME = f"meteostat_{station_info['id']}.csv"

        data.to_csv(CSV_NAME)

        with open(OUTPUT_NAME,"w") as glm:
            print(f"// generated by {BASENAME} at {datetime.datetime.now()}",file=glm)
            print("\nmodule climate;",file=glm)
            print("object weather {",file=glm)
            print(f"    name \"{OBJECT_NAME}\";",file=glm)
            print(f"    latitude {LOCATION[0]};",file=glm)
            print(f"    longitude {LOCATION[1]};",file=glm)
            print("}",file=glm)
            # print("\nmodule tape;",file=glm)
            # print("object player {",file=glm)
            # print(f"    parent \"{OBJECT_NAME}\";",file=glm)
            # print(f"    file \"{CSV_NAME}\";",file=glm)
            # print(f"    property \"{','.join(data.columns)}\";",file=glm)
            # print("}",file=glm)

except SystemExit:

    pass

except:

    e_type, e_name, e_trace = sys.exc_info()
    e_file = os.path.basename(e_trace.tb_frame.f_code.co_filename)
    e_line = e_trace.tb_lineno
    if DEBUG:
        raise
    else:
        error(f"{e_type.__name__} at {e_file}/{e_line}: {e_name}",E_EXCEPTION)
