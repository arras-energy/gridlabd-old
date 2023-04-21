"""Generate requirements.txt for this OS/CPU combination

Syntax
------

    $ python3 python/requirements.py [OPTION] 
    
Options
-------

The following options are available. Only one option may be used in any single command line

    --sysinfo       list the name of the current platform used to lookup versions
    
    --priority      create python calls to install module needed to build gridlabd
    
    FILENAME        create requirements.txt file needed to run gridlabd
    
Description
-----------

This script converts requirements.csv to requirements.txt by choosing the entries
that correspond to the current system information.

See Also
--------

* https://github.com/dchassin/gridlabd/blob/master/python/README.md
"""

import sys, os
import platform
import pandas

pandas.options.display.max_columns = None
pandas.options.display.max_colwidth = None
pandas.options.display.max_rows = None

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

requirements = pandas.DataFrame(pandas.read_csv(csvfile,index_col=['module'],dtype='str',na_values=[],keep_default_na=False))

if PRIORITY:
    priority = requirements[requirements['priority']!=''].reset_index().set_index(['priority','module']).sort_index()
    if not sysinfo in priority.columns:
        print(f"WARNING: sysinfo='{sysinfo}' not found in requirements specifications, using defaults",file=sys.stderr)
        sysinfo = 'default'
    # print(priority[['options','default',sysinfo]],file=sys.stderr)

    for n,row in priority.reset_index().iterrows():
        print(f"{os.environ['_']} -m pip install {row.options} {row.module}{'==' if row[sysinfo]!='' else ''}{row[sysinfo]}")

else:
    requirements = requirements[requirements['priority']=='']
    requirements.drop('priority',axis=1,inplace=True)
    requirements.drop('options',axis=1,inplace=True)
    requirements = requirements.stack().reset_index()
    requirements.columns = ["module","sysinfo","version"]
    requirements.set_index(["module","sysinfo"],inplace=True,verify_integrity=True)
    requirements.sort_index(inplace=True)
    requirements = requirements[requirements['version']!='']
    # print(requirements,file=sys.stderr)

    for module in requirements.index.get_level_values(0).unique():
        info = requirements.loc[module]
        try:
            version = info.loc[sysinfo]['version']
        except:
            version = info.loc['default']['version']            
        print(f"{module}{'==' if version!='' else ''}{version}")
