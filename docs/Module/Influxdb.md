[[/Module/Influxdb]] -- InfluxDB database access module

# Synopsis

GLM:

~~~
module database
{
	message_flags [DEBUG|QUIET|VERBOSE|WARNING];
	default_username "gridlabd";
	default_password "gridlabd";
	default_hostname "localhost";
	default_port 8086;
	default_database "gridlabd";
    synchronous_postdata [FALSE|TRUE];
}
~~~

# Description

The `influxdb` module supports `tape`-like classes that read and write data to and from InfluxDB servers.

# See also

* [[/Module/Influxdb/Collector]]
* [[/Module/Influxdb/Database]]
* [[/Module/Influxdb/Player]]
* [[/Module/Influxdb/Recorder]]
