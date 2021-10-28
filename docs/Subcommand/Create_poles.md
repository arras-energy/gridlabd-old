[[/Subcommand/Create_poles]] -- Creates poles and pole mounts for networks

# Synopsis

Shell:

~~~
bash$ gridlabd create_poles FILENAME [OPTIONS ...]
~~~

GLM:

~~~
#create_poles FILENAME [OPTIONS ...]
~~~

Python

~~~
>>> import create_poles
>>> create_poles.main(inputfile,options)
~~~

Options:

* `--ignore_length`: ignore the line length when computing pole locations
* `--ignore_location`: ignore node latitude/longitude when computer pole locations
* `--include_network`: nclude the input network in the output GLM file
* `--output=GLMNAME`: set the output GLM file name (default is /dev/stdout)
* `--pole_type=CONFIGURATION_NAME`: set the pole type to use
* `--spacing=FEET`: set the pole spacing in feet on overhead power lines

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
omitted, e.g., the command `gridlabd create_poles example.glm --output=model.glm  --spacing=100 --pole_type=WOOD-EC-45/4 --include_network` is equivalent to `create_poles.main('example.glm',dict(output='model.glm',spacing=100,pole_type='WOOD-EC-45/4',include_network=True))`

# See also

* [[/Module/Powerflow/Pole]]
* [[/Module/Powerflow/Pole_mount]]
* [[/Subcommand/Library]]
