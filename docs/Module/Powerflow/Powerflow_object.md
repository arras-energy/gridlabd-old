[[/Module/Powerflow/Powerflow_object]] -- Class powerflow_object

# Synopsis

GLM:

~~~
  object powerflow_object {
    phases "[ABCDNSG]";
    nominal_voltage "<decimal> V";
    supernode_name "<string>";
    violation_detected [NONE=0|CURRENT=1|VOLTAGE=2|POWER=4|THERMAL=8|CONTROL=16];
  }
~~~

# Description

The `powerflow_object` is the base class for all powerflow solver elements.  Any `node` or `link` object must be derived from this object.  

## Properties

### `phases`

~~~
  set {A, B, C, D, N, S, G} phases;
~~~

Specifies which phases of the network the object is involved in.

### `nominal_voltage`

~~~
  double nominal_voltage[V];
~~~

Specifies the nominal voltage to use when computing per-unit values.

### `supernode_name`

~~~
  char1024 supernode_name;
~~~

This a deprecated parameter used by tools that need to track hierarchical subnetworks.

### `violation_detected`

The output property is updated when a rating or limit is violated.  The value will be a combination of all the violations that are active at the time.  When the violation is no longer detected, the value is reset to `NONE`.

# Example

~~~
  object powerflow_object {
    phases "ABCN";
    nominal_voltage "2401.778 V";
  }
~~~

# See also

* [[/Module/Powerflow]]

