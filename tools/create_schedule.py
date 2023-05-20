"""Syntax: gridlabd create_schedule CSVFILE [-o|--output=GLMFILE] [OPTIONS ...]

Options
-------
    -o|--output=FILE            specify the output GLM file name
    -s|--skip_rows=LIST         specify row(s) to skip
    -i|--index_cols=LIST        specify which column(s) to use for the time
                                index
    -p|--parse_dates=LIST       specify which column(s) to parse as date/time
    -f|--infer_datetime_format  enable fast date/time parsing
    -c|--use_cols               specify which column to use for schedule
                                values when more than one column is present
    -P|--schedule_precision     specify the precision of the schedule values
    -O|--schedule_options       specify the list of schedule options to
                                enable
    -N|--schedule_name          specify the name of the schedule

Description
-----------

The `create_schedule` tool read the CSV file and constructs a three-tier
scheduled based on the data.  The first tier is the season with, the second
tier is the weekday/weekend daytype, and the third tier is the hour of day.

Example
-------

The following creates a GLM file containing a seasonal, weekday/weekend
schedule:

~~~
$ gridlabd create_schedule my_data.csv -o=my_schedule.glm
~~~

"""

import sys, os
import pandas

EXENAME = os.path.splitext(os.path.basename(sys.argv[0]))[0]

DEBUG = False
WARNING = True
QUIET = False
VERBOSE = False

E_OK = 0
E_INVALID = 1
E_FAILED = 2
E_SYNTAX = 8
E_EXCEPTION = 9
EXITCODE = E_OK

class GldException(Exception):
    pass

def error(msg,code=None):
    if type(code) is int:
        global EXITCODE
        EXITCODE = code
    raise GldException(msg)

# TESTFILE = f"autotest/test_{EXENAME}.csv"
# if os.path.exists(TESTFILE):
#     sys.argv.append(TESTFILE)

def main():

    if not CSVFILE:
        raise GldException("missing input CSV file")

    try:
        global EXITCODE
        data = pandas.read_csv(CSVFILE,
            skiprows = SKIPROWS,
            index_col = INDEXCOLS,
            parse_dates = PARSEDATES,
            usecols = USECOLS)

        if len(data.columns) > 1:

            error(f"too many columns, choose one of {list(data.columns)}",E_INVALID)

        else:

            data['hour'] = data.index.hour
            data['weekday'] = [int(x<5) for x in data.index.weekday]
            data['season'] = [int(x/4) for x in data.index.month]
            data.set_index(['season','weekday','hour'],inplace=True)
            groupby = data.groupby(data.index.names)
            avg = groupby.mean()
            std = groupby.std()
            cnt = groupby.count()
            month = ["1-3","4-6","7-9","10-12"]
            seasons = ["winter","spring","summer","fall"]
            day = ["1-5","6-0"]
            if GLMFILE:
                sys.stdout = open(GLMFILE,"w")
                print(f"schedule {data.columns[0] if not NAME else NAME} {{")
                for season in avg.index.get_level_values(0).unique():
                    if OPTIONS:
                        print(f"  {'; '.join(OPTIONS)};")
                    print(f"  {seasons[season]} {{")
                    for weekday in avg.index.get_level_values(1).unique():
                        for hour in avg.index.get_level_values(2).unique():
                            print(f"     * {hour} * {month[season]} {day[weekday]} {avg.loc[(season,weekday,hour)][0].round(PRECISION)};"
                                + f" // count = {cnt.loc[(season,weekday,hour)][0]},"
                                + f" error = {(std.loc[(season,weekday,hour)][0]/avg.loc[(season,weekday,hour)][0]*100).round(1)}%")
                    print("  }")
                print("}")

    except GldException:

        e_type, e_value, e_trace = sys.exc_info()
        print(f"ERROR [{EXENAME}]: {e_value} (exit code {EXITCODE})",file=sys.stderr)

    except:

        if DEBUG:
            raise

        e_type, e_value, e_trace = sys.exc_info()
        print(f"EXCEPTION [{EXENAME}@{e_trace.tb_lineno}]: {e_type.__name__} {e_value} (exit code {E_EXCEPTION})",file=sys.stderr)
        EXITCODE = E_EXCEPTION

    return EXITCODE

CSVFILE = None # sys.argv[1]
GLMFILE = None # -o|--output

SKIPROWS = None # -s|--skip_rows
INDEXCOLS = [0] # -i|--index_cols
PARSEDATES = [0] # -p|--parse_dates
INFERDATETIMEFORMAT = True # -f|--infer_datetime_format
USECOLS = [0,1] # -c|--use_cols

PRECISION = 3 # -P|--schedule_precision
OPTIONS = None # -O|--schedule_options
NAME = None # -N|--schedule_name

if __name__ == "__main__":

    if len(sys.argv) == 1:
        print(__doc__.split('\n')[0],file=sys.stderr)
        exit(E_SYNTAX)

    for arg in sys.argv[1:]:
        spec = arg.split("=")
        if len(spec) == 1:
            tag = arg
            value = None
        else:
            tag = spec[0]
            value = '='.join(spec[1:])
            if "," in value:
                value = value.split(',')

        if not CSVFILE:
            CSVFILE = arg
        elif tag in ["-o","--output"]:
            if type(value) is list:
                GldException("only one output GLM file is supported",E_INVALID)
            GLMFILE = value
        elif tag in ["-s","--skip_rows"]:
            try:
                SKIPROWS = [int(x) for x in value]
            except:
                SKIPROWS = value
        elif tag in ["-i","--index_cols"]:
            try:
                INDEXCOLS = [int(x) for x in value]
            except:
                INDEXCOLS = value
        elif tag in ["-p","--parse_dates"]:
            try:
                PARSEDATES = [int(x) for x in value]
            except:
                PARSEDATES = value
        elif tag in ["-f","--infer_datetime_format"]:
            INFERDATETIMEFORMAT = True
        elif tag in ["-c","--use_cols"]:
            try:
                USECOLS = [int(x) for x in value]
            except:
                USECOLS = value
        elif tag in ["-P","--schedule_precision"]:
            PRECISION = int(value)
        elif tag in ["-O","--schedule_options"]:
            OPTIONS = value if type(value) is list else [value]
        elif tag in ["-N","--schedule_name"]:
            NAME = value

    EXITCODE = main()
    exit(EXITCODE)

