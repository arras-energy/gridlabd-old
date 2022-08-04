"""Meteostat weather access

SYNOPSIS

Shell:

    $ python3 meteostat.py [OPTIONS] ...

GLM:

    #python3 meteostat.py [OPTIONS] ...

Options:

  -h|--help|help         Get this help information (exits with code 0)

  -c|--csv=CSVNAME       Name the CSV output file (optional)
  
  -g|--glm=GLMNAME       Name the GLM output file (optional)
  
  -n|--name=OBJNAME      Name the weather object (optional)
  
  -p|--position=LAT,LON  Specify the location (required)

  -s|--start=YYYY-MM-DD  Specify the start date (optional)

  -e|--end=YYYY-MM-DD    Specify the end date (optional)

DESCRIPTION

The meteostat_weather tool gathers data from the Meteostat website and converts it
to a GridLAB-D weather object. If neither CSVNAME or GLMNAME are provided the weather 
data is written to stdout.  Otherwise, the default CSVNAME is "meteostat_ID.csv" where
ID is the station identifier provided by Meteostat.  The default weather object is
also named "meteostat_ID".  If the start date and/or end date are not specified, then
the default is all available data.

"""

import os, sys, math
import requests
import json, gzip
import haversine
import pandas
import datetime, time
from pysolar.solar import get_altitude, radiation

share = os.getenv("GLD_ETC")
if not share:
    share = "/usr/local/share/gridlabd"
if share not in sys.path:
    sys.path.append("/usr/local/share/gridlabd")
import nsrdb_weather

VERBOSE = False
DEBUG = False
QUIET = False
WARNING = True

E_OK = 0 # no error
E_MISSING = 1 # missing argument
E_INVALID = 2 # invalid data
E_TIMEOUT = 3 # file lock timeout
E_EXCEPTION = 9 # exception caught

BASENAME = os.path.splitext(os.path.basename(sys.argv[0]))[0]
URL_STATIONS = "https://bulk.meteostat.net/v2/stations/full.json.gz"
URL_HOURLY = "https://bulk.meteostat.net/v2/hourly/{station}.csv.gz"

LOCATION = None
OBJECT_NAME = None
OUTPUT_NAME = None
CSV_NAME = None
START_TIME = None
END_TIME = None
TIMEOUT = 10
TIMESTEP = 0.1
GEOHASH_RESOLUTION = 5

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

SOLAR_CONDITIONS = { # factor to reduce direct solar based on conditions
    0 : 1.0, # unknown
    1 : 1.0, # Clear
    2 : 0.9, # Fair
    3 : 0.7, # Cloudy
    4 : 0.5, # Overcast
    5 : 0.3, # Fog
    6 : 0.2, # Freezing Fog
    7 : 0.4, #  Light Rain
    8 : 0.3, #  Rain
    9 : 0.2, # Heavy Rain
    10 : 0.3, # Freezing Rain
    11 : 0.2, # Heavy Freezing Rain
    12 : 0.3, # Sleet
    13 : 0.2, # Heavy Sleet
    14 : 0.3, # Light Snowfall
    15 : 0.2, # Snowfall
    16 : 0.2, # Heavy Snowfall
    17 : 0.2, # Rain Shower
    18 : 0.2, # Heavy Rain Shower
    19 : 0.2, # Sleet Shower
    20 : 0.2, # Heavy Sleet Shower
    21 : 0.2, # Snow Shower
    22 : 0.2, # Heavy Snow Shower
    23 : 0.2, # Lightning
    24 : 0.2, # Hail
    25 : 0.2, # Thunderstorm
    26 : 0.1, # Heavy Thunderstorm
    27 : 0.2, # Storm
}

try:
    CACHE_DIR = getenv("GLD_ETC") + "/weather/meteostat"
except:
    CACHE_DIR = "/tmp/meteostat"
os.makedirs(CACHE_DIR,exist_ok=True)
STATIONS_FILE = CACHE_DIR + "/stations.json.gz"
STATIONS_RECENT = CACHE_DIR + "/stations.json"
STATIONS = None

class MeteostatError(Exception):
    pass

def error(msg,code=None):
    if not QUIET:
        print(f"ERROR [{BASENAME}]: {msg} (code {code})", file=sys.stderr, flush=True)
    if DEBUG:
        raise MeteostatError(msg)
    if type(code) == int:
        sys.exit(code)

def warning(msg):
    if WARNING:
        print(f"WARNING [{BASENAME}]: {msg}", file=sys.stderr, flush=True)

def file_lock(pathname,timeout=TIMEOUT,wait=TIMESTEP):
    lockname = pathname + "-locked"
    t0 = datetime.datetime.now()
    ok = False
    while not ok:
        try:
            os.mkdir(lockname)
            ok = True
        except:
            if datetime.datetime.now() - t0 > datetime.timedelta(seconds=timeout):
                error(f"file lock timeout for {pathname}",E_TIMEOUT)
            time.sleep(wait)
    return lockname

