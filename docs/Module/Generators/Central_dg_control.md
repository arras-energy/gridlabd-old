[[/Module/Generators/Central_dg_control]] -- Class central_dg_control

# Synopsis
GLM:
~~~
  object central_dg_control {
    controlled_dgs "<string>";
    feederhead_meter "<string>";
    control_mode_0 "{PEAK_SHAVING,CONSTANT_PF,NO_CONTROL}";
    control_mode_1 "{PEAK_SHAVING,CONSTANT_PF,NO_CONTROL}";
    control_mode_2 "{PEAK_SHAVING,CONSTANT_PF,NO_CONTROL}";
    control_mode_3 "{PEAK_SHAVING,CONSTANT_PF,NO_CONTROL}";
    peak_S "<decimal> W";
    pf_low "<decimal> unit";
    pf_high "<decimal> unit";
  }
~~~

# Description

TODO

## Properties

### `controlled_dgs`
~~~
  char32 controlled_dgs;
~~~

The group ID of the dg objects the controller controls.

### `feederhead_meter`
~~~
  object feederhead_meter;
~~~

The name of the meter.

### `control_mode_0`
~~~
  enumeration {PEAK_SHAVING, CONSTANT_PF, NO_CONTROL} control_mode_0;
~~~

TODO

### `control_mode_1`
~~~
  enumeration {PEAK_SHAVING, CONSTANT_PF, NO_CONTROL} control_mode_1;
~~~

TODO

### `control_mode_2`
~~~
  enumeration {PEAK_SHAVING, CONSTANT_PF, NO_CONTROL} control_mode_2;
~~~

TODO

### `control_mode_3`
~~~
  enumeration {PEAK_SHAVING, CONSTANT_PF, NO_CONTROL} control_mode_3;
~~~

TODO

### `peak_S`
~~~
  double peak_S[W];
~~~

TODO

### `pf_low`
~~~
  double pf_low[unit];
~~~

TODO

### `pf_high`
~~~
  double pf_high[unit];
~~~

TODO

# Example

~~~
  object central_dg_control {
    controlled_dgs "";
    control_mode_0 "0";
    control_mode_1 "0";
    control_mode_2 "0";
    control_mode_3 "0";
    peak_S "0.0";
    pf_low "0.0";
    pf_high "0.0";
  }
~~~

# See also
* [[/Module/Generators]]

