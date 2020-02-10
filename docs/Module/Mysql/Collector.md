[[/Module/Mysql/Collector]] -- MySQL collector class 

# Synopsis 

~~~
 module mysql;
 object collector {
   property "<property-name>";
   group "<group-spec>";
   table|file "<source-table>";
   mode {"w","w+","a","a+"};
   limit "<max-rows>";
   interval "<seconds>";
   connection "<database-object-name>";
   options "PURGE";
 }
~~~

# Description 

The `mysql` collector is designed to be compatible the the `tape` collector object so that when the `mysql` module is used in place of the `tape` module, there are few changes, if any, required to the collector objects.

# Properties

## `connection` 

~~~
string connection;	
~~~

Specifies the database object used to connect to MySQL.  If none is provided, the last database defined is used.

## `file` 

~~~
string file;
~~~

A synonym for `table` for compability with `tape` collector `file` property.

## `interval` 

~~~
double interval[s];
~~~

Specifies the sampling interval.  By default the interval is specified in seconds, but if units are specifies conversion will be automatic.

## `mode` 

~~~
string mode;
~~~

Specifies the file I/O mode to use, which may be either "w", "w+", "a", or "a+".  If "w" or "w+" is used, the table is purged before data from the current run is added. If "a" or "a+" is used, the data from the current run is appended.

## `options` 

~~~
enumeration {NONE,PURGE} options;
~~~

Specifies the recorder options, as follows:

### `PURGE` 

Purges the table of all existing data (by dropping and recreating the table) before storing new data.

## `property` 

~~~
string property;	
~~~

Specifies the property (or properties) that are to be collected.  This is a comma-delimited list if more than one property is collected.  Each property must have an aggregator function included.  This may be one of the following

* `min(x)`, `max(x)`, `count(x)`

* `sum(x)`, `prod(x)`

* `mean(x)`, `avg(x)`, `std(x)`, `var(x)`, `mbe(x)`, `skew(x)`, `kur(x)`, `gamma(x)`

Each property may include any valid part, e.g.:

* for complex `.mag`, `.ang`, `.arg`, `.real`, `.imag`.

* for random `.a`, `.b`, `.high`, `.low`.

* for enduse `.total`, `.energy`, `.demand`, `breaker_amps`, `.admittance`, `.current`, and `.power` (as complex parts), or `.impedance_fraction`, `.power_fraction`, `.power_factor`, `.voltage_factor`, `.heatgain`, `.heatgain_fraction` (as double parts).

* for timestamp `.seconds`, `.minutes`, `.hours`, `.days`, `.year`, `.month`, `.day`, `.hour`, `.minute`, `.second`, `.weekday`, `.yearday`, `.isdst`.

## `table` 

~~~
string table;
~~~

Specifies the source table from which data is read.

# Example

The following example collects the mean, sum, and count of samples drawn from a set of random values in 10 test objects and stores them in the default `gridlabd` database on the local MySQL server using the `gridlabd` username.

~~~
 module mysql;
 object database {
 }
 class test {
   random x;
 }
 object test:..10 {
   x type:normal(0,1);
 }
 object collector {
   property "mean(x),sum(x),count(x)";
   group "class=test";
   mode "w";
   table "test";
   interval 1h;
   limit 1000;
 }
~~~

# Caveats

* The `limit` is not enforced until at least one row is added. Therefore, it is likely that after a given run, the number of rows is increased by 1 even though the maximum number of rows has been reached.

# See also

* [[/GLM/General/Collection]]
* [[/Module/Mysql]]
* [[/Module/Mysql/Database]]
* [[/Module/Tape/Collector]]
