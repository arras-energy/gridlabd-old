[[/Module/Powerflow/Pole_configuration]] -- Pole configuration library

# Synopsis

GLM:

~~~
  object pole_configuration {
    pole type "WOOD";
    design_ice_thickness 0.25;
    desing_wind_loading 4.0;
    design_temperature 15.0;
    overload_factor_vertical 1.9;
    overload_factor_transverse_general 1.75;
    overload_factor_transverse_crossing 2.2;
    overload_factor_transverse_wire 1.65;
    overload_factor_longitudinal_general 1.0;
    overload_factor_longitudinal_deadend 1.3;
    strength_factor_250b_wood 0.85;
    strength_factor_250b_support 1.0;
    pole_length 45.0;
    pole_depth 4.5;
    ground_diameter 32.5/3.14;
    top_diameter 19/3.14;
    fiber_strength 8000;
    treatment_method "CRESOTE";
  }
~~~

# Description

The `pole_configuration` object contains information about pole designs, and is referred to by `pole` objects.  See [Pole Loading Model](https://source.gridlabd.us/raw/master/module/powerflow/docs/pole_loading.pdf) for details on values for the pole library.

## Properties

### `pole_type`

~~~
  enumeration {STEEL=1, CONCRETE=2, WOOD=0} pole_type;
~~~

The material from which pole is made.

### `design_ice_thickness`

~~~
  double design_ice_thickness[in];
~~~

The design ice thickness on conductors.

### `design_wind_loading`

~~~
  double design_wind_loading[psi];
~~~

The design wind loading of the pole.

### `design_temperature`

~~~
  double design_temperature[degF];
~~~

The design temperature of the pole.

### `overload_factor_vertical`

~~~
  double overload_factor_vertical;
~~~

The vertical design overload factor.

### `overload_factor_transverse_general`

~~~
  double overload_factor_transverse_general;
~~~

The general transverse design overload factor.

### `overload_factor_transverse_crossing`

~~~
  double overload_factor_transverse_crossing;
~~~

The transverse crossing design overload factor.

### `overload_factor_transverse_wire`

~~~
  double overload_factor_transverse_wire;
~~~

The transverse wire design overload factor.

### `overload_factor_longitudinal_general`

~~~
  double overload_factor_longitudinal_general;
~~~

The longitudinal general design overload factor.

### `overload_factor_longitudinal_deadend`

~~~
  double overload_factor_longitudinal_deadend;
~~~

The longitudinal deadend design overload factor.

### `strength_factor_250b_wood`

~~~
  double strength_factor_250b_wood;
~~~

The Rule 250B wood structure design strength factor.

### `strength_factor_250b_support`

~~~
  double strength_factor_250b_support;
~~~

The Rule 250B support hardware design strength factor.

### `strength_factor_250c_wood`

~~~
  double strength_factor_250c_wood;
~~~

The Rule 250C wood structure design strength factor.

### `strength_factor_250c_support`

~~~
  double strength_factor_250c_support;
~~~

The Rule 250C support hardware design strength factor.

### `pole_length`

~~~
  double pole_length[ft];
~~~

The total length of pole including underground portion.

### `pole_depth`

~~~
  double pole_depth[ft];
~~~

The depth of pole underground.

### `ground_diameter`

~~~
  double ground_diameter[in];
~~~

The diameter of pole at ground level.

### `top_diameter`

~~~
  double top_diameter[in];
~~~

The diameter of pole at top.

### `fiber_strength`

~~~
  double fiber_strength[psi];
~~~

The pole structural strength.

### `treatment_method`

~~~
  enumeration {NONE, CREOSOTE, PENTA, CCA} treatment_method;
~~~

The pole treatment method.

# See also

* [[/Module/Powerflow/Pole]]
* [Pole Loading Model](https://source.gridlabd.us/raw/master/module/powerflow/docs/pole_loading.pdf)
* [UEP Bulletin 1728F-804](https://www.rd.usda.gov/files/UEP_Bulletin_1728F-804.pdf)
