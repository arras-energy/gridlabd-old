[[/Module/Influxdb/Recorder]] -- Influxdb recorder object

# Synopsis

GLM:
~~~
object recorder
{
	connection "<object-name>";
	interval "<real>[<time-unit>]";
	options [NONE|UNITS];
	measurement|table|file "<name>";
	parent "<object-name>";
	tags "<tag>=<value>,...";
	values|property "<property-name>,...";
}
~~~

# Description

The `recorder` object implement the primary data collection mechanism for InfluxDB.  The `influxdb.recorder` uses the same general definitions as `tape.recorder` and `mysql.recorder` to the extend possible given the difference in database design.

## Properties

The following properties are available.

### `connection`
~~~
object connection;
~~~

The `connection` property specifies which `database` object is used to perform the `insert` queries that transfer data from the simulation to the InfluxDB server.

### `fields`

~~~
string fields;
~~~

The `fields` property specifies one or more properties that will be collected as fields in the target InfluxDB table. To provide compatibility with `tape.recorder` and `mysql.recorder`, the property `property` is an alias for `fields`.

Fields can be specified using the syntax `<object-name>:<property-name>`.  In this case the recorder parent need not be specified.  

### `measurement`

~~~
string measurement;
~~~

The `measurement` property specifies to which InfluxDB measurement table the data is added. To provide compatibility with `tape.recorder` and `mysql.recorder`, the properties `table` and `file` are aliases for `measurement`.

### `interval`

~~~
double interval[s];
~~~

The `interval` property specifies the sampling interval. If `interval` is `0`, the properties are sampled every iteration.  If the interval is `-1`, the properties are sampled whenever any one of them changes.

### `options`

~~~
enumeration [NONE,UNITS] options;
~~~

The `options` property specifies how the real and complex values are stored in the InfluxDB table.  If `NONE` is specified, the values are stored with the units.  If `UNITS` is specified, the units are given in the field name, and the values are stored as real numbers or complex strings.

### `parent`

~~~
object parent;
~~~

The `parent` property specifies the object from which fields and tags will be read. The `parent` property can be omitted if the recorder is embedded in the definition of another object or if the recorder using only fields that specify the object name.

### `tags`

~~~
string tags;
~~~

The `tags` property specifies the data tags used when inserting data fields in the table. There are two valid specifications for tags.

- `name`: specifies an object property or header field.  These are copied when the recorder is initialized and are not updated if they change during the simulation.  These are inserted as `<name>=<value>`.

- `tag=value`: specifies are static tags that are inserted verbatim.

# Example

The following example collects 10 house air temperature, power, and energy records into tables at different sampling intervals. The 1 minute data is stored in separate table for each house with values stored as floats and the units specified in the field name. The 1 hour data is stored in a single table for all houses. Values stored as a string with units, and a tag is given to identify each house.

~~~
module influxdb;
object database
{
	name "example";
}
module residential;
object house:..10
{
	object recorder
	{
		connection "example";
		property "air_temperature,panel.power";
		interval 1 min;
		options UNITS;
	};
	object recorder
	{
		connection "example";
		property "panel.energy";
		tags `name`;
		interval 1 h;
		table "metering";
	};
}
~~~

# See also 

* [[/Module/Influxdb]]
* [[/Module/Influxdb/Collector]]
* [[/Module/Influxdb/Database]]
* [[/Module/Influxdb/Player]]
