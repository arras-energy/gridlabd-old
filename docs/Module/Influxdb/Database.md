[[/Module/Influxdb/Database]] -- Database connection to an InfluxDB server

# Synopsis

GLM:

~~~
object database
{
	username "<username>";
    password "<password>";
    hostname "<hostname>";
    port <port-number>;
    database "<dbname>;
    options [SHOWQUERY|NOCREATE|NEWDB|OVERWRITE];
    tags "<tag-list>";
    tag "<property-name>";
}
~~~

# Description

The database object is used to maintain a connection to an InfluxDB server. Multiple connections to the same server can be used.

## Properties

The following properties are supported by the `database` class.

### `tag`

~~~
method tag;
~~~

The `tag` method allows properties to be added to the tag list used by all InfluxDB write operations.  The `tag` method may be used multiple times, once for each property added.  Alternative, the properties may be listed as one or more comma-separate value lists.

### `tags`

~~~
string tags;
~~~

The `tags` property allows a list of constant tags to be added to all InfluxDB write operations. The tags must be of the form `<tag>=<value>`. Multiple tags may be used, separated by commas.  No spaces or special characters are permitted in tags or values.

### `options`

~~~
enumeration {SHOWQUERY,NOCREATE,NEWDB,OVERWRITE} options;
~~~

The `options` property control the database query options.  The following options are supported:

#### `SHOWQUERY`

Specifies that queries are to be showed in verbose output.

#### `NOCREATE`

Specifies that databases are not to be created automatically when starting the simulation.

#### `NEWDB`

Specifies that existing databases are to be destroyed when starting the simulation.

#### `OVERWRITE`

Specifies that existing records are to be overwritten. This is the default in InfluxDB.

### `database`

~~~
string database;
~~~

Specifies the name of the database to use.

### `port`

~~~
integer port;
~~~

Specifies the port number to use to connect to the server.

### `username`

~~~
string username;
~~~

Specifies the user name to use when authenticated server connections are required.

### `password`

~~~
string password;
~~~

Specifies the password to use when authenticated server connections are required.


### `hostname`

~~~
string hostname;
~~~

Specifies the DNS name to use when connecting to a server.

# See also

* [[/Module/Influxdb]]
* [[/Module/Influxdb/Collector]]
* [[/Module/Influxdb/Player]]
* [[/Module/Influxdb/Recorder]]
