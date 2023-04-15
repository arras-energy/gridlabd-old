# create_player [OPTIONS ...]
"""Player object creation tool

SYNOPSIS

Shell:

    bash$ gridlabd create_player [-i|--input=INPUTCSV[#READOPTION[#...]][,...]]]
        [-o|--output=OUTPUTCSV[#WRITEOPTION[#...]]]
        [-p|--player=OUTPUTGLM] [-n|--name=OBJNAME] [-t|--target=TARGETNAME]
        [OPTIONS ...]

GLM:

    #create_player -i|--input=INPUTCSV[#READOPTION[#...]][,...]]
        -o|--output=OUTPUTCSV [#WRITEOPTION[#...]]
        [-p|--player=OUTPUTGLM] [-n|--name=OBJNAME] [-t|--target=TARGETNAME]
        [OPTIONS ...]

Options:

    --collation=METHOD  input collation method. Valid methods are `merge`
                        or `concat`. If `merge` is specified, the method
                        of joining data can be specified using METHOD:JOIN.
                        Value join methods are `left`, `right`, `outer`,
                        `inner`, or `cross` (the default is "merge:inner").

    --stack[=NAME[#FORMAT]] 
                        stack input columns after collating using column names
                        as times. NAME specifies the name of the resulting
                        column. FORMAT specifies the format of the time
                        columns (e.g., "%H", "%H:%M", etc.). The default NAME
                        is `value` and the default format is `%H`.

    --resample[=SPEC]   resample the data with the specification SPEC.
                        SPEC=[METHOD]@[FREQ] where FREQ is of the form
                        [[[[[WEEKS:]DAYS:]HOURS:]MINUTES:]SECONDS]
                        ('mean@1:0:0' by default)

    --dropna[=AXIS]     remove rows (AXIS=0 by default) or columns
                        (AXIS=1) with NA values

    --fillna[=VALUE]    replace NA values with value (VALUE=0 by default)

DESCRIPTION

The `create_player` tool collated CSV files and prepares data for use by
GridLAB-D player objects.

Each input file is loaded using the specified read options and concatenated
into a single dataframe.  Date/time values are normalized. The first column
is used as the primary index and the resulting data is sorted accordingly.
Resampling and NA handing are completed and the output is generated to the
specified output file, or to `/dev/stdout` by default.

INPUT

The inputs are specified as comma delimited path names. Options are provided
using # separator in the format OPTION[:VALUE].  If VALUE is omitted, then
the `True` is used.  If the OPTION is already specified in the READOPTIONS
list then the type VALUE of much match the default value's type.  Otherwise
the type of VALUE is assumed to be `str`.

The following READOPTIONS are given by default (see Pandas read_csv for
details):

    delimiter : ','

    header : 0
    
    index_col : 0
    
    parse_dates : [0]

    infer_datetime_format : True
    
    dayfirst : False

The follow WRITEOPIONS are given by default (see Pandas to_csv for details):

    index_label : "datetime"

    date_format : "%Y-%m-%d %H:%M:%S"

    header : False

    float_format : "%.6g"
    
OUTPUT

The output is always written as CSV data that is compatible with GridLAB-D
players, i.e., timestamps in column 0 formatted at `yyyy-mm-dd HH:MM:SS`,
with no header row.

If the PLAYERGLM option is specified, then a player GLM component is written
to the specified PLAYERGLM file.  When used, the name option may specify the
OBJNAME. By the default the OBJNAME is the base name of the first input file.
The output GLM file include a `module` directive to load the `tape` module,
as well as `#define` statement to indicate the start and end dates of the
primary index of the player's input data. Note that the target option must
specify the TARGETNAME (the player's parent object), and the target object
must have properties that match the output column names.

EXAMPLES

The following example prepares the following input data to be used by a player
and generates a player object to read the resulting CSV file.

Input CSV:

    timestamp,real_power
    10/1/14 0:00,1798.85
    10/1/14 0:05,1782.63
    10/1/14 0:10,1766.58
    10/1/14 0:15,1768.91
    ...

Command:

    bash$ gridlabd create_player -i=example/weather.csv \
    -p=/tmp/test.glm -o=/tmp/test.csv -t=test 

CSV output:

    2014-10-01 00:00:00,1798.85
    2014-10-01 00:05:00,1782.63
    2014-10-01 00:10:00,1766.58
    2014-10-01 00:15:00,1768.91
    ...

GLM output:

    #define power_startdate "2014-10-01 00:00:00"
    #define power_stopdate "2017-09-30 23:55:00"
    module tape;
    object player {
        name "power";
        parent "test";
        property "real_power";
        file "/tmp/test.csv";
    }

The following example resamples the input data to obtain the maximum value
in each day.

    bash$ gridlabd create_player -i=example/power.csv -o=/dev/stdout#header \
    --resample=max@1:0:0 
    datetime,real_power
    2014-10-01 00:00:00,1782.63
    2014-10-01 01:00:00,1701.61
    2014-10-01 02:00:00,1634.6
    2014-10-01 03:00:00,1640.41

The following example merges two input files into a single dataset and renames
the columns.

    bash$ gridlabd python -m create_player -i=example/power.csv,example/weather.csv \
    -o=/dev/stdout#header:power,solar_global,air_temperature#index_label:timestamp
    timestamp,power,solar_global,air_temperature
    2014-10-01 00:00:00,1798.85,0,47.8
    2014-10-01 01:00:00,1701.61,0,47.8
    2014-10-01 02:00:00,1628.14,0,47.8
    2014-10-01 03:00:00,1606.77,0,47.8

The following example stacks input from a file with hours in columns.

    bash$ gridlabd create_player -i=example/power_panel.csv --stack
    datetime,value
    2014-10-01 00:00:00,1747.52
    2014-10-01 01:00:00,1669.69
    2014-10-01 02:00:00,1621.81
    ...

SEE ALSO

* [[/Module/Tape/Player]]

"""

