[[/Module/Powerflow/Powerflow_object]] -- Class powerflow_object

# Synopsis

GLM:

~~~
  object powerflow_object {
    phases "[ABCDNSG]";
    nominal_voltage "<decimal> V";
    supernode_name "<string>";
  }
~~~

# Description

TODO

## Properties

### `phases`
~~~
  set {A, B, C, D, N, S, G} phases;
~~~

TODO

### `nominal_voltage`
~~~
  double nominal_voltage[V];
~~~

TODO

### `supernode_name`
~~~
  char1024 supernode_name;
~~~

TODO

# Example

~~~
  object powerflow_object {
    phases "0";
    nominal_voltage "0.0";
    supernode_name "";
  }
~~~

# See also
* [[/Module/Powerflow]]

