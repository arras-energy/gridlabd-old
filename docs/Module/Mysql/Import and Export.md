[[/Module/Mysql/Import and Export]] -- Import/export for MySQL databases 

# Synopsis
~~~
export mysql <options> <schema>;
import mysql <options> <schema>;
~~~

# Description

The `import` and `export` functions are supported for MySQL databases.  The `export` directive saves the current model to the MySQL database named `schema`. The `import` directive loads a model from the MySQL database named `schema`. 

The module global `table_prefix` can be used to stored multiple models in the same database.  The default `table_prefix` is an empty string.

## Options

TODO

# Data Dictionary

The following tables are used by the `import`/`export` proceduce in the `mysql` module:

## Globals

Table of globals
~~~
| Column name | Data type | Flags | Default |
| ----------- | --------- | ----- | ------- | 
| name        | char(64)  | PK/NN |         | 
| type        | int(11)   |       | NULL    |
| flags       | int(11)   |       | NULL    |
| value       | text      |       | NULL    |
| unit        | text      |       | NULL    |
| description | text      |       | NULL    |
~~~

## Modules

The `#modules|modules` table lists the module in use and their corresponding version information.

Table of modules
~~~
| Column name | Data type | Flags | Default |
| ----------- | --------- | ----- | ------- |
| name        | char(64)  | PK/NN |         |
| major       | int(11)   |       | NULL    |
| minor       | int(11)   |       | NULL    |
~~~

## Classes

The `#classes|classes` table lists all the classes and properties defined (including those not in use).  Runtime classes and properties are listed with a NULL module name.

Table of classes
~~~
| Column name | Data type | Flags    | Default |
| ----------- | --------- | -------- | ------- |
| id          | int(11)   | PK/NN/AI |         |
| name        | char(64)  |          |         |
| module      | char(64)  |          |  NULL   |
| property    | char(64)  |          |  NULL   |
| type        | int(11)   |          |  NULL   |
| flags       | int(11)   |          |  NULL   |
| units       | text      |          |  NULL   |
| description | text      |          |  NULL   |
~~~

## Objects

Table of objects
~~~
| Column name   | Data type | Flags | Default               |
| ------------- | --------- | ----- | --------------------- |
| id            | int(11)   | PK/NN |                       |
| class         | char(64)  |       | NULL                  |
| name          | char(64)  |       | NULL                  |
| groupid       | char(32)  |       | NULL                  |
| parent        | int(11)   |       | NULL                  |
| rank          | int(11)   |       | NULL                  |
| clock         | timestamp | NN    | '1970-01-01 00:00:00' |
| valid_to      | timestamp | NN    | '1970-01-01 00:00:00' |
| schedule_skew | timestamp | NN    | '1970-01-01 00:00:00' |
| latitude      | double    |       | NULL                  |
| longitude     | double    |       | NULL                  |
| in_svc        | timestamp | NN    | '1970-01-01 00:00:00' |
| in_svc_micro  | int(11)   |       | 0                     |
| out_svc       | timestamp | NN    | '1970-01-01 00:00:00' |
| out_svc_micro | int(11)   |       | 0                     |
| rngstate      | int(11)   |       | NULL                  |
| heartbeat     | timestamp | NN    | '1970-01-01 00:00:00' |
| flags         | int(11)   |       | NULL                  |
~~~

## Properties

Some properties have extended specifications that are not captured by a simple read of the value.  These specifications are stored in the `#Properties|properties` table.

Table of Extended Properties
~~~
| Column name | Data type | Flags | Default |
| ----------- | --------- | ----- | ------- |
| id          | mediumint | NN    |         |
| property    | char(64)  | NN    |         |
| type        | char(16)  | NN    |         |
| specs       | text      |       |         |
~~~

## Transforms

Schedule transformations use the schedule name as the source.  A direct transformation uses the object name and property in the form `class:id.property`.  The target is always specified in the form `class:id.property`.  

The specification for the linear transformation uses the form `*scale+offset`.  An external function uses the form 
$$
	y=function(x_1,x_2,...,x_n)
$$
where $x_1, x_2, ..., x_n$ and $y$ are all specified in the form `class:id.property`
~~~
| Column name   | Data type | Flags | Default |
| ------------- | --------- | ----- | ------- |
| source        | char(255) | NN    |         |
| target        | char(255) | NN    |         |
| specification | text      | NN    |         |  
~~~

## Schedules

Schedules are recorded in the original input form which there were defined.  See `schedule` for details.

Table of schedules
~~~
| Column name | Data type | Flags | Default |
| ----------- | --------- | ----- | ------- |
| name        | char(64)  | PK/NN |         | 
| definition  | text      |       |         |
~~~

## Property types

Property types
~~~
| Value | Description                          |
| ----- | ------------------------------------ |
| 0     | void                                 |
| 1     | double                               |
| 2     | complex                              |
| 3     | enumeration                          |
| 4     | set                                  |
| 5     | int16                                |
| 6     | int32                                |
| 7     | int64                                |
| 8     | char8                                |
| 9     | char32                               |
| 10    | char256                              |
| 11    | char1024                             |
| 12    | object                               |
| 13    | delegated                            |
| 14    | bool                                 |
| 15    | timestamp                            |
| 16    | double_array                         |
| 17    | complex_array                        |
| 18    | real                                 |
| 19    | float                                |
| 20    | loadshape                            |
| 21    | enduse                               |
| 22    | randomvar                            |
| 23    | triple (unused)                      |
| 24    | triplex (unused)                     |
| 25    | property keyword (internal use only) |
| 29    | parent class (internal use only)     |
~~~

## Property flags

Property flags
~~~
| Value  | Description                                                         |
| ------ | ------------------------------------------------------------------- |
| 0x0001 | Property recalculation is pending                                   |
| 0x0002 | Property supports single character `set` and `enumeration` keywords |
| 0x0004 | Property was added at runtime                                       |
| 0x4000 | Property is deprecated without usage warning                        |
| 0x8000 | Property is deprecated with usage warning                           |
~~~

## Object flags

Object flags
~~~
| Value  | Description                                                            |
| ------ | ---------------------------------------------------------------------- |
| 0x0001 | External PLC is active (disables internal PLC code)                    |
| 0x0002 | Object is locked (write is pending)                                    |
| 0x0004 | (reserved)                                                             |
| 0x0008 | Object recalculation is pending                                        |
| 0x0010 | External memory (allocated in memory that cannot be freed by the core) |
| 0x0020 | Skipping updates is safe                                               |
| 0x0040 | Object is currently operating in delta mode                            |
| 0x0080 | Object supports forecasting                                            |
| 0x0100 | Object initiatlization was successful                                  |
| 0x4000 | Reranking flag (internal core use only)                                |
~~~

## Class tables

Class tables are created with the naming convention `module_class`, with the exception of runtime classes, which are named `_class`.  The fields in the class tables will depend on the properties defined in the classes. There is always an `id` field that is a key into the `objects` table.  All other fields are of type `text`.

The data recorded in properties fields is always formatted as text.  Double and complex values with units will include the units.  Complex values may be formatted in various ways depending on the convention, e.g., real/imaginary or magnitude/angle.  `Timestamp`s include the timezone.

# Caveats

Special data types that are implicitly `double` (e.g., `randomvar`, `loadshape`) will always save as `double`. The original specification for the data type is not stored and only the realization of the value is stored. The implies that round-robin `export`-`import` may not always yield exactly the same model as the original GLM file.  However, `import` may contain the full specification and will be interpreted accordingly.

# See also

* [[/Module/Mysql]]
* [[/Module/Mysql/Database]]
