[[/Module/Powerflow/Pole]] -- Power pole model

# Synopsis

~~~
object pole
{
    status "OK";
    tilt_angle "0 deg";
    tilt_direction 0 deg";
    weather "<climate-object>";
    configuration "<pole-configuration-object>";
    install_year "1970";
    repair_time "24 h";
    wind_speed 0.0 m/s;
    wind_direction 0.0 deg;
    wind_gusts 0.0 m/s;
    pole_stress "0 pu";
    pole_stress_polynomial_a "0 ft*lb";
    pole_stress_polynomial_b "0 ft*lb";
    pole_stress_polynomial_c "0 ft*lb";
    susceptibility "0 pu*s/m";
    total_moment "0 ft*lb";
    resisting_moment "0 ft*lb";
    pole_moment "0 ft*lb"; 
    pole_moment_nowind "0 ft*lb"; 
    equipment_moment "0 ft*lb"; 
    equipment_moment_nowind "0 ft*lb"; 
    critical_wind_speed "0 m/s";
    wire_moment "0 ft*lb";
    wire_tension "0 ft*lb";
    guy_height "0 ft";
}
~~~

# Description

The pole object models poles in powerflow network. The current pole object models pole failure by overstress at the base the pole from wind pressure on the equipment, wire, and tilt.  The model does not model failures from ice loading on the lines or from foundation failure.

Generally, any link object in a powerflow model can be mounted on a pole by creating a `pole_mount` object that uses the pole as a parent, and providing just the `weather` and `pole_configuration` object references to the pole. The `pole_mount` object specifies the position, size, and weight of the equipment on the pole to model the effect of wind give the aerodynamic cross-section.

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

### `install_year`

~~~
    int32 install_year;
~~~

Year the pole was installed.

### `repair_time`

~~~
    double repair_time[h];
~~~

Time required to repair or replace the pole after failure. Includes time to identify the failure and reach the site.

### `wind_speed`

~~~
    int32 wind_speed [m/s];
~~~

The wind speed at the location of the pole.  This is automatically set is the
`weather` object is provided.

### `wind_direction`

~~~
    int32 wind_direction [deg];
~~~

The wind direction at the location of the pole.  This is automatically set is the
`weather` object is provided.

### `wind_gusts`

~~~
    int32 wind_gusts [m/s];
~~~

The wind gusts at the location of the pole.  This is automatically set is the
`weather` object is provided.

### `pole_stress`

~~~
    double pole_stress[pu];
~~~

Ratio of actual stress to critical stress. When this reaches 1.0 or higher, the pole failes.

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

Susceptibility of pole to wind stress (derivative of pole stress with respect to wind speed).

### `total_moment`

~~~
    double total_moment[ft*lb];
~~~

The total moment on the pole. Calculated by summing each (vector) moment considered.

### `resisting_moment`

~~~
    double resisting_moment[ft*lb];
~~~

Maximum moment the pole can withstand before snapping. This is calculated based on the fiber strength of the wood species and the pole diameter where it is supported - either at ground line or where guy wire(s) are attached.

### `pole_moment`

~~~
    double pole_moment[ft*lb];
~~~

The moment of the pole.

### `pole_moment_nowind`

~~~
    double pole_moment_nowind[ft*lb];
~~~

The moment of the pole without wind, due to its weight and tilt. 

### `equipment_moment`

~~~
    double equipment_moment[ft*lb];
~~~

The moment of the equipment. 

### `equipment_moment_nowind`

~~~
    double equipment_moment_nowind[ft*lb];
~~~

The moment of the equipment without wind. 

The resisting moment on the pole.

### `critical_wind_speed`

~~~
    double critical_wind_speed[m/s];
~~~

Wind speed at pole failure.

### `wire_moment`

~~~
    double wire_moment[ft*lb];
~~~

Wire moment due to conductor weight.

### `wire_tension`

~~~
    double wire_tension[ft*lb];
~~~

Wire moment due to conductor tension.

### `guy_height`

~~~
    double guy_height[m/s];
~~~

Guy wire attachment height.

# Model

The pole failure model is described in [Pole Loading Model](https://source.gridlabd.us/raw/master/module/powerflow/docs/pole_loading.pdf).

[Out of date, change this:] The pole reaches end of life status based on a degradation rate that is defined by minimum shell thickness of 2". See [Pole Degradation Model](https://www.sciencedirect.com/science/article/pii/S0167473005000457) details.

# See also

* [[/Module/Powerflow/Pole_configuration]](https://github.com/arras-energy/gridlabd/blob/master/docs/Module/Powerflow/Pole_configuration.md)
* [Pole Loading Model](https://source.gridlabd.us/raw/master/module/powerflow/docs/pole_loading.pdf)
* [Pole Degradation Model](https://www.sciencedirect.com/science/article/pii/S0167473005000457)
