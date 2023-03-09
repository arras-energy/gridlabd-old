[[/Utilities/Fire_danger]] -- Fire danger forecast data tool

# Synopsis

Shell:
~~~
$ gridlabd fire_danger [OPTIONS ...]
	-f|--forecast=DAY1[,DAY2[,...] 
	-d|--date=DATE1[,DATE2[,...]] 
	-t|--type=TYPE1[,TYPE2[,...]]  
~~~

GLM:
~~~
#python -m fire_danger [OPTIONS ...]
	-f|--forecast=DAY1[,DAY2[,...] 
	-d|--date=DATE1[,DATE2[,...]] 
	-t|--type=TYPE1[,TYPE2[,...]]
~~~

Python:
~~~
>>> import sys
>>> sys.path.append("/usr/local/share/gridlabd/")
>>> import fire_danger
>>> 
~~~

# Options

- `-h|--help|help`:   display this help
- `-r|--refresh`:     force refresh of files in the cache
- `-s|--show`:        show image instead of print image name

# Description

Downloads the USGS fire danger `DAY`-day forecast map for the `DATE` given. The
map is stored in the `CACHEDIR` (by default `$GLD_ETC/usgs/firedanger/`) using
the file name `TYPE_DATE_DAY.tif`. The output is the full pathname where the
data is stored. `DAY`, `DATE`, and `TYPE` may be specified as comma delimited values,
in which case, all the combination of the specified values are downloaded.

DAY may be specified as 1 through 7.  DATE must be specified using the format
YYYY-MM-DD. Valid TYPE values are:

- `fpi` - Fire Potential Index
- `lfp` - Large Fire Probability
- `fsp` - Fire Spread Probability

# Example

The following example displays the 5-day fire potential index forecast map for August 20, 2021.

~~~
sh$ gridlabd fire_danger -f=5 -d=2021-08-20 -t=fpi --show
~~~

The following example obtains the image name the same data

~~~
sh$ gridlabd fire_danger -f=5 -d=2021-08-20 -t=fpi
/usr/local/opt/gridlabd/gridlabd/current/share/gridlabd/usgs/firedanger/fpi_20210820_5.tif
~~~

The following example obtains the image name in Python:

~~~
>>> import sys
>>> sys.path.append("/usr/local/share/gridlabd")
>>> import fire_danger
>>> fire_danger.get_data(5,'20210820','fpi')
'/usr/local/opt/gridlabd/gridlabd/current/share/gridlabd/usgs/firedanger/fpi_20210820_5.tif'
~~~
