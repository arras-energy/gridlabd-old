[[/Module/Mysql/Player]] -- MySQL player object

# Synopsis
~~~
module mysql;
object player {
    property "<property-name>";
    table|file "<source-table>";
    mode {"r","r+"};
    filetype "CSV";
    connection "<object-name>";
    options "";
    loop "<integer-value>";
}
~~~

# Description

The `mysql` player is designed to be compatible the the `tape` player object so that when the mysql module is used in place of the tape module, there are few changes, if any, required to the player objects.

The source table must have a sequence field, `id`, and a time field, `t`, to function as a player data source.  

* The sequence field `id` must be an integer and determines the sequence in which records are presented to the player.  

* The time `t` field must be a timestamp, datetime, or numeric value and determines the time value used to post values to objects. If the time field is a timestamp or datetime, values are posted using absolute time.  If the time field is a numeric value, values are posted in relative time with the first time value being relative to the epoch (1970-01-01 00:00:00 UTC).  If the time field is a floating point value or the time field includes microseconds, the player will be operate using subsecond intervals using `deltamode`.

## Advanced queries

The player object supports advanced queries. The advanced query must return two columns, `t` and `x`, ordered by `t` ascending.  The syntax is
~~~~
object player {
    query "select time_value as t, double_value as x order by t asc";
}
~~~~

## Asynchronous queries

The player object supports asynchronous initialization using the `maximum_threads` module global.  When `maximum_threads` is non-zero, each player object will initialize asynchronously, meaning that other objects will be allowed to initialize while the player object completes its initialization sequence.  When `maximum_threads` is positive, only that number of simultaneous initializations will be allowed.  When `maximum_threads` is negative there is no limit to the number of simultaneous initializations allowed.

# Properties

## `connection`
~~~
  object connection;
~~~

Specifies the database object used to connect to MySQL.  If none is provided, the last database defined is used.

## `file`
~~~
  char1024 file;
~~~

This is a synonym for `table`, and is provided for compatibility with tape player.

## `filetype`
~~~
  char8 filetype;
~~~

Provided for compability with tape player and has no real effect.

## loop
~~~
  int32 loop;
~~~

The input data table will be treated as a tape loop if the field type is an integer or double.  The loop property determines how many time the input data table will rewind before the player ceases to update the target object's properties.

## mode
~~~
  char32 mode;
~~~

Specifies the read mode to use, which may be either "r" or "r+".  This is provided for compatibility with tape player and has no real affect.

## options
~~~
  void options;
~~~

No options are supported at this time.

## property

~~~
  char256 property;
~~~

Specifies the target property (or properties) that are to be updated.  The properties must match the field names in the source table.  The data types are automatically converted using the following rules:

* Timestamps are parsed as `yyyy-mm-dd HH:MM:SS` in the model timezone.

* Real numbers are parsed using unit conversion.  If the target property has units and the property's field also specifies a unit, then the values are converted from field's unit to the target property's unit.

* Integers are converted at 64-bit precision before being cast into the size of the target property.

* Sets and enumerations may be received as integers, but string values are accepted (and probably preferable).

* All other data types are extracted from the database as strings and are converted by GridLAB-D's built-in types conversion routines.

## table
~~~
  char1024 table;
~~~

Specifies the source table from which data is read.

# Caveats

Player objects perform queries to the mysql server(s). Most servers have either firm or practical limits on the number queries they can handle simultaneously.  It is strongly recommended that the value of `maximum_threads` be chosen with due consideration to the limits of the server(s) from which the player data is obtained.

# See also

* [[/Module/Mysql]]
* [[/Module/Mysql/Database]]
* [[/Module/Tape/Player]]
