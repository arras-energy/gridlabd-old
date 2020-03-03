[[/Module/Tape/Recorder]] -- Class recorder

# Synopsis

GLM:

~~~
  object recorder {
    trigger "<string>";
    file "<string>";
    filetype "<string>";
    mode "<string>";
    multifile "<string>";
    limit "<integer>";
    plotcommands "<string>";
    xdata "<string>";
    columns "<string>";
    flush "<integer>";
    format "<string>";
    interval "<decimal> s";
    strftime_format "<string>";
    property "<string>";
    output "{SVG,PNG,PDF,JPG,GIF,EPS,SCREEN}";
    header_units "{NONE,ALL,DEFAULT}";
    line_units "{NONE,ALL,DEFAULT}";
  }
~~~

# Description

TODO

## Properties

### `trigger`

~~~
  char32 trigger;
~~~

TODO

### `file`

~~~
  char1024 file;
~~~

TODO

### `filetype`

~~~
  char8 filetype;
~~~

TODO

### `mode`

~~~
  char32 mode;
~~~

TODO

### `multifile`

~~~
  char1024 multifile;
~~~

TODO

### `limit`

~~~
  int32 limit;
~~~

TODO

### `plotcommands`

~~~
  char1024 plotcommands;
~~~

TODO

### `xdata`

~~~
  char32 xdata;
~~~

TODO

### `columns`

~~~
  char32 columns;
~~~

TODO

### `flush`

~~~
  int32 flush;
~~~

TODO

### `format`

~~~
  bool format;
~~~

TODO

### `interval`

~~~
  double interval[s];
~~~

TODO

### `strftime_format`

~~~
  char256 strftime_format;
~~~

TODO

### `property`

~~~
method property;
~~~

The `property` field enumerates the targets that will be recorded into the file specified.  The properties must belong to the parent object. When multiple properties are specified they must be separated by commas.  Property may be specified with a unit and a format specification using the syntax:

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

### `output`

~~~
  enumeration {SVG, PNG, PDF, JPG, GIF, EPS, SCREEN} output;
~~~

TODO

### `header_units`

~~~
  enumeration {NONE, ALL, DEFAULT} header_units;
~~~

TODO

### `line_units`

~~~
  enumeration {NONE, ALL, DEFAULT} line_units;
~~~

TODO

# Example

~~~
  object recorder {
    trigger "";
    file "";
    filetype "";
    mode "";
    multifile "";
    limit "0";
    plotcommands "";
    xdata "";
    columns "";
    flush "0";
    format "FALSE";
    interval "0.0";
    strftime_format "";
    output "0";
    header_units "0";
    line_units "0";
  }
~~~

# See also

* [[/Module/Tape]]

