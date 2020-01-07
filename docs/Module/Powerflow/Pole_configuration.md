[[/Module/Powerflow/Pole_configuration]] -- Pole configuration library

# Synopsis
GLM:
~~~
  object pole_configuration {
    pole_status "OK";
    tilt_angle "+0 rad";
    tilt_direction "+0 deg";
    weather "";
    configuration "";
    equipment_area "+0 sf";
    equipment_height "+0 ft";
    pole_stress "+0 pu";
    pole_stress_polynomial_a "+0 ft*lb";
    pole_stress_polynomial_b "+0 ft*lb";
    pole_stress_polynomial_c "+0 ft*lb";
    susceptibility "+0 pu*s/m";
    total_moment "+0 ft*lb";
    resisting_moment "+0 ft*lb";
    critical_wind_speed "+0 m/s";
    install_year "0";
    repair_time "+0 h";
  }
~~~

# Description

The `pole_configuration` object contains information about pole designs, and is referred to by `pole` objects.  See [Pole Loading Model](https://github.com/slacgismo/gridlabd/raw/grip/powerflow/docs/pole_loading.pdf) for details on values for the pole library.

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

### `repair_time`
~~~
  double repair_time[h]; 
~~~

The nominal pole repair time.

### `treatment_method`
~~~
  enumeration {NONE, CREOSOTE, PENTA, CCA} treatment_method; 
~~~

The pole treatment method.

# See also

* [[/Module/Powerflow/Pole]]
* [Pole Loading Model](https://github.com/slacgismo/gridlabd/raw/grip/powerflow/docs/pole_loading.pdf)