import sys, os
import datetime as dt
import pandas as pd

E_OK = 0 # success
E_INVALID = 1 # invalid command option
E_FAILED = 2 # processing failed

DEBUG = False
QUIET = False
VERBOSE = False
WARNING = True

INPUTCSV = ["/dev/stdin"]
OUTPUTCSV = "/dev/stdout"
PLAYERGLM = None
PLAYERNAME = None
TARGETNAME = None
STACKINPUT = ''

READOPTIONS = {
    "delimiter" : ',',
    "header" : 0,
    "index_col" : 0,
    "parse_dates" : [0],
    "infer_datetime_format" : True,
    "dayfirst" : False,
    }

WRITEOPTIONS = {
    "index_label" : "datetime",
    "date_format" : "%Y-%m-%d %H:%M:%S",
    "header" : [],
    "float_format" : "%.6g",
    }

OPTIONS = {
    "collation" : "merge:inner",
}

def main(argv):

    # no arguments provided
    if len(argv) == 1:
        print("""Syntax: gridlabd create_player [-i|--input=INPUTCSV[#READOPTIONS[#...]][,...]] 
        [-o|--output=OUTPUTCSV[#WRITEOPTIONS[#...]] 
        [-p|--player=OUTPUTGLM] [-n|--name=OBJNAME] [-t|--target=TARGETNAME]
        [OPTIONS ...]""",file=sys.stderr)
        exit(E_FAILED)

    # parse argument list
    for arg in [x.split('=') for x in argv[1:]]:
        tag = arg[0]
        if len(arg) == 1:
            value = None
        else:
            value = '='.join(arg[1:]).split(',')
            if len(value) == 1:
                value = value[0]
        if tag in ["-h","--help","help"]:
            print(__doc__)
            exit(E_OK)
        elif tag in ["-v","--verbose"]:
            global VERBOSE
            VERBOSE = True
        elif tag in ["-w","--warn","--warning"]:
            global WARNING
            WARNING = False
        elif tag in ["-q","--quiet","-s","--silent"]:
            global QUIET
            QUIET = True
        elif tag in ["-d","--debug"]:
            global DEBUG
            DEBUG = True
        elif tag in ["-i","--input"]:
            global INPUTCSV
            if type(value) is list:
                INPUTCSV = value
            else:
                INPUTCSV = [value]
        elif tag in ["-o","--output"]:
            global OUTPUTCSV
            if type(value) is list:
                OUTPUTCSV = ','.join(value)
            else:
                OUTPUTCSV = value
        elif tag in ["-p","--player"]:
            global PLAYERGLM
            if type(value) is list:
                error(f"only one player file can be specified ({len(list)} specified)",E_INVALID)
            else:
                PLAYERGLM = value
        elif tag in ["-n","--name"]:
            global PLAYERNAME
            if type(value) is list:
                error(f"only one player object name can be specified ({len(list)} specified)",E_INVALID)
            else:
                PLAYERNAME = value
        elif tag in ["-t","--target"]:
            global TARGETNAME
            if type(value) is list:
                error(f"only one target object name can be specified ({len(list)} specified)",E_INVALID)
            else:
                TARGETNAME = value
        elif tag == "--stack":
            global STACKINPUT
            if value:
                STACKINPUT = value
            else:
                STACKINPUT = "value"
        elif tag[0:2] == "--":
            global OPTIONS
            OPTIONS.update({tag[2:]:value})
        else:
            error(f"option '{'='.join(arg)}' is not valid",E_INVALID)


    if INPUTCSV == ["/dev/stdin"] and sys.stdin.isatty():
        error("no input provided (tty input)",E_INVALID)

    try:

        # load the data
        DATA = []
        for file in INPUTCSV:
            specs = file.split("#")
            file = specs[0]
            if not PLAYERNAME:
                PLAYERNAME = os.path.splitext(os.path.basename(file))[0]
            for item in specs[1:]:
                opt = item.split(":")
                if len(opt) == 1:
                    opt.append(True)
                elif len(opt) > 2:
                    opt = [opt[0],':'.join(opt[1:])]
                if opt[0] in READOPTIONS.keys():
                    otype = type(READOPTIONS[opt[0]])
                else:
                    otype = str
                READOPTIONS[opt[0]] = otype(opt[1])
            DATA.append(pd.read_csv(file,**READOPTIONS))
        
        # collate 
        if OPTIONS["collation"] == "concat":
            DATA = pd.concat(DATA)
        elif OPTIONS["collation"] == "merge" or OPTIONS["collation"].startswith("merge:"):
            specs = OPTIONS["collation"].split(":")
            if len(specs) > 1:
                how = specs[1]
            else:
                how = "inner"
            LEFT = DATA[0]
            for RIGHT in DATA[1:]:
                LEFT = LEFT.merge(RIGHT,left_index=True,right_index=True,how=how)
            DATA = LEFT
        else:
            error(f"collation '{OPTIONS['collation']}' is invalid",E_INVALID)
        del OPTIONS["collation"]

        # stack
        if STACKINPUT:
            specs = STACKINPUT.split("#")
            if len(specs) == 1:
                specs.append("%H")
            DATA = DATA.stack().reset_index()
            dt0 = dt.datetime.strptime("0","%H")
            DATA[DATA.columns[0]] += pd.Series(map(lambda x: dt.datetime.strptime(x,specs[1])-dt0,DATA["level_1"]))
            DATA.set_index(DATA.columns[0],inplace=True)
            DATA.drop(DATA.columns[0],axis=1,inplace=True)
            DATA.columns = [specs[0]]

        # resample
        if "resample" in OPTIONS.keys():
            value = OPTIONS["resample"]
            freq = dt.timedelta(hours=1)
            method = "mean"
            if value:
                spec = value.split('@')
                if spec[0]:
                    method = spec[0]
                if len(spec) > 1 and spec[1]:
                    freq = [int(x) for x in spec[1].split(":")]
                    while len(freq) < 5:
                        freq.insert(0,0)
                    freq = dt.timedelta(weeks=freq[0],days=freq[1],hours=freq[2],minutes=freq[3],seconds=freq[4])
                    if freq.total_seconds() <= 0:
                        error(f"resample frequency must be strictly positive (total_seconds={freq.total_seconds()})",E_INVALID)
            sampler = DATA.resample(freq)
            if not method in dir(sampler):
                error(f"resample cannot apply method '{method}'",E_FAILED)
            DATA = getattr(sampler,method)()
            del OPTIONS["resample"]

        # fill NA
        if "fillna" in OPTIONS.keys():

            if OPTIONS["fillna"] == None:
                OPTIONS["fillna"] = 'pad'
            if OPTIONS["fillna"] in ["pad","backfill","bfill","ffill"]:
                DATA.fillna(method=OPTIONS["fillna"],inplace=True)
            else:
                DATA.fillna(value=float(OPTIONS["fillna"]),inplace=True)
            del OPTIONS["fillna"]
        
        # drop NA
        elif "dropna" in OPTIONS.keys():

            if OPTIONS["dropna"] in ["any","all"]:
                DATA.dropna(how=OPTIONS["dropna"],inplace=True)
            elif OPTIONS["dropna"] == None:
                DATA.dropna(inplace=True)
            else:
                error(f"dropna={OPTIONS['dropna']} is invalid",E_INVALID)
            del OPTIONS["dropna"]

        # handle unknown options
        if len(OPTIONS) > 1:

            error(f"option '--{OPTIONS.keys()[0]}' is not recognized",E_INVALID)

        # prepare to generate output
        specs = OUTPUTCSV.split("#")
        OUTPUTCSV = specs[0]
        for item in specs[1:]:
            opt = item.split(":")
            if len(opt) == 1:
                opt.append('true')
            elif len(opt) > 2:
                opt = [opt[0],':'.join(opt[1:])]
            if opt[0] in WRITEOPTIONS.keys():
                otype = type(WRITEOPTIONS[opt[0]])
            if opt[1].lower() in ['true','false','yes','no']:
                opt[1] = ( opt[1].lower() in ['true','yes'] )
                otype = bool
            elif otype is list:
                opt[1] = opt[1].split(',')
            else:
                otype = str
            WRITEOPTIONS[opt[0]] = otype(opt[1])

        # generate GLM output
        if PLAYERGLM:

            if OUTPUTCSV == "/dev/stdout":
                error(f"unable to generate GLM when CSV is output to stdout",E_FAILED)

            if not TARGETNAME:
                error(f"player does not have a target object name",E_FAILED)

            with open(PLAYERGLM,"w") as glm:
                glm.write(f'''// generated by {' '.join(argv)}
#define {PLAYERNAME}_startdate="{DATA.index.min()}"
#define {PLAYERNAME}_stopdate="{DATA.index.max()}"
#ifndef PLAYERLIST
#define PLAYERLIST={PLAYERNAME}
#else
#set PLAYERLIST={PLAYERNAME} ${{PLAYERLIST}}
#endif
module tape;
object player {{
    name "{PLAYERNAME}";
    parent "{TARGETNAME}";
    property "{','.join(DATA.columns)}";
    file "{OUTPUTCSV}";
}}
''')
            WRITEOPTIONS["header"] = False
        elif not WRITEOPTIONS["header"]:
            WRITEOPTIONS["header"] = DATA.columns

        # generate CSV output
        DATA.sort_index().to_csv(OUTPUTCSV,**WRITEOPTIONS)

    except BrokenPipeError:

        pass

    except KeyboardInterrupt:

        error("keyboard interrupt",E_FAILED)

    except Exception as err:

        etype, evalue, etrace = sys.exc_info()
        fname = os.path.basename(etrace.tb_frame.f_code.co_filename)
        lineno = etrace.tb_lineno
        ename = etype.__name__
        error(f"exception caught at {fname}:{lineno} ({ename} {evalue})",E_FAILED,err)

    exit(E_OK)

def error(msg,code=None,exception=None):
    """Error processing

    msg - error message to display when QUIET is not enabled

    code - exit code to use (None if no exit is desired)

    err - base exception is use when DEBUG is enabled
    """
    if DEBUG:
        raise Exception(msg) from exception
    elif not QUIET:
        print(f"ERROR [create_player]: {msg}",file=sys.stderr)
    if type(code) is int:
        exit(code)

def warning(msg):
    if WARNING:
        print(f"WARNING [create_player]: {msg}",file=sys.stderr)

def verbose(msg):
    if VERBOSE:
        print(f"VERBOSE [create_player]: {msg}",file=sys.stderr)

def debug(msg):
    if DEBUG:
        print(f"DEBUG [create_player]: {msg}",file=sys.stderr)

if __name__ == "__main__":
    main(sys.argv)
