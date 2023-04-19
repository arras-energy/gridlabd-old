"""Generate requirements.txt for this OS/CPU combination

This script converts requirements.csv to requirements.txt by choosing the entries
that correspond to the current system information
"""

import sys
import platform
import pandas

sysinfo = "-".join([platform.system(),platform.release().split('.')[0],platform.machine()])
csvfile = sys.argv[0].replace(".py",".csv")

if len(sys.argv) == 2:
    if sys.argv[1] == "--sysinfo":
        print(sysinfo)
        exit(0)
    else:
        csvfile = sys.argv[1]

requirements = pandas.DataFrame(pandas.read_csv(csvfile,index_col=['module']).stack().reset_index())
requirements.columns = ["module","system","version"]
requirements.set_index(["module","system"],inplace=True,verify_integrity=True)
requirements.sort_index(inplace=True)

for module in requirements.index.get_level_values(0).unique():
    info = requirements.loc[module]
    try:
        version = info.loc[sysinfo]['version']
    except:
        version = info.loc['default']['version']
    print(f"{module}{version}")
