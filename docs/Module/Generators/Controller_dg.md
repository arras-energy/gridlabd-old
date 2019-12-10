[[/Module/Generators/Controller_dg]] -- Class controller_dg

# Synopsis
GLM:
~~~
  object controller_dg {
    groupid_dg "<string>";
    ki "<decimal>";
    kp "<decimal>";
    gain "<decimal>";
    ki_QV "<decimal>";
    kp_QV "<decimal>";
    gain_QV "<decimal>";
  }
~~~

# Description

TODO

## Properties

### `groupid_dg`
~~~
  char32 groupid_dg;
~~~

The group ID of the dg objects the controller controls.

### `ki`
~~~
  double ki;
~~~

Parameter of the propotional control

### `kp`
~~~
  double kp;
~~~

Parameter of the integral control

### `gain`
~~~
  double gain;
~~~

Gain of the controller

### `ki_QV`
~~~
  double ki_QV;
~~~

Parameter of the propotional control for secondary voltage control

### `kp_QV`
~~~
  double kp_QV;
~~~

Parameter of the integral control for secondary voltage control

### `gain_QV`
~~~
  double gain_QV;
~~~

Gain of the controller for secondary voltage control

# Example

~~~
  object controller_dg {
    groupid_dg "";
    ki "0.0";
    kp "0.0";
    gain "0.0";
    ki_QV "0.0";
    kp_QV "0.0";
    gain_QV "0.0";
  }
~~~

# See also
* [[/Module/Generators]]

