[[/Utilities/Create_poles]] -- Creates poles and pole mounts for networks

# Synopsis

Shell:

~~~
bash$ gridlabd create_poles INPUTFILE OPTIONS ...
~~~

GLM:

~~~
#python -m create_poles INPUTFILE OPTIONS ...
~~~

Python

~~~
>>> import create_poles
>>> create_poles.main(INPUTFILE,OPTIONS)
~~~

Output options:

  * `--include_network`                 include the input network in the output GLM file
  * `--output=GLMNAME`                  set the output GLM file name (default is /dev/stdout)
  * `--format={GLM,JSON}`               specify the output format (default is GLM)

Pole options:

  * `--ignore_length`                   ignore the line length when computing pole locations
  * `--ignore_location`                 ignore node latitude/longitude when computer pole locations
  * `--pole_type=CONFIGURATION_NAME`    set the pole type to use
  * `--pole_data=POLEDATA_CSV`          use CSV data of pole properties

  * `--mount_data=MOUNTDATA_CSV`        use CSV data for equipment and line mounts
  
  * `--spacing=FEET`                    set the pole spacing in feet on overhead power lines

Weather options:

  * `--include_weather`                 include automatically downloaded weather for poles
  * `--location=LAT,LON`                specify the weather location
  * `--timezone=TZSPEC`                 specify the timezone (overrides default based on location)
  * `--weather_name=NAME`               use named weather object
  * `--year=YEAR`                       specify the weather year (default is forecasted)

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

## Default properties

Pole and pole_mount objects are created with the following default properties:

  * pole
    - install_year: 2000
    - tilt_angle: 0 deg
    - tilt_direction: 0 deg
  * pole_mount
    height: 40 ft
    offset: 0 ft
    area: 0 sf
    direction: 0 deg
    weight: 0 lb

The properties may be set at the command line using the option `--TYPE.PROPERTY=VALUE`, e.g. `--pole.install_year=2010`.

## Weather

If `--include_weather` is specified, then the weather forecast data is linked based on location, if any, and the clock is automatically set based on the weather window.  If the `--weather_name` option is provided, all poles created will use the specified weather object instead of using downloaded weather, and the clock will not set.  If the `--year` option is specified, then the historical weather data for that year and location is used, and the clock is set to run the entire year.  Without the `--year` specification, a realtime weather forecast is used, and the clock is set to the forecast window. By default the timezone is determined from the location, unless the `--timezone=TZSPEC` option is used to override it. If `--ignore_location` is specified, then the local system timezone specification is used.

# Caveat

When saving to JSON, only the new pole data is included.  Options that
change the clock or include networks and weather are ignored.

The `create_poles` subcommand is a python extra command. It is not supported directly using the `#subcommand` macro.  To use python extras as subcommand, use the `#gridlabd` macro instead.

# See also

* [[/Module/Powerflow/Pole]]
* [[/Module/Powerflow/Pole_mount]]
* [[/Utilities/Library]]
* [[/Utilities/Noaa_forecast]]
* [[/Utilities/Nsrdb_weather]]
