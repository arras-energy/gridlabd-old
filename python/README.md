# Python Requirements

The `requirements.csv` file provides platform specific information about python modules required to build and run GridLAB-D.  

## Sysinfo

You can determine which platform the python build system is running on by using the command `python3 python/requirements.py --sysinfo`.

## Default

The `default` column specified the module version required if none is specified for the current platform.

## Priority

The `priority` column indicates which modules are required to build `gridlabd` and the order in which they must be installed by `pip`.  When `priority` is specified, the `options` column is used to provide command-line options to `pip`, if necessary.

# Documentation

See [Python Module Documentation](https://docs.gridlabd.us/index.html?owner=arras-energy&project=gridlabd&branch=master&folder=/Module&doc=/Module/Python.md) for details.

