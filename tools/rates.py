"""Manage OpenEI rate database

SYNOPSIS

Shell:

    $ gridlabd openei COMMAND [ARGS ...]

GLM:

    #python -m openei COMMAND [ARGS ...]

COMMAND

Valid commands are

- `update [URL]`

- `find EIAID [NAME_REGEX] [STARTDATE]`

DESCRIPTION

EXAMPLE

SEE ALSO

"""

import sys, os
import requests
import pandas

EXECNAME = os.path.basename(os.path.splitext(sys.argv[0])[0])

DEBUG = False
VERBOSE = False
WARNING = True
QUIET = False

RATESURL = "http://tariff.gridlabd.us/rates.csv.gz"
CACHEDIR = "/usr/local/share/gridlabd/openei"
CACHEFILE = "rates.csv.gz"

E_OK = 0
E_INVALID = 1
E_FAILED = 2
E_EXCEPTION = 9

def verbose(msg):
    if VERBOSE:
        print(f"VERBOSE [{EXECNAME}]: {msg}",file=sys.stderr,flush=True)

def warning(msg):
    if WARNING:
        print(f"WARNING [{EXECNAME}]: {msg}",file=sys.stderr,flush=True)

def error(msg,code=None):
    if not QUIET:
        print(f"ERROR [{EXECNAME}]: {msg}",file=sys.stderr,flush=True)
    if DEBUG:
        raise msg
    if type(code) is int:
        return code

def load_rates(eiaid,name=None,startdate=None):
    data = pandas.read_csv(f"{CACHEDIR}/{CACHEFILE}",index_col=['eiaid','name','startdate'])
    print(data)
    return data

def main(argv):

    if len(argv) < 2:
        print(f"Syntax: gridlabd {EXECNAME} COMMAND [ARGS ...]",file=sys.stderr)
        return E_INVALID
    elif sys.argv[1] in ['-h','--help','help']:
        print(__doc__)
        return E_OK

    COMMAND = argv[1]

    if COMMAND == "update":

        os.makedirs(CACHEDIR,exist_ok=True)
        reply = requests.get(RATESURL,stream=True)
        if 200 <= reply.status_code < 300:
            with open(f"{CACHEDIR}/{CACHEFILE}","wb") as fh:
                fh.write(reply.content)
        else:
            return error(f"download from {RATESURL} failed with error code {reply.status_code}",E_FAILED)
    
    elif COMMAND == "find":
    
        if len(argv) < 3:
            return error(f"missing EIA ID for utility",E_INVALID)

        data = load_rates(*sys.argv[2:])
        # if len(argv) < 3:
        #     error(f"missing export table name",1)
        # REGEX = argv[3]
        # for TABLE in mdb.list_tables(MDBFILE):
        #     if re.match(REGEX,TABLE):
        #         DF = mdb.read_table(MDBFILE,TABLE)
        #         DF.to_csv(TABLE+".csv")

    else:
        return error(f"commmand '{COMMAND}' is not valid",E_INVALID)
    return 0
 
if __name__ == "__main__":
    try:
        rc = main(sys.argv)
    except SystemExit as err:
        rc = err
    except:
        e_type,e_value,e_trace = sys.exc_info()
        if DEBUG:
            raise
        error(f"{e_type} {e_value}")
        rc = E_EXCEPTION
    exit(rc)
