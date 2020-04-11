[[/Module/Mysql/Database]] -- MySQL database connection

# Synopsis 

~~~
module mysql;
object database {
   hostname "localhost";
   username "gridlabd";
   password "";
   schema "gridlabd";
   port "3306";
   socketname "/tmp/mysql.sock";
   clientflags "COMPRESS|FOUND_ROWS|IGNORE_SIGPIPE|INTERACTIVE|LOCAL_FILES|MULTI_RESULTS|MULTI_STATEMENTS|NO_SCHEMA|ODBC|SSL|REMEMBER_OPTIONS";
   options "SHOWQUERY|NOCREATE|NEWDB|OVERWRITE";
   on_init "<file-name>";
   on_sync "<file-name>";
   on_term "<file-name>";
   sync_interval "<seconds>";
   tz_offset "<seconds>"; 
   uses_dst FALSE; 
}
~~~

# Description 
At least one database object must defined for recorder, player, and collector objects to function using a database.  Typically, if no connection is specified by these objects, the last database defined will be used automatically.

# Properties

## `hostname`

~~~
  char256 hostname;
~~~

Specifies the MySQL server hostname.  If not specified, the object uses the hostname provided in the MySLQ module global `hostname`, which is by default `127.0.0.1` or `localhost`. Note that using `localhost` will default to using a socket connection, whereas using `127.0.0.1` will default to using a TCP/IP connection.

## `username`

~~~
   char32 username;
~~~

Specifies the MySQL username.  If not specified, this is the value provided in the MySLQ module global `username`, which is by default `gridlabd`.

## `password`

~~~
  char32 password;
~~~

Specifies the MySQL password.  If not specified, this is the value provided in the MySQL module global `password`, which is by default an empty string (i.e., no password needed).

## `schema`

~~~
  char256 schema;
~~~

Specifies the database schema to use.  If none is specified, the value specified in the MySQL module global `schema` is used, which is by default `gridlabd`.  If a blank schema name is specified, the modelname is used.  The schema will be automatically created if it does not exist unless the `NOCREATE` option is specified.

## `port`

~~~
  int32 port;
~~~

Specifies the port used to access the MySQL server. If not specified, the value specified in the MySQL module global `port` is used, which is by default `6266`.

## `socketname`

~~~
  char1024 socketname;
~~~

Specifies the socketname used to access the MySQL server. If none specified, the value specified in the MySQL module global `socketname` is used, which is by default `/tmp/mysql.sock`.

## `clientflags`

~~~
  set {COMPRESS,FOUND_ROWS,IGNORE_SIGPIPE,INTERACTIVE,LOCAL_FILES,MULTI_RESULTS,MULTI_STATEMENTS,NO_SCHEMA,ODBC,SSL,REMEMBER_OPTIONS} clientflags;
~~~

Sets client flags used when connecting to the server.

### `COMPRESS`

Enables data compression in the client/server protocol.

### `FOUND_ROWS`

Returns the number of rows found instead of the number of rows changed.

### `IGNORE_SIGPIPE`

Prevents the client library from installing a `SIGPIPE` signal handler. This can be used to avoid conflicts with a handler that the application has already installed.

### `INTERACTIVE`

Permit interactive_timeout seconds of inactivity (rather than wait_timeout seconds) before closing the connection. The client's session wait_timeout variable is set to the value of the session interactive_timeout variable.

### `LOCAL_FILES`

Enable `LOAD DATA LOCAL` handling.

### `MULTI_RESULTS`

Tell the server that the client can handle multiple result sets from multiple-statement executions or stored procedures. This flag is automatically enabled if `MULTI_STATEMENTS` is enabled. `MULTI_RESULTS` can be enabled when you call `mysql_real_connect()`, either explicitly by passing the `MULTI_RESULTS` flag itself, or implicitly by passing `MULTI_STATEMENTS` (which also enables `MULTI_RESULTS`). As of MySQL 5.7, `MULTI_RESULTS` is enabled by default.

### `MULTI_STATEMENTS`

Tell the server that the client may send multiple statements in a single string (separated by ; characters). If this flag is not set, multiple-statement execution is disabled. If you enable `MULTI_STATEMENTS` or `MULTI_RESULTS`, process the result for every call to `mysql_query()` or `mysql_real_query()` by using a loop that calls `mysql_next_result()` to determine whether there are more results. For an example, see [MySQL Manual Section 25.8.17, "C API Support for Multiple Statement Execution"](http://dev.mysql.com/doc/refman/5.7/en/c-api-multiple-queries.html).

### `NO_SCHEMA`

Do not permit `db_name.tbl_name.col_name` syntax. This is for ODBC. It causes the parser to generate an error if you use that syntax, which is useful for trapping bugs in some ODBC programs.

### `ODBC`

Unused.

### `SSL`

Use SSL (encrypted protocol). Do not set this option within an application program; it is set internally in the client library. Instead, use `mysql_ssl_set()` before calling `mysql_real_connect()`.

### `REMEMBER_OPTIONS`

Remember options specified by calls to mysql_options(). Without this option, if `mysql_real_connect()` fails, you must repeat the `mysql_options()` calls before trying to connect again. With this option, the `mysql_options()` calls need not be repeated.

## `options`

~~~
  set {SHOWQUERY,NOCREATE,NEWDB,OVERWRITE} options;
~~~

Set the table handling options.

### `SHOWQUERY`

When the SHOWQUERY is specified, each query executed by the client is displayed as a verbose message.

### `NOCREATE`

Disable automatic creation of tables that don't exist.

### `NEWDB`

When the NEWDB options|option is specified, then the schema is destroyed before it used.  

**CAUTION**: this may cause loss of data and should be used with care, especially if the default schema `gridlabd` is used.

### `OVERWRITE`

Enable deletion of existing data when an object initializes for output to a table. 

**CAUTION**: this may cause loss of data.

## `on_init`

~~~
  char1024 on_init;
~~~

Specifies the MySQL script to execute when the database object is initialized.  Scripts support all valid MySQL commands, as well as the DUMP and BACKUP commands.

## `on_sync`

~~~
  char1024 on_sync;
~~~

Specifies the MySQL script to execute when the database object is synchronized.  Scripts support all valid MySQL commands, as well as the DUMP and BACKUP commands.

## `on_term`

~~~
  char1024 on_term;
~~~

Specifies the MySQL script to execute when the database object is termination.  Scripts support all valid MySQL commands, as well as the DUMP and BACKUP commands.

## `sync_interval`

~~~
  double sync_interval[s];
~~~

Specifies the interval between database object synchronization events.

## `tz_offset`

~~~
  int32 tz_offset;
~~~

Specifies the number of seconds offset between database times and object timestamps.

## `uses_dst`

~~~
  bool use_date;
~~~

Specifies whether tz_offset should consider summer time or daylight savings time rules.

# See also

* [[/Module/Mysql]]

