# Syntax: isone [OPTIONS ...]
"""ISO New England data access module

Syntax
------

Shell:

~~~
gridlabd isone [OPTIONS ...]
~~~

GLM:

~~~
#python -m isone [OPTIONS ...]
~~~

Options
-------
    -d|--dataset=DATASET/SUBSET     Download data from the specified dataset
                                    (and subset, default is `smd/ISO NE CA`)
    --debug                         Enable traceback output on exceptions
    -c|--csv=FILENAME               Save CSV data to the specified file
                                    (default is /dev/stdout)
    -C|--CLASS=CLASSNAME            Specify the GLM class name (default is
                                    `isone`)
    -e|--end=YEAR|STOPDATE          Download data ending on the specified year
                                    or date (last month default)
    -f|--freshen                    Freshen the data cache
    -g|--glm=FILENAME               Save GLM data to the specified file
                                    (default is /dev/null)
    -h|--help|help                  Output the help documentation
    -l|--list                       Output a list of available sheets in dataset
    -N|--NAME=OBJECTNAME            Specify the GLM object name (default is
                                    `isone`)
    -q|--quiet                      Suppress error output messages
    -s|--start=YEAR|STARTDATE       Download data starting on the specified
                                    year or date (last month by default)
    -v|--verbose                    Enable verbose output
    -y|--year[=YEAR]                Download data for the specified year
                                    (current year by default)

Currently the only supported dataset is `smd`. The valid subsets depend on the
dataset and year. See https://www.iso-ne.com/isoexpress/web/reports/load-and-demand/-/tree/zone-info
for more information.

Example
-------

The following generates the wholesale market data New Hampshire for May 2023:

~~~
sh$ gridlabd isone -d=smd/NH -f -s=2023-05-01 -e=2023-06-01
timestamp,DA_Demand,RT_Demand,DA_LMP,DA_EC,DA_CC,DA_MLC,RT_LMP,RT_EC,RT_CC,RT_MLC,Dry_Bulb,Dew_Point
2023-05-01 00:00:00,801.30,924.94,24.63,24.64,-0.05,0.04,18.96,18.75,0.18,0.03,53,51
2023-05-01 01:00:00,807.90,899.31,20.61,20.59,-0.03,0.05,18.88,18.70,0.18,0.00,54,53
2023-05-01 02:00:00,765.20,885.84,20.56,20.57,-0.03,0.02,18.12,17.73,0.41,-0.02,54,53
...
2023-06-01 21:00:00,1753.00,1594.43,58.33,57.55,0.00,0.78,41.90,41.65,0.00,0.25,75,61
2023-06-01 22:00:00,1495.20,1425.32,50.57,50.31,0.00,0.26,35.95,35.82,0.00,0.13,77,59
2023-06-01 23:00:00,1269.60,1283.86,53.36,52.86,0.00,0.50,36.52,36.40,0.00,0.12,77,59
~~~
"""

import sys, os
import datetime
import pandas
import requests
import traceback

EXECNAME = os.path.splitext(os.path.basename(sys.argv[0]))[0]
DATASET = "smd"
CSVFILE = "/dev/stdout"
GLMFILE = None
CLASSNAME = "isone"
OBJNAME = "isone"
STARTDATE = None
STOPDATE = None
CACHEDIR = "/usr/local/opt/gridlabd/current/share/gridlabd/isone.d/" \
    if not "GLD_ETC" in os.environ else os.path.join(os.environ['GLD_ETC'],"isone")
DATEFORMAT = "%Y-%m-%d"
FLOATFORMAT = "%.2f"
FRESHEN = False
VERBOSE = False
QUIET = False
DEBUG = False

E_OK = 0
E_SYNTAX = 1
E_INVALID = 2
E_FAILED = 3

def error(msg,code=None):
    if not QUIET:
        print(f"ERROR [{EXECNAME}]: {msg}",flush=True,file=sys.stderr)
    if not code is None:
        exit(code)

