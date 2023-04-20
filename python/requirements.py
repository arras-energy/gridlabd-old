"""Generate requirements.txt for this OS/CPU combination

This script converts requirements.csv to requirements.txt by choosing the entries
that correspond to the current system information
"""

import sys, os
import platform
import pandas

if platform.system() == "Linux":
    osrelease = pandas.read_csv("/etc/os-release",delimiter="=",header=None,index_col=0).to_dict(orient='index')
    sysinfo = f"{osrelease['ID'][1]}-{osrelease['VERSION_ID'][1]}-{platform.machine()}"
else:
    sysinfo = "-".join([platform.system(),platform.release().split('.')[0],platform.machine()])
csvfile = sys.argv[0].replace(".py",".csv")

PRIORITY = False

if len(sys.argv) == 2:
    if sys.argv[1] == "--sysinfo":
        print(sysinfo)
        exit(0)
    elif sys.argv[1] == "--priority":
        PRIORITY = True
    else:
        csvfile = sys.argv[1]

requirements = pandas.DataFrame(pandas.read_csv(csvfile,index_col=['module']))

if not sysinfo in requirements.columns:
    sysinfo = 'default'

if PRIORITY:
    priority = requirements[~requirements['priority'].isna()].reset_index().set_index(['priority','module']).sort_index()

    for n,row in priority.reset_index().iterrows():
        options = "" if str(row.options) == "nan" else row.options
        print(f"{os.environ['_']} -m pip install {options} {row.module}=={row[sysinfo]}")

else:
    requirements = requirements[requirements['priority'].isna()].stack().reset_index()
    requirements.columns = ["module","system","version"]
    requirements.set_index(["module","system"],inplace=True,verify_integrity=True)
    requirements.sort_index(inplace=True)

    for module in requirements.index.get_level_values(0).unique():
        info = requirements.loc[module]
        version = info.loc[sysinfo]['version']
        print(f"{module}=={version}")
