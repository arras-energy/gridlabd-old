[[/Module/Powerflow]] -- Module powerflow

# Synopsis
GLM:
~~~
  object load_tracker {
    target "<string>";
    target_property "<string>";
    operation "{ANGLE,MAGNITUDE,IMAGINARY,REAL}";
    full_scale "<decimal>";
    setpoint "<decimal>";
    deadband "<decimal>";
    damping "<decimal>";
    output "<decimal>";
    feedback "<decimal>";
  }
~~~

# Description

TODO

## Properties

### `target`
~~~
  object target;
~~~

Target object to track the load of

### `target_property`
~~~
  char256 target_property;
~~~

Property on the target object representing the load

### `operation`
~~~
  enumeration {ANGLE, MAGNITUDE, IMAGINARY, REAL} operation;
~~~

Operation to perform on complex property types

### `full_scale`
~~~
  double full_scale;
~~~

Magnitude of the load at full load, used for feed-forward control

### `setpoint`
~~~
  double setpoint;
~~~

Load setpoint to track to

### `deadband`
~~~
  double deadband;
~~~

Percentage deadband

### `damping`
~~~
  double damping;
~~~

Load setpoint to track to

### `output`
~~~
  double output;
~~~

Output scaling value

### `feedback`
~~~
  double feedback;
~~~

The feedback signal, for reference purposes

# Example

~~~
  object load_tracker {
    target_property "";
    operation "0";
    full_scale "0.0";
    setpoint "0.0";
    deadband "0.0";
    damping "0.0";
    output "0.0";
    feedback "0.0";
  }
~~~

# See also
* [[/Module/Powerflow]]

