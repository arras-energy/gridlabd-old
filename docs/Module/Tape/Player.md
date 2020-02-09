[[/Module/Tape/Player]] -- Class player

# Synopsis

GLM:

~~~
  object player {
    property "<string>";
    file "<string>";
    filetype "<string>";
    mode "<string>";
    loop "<integer>";
  }
~~~

# Description

TODO

## Properties

### `property`
~~~
  char256 property;
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

### `loop`
~~~
  int32 loop;
~~~

TODO

# Example

~~~
  object player {
    property "";
    file "";
    filetype "";
    mode "";
    loop "0";
  }
~~~

# See also
* [[/Module/Tape]]

