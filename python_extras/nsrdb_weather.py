"""NSRDB weather data tool

SYNOPSIS

Shell:
    bash$ gridlabd nsrdb_weather -y|--year=YEARS -p|-position=LAT,LON 
        [-i|--interpolate=MINUTES|METHOD] [-e|--encode=LAT,LON]
        [-g|--glm=GLMNAME] [-n|--name=OBJECTNAME] [-c|--csv=CSVNAME] 
        [--whoami] [--signup=EMAIL] [--apikey[=APIKEY]]
        [--test] [-v|--verbose] [--clear] [-h|--help|help] 

GLM:
    #system gridlabd nsrdb_weather -y|--year=YEARS -p|-position=LAT,LON 
        [-i|--interpolate=MINUTES|METHOD] [-e|--encode=LAT,LON]
        [-g|--glm=GLMNAME] [-n|--name=OBJECTNAME] [-c|--csv=CSVNAME] 
        [--whoami] [--signup=EMAIL] [--apikey[=APIKEY]]
        [--test] [-v|--verbose] [--clear] [-h|--help|help]
    #include "GLMNAME"

Python:
    bash$ gridlabd python
    >>> import nsrdb_weather as ns
    >>> data = ns.getyears(YEARS,LAT,LON)
    >>> ns.writeglm(data,GLMNAME,OBJECTNAME,CSVNAME)

DESCRIPTION

This module downloads weather data from NSRDB and writes GLM files.  This can
be done from the command line or using call the python API.

Downloaded weather data is for a specified location and year, which must be
provided. The data is downloaded in either 30 or 60 intervals and cached for
later used.  The data that is delivered from the cache can be further
interpolated down to 1 minute. Use the `--clear` option to clear the cache.

By default the weather data is output to /dev/stdout.  If the CSV file name 
is specified using `-c|--csv=CSVNAME, the data will be written to that file.  

If the GLM file name is specified, the CSV file will be formatted for
compatibility with GridLAB-D players and the GLM file will contain a
definition of the weather class, a weather object, and a player object to
feed the weather data in from the CSV.  If the weather object name is not
provided, then the name is automatically generated using a geohash code at
about 2.5 km resolution, e.g., "weather@9q9j6".  To change the geohash
resolution, you must change the `geocode_precision` parameter. To determine
the geohash for a location use the `-e|--encode` option.

The GLM file can be output to "/dev/stdout" for embedding in other GLM files.
For example:

    #include (gridlabd nsrdb_weather -y=2010 -p=37.5,-122.2 -g=/dev/stdout)

This is equivalent to 

    #gridlabd gridlabd nsrdb_weather -y=2010 -p=37.5,-122.2 -g=/tmp/example.glm
    #include "/tmp/example.glm"

without the creation of the temporary file.

The global `${WEATHER}` is set to a space-delimited list of the weather 
objects defined in the GLM file.

PARAMETERS

The module uses several parameters to control its behavior. 

    leap = True # include leap day in data
    interval = 60 # sample interval, may be 30 or 60 minutes
    utc = False # timestamps in UTC
    email="gridlabd@gmail.com" # credential email
    verbose = False # verbose output enable
    server = "https://developer.nrel.gov/api/solar/nsrdb_psm3_download.csv" # NSRDB server URL
    cachedir = "/usr/local/share/gridlabd/weather" # local NSRDB cache folder
    attributes = 'ghi,dhi,dni,cloud_type,dew_point,air_temperature,surface_albedo,wind_speed,wind_direction,solar_zenith_angle' # NSRDB fields to download
    credential_file = f"{os.getenv('HOME')}/.nsrdb/credentials.json" # local credential file location
    geocode_precision = 5 # about 2.5 km geohash resolution (uses for automatic naming of weather objects)
    float_format="%.1f"

The geocode precisions are roughly as follows:

    1   2,500 km
    2   600 km
    3   80 km
    4   20 km
    5   2.5 km
    6   200 m
    7   80 m
    8   20 m
    9   2.5 m
    10  60 cm
    11  7.5 cm

You can change these options in Python scripts.

    >>> import nsrdb_weather as ns
    >>> ns.interval = 30
    >>> data = ns.getyear(2014,45.62,-122.70)

You can permanently change these options by creating the local or shared file
called `nsrdb_weather_config.py`. If found, this file will be imported after
the defaults are set. Note that the default year, position, glm, csv, and name
cannot be changed.

CREDENTIALS

You must obtain an API key from https://developer.nrel.gov/signup/.  Save the key
in the credentials file, which is by default `$HOME/.nsrdb/credentials.json`.

You can run this process in a semi-automated manner using the command

    bash$ gridlabd nsrdb_weather --signup

with which you can copy and paste a new key in the credential file.

CONFIGURATION

The nsrdb_weather module loads the file `nsrdb_weather_config.py` after setting the 
default values of the configure.  The configuration can be changed by creating this
file in the current folder, or in a folder in the python path.

CAVEATS

Normally the column units are included in the column names when the CSV file is written. However
when the GLM file is written, the column units are not included in the column names.  The units
are given as part of the `weather` class definition generated by the GLM writer.

EXAMPLE

The following command downloads only the CSV data for a location:

    bash$ gridlabd nsrdb_weather -y=2014,2015 -p=45.62,-122.70 -c=test.csv

The following command downloads the CSV data and creates a GLM file with the data linked and weather object named:

    bash$ gridlabd nsrdb_weather -y=2014,2015 -p=45.62,-122.70 -c=test.csv -n=test -g=test.glm

SEE ALSO

* [https://nsrdb.nrel.gov/data-sets/api-instructions.html]
"""

