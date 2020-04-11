[[/Module/Climate/Csv_reader]] -- Class csv_reader

# Synopsis

GLM:

~~~
  object csv_reader {
    index "<integer>";
    city_name "<string>";
    state_name "<string>";
    lat_deg "<decimal>";
    lat_min "<decimal>";
    long_deg "<decimal>";
    long_min "<decimal>";
    low_temp "<decimal>";
    high_temp "<decimal>";
    peak_solar "<decimal>";
    elevation "<integer>";
    status "{ERROR,OPEN,INIT}";
    timefmt "<string>";
    timezone "<string>";
    timezone_offset "<decimal>";
    columns "<string>";
    filename "<string>";
  }
~~~

# Description

TODO

## Properties

### `index`

~~~
  int32 index;
~~~

TODO

### `city_name`

~~~
  char32 city_name;
~~~

TODO

### `state_name`

~~~
  char32 state_name;
~~~

TODO

### `lat_deg`

~~~
  double lat_deg;
~~~

TODO

### `lat_min`

~~~
  double lat_min;
~~~

TODO

### `long_deg`

~~~
  double long_deg;
~~~

TODO

### `long_min`

~~~
  double long_min;
~~~

TODO

### `low_temp`

~~~
  double low_temp;
~~~

TODO

### `high_temp`

~~~
  double high_temp;
~~~

TODO

### `peak_solar`

~~~
  double peak_solar;
~~~

TODO

### `elevation`

~~~
  int32 elevation;
~~~

TODO

### `status`

~~~
  enumeration {ERROR, OPEN, INIT} status;
~~~

TODO

### `timefmt`

~~~
  char32 timefmt;
~~~

TODO

### `timezone`

~~~
  char32 timezone;
~~~

TODO

### `timezone_offset`

~~~
  double timezone_offset;
~~~

TODO

### `columns`

~~~
  char256 columns;
~~~

TODO

### `filename`

~~~
  char256 filename;
~~~

TODO

# Example

~~~
  object csv_reader {
    index "0";
    city_name "";
    state_name "";
    lat_deg "0.0";
    lat_min "0.0";
    long_deg "0.0";
    long_min "0.0";
    low_temp "0.0";
    high_temp "0.0";
    peak_solar "0.0";
    elevation "0";
    status "0";
    timefmt "";
    timezone "";
    timezone_offset "0.0";
    columns "";
    filename "";
  }
~~~

# See also

* [[/Module/Climate]]

