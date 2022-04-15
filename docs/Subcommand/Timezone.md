[[/Subommand/Timezone]] -- Subcommand to obtain the timezone at a location

# Synopsis

~~~
bash$ gridlabd timezone [LATITUDE,LONGITUDE]
~~~

# Description

The gridlabd timezone subcommand obtains the timezone for the location given by
LATITUDE,LONGITUDE or the local timezone based on the hostname's IP address.

## Options

* `-d|--debug`: output debugging output on error
* `-h|--help|help`: print full documentation
* `-v|--verbose`: output intermediate results

# Examples

The following command obtains the timezone for the current location as determine by the IP address of the host machine:

~~~
shell$ gridlabd timezone
~~~

The following command obtains the timezone for the location specified:

~~~
shell$ gridlabd timezone 37.5,-122.2
~~~

# See also

* [[/GLM/Directive/Clock]]