import sys, os, json, requests, pandas, numpy, datetime

leap = True
interval = 60
utc = False
email = None # by default this will be the first key in the credentials file
interpolate_time = None
interpolate_method = 'linear'
server = "https://developer.nrel.gov/api/solar/nsrdb_psm3_download.csv"
cachedir = "/usr/local/share/gridlabd/weather"
attributes = 'ghi,dhi,dni,cloud_type,dew_point,air_temperature,surface_albedo,wind_speed,wind_direction,solar_zenith_angle,relative_humidity,surface_pressure'
credential_file = f"{os.getenv('HOME')}/.nsrdb/credentials.json"
geocode_precision = 6 
float_format="%.1f"
date_format="%Y-%m-%d %H:%M:%S"
verbose_enable = False

try:
    from nsrdb_weather_config import *
except:
    pass

def error(msg,code=None):
    """Display an error message and exit if code is a number"""
    if code != None:
        print(f"ERROR [nsrdb_weather]: {msg}",file=sys.stderr)
        exit(code)
    else:
        raise Exception(msg)

def syntax(code=0):
    """Display docs (code=0) or syntax help and exit (code!=0)"""
    if code == 0:
        print(__doc__)
    else:
        print(f"Syntax: {os.path.basename(sys.argv[0])} -y|--year=YEARS -p -position=LAT,LON")
        print("\t[-i|--interpolate=MINUTES|METHOD]\n\t[-g|--glm[=GLMNAME]] [-n|--name=OBJECTNAME] [-c|--csv=CSVNAME]\n\t[--whoami] [--signup=EMAIL] [--apikey[=APIKEY]]\n\t[--test] [-v|--verbose] [-h|--help|help]")
    exit(code)

def verbose(msg):
    """Display a verbose message (verbose_enable must be True"""
    if verbose_enable:
        print(f"[{os.path.basename(sys.argv[0])}]: {msg}",file=sys.stderr)

def getemail():
    """Get the default email"""
    global email
    if not email:
        keys = getkeys().keys()
        if keys:
            email = list(getkeys().keys())[0]
        else:
            email = None
    return email

def addkey(apikey=None):
    """Manage NSRDB API keys"""
    global email
    global credential_file
    if not email:
        email = getemail()
    keys = getkeys()
    if email:
        if apikey or not email in keys.keys():
            keys[email] = apikey
        elif not apikey and email in keys.keys():
            del keys[email]
        with open(credential_file,"w") as f:
            json.dump(keys,f)

def getkeys():
    """Get all NSRDB API keys"""
    global credential_file
    try:
        with open(credential_file,"r") as f: 
            keys = json.load(f)
    except:
        keys = {}
    return keys

def getkey(email=None):
    """Get a single NSRDB API key"""
    if not email:
        email = getemail()
    if email:
        return getkeys()[email]
    else:
        return None

def getyears(years,lat,lon,concat=True):
    """Get NSRDB weather data for multiple years"""
    try:
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
        if interpolate_time:
            final = []
            if concat:
                dflist = [result["DataFrame"]]
            else:
                dflist = result["DataFrame"]
            for data in dflist:
                verbose(f"getyears(years={years},lat={lat},lon={lon}): interpolating {interval} minute data to {interpolate_time} minutes using {interpolate_method} method")
                starttime = data.index.min()
                stoptime = data.index.max()
                daterange = pandas.DataFrame(index=pandas.date_range(starttime,stoptime,freq=f"{interpolate_time}min"))
                final.append(data.join(daterange,how="outer",sort=True).interpolate(interpolate_method))
            if concat:
                result["DataFrame"] = pandas.concat(final)
            else:
                result["DataFrame"] = final
        return result
    except Exception as err:
        if verbose_enable:
            raise
        else:
            error(f"unable to get data ({err})",2)