def verbose(msg):
    if VERBOSE:
        print(f"VERBOSE [{EXECNAME}]: {msg}",flush=True,file=sys.stderr)

def get_year(year=None,dataset=None):
    """Get data from a year
    :param year: int - the starting date in DATEFORMAT
    :return: pandas.DataFrame - the requested dataset
    :return: list - list of available subsets (if dataset is None)
    """
    if year == None:
        year = datetime.datetime.now().year
        verbose(f"setting to default year '{year}'")
    assert(type(year)==int)
    if dataset == None:
        dataset = DATASET
        verbose(f"setting to default dataset '{dataset}'")
    specs = dataset.split("/")
    cachefile = os.path.join(CACHEDIR,f"{specs[0]}_{year}.xlsx")
    if FRESHEN or not os.path.exists(cachefile):
        verbose(f"creating cache directory '{CACHEDIR}'")
        os.makedirs(CACHEDIR,exist_ok=True)
        try:
            url = f"https://www.iso-ne.com/static-assets/documents/{year}/02/{year}_{specs[0]}_hourly.xlsx"
            verbose(f"downloading data from '{url}'")
            req = requests.get(url)
            if req.status_code == 200:
                verbose(f"saving data to '{cachefile}'")
                with open(cachefile,"wb") as xls:
                    xls.write(req.content)
            else:
                error(f"unable to download data from '{url}' (HTTP code {req.status_code})",E_FAILED)
        except Exception as err:
            error(f"unable to get data from '{url}' ({err})",E_FAILED)
    if len(specs) > 1:
        verbose(f"loading '{specs[1]}' from '{cachefile}'")
        return pandas.read_excel(cachefile,sheet_name=specs[1],index_col=[0,1],parse_dates=[0])
    else:
        verbose(f"loading data from '{cachefile}'")
        return pandas.read_excel(cachefile,sheet_name=None)

def get_data(startdate=None,stopdate=None,dataset=None):
    """Get data from a date range
    :param startdate: str - the starting date in DATEFORMAT
    :param stopdate: str - the stopping date in DATEFORMAT
    :param dataset: str - the dataset/subset specification
    :return: pandas.DataFrame - the requested data
    """
    if startdate == None:
        startdate = datetime.datetime(year=datetime.datetime.now().year)
    else:
        startdate = datetime.datetime.strptime(startdate,DATEFORMAT)
    if startdate == None:
        stopdate = datetime.datetime.now()
    else:
        stopdate = datetime.datetime.strptime(stopdate,DATEFORMAT)
    if startdate > stopdate:
        error("startdate is after stopdate",E_INVALID)
    data = []
    for year in range(startdate.year,stopdate.year+1):
        data.append(get_year(year,dataset))
    data = pandas.concat(data)
    maxdate = min(data.index.get_level_values(0).max(),stopdate)
    return data.loc[pandas.date_range(startdate,maxdate)]

def fix_timestamps(df):
    """Fix timestamp in dataframe
    :param data: pandas.DataFrame - data to fix
    :return: pandas.DataFrame - fixed data
    """
    df['timestamp'] = [x[0] + datetime.timedelta(hours = int(x[1])-1) for x in list(df.index)]
    return df.set_index('timestamp')

