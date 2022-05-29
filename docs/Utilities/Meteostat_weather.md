[[/Utilities/Meteostat_weather]] - Meteostat weather access

# Synopsis

Shell:

~~~
    $ python3 meteostat.py [OPTIONS] ...
~~~

GLM:

~~~
    #python3 meteostat.py [OPTIONS] ...
~~~

Options:

-  `-h|--help|help`:         Get this help information (exits with code 0)

-  `-c|--csv=CSVNAME`:       Name the CSV output file (optional)
  
-  `-g|--glm=GLMNAME`:       Name the GLM output file (optional)
  
-  `-n|--name=OBJNAME`:      Name the weather object (optional)
  
-  `-p|--position=LAT,LON`:  Specify the location (required)

-  `-s|--start=YYYY-MM-D`D:  Specify the start date (optional)

-  `-e|--end=YYYY-MM-DD`:    Specify the end date (optional)

# Description

The `meteostat_weather` tool gathers data from the Meteostat website and converts it
to a GridLAB-D weather object. If neither `CSVNAME` or `GLMNAME` are provided the weather 
data is written to stdout.  Otherwise, the default `CSVNAME` is `meteostat_ID.csv` where
`ID` is the station identifier provided by Meteostat.  The default weather object is
also named `meteostat_ID`.  If the start date and/or end date are not specified, then
the default is all available data.

# See also

* [https://dev.meteostat.net/]