def file_unlock(lockname):
    os.rmdir(lockname)

def verbose(msg):
    if VERBOSE:
        print(f"VERBOSE [{BASENAME}]: {msg}", file=sys.stderr, flush=True)

def get_stations(refresh=True):
    lockname = file_lock(STATIONS_FILE)
    try:
        if not os.path.exists(STATIONS_FILE) or refresh:
            reply = requests.get(URL_STATIONS)
            if reply.status_code != 200:
                error(f"{URL_STATIONS} error {reply.status_code}",E_INVALID)
            with open(STATIONS_FILE,"wb") as fh:
                fh.write(reply.content)
        with gzip.open(STATIONS_FILE,"r") as fh:
            STATIONS = json.load(fh)
            return STATIONS
    except:
        pass
    finally:
        file_unlock(lockname)
    error(f"{STATIONS_FILE} is not a valid jzon gz file",E_INVALID)

def find_station(lat,lon):
    try:
        lockname = file_lock(STATIONS_RECENT)
        geohash = nsrdb_weather.geohash(lat,lon,GEOHASH_RESOLUTION)
        verbose(f"checking for geohash '{geohash}' in {STATIONS_RECENT}")
        with open(STATIONS_RECENT,"r") as fh:
            recents = json.load(fh)
        if geohash in recents.keys():
            best_station = recents[geohash]
            verbose(f"find_station(lat={lat},lon={lon}) --> {best_station['id']} at geohash {geohash}")
            return best_station
        verbose(f"geohash '{geohash}' not in recents")
    except:
        recents = {}
        verbose(f"no recents yet in {STATIONS_RECENT} ({sys.exc_info()})")
    finally:
        file_unlock(lockname)

    best_distance = None
    best_station = None
    for station in get_stations():
        location = (station["location"]["latitude"],station["location"]["longitude"])
        distance = haversine.haversine(location,(lat,lon))
        if not best_station or distance < best_distance:
            best_distance = distance
            best_station = station
    verbose(f"find_station(lat={lat},lon={lon}) --> {best_station['id']}")
    
    lockname = file_lock(STATIONS_RECENT)
    try:
        try:
            with open(STATIONS_RECENT,"r") as fh:
                recents = json.load(fh)
        except:
            recents = {}
        recents[geohash] = best_station
        with open(STATIONS_RECENT,"w") as fh:
            json.dump(recents,fh)
    except:
        e_type, e_value, e_trace = sys.exc_info()
        e_file = os.path.basename(e_trace.tb_frame.f_code.co_filename)
        e_line = e_trace.tb_lineno
        warning(f"unable to update recent stations file {STATIONS_RECENT} ({e_file}@{e_line}: {e_type.__name__} {e_value})")
    finally:
        file_unlock(lockname)
    return best_station

def change_column(data,from_name,to_name,transformation):
    data[to_name] = data[from_name].transform(transformation)
    return data.drop(from_name,axis=1)

def get_weather(station,start=None,stop=None):
    station_file = f"{CACHE_DIR}/{station}.csv.gz"


# weather may not be updating if using more recent date



    if not os.path.exists(station_file):
        url = URL_HOURLY.format(station=station)
        verbose(f"downloading data from {url}")
        reply = requests.get(url)
        if reply.status_code != 200:
            error(f"{url} error {reply.status_code}",E_INVALID)
        with open(station_file,"wb") as fh:
            fh.write(reply.content)
    data = pandas.read_csv(station_file,names=DATA_COLUMNS,index_col=0,parse_dates=[DATA_COLUMNS[0:2]],).sort_index()

    # convert to gridlabd weather format/units
    data = change_column(data,"temperature[degC]","temperature[degF]",lambda x:round(x*9/5+32,1))
    data = change_column(data,"dewpoint[degC]","dewpoint[degF]",lambda x:round(x*9/5+32,1))
    data = change_column(data,"rainfall[mm]","rainfall[in/h]",lambda x:round(x/25.4,2))
    data = change_column(data,"snowdepth[mm]","snowdepth[in]",lambda x:round(x/25.4,2))
    data = change_column(data,"wind_speed[km/h]","wind_speed[mph]",lambda x:round(x*0.6213712,1))
    data = change_column(data,"wind_gusts[km/h]","wind_gusts[mph]",lambda x:round(x*0.6213712,1))
    data = change_column(data,"air_pressure[hPa]","pressure[mbar]",lambda x:round(x,1))
    data = change_column(data,"wind_direction[deg]","wind_dir[deg]",lambda x:round(x,1))

    if start:
        if stop:
            data = data.loc[start:stop]
        else:
            data = data.loc[start:]
    elif stop:
        data = data.loc[:stop]

    verbose(f"get_weather(station='{station}',start={start},stop={stop}) --> {len(data)} rows")
    return data

