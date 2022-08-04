"""Manage OpenEI rate database

SYNOPSIS

Shell:

    $ gridlabd openei COMMAND [ARGS ...]

GLM:

    #python -m openei COMMAND [ARGS ...]

COMMAND

Valid commands are

- `update [URL]`: Downloads utility rate data from the GridLAB-D tariff repository

- `eiaid NAME_REGEX`: Finds a utility EIA identifier based on a pattern. The return
  value is the id and utility name. If more than one utility matches, each output
  line contain one matching record.

- `extract EIAID`: Extracts the data for the specified utility and stores it in
  the rates cache folder for access by the `revenue` module's `tariff` class.

- `rate` EIAID RATE_REGEX DATE`: Find the rate label for the tariff name that
  matches the pattern RATE_REGEX on the date given.

- `schedule EIAID LABEL`: Create a GridLAB-D schedule for the specified
  rates on the given date.

- `player` EIAID LABEL`: Create a GridLAB-D player object
  for the specified rates over the date range given.

DESCRIPTION

EXAMPLE

SEE ALSO

"""

import sys, os
import requests
import pandas
from datetime import *

EXECNAME = os.path.basename(os.path.splitext(sys.argv[0])[0])

DEBUG = True
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

def exception(msg,code=None):
    if not QUIET:
        print(f"EXCEPTION [{EXECNAME}]: {msg}",file=sys.stderr,flush=True)

def load_rates(eiaid=None):
    if not eiaid:
        return pandas.read_csv(f"{CACHEDIR}/{CACHEFILE}",index_col=['eiaid','name','startdate'],low_memory=False)
    else:
        return pandas.read_csv(f"{CACHEDIR}/{eiaid}.csv",index_col=['name','startdate'],low_memory=False,parse_dates=['startdate','enddate','latest_update'])

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
    
    elif COMMAND in ["eiaid","id"]:
    
        if len(argv) < 3:
            return error(f"missing utility name pattern",E_INVALID)

        data = load_rates()
        data = data[data["utility"].str.contains(sys.argv[2])]
        for eiaid in data.index.get_level_values(0).unique():
            print(f"{int(eiaid)},\"{data.loc[eiaid]['utility'].unique()[0]}\"")
        return E_OK

    elif COMMAND == 'extract':

        if len(argv) < 3:
            return error(f"missing extract parameters",E_INVALID)
        eiaid = sys.argv[2]

        data = load_rates().reset_index()
        data = data[data["eiaid"]==float(eiaid)]
        data.set_index(['name','startdate']).sort_index().to_csv(f"{CACHEDIR}/{eiaid}.csv",index=True,header=True)
        print(f"{CACHEDIR}/{eiaid}.csv")
        return E_OK

    elif COMMAND in ["rate","tariff"]:

        if len(argv) < 5:
            return error(f"missing rate parameters",E_INVALID)
        
        eiaid = sys.argv[2]
        rate = sys.argv[3]
        date = sys.argv[4]
        data = load_rates(eiaid).reset_index()
        data = data[data["name"].str.contains(rate)]
        data = data[data["startdate"]>=date]
        # data = data[data["enddate"]>date || data["enddate"]=='']
        data.set_index(['startdate','label'],inplace=True)
        data.sort_index(inplace=True)
        print(",".join(data.index.get_level_values(1).unique()))

    elif COMMAND == 'schedule':

        if len(argv) < 4:
            return error(f"missing schedule parameters",E_INVALID)
        
        eiaid = sys.argv[2]
        label = sys.argv[3]
        data = load_rates(eiaid).set_index('label').loc[label]

        print("schedule","rate_"+label,"{")
        print("}")

    elif COMMAND == 'player':

        if len(argv) < 4:
            return error(f"missing player parameters",E_INVALID)
        eiaid = sys.argv[2]
        label = sys.argv[3]
        data = load_rates(eiaid).reset_index().set_index('label').loc[label.split(',')]
        print("timestamp,fixed_charge,demand_charge,energy_charge")
        for label in data.index.get_level_values(0):
            print(",".join([
                data.loc[label]['startdate'].strftime("%Y-%m-%d %H:%M:%S"),
                str(data.loc[label]['fixedchargefirstmeter']),
                str(data.loc[label]['flatdemandstructure/period0/tier0rate']),
                str(data.loc[label]['energyratestructure/period0/tier0rate']),
                ]))

    elif COMMAND == 'class':

        if len(argv) < 4:
            return error(f"missing player parameters",E_INVALID)
        eiaid = sys.argv[2]
        label = sys.argv[3]

        data = load_rates(eiaid).set_index('label').loc[label]
        print("class rate {")
        print("    double fixed_charge[$/unit];")
        print("    double demand_charge[$/kW];")
        print("    double energy_charge[$/kWh];")
        print("}")

    elif COMMAND == 'object':

        print("object rate {")
        print("    object player {")
        print("        file rate_;")
        print("    };")
        print("}")

    else:

        return error(f"commmand '{COMMAND}' is not valid",E_INVALID)

    return E_OK
 
if __name__ == "__main__":
    try:
        rc = main(sys.argv)
    except SystemExit as err:
        rc = err
    except:
        if DEBUG:
            raise
        e_type,e_value,e_trace = sys.exc_info()
        exception(f"({e_type.__name__}) {e_value} ")
        rc = E_EXCEPTION
    exit(rc)
