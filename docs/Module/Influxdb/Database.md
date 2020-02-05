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
    options [SHOWQUERY|NEWDB];
    logname "<measurement-name>";
    logtag "<property-name>";
    logtag "<tag-name>=<tag-value>";
    logtag "<logtag>,<logtag>,...,<logtag>";
}
~~~

# Description

The database object is used to maintain a connection to an InfluxDB server. Multiple connections to the same server can be used.

## Properties

The following properties are supported by the `database` class.

### `database`

~~~
string database;
~~~

Specifies the name of the database to use. The default database is `gridlabd`.

### `hostname`

~~~
string hostname;
~~~

Specifies the DNS name to use when connecting to a server. The default hostname is `localhost`.

### `logname`

~~~
string logname;
~~~

The `logname` property enables logging of database operations in a measurement so named.

### `logtag`

~~~
method logtag;
~~~

The `logtag` method allows properties and `tag=value` pairs to be added to the log tag list used by log write operations.  The `logtag` method may be used multiple times, and tags may be listed as one or more comma-separate values.

### `options`

~~~
enumeration {SHOWQUERY,NEWDB} options;
~~~

The `options` property control the database query options.  The following options are supported:

#### `NEWDB`

Specifies that existing databases are to be destroyed when starting the simulation.

#### `NONE`

Specifies that no database options have been provided.

### `password`

~~~
string password;
~~~

Specifies the password to use when authenticated server connections are required. The default password is `gridlabd`.

### `port`

~~~
integer port;
~~~

Specifies the port number to use to connect to the server. The default port is `8086`.

### `username`

~~~
string username;
~~~

Specifies the user name to use when authenticated server connections are required. The default username is `gridlabd`.

# Caveats

For compatibility with MySQL, the properties `tz_offset`, `sync_interval`, and `use_dst` are allowed but ignored.

# See also

* [[/Module/Influxdb]]
* [[/Module/Influxdb/Collector]]
* [[/Module/Influxdb/Player]]
* [[/Module/Influxdb/Recorder]]
