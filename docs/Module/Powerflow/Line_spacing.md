[[/Module/Powerflow/Line_spacing]] -- Class line_spacing

# Synopsis

GLM:

~~~
  object line_spacing {
    distance_AB "<decimal> ft";
    distance_BC "<decimal> ft";
    distance_AC "<decimal> ft";
    distance_AN "<decimal> ft";
    distance_BN "<decimal> ft";
    distance_CN "<decimal> ft";
    distance_AE "<decimal> ft";
    distance_BE "<decimal> ft";
    distance_CE "<decimal> ft";
    distance_NE "<decimal> ft";
  }
~~~

# Description

TODO

## Properties

### `distance_AB`

~~~
  double distance_AB[ft];
~~~

TODO

### `distance_BC`

~~~
  double distance_BC[ft];
~~~

TODO

### `distance_AC`

~~~
  double distance_AC[ft];
~~~

TODO

### `distance_AN`

~~~
  double distance_AN[ft];
~~~

TODO

### `distance_BN`

~~~
  double distance_BN[ft];
~~~

TODO

### `distance_CN`

~~~
  double distance_CN[ft];
~~~

TODO

### `distance_AE`

~~~
  double distance_AE[ft];
~~~

Distance between phase A wire and earth

### `distance_BE`

~~~
  double distance_BE[ft];
~~~

Distance between phase B wire and earth

### `distance_CE`

~~~
  double distance_CE[ft];
~~~

Distance between phase C wire and earth

### `distance_NE`

~~~
  double distance_NE[ft];
~~~

Distance between neutral wire and earth

# Example

~~~
  object line_spacing {
    distance_AB "0.0";
    distance_BC "0.0";
    distance_AC "0.0";
    distance_AN "0.0";
    distance_BN "0.0";
    distance_CN "0.0";
    distance_AE "0.0";
    distance_BE "0.0";
    distance_CE "0.0";
    distance_NE "0.0";
  }
~~~

# See also

* [[/Module/Powerflow]]

