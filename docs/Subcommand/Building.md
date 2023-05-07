[[/Subcommand/Building]] -- Building model compiler

# Synopsis

GLM:

~~~
#building [OPTIONS ...]
~~~

Shell:

~~~
$gridlabd building [OPTIONS ...]
~~~

## Options

  -c|--continuous ss A B C D    convert from continuous state-space model
  -c|--continuous tf NUM DEN    convert from continuous transfer-function model
  -d|--discrete ss A B C D TS   convert from discrete state-space model
  -d|--discrete tf NUM DEN TS   convert from discrete transfer-function model
  -i|--input FILENAME           read building model data from file
  -n|--name NAME                specify building name
  -o|--output FILENAME          output the model to the specified file
  --verbose                     toggle verbose output to stderr
  --debug                       toggle debug output to stderr
  --warning                     toggle warning output to stderr
  --quiet                       toggle error output to stderr

# Description

  The building subcommand is used to create a building model from input data.

# Examples

  $ gridlabd building -s statespace.json 

# See Also
