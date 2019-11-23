[[/Module/Powerflow/Pole_configuration]] -- Pole configuration library

# Synopsis
~~~
  object pole_configuration {
    enumeration {STEEL=1, CONCRETE=2, WOOD=0} pole_type; // material from which pole is made
    double design_ice_thickness[in]; // design ice thickness on conductors
    double design_wind_loading[psi]; // design wind loading on pole
    double design_temperature[degF]; // design temperature for pole
    double overload_factor_vertical; // design overload factor (vertical)
    double overload_factor_transverse_general; // design overload factor (transverse general)
    double overload_factor_transverse_crossing; // design overload factor (transverse crossing)
    double overload_factor_transverse_wire; // design overload factor (transverse wire)
    double overload_factor_longitudinal_general; // design overload factor (longitudinal general)
    double overload_factor_longitudinal_deadend; // design overload factor (longitudinal deadend)
    double strength_factor_250b_wood; // design strength factor (Rule 250B wood structure)
    double strength_factor_250b_support; // design strength factor (Rule 250B support hardware)
    double strength_factor_250c_wood; // design strength factor (Rule 250C wood structure)
    double strength_factor_250c_support; // design strength factor (Rule 250C support hardware)
    double pole_length[ft]; // total length of pole (including underground portion)
    double pole_depth[ft]; // depth of pole underground
    double ground_diameter[in]; // diameter of pole at ground level
    double top_diameter[in]; // diameter of pole at top
    double fiber_strength[psi]; // pole structural strength
    double repair_time[h]; // pole repair time
    enumeration {NONE, CREOSOTE, PENTA, CCA} treatment_method; // pole treatment method
  }
~~~

# Description

The `pole_configuration` object contains information about pole designs, and is referred to by `pole` objects.  See [Pole Loading Model](https://github.com/dchassin/gridlabd/raw/grip/powerflow/docs/pole_loading.pdf) for details on values for the pole library.

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

* [[Pole]] object
* [Pole Loading Model](https://github.com/dchassin/gridlabd/raw/grip/powerflow/docs/pole_loading.pdf)
