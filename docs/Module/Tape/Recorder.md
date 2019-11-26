[[/Module/Tape]] -- Module tape

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

TODO

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