def heat_index(T,RH):
    """Compute the heat index for a temperature T (in degF) and relative humidity RH (in %)"""
    if T < 80 :
        return 0.75*T + 0.25*( 61.0+1.2*(T-68.0)+0.094*RH)
    else:
        HI = -42.379 \
            + 2.04901523*T \
            + 10.14333127*RH \
            - 0.22475541*T*RH \
            - 0.00683783*T*T \
            - 0.05481717*RH*RH \
            + 0.00122874*T*T*RH \
            + 0.00085282*T*RH*RH \
            - 0.00000199*T*T*RH*RH
        if RH < 13 and T < 112:
            return HI - ((13-RH)/4)*sqrt((17-fabs(T-95.))/17)
        elif RH > 85 and T < 87:
            return HI + ((RH-85)/10) * ((87-T)/5)
        else:
            return HI

def getcache(year,lat,lon,refresh=False):
    cache = f"{cachedir}/nsrdb/{year}/{geohash(lat,lon)}.csv"
    os.makedirs(os.path.dirname(cache),exist_ok=True)
    api = getkey()
    if not os.path.exists(cache) or refresh:
        with open(cache,"w") as fout:
            url = f"{server}?wkt=POINT({lon}%20{lat})&names={year}&leap_day={str(leap).lower()}&interval={interval}&utc={str(utc).lower()}&api_key={api}&attributes={attributes}&email={email}&full_name=None&affiliation=None&mailing_list=false&reason=None"
            verbose(f"getyear(year={year},lat={lat},lon={lon}): downloading data from {url}")
            fout.write(requests.get(url).content.decode("utf-8"))
            verbose(f"getyear(year={year},lat={lat},lon={lon}): saved data to {cache}")
    return cache

def getyear(year,lat,lon):
    """Get NSRDB weather data for a single year"""
    api = getkey()
    url = f"{server}?wkt=POINT({lon}%20{lat})&names={year}&leap_day={str(leap).lower()}&interval={interval}&utc={str(utc).lower()}&api_key={api}&attributes={attributes}&email={email}&full_name=None&affiliation=None&mailing_list=false&reason=None"
    cache = f"{cachedir}/nsrdb/{year}/{geohash(lat,lon)}.csv"
    try:
        result = pandas.read_csv(cache,nrows=1).to_dict(orient="list")
        try:
            result.update(dict(Year=[year],DataFrame=[pandas.read_csv(cache,skiprows=2)]))
            verbose(f"getyear(year={year},lat={lat},lon={lon}): reading data from {cache}")
        except Exception as err:
            os.remove(cache)
            raise Exception(f"cache file '{cache}' is not readable ({err}), try again later")
    except:
        result = None
    if not result:
        os.makedirs(os.path.dirname(cache),exist_ok=True)
        with open(cache,"w") as fout:
            verbose(f"getyear(year={year},lat={lat},lon={lon}): downloading data from {url}")
            fout.write(requests.get(url).content.decode("utf-8"))
            verbose(f"getyear(year={year},lat={lat},lon={lon}): saved data to {cache}")
        try:
            result = pandas.read_csv(cache,nrows=1).to_dict(orient="list")
        except Exception as err:
            os.remove(cache)
            raise Exception(f"cache file '{cache}' is not readable ({err}), try again later")
        result.update(dict(Year=[year],DataFrame=[pandas.read_csv(cache,skiprows=2)]))
    for data in result["DataFrame"]:
        data["datetime"] = list(map(lambda x: datetime.datetime(x[0],x[1],x[2],x[3],0,0),numpy.array([data.Year,data.Month,data.Day,data.Hour]).transpose()))
        data.set_index("datetime",inplace=True)
        data.drop(columns=["Year","Day","Month","Hour","Minute"],inplace=True)
        data.columns = [
            "solar_global[W/sf]",
            "solar_horizontal[W/sf]",
            "solar_direct[W/sf]",
            "clouds",
            "dewpoint[degF]",
            "temperature[degF]",
            "ground_reflectivity[pu]",
            "wind_speed[m/s]",
            "wind_dir[rad]",
            "solar_altitude[deg]",
            "humidity[%]",
            "pressure[mbar]",
            ]
        data["solar_global[W/sf]"] /= 10.7639
        data["solar_horizontal[W/sf]"] /= 10.7639
        data["solar_direct[W/sf]"] /= 10.7639
        data["dewpoint[degF]"] = data["dewpoint[degF]"]*9/5+32
        data["temperature[degF]"] = data["temperature[degF]"]*9/5+32
        data["wind_dir[rad]"] *= 3.141592635/180
        data["heat_index[degF]"] = list(map(lambda x:heat_index(x[0],x[1]),zip(data["temperature[degF]"],data["humidity[%]"])))
        data.index.name = "datetime"
    return result

