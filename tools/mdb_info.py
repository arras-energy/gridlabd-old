# mdb_info MDBFILE COMMAND [ARGS ...
"""Convert MDB table to a CSV player

SYNOPSIS

Shell:

    $ gridlabd mdb_info MDBFILE COMMAND [ARGS ...]

GLM:

    #python -m mdb_info MDBFILE COMMAND [ARGS ...]

COMMAND

Valid commands are

- `class REGEX`: create GLM classes from table data (TODO: not implemented yet)

- `tables [REGEX]`: display list of tables matching pattern (all by default)

- `export REGEX`: convert table to CSV format

DESCRIPTION

EXAMPLE

SEE ALSO

"""
import sys, os
from datetime import datetime, timedelta
import pandas_access as mdb
import re

DEBUG = False
VERBOSE = False
WARNING = True
QUIET = False

reload = False

def verbose(msg):
    if VERBOSE:
        print(f"VERBOSE [mdb_info]: {msg}",file=sys.stderr,flush=True)

def warning(msg):
    if WARNING:
        print(f"WARNING [mdb_info]: {msg}",file=sys.stderr,flush=True)

def error(msg,code=None):
    if not QUIET:
        print(f"ERROR [mdb_info]: {msg}",file=sys.stderr,flush=True)
    if DEBUG:
        raise msg
    if type(code) is int:
        exit(code)

def main(argv):

    if len(sys.argv) == 2 and argv[1] in ['-h','--help','help']:
        print(__doc__)
        exit(0)
    elif len(argv) < 3:
        print("Syntax: gridlabd mdb_info MDBFILE COMMAND [ARGS ...]",file=sys.stderr)
        exit(1)

    MDBFILE = argv[1]
    COMMAND = argv[2]

    if COMMAND == "tables":

        if len(argv) > 3:
            REGEX = argv[3]
        else:
            REGEX = ".*"
        for TABLE in mdb.list_tables(MDBFILE):
            if re.match(REGEX,TABLE):
                print(TABLE)
    
    elif COMMAND == "export":
    
        if len(argv) < 3:
            error(f"missing export table name",1)
        REGEX = argv[3]
        for TABLE in mdb.list_tables(MDBFILE):
            if re.match(REGEX,TABLE):
                DF = mdb.read_table(MDBFILE,TABLE)
                DF.to_csv(TABLE+".csv")

    else:
        error(f"commmand '{COMMAND}' is not valid",2)
 
if __name__ == "__main__":
    main(sys.argv)
