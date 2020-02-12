[[/Module/Powerflow/Power_metrics]] -- Class power_metrics

# Synopsis

GLM:

~~~
  object power_metrics {
    SAIFI "<decimal>";
    SAIFI_int "<decimal>";
    SAIDI "<decimal>";
    SAIDI_int "<decimal>";
    CAIDI "<decimal>";
    CAIDI_int "<decimal>";
    ASAI "<decimal>";
    ASAI_int "<decimal>";
    MAIFI "<decimal>";
    MAIFI_int "<decimal>";
    base_time_value "<decimal> s";
  }
~~~

# Description

TODO

## Properties

### `SAIFI`

~~~
  double SAIFI;
~~~

Displays annual SAIFI values as per IEEE 1366-2003

### `SAIFI_int`

~~~
  double SAIFI_int;
~~~

Displays SAIFI values over the period specified by base_time_value as per IEEE 1366-2003

### `SAIDI`

~~~
  double SAIDI;
~~~

Displays annual SAIDI values as per IEEE 1366-2003

### `SAIDI_int`

~~~
  double SAIDI_int;
~~~

Displays SAIDI values over the period specified by base_time_value as per IEEE 1366-2003

### `CAIDI`

~~~
  double CAIDI;
~~~

Displays annual CAIDI values as per IEEE 1366-2003

### `CAIDI_int`

~~~
  double CAIDI_int;
~~~

Displays SAIDI values over the period specified by base_time_value as per IEEE 1366-2003

### `ASAI`

~~~
  double ASAI;
~~~

Displays annual AISI values as per IEEE 1366-2003

### `ASAI_int`

~~~
  double ASAI_int;
~~~

Displays AISI values over the period specified by base_time_value as per IEEE 1366-2003

### `MAIFI`

~~~
  double MAIFI;
~~~

Displays annual MAIFI values as per IEEE 1366-2003

### `MAIFI_int`

~~~
  double MAIFI_int;
~~~

Displays MAIFI values over the period specified by base_time_value as per IEEE 1366-2003

### `base_time_value`

~~~
  double base_time_value[s];
~~~

Time period over which _int values are claculated

# Example

~~~
  object power_metrics {
    SAIFI "0.0";
    SAIFI_int "0.0";
    SAIDI "0.0";
    SAIDI_int "0.0";
    CAIDI "0.0";
    CAIDI_int "0.0";
    ASAI "0.0";
    ASAI_int "0.0";
    MAIFI "0.0";
    MAIFI_int "0.0";
    base_time_value "0.0";
  }
~~~

# See also

* [[/Module/Powerflow]]