def decode_exactly(geohash):
    """
    Decode the geohash to its exact values, including the error
    margins of the result.  Returns four float values: latitude,
    longitude, the plus/minus error for latitude (as a positive
    number) and the plus/minus error for longitude (as a positive
    number).
    """
    __base32 = '0123456789bcdefghjkmnpqrstuvwxyz'
    __decodemap = { }
    for i in range(len(__base32)):
        __decodemap[__base32[i]] = i
    del i
    lat_interval, lon_interval = (-90.0, 90.0), (-180.0, 180.0)
    lat_err, lon_err = 90.0, 180.0
    is_even = True
    for c in geohash:
        cd = __decodemap[c]
        for mask in [16, 8, 4, 2, 1]:
            if is_even: # adds longitude info
                lon_err /= 2
                if cd & mask:
                    lon_interval = ((lon_interval[0]+lon_interval[1])/2, lon_interval[1])
                else:
                    lon_interval = (lon_interval[0], (lon_interval[0]+lon_interval[1])/2)
            else:      # adds latitude info
                lat_err /= 2
                if cd & mask:
                    lat_interval = ((lat_interval[0]+lat_interval[1])/2, lat_interval[1])
                else:
                    lat_interval = (lat_interval[0], (lat_interval[0]+lat_interval[1])/2)
            is_even = not is_even
    lat = (lat_interval[0] + lat_interval[1]) / 2
    lon = (lon_interval[0] + lon_interval[1]) / 2
    return lat, lon, lat_err, lon_err

def geocode(geohash):
    """
    Decode geohash, returning two strings with latitude and longitude
    containing only relevant digits and with trailing zeroes removed.
    """
    lat, lon, lat_err, lon_err = decode_exactly(geohash)
    from math import log10
    # Format to the number of decimals that are known
    lats = "%.*f" % (max(1, int(round(-log10(lat_err)))) - 1, lat)
    lons = "%.*f" % (max(1, int(round(-log10(lon_err)))) - 1, lon)
    if '.' in lats: lats = lats.rstrip('0')
    if '.' in lons: lons = lons.rstrip('0')
    return float(lats), float(lons)

def geohash(latitude, longitude, precision=geocode_precision):
    """Encode a position given in float arguments latitude, longitude to
    a geohash which will have the character count precision.
    """
    from math import log10
    __base32 = '0123456789bcdefghjkmnpqrstuvwxyz'
    __decodemap = { }
    for i in range(len(__base32)):
        __decodemap[__base32[i]] = i
    del i
    lat_interval, lon_interval = (-90.0, 90.0), (-180.0, 180.0)
    geohash = []
    bits = [ 16, 8, 4, 2, 1 ]
    bit = 0
    ch = 0
    even = True
    while len(geohash) < precision:
        if even:
            mid = (lon_interval[0] + lon_interval[1]) / 2
            if longitude > mid:
                ch |= bits[bit]
                lon_interval = (mid, lon_interval[1])
            else:
                lon_interval = (lon_interval[0], mid)
        else:
            mid = (lat_interval[0] + lat_interval[1]) / 2
            if latitude > mid:
                ch |= bits[bit]
                lat_interval = (mid, lat_interval[1])
            else:
                lat_interval = (lat_interval[0], mid)
        even = not even
        if bit < 4:
            bit += 1
        else:
            geohash += __base32[ch]
            bit = 0
            ch = 0
    return ''.join(geohash)


