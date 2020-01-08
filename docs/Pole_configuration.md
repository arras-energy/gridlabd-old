[[Pole_configuration]] -- Pole configuration library

# Synopsis
~~~
 class pole_configuration {
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

See [Pole Loading Model](https://github.com/slacgismo/gridlabd/raw/grip/powerflow/docs/pole_loading.pdf) for details on values for the pole library.

# See also

* [[Pole]] object
* [Pole Loading Model](https://github.com/slacgismo/gridlabd/raw/grip/powerflow/docs/pole_loading.pdf)
