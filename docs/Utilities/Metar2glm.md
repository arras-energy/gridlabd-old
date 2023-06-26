[[/Utilities/Metar2glm]] - METAR to GLM realtime weather

# Synopsis

Shell:

~~~
    $ gridlabd python -m metar2glm -m metar2glm [--index|STATION ...] [-c|-class CLASSNAME]
~~~

GLM:

~~~
    #python -m metar2glm -m metar2glm [--index|STATION ...] [-c|-class CLASSNAME]
~~~

Options:

-  `-h|--help|help`:         Get this help information (exits with code 0)

-  `-c|--class CLASSNAME`:   Name the GLM class to use for the weather data class

-  `--index`:                Output a list of valid METAR stations  

# Description

The `metar2glm` tool creates an object that represents the realtime weather at the specifies weather stations.

# Example

The following outputs the realtime weather for San Francisco airport.

~~~
$ gridlabd python -m metar2glm KSFO
~~~

# See also

* [https://github.com/python-metar/python-metar]
* [https://www.ucar.edu]

