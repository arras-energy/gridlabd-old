"""Convert MDB table to a GLM objects

Options:

- `table=TABLE`: specify the table to read

- `csvfile`: specify the csvfile output file name

- `index=[DATECOL[,HOURCOL]]`: specify the index column name (required)

- `parent=COLUMN`: specify the parent column name (required)

- `values=COLUMN[,COLUMN[,...]]`: specify the value column name(s) (required)

- `scale=FLOAT[,FLOAT[,...]]`: specify the scalar for value data (default 1000)

- `chunksize=INTEGER`: specify the chunksize for processing data (default 100000)

- `playermap=CSVNAME`: specify the file to convert parent names to object name (default None)

- `properties=PROPERTY[,PROPERTY[,...]]: specify parent object properties to play into (default is `values`)
"""
import sys, os, subprocess
from io import StringIO
from datetime import datetime, timedelta
import pandas_access as mdb
import pandas

EXENAME = os.path.splitext(os.path.basename(sys.argv[0]))[0]
VERBOSE = False
WARNING = True
DEBUG = False
QUIET = False

# option defaults
CHUNKSIZE = 100000
TABLE = None
CSVFILE = None
INDEXCOLS = None
PARENTCOL = None
SCALE = 1.0
ROUND = 1
VALUECOLS = None
DATEFORMAT = "%m/%d/%y %H:%M:%S"
PROPERTIES = []

def verbose(msg):
    if VERBOSE:
        print(f"VERBOSE [{EXENAME}]: {msg}",file=sys.stderr,flush=True)

def warning(msg):
    if WARNING:
        print(f"WARNING [{EXENAME}]: {msg}",file=sys.stderr,flush=True)

def error(msg,code=None):
    if msg == None:
        etype, evalue, etrace = sys.exc_info()
        ename = etype.__name__
        msg = f"{ename} {evalue}"
    elif DEBUG:
        raise Exception(msg)
    elif not QUIET:
        print(f"ERROR [{EXENAME}]: {msg}",file=sys.stderr,flush=True)
    if type(code) != type(None):
        exit(code)

def debug(msg):
    if DEBUG:
        print(f"DEBUG [{EXENAME}]: {msg}",file=sys.stderr,flush=True)

def convert(input_name,
            output_name = None,
            options = {}):
    """Convert MDB table to a GLM object list
    """

    debug(f"input_name = '{input_name}'")
    debug(f"output_name = '{output_name}'")
    debug(f"options = '{options}'")

    global PROPERTIES
    # options
    if "chunksize" in options.keys():
        global CHUNKSIZE
        CHUNKSIZE = int(options.pop("chunksize"))
        debug(f"chunksize = {CHUNKSIZE}")
    if "table" in options.keys():
        global TABLE
        TABLE = options.pop("table")
        debug(f"table = {TABLE}")
    if "index" in options.keys():
        global INDEXCOLS
        INDEXCOLS = options.pop("index").split(",")
        debug(f"index = {INDEXCOLS}")
    if "parent" in options.keys():
        global PARENTCOL
        PARENTCOL = options.pop("parent")
        debug(f"parent = {PARENTCOL}")
    if "values" in options.keys():
        global VALUECOLS
        VALUECOLS = options.pop("values").split(",")
        PROPERTIES = VALUECOLS
        debug(f"values = {VALUECOLS}")
    if "csvfile" in options.keys():
        global CSVFILE
        CSVFILE = options.pop("csvfile")
        debug(f"csvfile = {CSVFILE}")
    if "scale" in options.keys():
        global SCALE
        SCALE = options.pop("scale").split(",")
        try:
            SCALE = list(map(lambda x:float(x),SCALE))
        except Exception as err:
            error(f"scale is invalid ({err})",1)
        debug(f"scale = {SCALE}")
    if "round" in options.keys():
        global ROUND
        ROUND = options.pop("round)").split(",")
        try:
            ROUND = list(map(lambda x:int(x),ROUND))
        except Exception as err:
            error(f"round is invalid ({err}),1)")
        debug(f"round = {ROUND}")
    if "properties" in options.keys():
        PROPERTIES = options.pop("properties").split(",")
        if len(PROPERTIES) != len(VALUECOLS):
            error("incorrect number of properties",1)
        debug(f"properties = {PROPERTIES}")
    if options:
        error(f"option '{list(options.keys())[0]}' not valid",1)

    # check mdb name
    if not os.path.exists(input_name):
        error(f"'{input_name}' not found",1)
    debug(f"file '{input_name}' ok")

    # check schema
    schema = mdb.read_schema(input_name)
    if TABLE not in schema.keys():
        error(f"table '{TABLE}' not found")
    debug(f"table '{TABLE}' ok")

    # load data
    try:
        DB = mdb.read_table(input_name,TABLE,chunksize=CHUNKSIZE)
    except Exception as err:
        error(f"{input_name} open failed ({err})",1)
    data = []
    index_col = [PARENTCOL]
    index_col.extend(INDEXCOLS)
    usecols = VALUECOLS.copy()
    usecols.extend(index_col)
    for chunk in DB:
        data.append(pandas.read_csv(StringIO(chunk.to_csv()),
            index_col = index_col,
            usecols = usecols,
            parse_dates = [INDEXCOLS[0]],
            infer_datetime_format = False,
            # date_parser = lambda x:[datetime.strptime(y,DATEFORMAT) for y in x],
            ).dropna())
        debug(f"*** Block {len(data)} ***\n {data[-1]}")
        break;
    data = pandas.concat(data).sort_index()
    data.index.names = ["parent","date","hour"]
    data["datetime"] = data.index.get_level_values(1) + pandas.TimedeltaIndex(data.index.get_level_values(2),unit="h")
    data.reset_index(inplace=True)
    data.set_index(["parent","datetime"],inplace=True)
    data.drop(["date","hour"],inplace=True,axis=1)
    for n, name in enumerate(VALUECOLS):
        if type(SCALE) == float:
            data[name] *= SCALE
        elif n > len(SCALE):
            data[name] *= SCALE[-1]
        else:
            data[name] *= SCALE[n]
        if type(ROUND) == int:
            data[name] = data[name].round(ROUND)
        elif n > len(SCALE):
            data[name] = data[name].round(ROUND[-1])
        else:
            data[name] = data[name].round(ROUND[n])

    # write player object
    if not output_name:
        output_name = TABLE.lower() + ".glm"
    try:
        GLM = open(output_name,"w")
    except Exception as err:
        error(f"{output_name} open failed ({err})",1)
    GLM.write(f'''// generated by '{' '.join(sys.argv)}'
#define {TABLE}_STARTTIME={data.index.get_level_values(1).min()}
#define {TABLE}_STOPTIME={data.index.get_level_values(1).max()}
#define {TABLE}_PLAYERS={" ".join(data.index.get_level_values(0).unique())}
module tape;
''')
    for parent in data.index.get_level_values(0).unique():
        GLM.write(f'''object tape.player
{{
    parent "{parent}";
    file "{TABLE}_{parent}.csv";
    property "{','.join(PROPERTIES)}";
}}
''')
        data.loc[parent].to_csv(f"{TABLE}_{parent}.csv",header=False,index=True)