if __name__ == "__main__":
    if len(sys.argv) == 1:
        for line in __doc__.split("\n"):
            if line.startswith("Syntax: "):
                print(line,file=sys.stderr)
                exit(E_SYNTAX)
    elif sys.argv[1] in ["-h","--help","help"]:
        print(__doc__)
        exit(E_OK)
    for arg in sys.argv[1:]:
        spec = arg.split("=")
        if len(spec) == 1:
            token = arg
            value = None
        elif len(spec) == 2:
            token = spec[0]
            value = spec[1]
        else:
            token = spec[0]
            value = "=".join(spec[1:])

        if token in ["-v","--verbose"]:
            VERBOSE = True
        elif token in ["-q","--quiet"]:
            QUIET = True
        elif token in ["--debug"]:
            DEBUG = True

        # -d|--dataset=DATASET/SUBSET     Download data from the specified dataset
        #                                 (and subset, default is `smd/ISO NE CA`)
        elif token in ["-d","--dataset"]:
            if value:
                DATASET = value 
            else:
                error("dataset not specified",E_SYNTAX)

        # -c|--csv=FILENAME               Save CSV data to the specified file
        #                                 (default is /dev/stdout)
        elif token in ["-c","--csv"]:
            if value:
                CSVFILE = value
            else:
                error("csvfile not specified",E_SYNTAX)

        # -C|--CLASS=CLASSNAME            Specify the GLM class name (default is
        #                                 `isone`)
        elif token in ["-C","--CLASS"]:
            if value:
                CLASSNAME = value
            else:
                error("class name not specified",E_SYNTAX)

        # -e|--end=YEAR|STOPDATE          Download data ending on the specified year
        #                                 or date (last month default)
        elif token in ["-e","--end"]:
            if value:
                try:
                    STOPDATE = int(value)
                except:
                    STOPDATE = value

        # -f|--freshen                    Freshen the data cache
        elif token in ["-f","--freshen"]:
            FRESHEN = True

        # -g|--glm=FILENAME               save GLM data to the specified file
        #                                 (default is /dev/null)
        elif token in ["-g","--glm"]:
            if value:
                GLMFILE = value
            else:
                error("GLM name not specified",E_SYNTAX)

        # -l|--list                       output a list of available datasets
        elif token in ["-l","--list"]:
            print("\n".join(get_year(value)),file=sys.stdout)
            exit(E_OK)

        # -N|--NAME=OBJECTNAME            specify the GLM object name (default is
        #                                 `isone`)
        elif token in ["-N","--NAME"]:
            if value:
                OBJNAME = value
            else:
                error("object name not specified",E_SYNTAX)

        # -s|--start=YEAR|STARTDATE       download data starting on the specified
        #                                 year or date (last month by default)
        elif token in ["-s","--start"]:
            if value:
                try:
                    STARTDATE = str(int(value))
                except:
                    STARTDATE = value

        # -y|--year[=YEAR]                download data for the specified year
        #                                 (current year by default)
        elif token in ["-y","--year"]:
            if value:
                try:
                    STARTDATE = value + "-01-01"
                    STOPDATE = value + "-12-31"
                except:
                    error(f"year '{value}' is invalid",E_INVALID)
            else:
                error("year not specified",E_SYNTAX)

        # invalid argument
        else:
            error(f"argument '{arg}' is not recognized",E_SYNTAX)

    try:
        data = get_data(STARTDATE,STOPDATE)

        if GLMFILE:
            if CSVFILE.startswith("/dev/"):
                error(f"CSV file '{CSVFILE}' must be a regular file",E_INVALID)

            # classinfo = CLASSNAME.split(".")

            properties = "\n    ".join([f'double {x};' for x in data.columns])
            with open(GLMFILE,"w") as glm:
                glm.write(f"""// created by '{' '.join(sys.argv)}' at {datetime.datetime.now()}
class {CLASSNAME}
{{
    {properties}
}}
module tape;
object {CLASSNAME}
{{
    name {OBJNAME};
    object player
    {{
        file "{CSVFILE}";
        property "{','.join(data.columns)}";
    }};
}}
#define ISONE_TIMEZONE=EST+5EDT
#define ISONE_STARTDATE={data.index.get_level_values(0).min()}
#define ISONE_STOPDATE={data.index.get_level_values(0).max()+datetime.timedelta(days=1)}
""")

        fix_timestamps(data).to_csv(CSVFILE,float_format=FLOATFORMAT,header=(GLMFILE is None))

        exit(E_OK)
    except SystemExit as err:
        exit(err.code)
    except:
        e_type,e_value,e_trace = sys.exc_info()
        if DEBUG:
            traceback.print_tb(e_trace)
        error(f"{e_value} ({e_type.__name__} exception)",E_FAILED)
