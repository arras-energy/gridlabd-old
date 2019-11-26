[[/Module/Generators]] -- Module generators

# Synopsis
GLM:
~~~
  module generators {
    message_flags "DEBUG|VERBOSE";
    enable_subsecond_models "FALSE";
    deltamode_timestep "+1e+07 ns";
  }
  object solar {
    generator_mode "0";
    generator_status "0";
    panel_type "0";
    power_type "0";
    INSTALLATION_TYPE "0";
    SOLAR_TILT_MODEL "0";
    SOLAR_POWER_MODEL "0";
    a_coeff "0.0";
    b_coeff "0.0";
    dT_coeff "0.0";
    T_coeff "0.0";
    NOCT "0.0";
    Tmodule "0.0";
    Tambient "0.0";
    wind_speed "0.0";
    ambient_temperature "0.0";
    Insolation "0.0";
    Rinternal "0.0";
    Rated_Insolation "0.0";
    Pmax_temp_coeff "0.0";
    Voc_temp_coeff "0.0";
    V_Max "0+0i";
    Voc_Max "0+0i";
    Voc "0+0i";
    efficiency "0.0";
    area "0.0";
    soiling "0.0";
    derating "0.0";
    Tcell "0.0";
    Rated_kVA "0.0";
    rated_power "0.0";
    P_Out "0+0i";
    V_Out "0+0i";
    I_Out "0+0i";
    VA_Out "0+0i";
    weather "";
    shading_factor "0.0";
    tilt_angle "0.0";
    orientation_azimuth "0.0";
    latitude_angle_fix "FALSE";
    latitude "0.0";
    longitude "0.0";
    orientation "0";
    phases "0";
  }
~~~

# Description

TODO

## Properties

### `generator_mode`
~~~
  enumeration generator_mode;
~~~

TODO

### `generator_status`
~~~
  enumeration generator_status;
~~~

TODO

### `panel_type`
~~~
  enumeration panel_type;
~~~

TODO

### `power_type`
~~~
  enumeration power_type;
~~~

TODO

### `INSTALLATION_TYPE`
~~~
  enumeration INSTALLATION_TYPE;
~~~

TODO

### `SOLAR_TILT_MODEL`
~~~
  enumeration SOLAR_TILT_MODEL;
~~~

Solar tilt model used to compute insolation values

### `SOLAR_POWER_MODEL`
~~~
  enumeration SOLAR_POWER_MODEL;
~~~

TODO

### `a_coeff`
~~~
  double a_coeff;
~~~

A coefficient for module temperature correction formula

### `b_coeff`
~~~
  double b_coeff;
~~~

B coefficient for module temperature correction formula

### `dT_coeff`
~~~
  double dT_coeff;
~~~

Temperature difference coefficient for module temperature correction formula

### `T_coeff`
~~~
  double T_coeff;
~~~

Maximum power temperature coefficient for module temperature correction formula

### `NOCT`
~~~
  double NOCT;
~~~

TODO

### `Tmodule`
~~~
  double Tmodule;
~~~

TODO

### `Tambient`
~~~
  double Tambient;
~~~

TODO

### `wind_speed`
~~~
  double wind_speed;
~~~

TODO

### `ambient_temperature`
~~~
  double ambient_temperature;
~~~

Current ambient temperature of air

### `Insolation`
~~~
  double Insolation;
~~~

TODO

### `Rinternal`
~~~
  double Rinternal;
~~~

TODO

### `Rated_Insolation`
~~~
  double Rated_Insolation;
~~~

TODO

### `Pmax_temp_coeff`
~~~
  double Pmax_temp_coeff;
~~~

TODO

### `Voc_temp_coeff`
~~~
  double Voc_temp_coeff;
~~~

TODO

### `V_Max`
~~~
  complex V_Max;
~~~

TODO

### `Voc_Max`
~~~
  complex Voc_Max;
~~~

TODO

### `Voc`
~~~
  complex Voc;
~~~

TODO

### `efficiency`
~~~
  double efficiency;
~~~

TODO

### `area`
~~~
  double area;
~~~

TODO

### `soiling`
~~~
  double soiling;
~~~

Soiling of array factor - representing dirt on array

### `derating`
~~~
  double derating;
~~~

Panel derating to account for manufacturing variances

### `Tcell`
~~~
  double Tcell;
~~~

TODO

### `Rated_kVA`
~~~
  double Rated_kVA;
~~~

This variable has issues with inconsistent handling in the code, so we will deprecate this in the future (VA maps to kVA, for example).

### `rated_power`
~~~
  double rated_power;
~~~

Used to define the size of the solar panel in power rather than square footage.

### `P_Out`
~~~
  complex P_Out;
~~~

TODO

### `V_Out`
~~~
  complex V_Out;
~~~

TODO

### `I_Out`
~~~
  complex I_Out;
~~~

TODO

### `VA_Out`
~~~
  complex VA_Out;
~~~

TODO

### `weather`
~~~
  object weather;
~~~

TODO

### `shading_factor`
~~~
  double shading_factor;
~~~

Shading factor for scaling solar power to the array

### `tilt_angle`
~~~
  double tilt_angle;
~~~

Tilt angle of PV array

### `orientation_azimuth`
~~~
  double orientation_azimuth;
~~~

Facing direction of the PV array

### `latitude_angle_fix`
~~~
  bool latitude_angle_fix;
~~~

Fix tilt angle to installation latitude value

### `latitude`
~~~
  double latitude;
~~~

The location of the array in degrees latitude

### `longitude`
~~~
  double longitude;
~~~

The location of the array in degrees longitude

### `orientation`
~~~
  enumeration orientation;
~~~

TODO

### `phases`
~~~
  set phases;
~~~

TODO

# Example

~~~
  object solar {
    generator_mode "0";
    generator_status "0";
    panel_type "0";
    power_type "0";
    INSTALLATION_TYPE "0";
    SOLAR_TILT_MODEL "0";
    SOLAR_POWER_MODEL "0";
    a_coeff "0.0";
    b_coeff "0.0";
    dT_coeff "0.0";
    T_coeff "0.0";
    NOCT "0.0";
    Tmodule "0.0";
    Tambient "0.0";
    wind_speed "0.0";
    ambient_temperature "0.0";
    Insolation "0.0";
    Rinternal "0.0";
    Rated_Insolation "0.0";
    Pmax_temp_coeff "0.0";
    Voc_temp_coeff "0.0";
    V_Max "0+0i";
    Voc_Max "0+0i";
    Voc "0+0i";
    efficiency "0.0";
    area "0.0";
    soiling "0.0";
    derating "0.0";
    Tcell "0.0";
    Rated_kVA "0.0";
    rated_power "0.0";
    P_Out "0+0i";
    V_Out "0+0i";
    I_Out "0+0i";
    VA_Out "0+0i";
    shading_factor "0.0";
    tilt_angle "0.0";
    orientation_azimuth "0.0";
    latitude_angle_fix "FALSE";
    latitude "0.0";
    longitude "0.0";
    orientation "0";
    phases "0";
  }
~~~

# See also
* [[/Module/Generators]]

