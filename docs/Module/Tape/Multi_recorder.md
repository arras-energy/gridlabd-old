[[/Module/Tape/Multi_recorder]] -- Class multi_recorder

# Synopsis

GLM:

~~~
  object multi_recorder {
    interval "<decimal> s";
    property "<string>";
    trigger "<string>";
    file "<string>";
    filetype "<string>";
    mode "<string>";
    multifile "<string>";
    limit "<integer>";
    plotcommands "<string>";
    xdata "<string>";
    columns "<string>";
    format "<string>";
    output "{SVG,PNG,PDF,JPG,GIF,EPS,SCREEN}";
    header_units "{NONE,ALL,DEFAULT}";
    line_units "{NONE,ALL,DEFAULT}";
  }
~~~

# Description

TODO

## Properties

### `interval`
~~~
  double interval[s];
~~~

TODO

### `property`
~~~
  method property;
~~~

TODO

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

### `format`
~~~
  char32 format;
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
  object multi_recorder {
    interval "0.0";
    trigger "";
    file "";
    filetype "";
    mode "";
    multifile "";
    limit "0";
    plotcommands "";
    xdata "";
    columns "";
    format "";
    output "0";
    header_units "0";
    line_units "0";
  }
~~~

# See also
* [[/Module/Tape]]

