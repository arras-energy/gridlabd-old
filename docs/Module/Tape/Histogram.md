[[/Module/Tape/Histogram]] -- Class histogram

# Synopsis

GLM:

~~~
  object histogram {
    filename "<string>";
    filetype "<string>";
    mode "<string>";
    group "<string>";
    bins "<string>";
    property "<string>";
    min "<decimal>";
    max "<decimal>";
    samplerate "<decimal> s";
    countrate "<decimal> s";
    bin_count "<integer>";
    limit "<integer>";
  }
~~~

# Description

TODO

## Properties

### `filename`

~~~
  char1024 filename;
~~~

The name of the file to write

### `filetype`

~~~
  char8 filetype;
~~~

The format to output a histogram in

### `mode`

~~~
  char32 mode;
~~~

The mode of file output

### `group`

~~~
  char1024 group;
~~~

The GridLAB-D group expression to use for this histogram

### `bins`

~~~
  char1024 bins;
~~~

The specific bin values to use

### `property`

~~~
  char256 property;
~~~

The property to sample

### `min`

~~~
  double min;
~~~

The minimum value of the auto-sized bins to use

### `max`

~~~
  double max;
~~~

The maximum value of the auto-sized bins to use

### `samplerate`

~~~
  double samplerate[s];
~~~

The rate at which samples are read

### `countrate`

~~~
  double countrate[s];
~~~

The reate at which bins are counted and written

### `bin_count`

~~~
  int32 bin_count;
~~~

The number of auto-sized bins to use

### `limit`

~~~
  int32 limit;
~~~

The number of samples to write

# Example

~~~
  object histogram {
    filename "";
    filetype "";
    mode "";
    group "";
    bins "";
    property "";
    min "0.0";
    max "0.0";
    samplerate "0.0";
    countrate "0.0";
    bin_count "0";
    limit "0";
  }
~~~

# See also

* [[/Module/Tape]]

