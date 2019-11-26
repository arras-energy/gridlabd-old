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
    generator_mode [SUPPLY_DRIVEN|CONSTANT_PF|CONSTANT_PQ|CONSTANT_V|UNKNOWN];
    generator_status [ONLINE|OFFLINE];
    panel_type [CONCENTRATOR|THIN_FILM_GA_AS|AMORPHOUS_SILICON|MULTI_CRYSTAL_SILICON|SINGLE_CRYSTAL_SILICON];
    power_type [DC|AC];
    INSTALLATION_TYPE [GROUND_MOUNTED|ROOF_MOUNTED];
    SOLAR_TILT_MODEL [PLAYERVALUE|SOLPOS|DEFAULT];
    SOLAR_POWER_MODEL [FLATPLATE|DEFAULT];
    a_coeff "<decimal>";
    b_coeff "<decimal>";
    dT_coeff "<decimal>";
    T_coeff "<decimal>";
    NOCT "<decimal>";
    Tmodule "<decimal>";
    Tambient "<decimal>";
    wind_speed "<decimal>";
    ambient_temperature "<decimal>";
    Insolation "<decimal>";
    Rinternal "<decimal>";
    Rated_Insolation "<decimal>";
    Pmax_temp_coeff "<decimal>";
    Voc_temp_coeff "<decimal>";
    V_Max "<string>";
    Voc_Max "<string>";
    Voc "<string>";
    efficiency "<decimal>";
    area "<decimal>";
    soiling "<decimal>";
    derating "<decimal>";
    Tcell "<decimal>";
    Rated_kVA "<decimal>";
    rated_power "<decimal>";
    P_Out "<string>";
    V_Out "<string>";
    I_Out "<string>";
    VA_Out "<string>";
    weather "<string>";
    shading_factor "<decimal>";
    tilt_angle "<decimal>";
    orientation_azimuth "<decimal>";
    latitude_angle_fix "<string>";
    latitude "<decimal>";
    longitude "<decimal>";
    orientation [FIXED_AXIS|DEFAULT];
    phases [S|N|C|B|A];
  }
~~~

# Description

TODO

## Properties

### `generator_mode`
~~~
  enumeration {SUPPLY_DRIVEN, CONSTANT_PF, CONSTANT_PQ, CONSTANT_V, UNKNOWN} generator_mode;
~~~

TODO

### `generator_status`
~~~
  enumeration {ONLINE, OFFLINE} generator_status;
~~~

TODO

### `panel_type`
~~~
  enumeration {CONCENTRATOR, THIN_FILM_GA_AS, AMORPHOUS_SILICON, MULTI_CRYSTAL_SILICON, SINGLE_CRYSTAL_SILICON} panel_type;
~~~

TODO

### `power_type`
~~~
  enumeration {DC, AC} power_type;
~~~

TODO

### `INSTALLATION_TYPE`
~~~
  enumeration {GROUND_MOUNTED, ROOF_MOUNTED} INSTALLATION_TYPE;
~~~

TODO

### `SOLAR_TILT_MODEL`
~~~
  enumeration {PLAYERVALUE, SOLPOS, DEFAULT} SOLAR_TILT_MODEL;
~~~

Solar tilt model used to compute insolation values

### `SOLAR_POWER_MODEL`
~~~
  enumeration {FLATPLATE, DEFAULT} SOLAR_POWER_MODEL;
~~~

TODO

### `a_coeff`
~~~
  double a_coeff;
~~~

A coefficient for module temperature correction formula

### `b_coeff`
~~~
  double b_coeff[s/m];
~~~

B coefficient for module temperature correction formula

### `dT_coeff`
~~~
  double dT_coeff[m*m*degC/kW];
~~~

Temperature difference coefficient for module temperature correction formula

### `T_coeff`
~~~
  double T_coeff[%/degC];
~~~

Maximum power temperature coefficient for module temperature correction formula

### `NOCT`
~~~
  double NOCT[degF];
~~~

TODO

### `Tmodule`
~~~
  double Tmodule[degF];
~~~

TODO

### `Tambient`
~~~
  double Tambient[degC];
~~~

TODO

### `wind_speed`
~~~
  double wind_speed[mph];
~~~

TODO

### `ambient_temperature`
~~~
  double ambient_temperature[degF];
~~~

Current ambient temperature of air

### `Insolation`
~~~
  double Insolation[W/sf];
~~~

TODO

### `Rinternal`
~~~
  double Rinternal[Ohm];
~~~

TODO

### `Rated_Insolation`
~~~
  double Rated_Insolation[W/sf];
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
  complex V_Max[V];
~~~

TODO

### `Voc_Max`
~~~
  complex Voc_Max[V];
~~~

TODO

### `Voc`
~~~
  complex Voc[V];
~~~

TODO

### `efficiency`
~~~
  double efficiency[unit];
~~~

TODO

### `area`
~~~
  double area[sf];
~~~

TODO

### `soiling`
~~~
  double soiling[pu];
~~~

Soiling of array factor - representing dirt on array

### `derating`
~~~
  double derating[pu];
~~~

Panel derating to account for manufacturing variances

### `Tcell`
~~~
  double Tcell[degC];
~~~

TODO

### `Rated_kVA`
~~~
  double Rated_kVA[kVA];
~~~

This variable has issues with inconsistent handling in the code, so we will deprecate this in the future (VA maps to kVA, for example).

### `rated_power`
~~~
  double rated_power[W];
~~~

Used to define the size of the solar panel in power rather than square footage.

### `P_Out`
~~~
  complex P_Out[kW];
~~~

TODO

### `V_Out`
~~~
  complex V_Out[V];
~~~

TODO

### `I_Out`
~~~
  complex I_Out[A];
~~~

TODO

### `VA_Out`
~~~
  complex VA_Out[VA];
~~~

TODO

### `weather`
~~~
  object weather;
~~~

TODO

### `shading_factor`
~~~
  double shading_factor[pu];
~~~

Shading factor for scaling solar power to the array

### `tilt_angle`
~~~
  double tilt_angle[deg];
~~~

Tilt angle of PV array

### `orientation_azimuth`
~~~
  double orientation_azimuth[deg];
~~~

Facing direction of the PV array

### `latitude_angle_fix`
~~~
  bool latitude_angle_fix;
~~~

Fix tilt angle to installation latitude value

### `latitude`
~~~
  double latitude[deg];
~~~

The location of the array in degrees latitude

### `longitude`
~~~
  double longitude[deg];
~~~

The location of the array in degrees longitude

### `orientation`
~~~
  enumeration {FIXED_AXIS, DEFAULT} orientation;
~~~

TODO

### `phases`
~~~
  set {S, N, C, B, A} phases;
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

