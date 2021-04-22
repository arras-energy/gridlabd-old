[[/Module/Powerflow/Pole]] -- Power pole model

# Synopsis

~~~
  object pole {
    // node properties
    pole_status "OK";
    tilt_angle "0 deg";
    tilt_direction 0 deg";
    weather "<climate-object>";
    configuration "<pole-configuration-object>";
    equipment_area "0 sf";
    equipment_height "0 ft":
    install_year "1970";
    repair_time "24 h";
    pole_stress "0 pu";
    pole_stress_polynomial_a "0 ft*lb";
    pole_stress_polynomial_b "0 ft*lb";
    pole_stress_polynomial_c "0 ft*lb";
    susceptibility "0 pu*s/m";
    total_moment "0 ft*lb";
    resisting_moment "0 ft*lb";
    critical_wind_speed "0 m/s";
  }
~~~

# Description

The pole object models poles at nodes. The current pole object models pole failure by overstress failure at the base the pole from wind pressure on the equipment, wire, and tilt.  The model does not model failures from ice loading on the lines or from foundation failure.

Generally, any node in a powerflow model can be upgraded to a pole by providing just the `weather` and `pole_configuration` object references. 

Specifying `equipment_area` and `equipment_height` will cause the equipment wind load to be computed.

## Properties

### `pole_status`

~~~
  enumeration {FAILED=0,OK=1} pole_status;
~~~

The status of the pole.

### `tilt_angle`

~~~
  double tilt_angle[deg];
~~~

The tilt angle of the pole.

### `tilt_direction`

~~~
  double tilt_direction[deg];
~~~

The tilt direction of the pole.

### `weather`

~~~
  object weather;
~~~

Reference to the source of the weather data.

### `configuration`

~~~
  object configuration;
~~~

Reference to the pole configuration.

### `equipment_area`

~~~
  double equipment_area[sf];
~~~

Cross sectional area of the equipment.

### `equipment_height`

~~~
  double equipment_height[ft];
~~~

Height of the equipment centroid on the pole.

### `install_year`

~~~
  int32 install_year;
~~~

Year the pole was installed.

### `repair_time`

~~~
  double repair_time[h];
~~~

Time required to repair or replace the pole after failure.

### `pole_stress`

~~~
  double pole_stress[pu]; 
~~~

Ratio of actual stress to critical stress.

### `pole_stress_polynomial_a`

~~~
  double pole_stress_polynomial_a[ft*lb]; 
~~~

Constant a of the pole stress polynomial function.

### `pole_stress_polynomial_b`

~~~
  double pole_stress_polynomial_b[ft*lb]; 
~~~

Constant b of the pole stress polynomial function

### `pole_stress_polynomial_c`

~~~
  double pole_stress_polynomial_c[ft*lb]; 
~~~

Constant c of the pole stress polynomial function.

### `susceptibility`

~~~
  double susceptibility[pu*s/m]; 
~~~

Susceptibility of pole to wind stress (derivative of pole stress w.r.t wind speed).

### `total_moment`

~~~
  double total_moment[ft*lb];
~~~

The total moment on the pole.

### `resisting_moment`

~~~
  double resisting_moment[ft*lb]; 
~~~

The resisting moment on the pole.

### `critical_wind_speed`

~~~
  double critical_wind_speed[m/s];
~~~

Wind speed at pole failure.

# Model

The pole failure model is described in [Pole Loading Model](https://github.com/hipas/gridlabd/raw/master/module/powerflow/docs/pole_loading.pdf). 

The pole reaches end of life status based on a degradation rate that is defined by minimum shell thickness of 2". See [Pole Degradation Model](https://www.sciencedirect.com/science/article/pii/S0167473005000457) details.

# See also

* [[/Module/Powerflow/Pole_configuration]]
* [Pole Loading Model](https://github.com/hipas/gridlabd/raw/master/powerflow/docs/pole_loading.pdf)
* [Pole Degradation Model](https://www.sciencedirect.com/science/article/pii/S0167473005000457)
