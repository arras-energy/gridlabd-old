[[/Subcommand/Create_poles]] -- Creates poles and pole mounts for networks

# Synopsis

Shell:

~~~
bash$ gridlabd create_poles INPUTFULE [OPTIONS ...]
~~~

GLM:

~~~
#create_poles INPUTFULE [OPTIONS ...]
~~~

Python

~~~
>>> import create_poles
>>> create_poles.main(INPUTFULE,OPTIONS)
~~~

Output options:

  * --include_network                 include the input network in the output GLM file
  * --output=GLMNAME                  set the output GLM file name (default is /dev/stdout)
  * --format={GLM,JSON}               specify the output format (default is GLM)

Pole options:

  * --ignore_length                   ignore the line length when computing pole locations
  * --ignore_location                 ignore node latitude/longitude when computer pole locations
  * --pole_type=CONFIGURATION_NAME    set the pole type to use
  * --spacing=FEET                    set the pole spacing in feet on overhead power lines

Weather options:

  * --weather=NAME                    use named weather object
  * --location=LAT,LON                specify the weather location
  * --year=YEAR                       specify the weather year (default is forecasted)
  * --timezone=TZSPEC                 specify the timezone (overrides default based on location)

# Description

The `create_poles` subcommand automatically generates a pole model for a network model and
mounts the overhead lines and equipment to the newly created poles.  The output is written to
`/dev/stdout` unless the `--output=GLMNAME` option is given.

The `--pole_type=CONFIGURATION_NAME` and `--spacing=FEET` options are required.  Configuration names
may be obtained from the `pole_configuration.glm` library (see [[/Subcommand/Library]] for details
on using libraries.

Some network models include latitude and longitude information.  When this information is present,
the line length information checked.  If there is a discrepancy between these, a warning is printed
and the latitude/longitude information is used.  The `--ignore_length` option will suppress this
warning. The `--ignore_location` warning will cause the model to use the line length data instead.

The `--include_network` adds a `#include "FILENAME"` directive in the output to ensure that the 
resulting GLM file contains all the objects required to run the simulation, e.g.,

~~~
$ gridlabd create_poles example.glm --output=model.glm --spacing=100 --pole_type=WOOD-EC-45/4
$ gridlabd example.glm model.glm
~~~

Alternative, when the input is a GLM, the two GLM files can be used together in a single command, e.g.,

~~~
$ gridlabd create_poles example.glm --output=model.glm  --spacing=100 --pole_type=WOOD-EC-45/4 --include_network
$ gridlabd model.glm
~~~

The python usage requires the options be provided as a keyword arguments where the leading `--` is
omitted, e.g., the command `gridlabd create_poles example.glm --output=model.glm  --spacing=100 --pole_type=WOOD-EC-45/4 --include_network` is equivalent to `create_poles.main('example.glm',output='model.glm',spacing=100,pole_type='WOOD-EC-45/4',include_network=True)`

## Weather

By default, the weather forecast data is linked based on location, if any, and the clock is automatically set based on the forecast window.  If the `weather_name` option is provided, all poles created will use the specified weather object, and the clock will not set.  If the`year` is specified, then the weather data for that year and location is used, and the clock is set to run the entire year.  By default the timezone is determined from the locaiton, unless the `--timezone=TZSPEC` option is used to override it.

# See also

* [[/Module/Powerflow/Pole]]
* [[/Module/Powerflow/Pole_mount]]
* [[/Subcommand/Library]]
* [[/Subcommand/Noaa_forecast]]
* [[/Subcommand/Nsrdb_weather]]