def get_solar(data,latitude,longitude):
    verbose(f"calculating solar data for position ({latitude},{longitude})")
    solar_direct = []
    data["condition_code"] = data["condition_code"].fillna(0)
    for dt in data.index:
        date = datetime.datetime.fromtimestamp(dt.timestamp(),datetime.timezone.utc)
        altitude = get_altitude(latitude, longitude, date)
        if altitude > 0:
            condition_factor = SOLAR_CONDITIONS[int(data.loc[dt]["condition_code"])]
            solar_direct.append(round(radiation.get_radiation_direct(date, altitude)*condition_factor/10.764,1))
        else:
            solar_direct.append(0.0)
    return solar_direct # W/sf

try:

    for arg in sys.argv[1:]:
        spec = arg.split("=")
        if len(spec) == 1:
            value = None
        elif len(spec) == 2:
            value = spec[1]
        else:
            value = spec[1:]
        tag = spec[0]
        if tag in ["-h","--help","help"]:
            print(__doc__)
            exit(E_OK)
        elif tag in ["-g","--glm"]:
            OUTPUT_NAME = value
        elif tag in ["-c","--csv"]:
            CSV_NAME = value
        elif tag in ["-n","--name"]:
            OBJECT_NAME = value
        elif tag in ["-s","--start"]:
            START_TIME = datetime.datetime.strptime(value,"%Y-%m-%d")
        elif tag in ["-e","--end"]:
            END_TIME = datetime.datetime.strptime(value,"%Y-%m-%d")
        elif tag in ["-p","--position"]:
            LOCATION = list(map(lambda x:float(x),value.split(",")))
            if len(LOCATION) != 2:
                error(f"position '{LOCATION}' is invalid",E_INVALID)
        elif tag in ["-d","--debug"]:
            DEBUG = True
        elif tag in ["-v","--verbose"]:
            VERBOSE = True
        elif tag in ["-q","--quiet"]:
            QUIET = True
        elif tag in ["-v","--verbose"]:
            WARNING = False
        else:
            error(f"option '{arg}' is invalid",E_INVALID)

    if not LOCATION:
        error("location not specified",E_MISSING)

    station_info = find_station(*LOCATION)
    if not START_TIME:
        START_TIME = station_info["inventory"]["hourly"]["start"]
    if not END_TIME:
        END_TIME = station_info["inventory"]["hourly"]["end"]

    station_id = station_info["id"]
    station_cache = CACHE_DIR + "/" + station_id + "_" + START_TIME + "_" + END_TIME + ".csv"
    if os.path.exists(station_cache):
        data = pandas.read_csv(station_cache)
    else:
        data = get_weather(station_id,START_TIME,END_TIME)
        data['solar_direct[W/sf]'] = get_solar(data,*LOCATION)
        data.to_csv(station_cache)

    if not OUTPUT_NAME:

        verbose("writing CSV to stdout")
        print(data.to_csv(),end='')

    elif os.path.splitext(OUTPUT_NAME)[1] == ".csv":

        verbose(f"writing CSV to {OUTPUT_NAME}")
        data.to_csv(OUTPUT_NAME)

    elif os.path.splitext(OUTPUT_NAME)[1] == ".glm":

        if not OBJECT_NAME:
            OBJECT_NAME = f"meteostat_{station_info['id']}"

        if not CSV_NAME:
            CSV_NAME = f"meteostat_{station_info['id']}.csv"

        verbose(f"writing CSV to {CSV_NAME}")
        data.to_csv(CSV_NAME,header=None,na_rep="0")

        verbose(f"writing GLM to {OUTPUT_NAME}")
        with open(OUTPUT_NAME,"w") as glm:
            print(f"// generated by {BASENAME} at {datetime.datetime.now()}",file=glm)
            print("\nmodule climate;",file=glm)
            print("\nclass weather {",file=glm)
            print("    double sunshine_duration[m];",file=glm)
            print("    int16 condition_code;",file=glm)
            print("    double dewpoint[degF];",file=glm)
            print("    double wind_gusts[mph];",file=glm)
            print("}",file=glm)
            print("object weather {",file=glm)
            print(f"    name \"{OBJECT_NAME}\";",file=glm)
            print(f"    latitude {LOCATION[0]};",file=glm)
            print(f"    longitude {LOCATION[1]};",file=glm)
            print("}",file=glm)
            print("\nmodule tape;",file=glm)
            print("object player {",file=glm)
            print(f"    parent \"{OBJECT_NAME}\";",file=glm)
            print(f"    file \"{CSV_NAME}\";",file=glm)
            property_names = [ x.split('[')[0] for x in data.columns ]
            print(f"    property \"{','.join(property_names)}\";",file=glm)
            print("}",file=glm)

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