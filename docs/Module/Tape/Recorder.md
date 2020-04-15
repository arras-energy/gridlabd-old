[[/Module/Tape/Recorder]] -- Record object data to a file

# Synopsis

GLM:

~~~
object recorder {
  // required
  property "<string>";
  // optional
  file "<string>";
  filetype "<string>";
  flush "<integer>";
  interval "<decimal> s";
  header_units "{NONE,ALL,DEFAULT}";
  line_units "{NONE,ALL,DEFAULT}";
  limit "<integer>";
  mode "<string>";
  trigger "<string>";
}
~~~

# Description

The `recorder` object collects data from its parent's properties and saves it in a CSV file.  The first column is always the date and time of the sample, whose format is specified by the [[/Global/Dateformat]] variable.

## Properties

The recorder object uses the following properties to record CSV files.

### `file`

~~~
char1024 file;
~~~

The `file` property specifies the name of the CSV file in which the data will be stored. The default is the parent object name with a `.csv` extension. Invalid filename characters (e.g., `:`) in the parent object name are converted to underscores.

### `filetype`

~~~
char8 filetype;
~~~

The `filetype` property specifies type of file and determines the extension used. The default is `csv`.

### `flush`

~~~
int32 flush;
~~~

Specifies how often the output buffer is flushed.  By default the output buffer is flushed to disk when it is full (the size of the buffer is system specific). This default corresponds to the flush value -1. If flush is set to 0, the buffer is flushed every time a record is written. If flush is set to a value greater than 0, the buffer is flushed whenever the condition clock mod flush == 0 is satisfied.

### `interval`

~~~
  double interval[s];
~~~

The frequency at which the recorder samples the specified properties, in seconds. A frequency of 0 indicates that they should be read & written every iteration (note, that each timestep often requires multiple iterations, so a frequency of zero may lead to multiple measurements in a timestep). A frequency of -1 indicates that they should be read every timestep, but only written if one or more values change. By default, this is TS_NEVER.

### `header_units`

~~~
  enumeration {NONE, ALL, DEFAULT} header_units;
~~~

Used to control the appearance of units for the column headers. 

#### `ALL` 

Force the units to be printed for every column, if present. 

#### `NONE` 

Suppress the printing of units on the column header, even if conversions are being made. 

#### `DEFAULT` 

Will only print the units if they are defined in the GLM.

### `limit`

~~~
  int32 limit;
~~~

The number of rows to write to the output stream. A non-positive value puts no limit on the file size (use at your own risk). By default, this is 0. The limit is only checked when output non-subsecond value.

### `line_units`

~~~
  enumeration {NONE, ALL, DEFAULT} line_units;
~~~

Used to control the appearance of units in the data rows. 

#### `ALL` 

Force the units to be printed for every row entry, if present. 

#### `NONE` 

Suppress the printing of units within the rows, even if conversions are being made. 

#### `DEFAULT` 

Only print the units if they are defined in the GLM for that column.

### `property`

~~~
method property;
~~~

The `property` field enumerates the targets that will be recorded into the file specified.  The properties must belong to the parent object. When multiple properties are specified they must be separated by commas.  Properties may be specified with a unit and a format specification using the syntax:

~~~
property "<name>[<unit>:<format]";
~~~

where `<unit>` must be a valid unit that is compatible with the unit of property.  The `<format>` value must be specified as follows for `double` properties:

~~~
  <digit><letter>
~~~

where `<digit>` is a number from 0 to 9 specifying the precision of the output number, and `<letter>` is one of ['a','e','f','g','A','E','F','G'], corresponding to the double formats used by `sprintf()`.

When the properties is a `complex`, the format specification must also include one of ['i','j','d','r',M','D','R','X','Y'], indicating the follow forms of the complex value

* `i`: Rectangular form with 'i' as the complex variable.

* `j`: Rectangular form with 'j' as the complex variable.

* `r`: Polar form with angle specified in radians.

* `d`: Polar form with angle specified in degrees.

* `M`: Magnitude only

* `D`: Angle only in degrees

* `R`: Angle only in radians

* `X`: Real part only

* `Y`: Imaginary part only

### `trigger`

~~~
char32 trigger;
~~~

A short string that contains an equality or inequality and provides a simple trigger that will delay the opening of a recorder until the condition is met for the first property in the list. The first character must be the operator, <, =, or >, and it must be immediately followed by the signed value to compare the property to. An example would be ">+90.0" for the house air temperature. Once the trigger is met, the recorder will read to its limit as normal.

# Example

The following example record the property `property-name` of the object `my-object` into the files `my-file.csv`. It sample the value every 10 seconds, limits the output to 20 records, starts recording when `property-name` is strictly positive, and flushes the buffer every 60 seconds.

~~~
object recorder {
  parent "my-object";
  property "property-name";
  file "my-file.csv";
  limit 20;
  flush 60;
  interval 10;
  trigger ">0";
}
~~~

# See also

* [[/Module/Tape]]
* [[/Global/Dateformat]]
* [[/Module/Tape/Global/Csv_data_only]]