def writeglm(data, glm=None, name=None, csv=None):
    """Write weather object based on NSRDB data

    Default GLM and CSV values are handled as follows
    GLM    CSV    Output
    ------ ------ ------
    None   None   CSV->stdout
    GLM    None   GLM, CSV->GLM/.glm/.csv
    None   CSV    GLM->stdout, CSV
    GLM    CSV    GLM, CSV

    The default name is "weather@GEOCODE"

    The WEATHER global is set to the list of weather object names.
    """
    lat = data['Latitude'][0]
    lon = data['Longitude'][0]
    if not name:
        name = f"weather@{geohash(lat,lon,geocode_precision)}"
    if type(data["DataFrame"]) is list:
        weather = pandas.concat(data["DataFrame"])
    else:
        weather = data["DataFrame"]
    if not csv and not glm:
        weather.to_csv("/dev/stdout",header=True,float_format=float_format,date_format=date_format)
        return dict(glm=None,csv="/dev/stdout",name=None)
    if not glm:
        glm = "/dev/stdout"
    if not csv:
        csv = f"{name}.csv"
    with open(glm,"w") as f:
        f.write("class weather\n{\n")
        for column in weather.columns:
            f.write(f"\tdouble {column};\n")
        f.write("}\n")
        weather.columns = list(map(lambda x:x.split('[')[0],weather.columns))
        f.write("module tape;\n")
        f.write("#ifdef WEATHER\n")
        f.write(f"#set WEATHER=$WEATHER {name}\n")
        f.write("#else\n")
        f.write(f"#define WEATHER={name}\n")
        f.write("#endif\n")
        f.write("object weather\n{\n")
        f.write(f"\tname \"{name}\";\n")
        f.write(f"\tlatitude {lat};\n")
        f.write(f"\tlongitude {lon};\n")
        f.write("\tobject player\n\t{\n")
        f.write(f"\t\tfile \"{csv}\";\n")
        f.write(f"\t\tproperty \"{','.join(weather.columns)}\";\n")
        f.write("\t};\n")
        f.write("}\n")
    weather.to_csv(csv,header=False,float_format=float_format,date_format="%s")
    return dict(glm=glm,csv=csv,name=name)

if __name__ == "__main__":
    year = None
    position = None
    glm = None
    name = None
    csv = None
    if len(sys.argv) == 1:
        syntax(1)
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
        elif token == "--debug":
            debug_enable = True
        elif token in ["-y","--year"]:
            year = []
            for y in value.split(","):
                yy = y.split("-")
                if len(yy) == 1:
                    year.append(int(yy[0]))
                elif len(yy) == 2:
                    year.extend(range(int(yy[0]),int(yy[1])+1))
                else:
                    raise Exception("'{value}' is not a valid invalid year specification")
        elif token in ["-p","--position"]:
            position = value.split(",")
            if len(position) != 2:
                error("position is not a tuple",1)
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
            year = [2014,2015]
            position = [45.62,-122.70]
            glm = "test.glm"
            writeglm(getyears(year,float(position[0]),float(position[1])),glm,name,csv)
            exit(os.system(f"gridlabd {glm}"))
        elif token == "--signup":
            if not value:
                error("you must provide an email address for the new credential",1)
            credentials = getkeys()
            if getemail() in credentials.keys():
                error(f"you already have credentials for {value}",1)
            else:
                email = value
                addkey("PASTE_YOUR_APIKEY_HERE")
            import webbrowser
            webbrowser.open("https://developer.nrel.gov/signup/")
            print(f"use `gridlabd nsrdb_weather --apikey=<your-apikey>` to set your api key")
        elif token == "--apikey":
            if not getemail():
                error(f"you have not signed up yet, use `gridlabd {os.path.basename(sys.argv[0]).replace('.py','')} --signup=<your-email>` to sign up",1)
            key = getkey(email)
            addkey(value)
            if not value:
                print(f"key for {email} deleted, use `gridlabd {os.path.basename(sys.argv[0]).replace('.py','')} --apikey={key}` to restore it")
        elif token == "--whoami":
            if not getemail():
                error(f"you have not signed up yet, use `gridlabd {os.path.basename(sys.argv[0]).replace('.py','')} --signup=<your-email>` to sign up",1)
            print(email,file=sys.stdout)
        elif token in ["-v","--verbose"]:
            verbose_enable = not verbose_enable
        elif token in ["-e","--encode"]:
            position = value.split(",")
            if len(position) != 2:
                error("position is not a tuple",1)
            print(geohash(float(position[0]),float(position[1])),file=sys.stdout)
        elif token in ["--clear"]:
            import shutil
            shutil.rmtree(cachedir)
        else:
            error(f"option '{token}' is not valid",1)
    if position and year:
        data = getyears(year,float(position[0]),float(position[1]))
        writeglm(data,glm,name,csv)

    if position and year:
        try:
            data = getyears(year,float(position[0]),float(position[1]))
            writeglm(data,glm,name,csv)
        except Exception as err:
            if not debug_enable:
                error(err,1)
            raise

