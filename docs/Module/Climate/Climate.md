[[/Module/Climate/Climate]] -- Class climate

# Synopsis

GLM:

~~~
  object climate {
    solar_elevation "<decimal> rad";
    solar_azimuth "<decimal> rad";
    solar_zenith "<decimal> rad";
    city "<string>";
    tmyfile "<string>";
    temperature "<decimal> degF";
    humidity "<decimal> pu";
    solar_flux "<decimal> W/sf";
    solar_direct "<decimal> W/sf";
    solar_diffuse "<decimal> W/sf";
    solar_global "<decimal> W/sf";
    extraterrestrial_global_horizontal "<decimal> W/sf";
    extraterrestrial_direct_normal "<decimal> W/sf";
    pressure "<decimal> mbar";
    wind_speed "<decimal> m/s";
    wind_dir "<decimal> rad";
    wind_gust "<decimal> m/s";
    record.low "<decimal> degF";
    record.low_day "<integer>";
    record.high "<decimal> degF";
    record.high_day "<integer>";
    record.solar "<decimal> W/sf";
    rainfall "<decimal> in/h";
    snowdepth "<decimal> in";
    interpolate "{QUADRATIC,LINEAR,NONE}";
    solar_horiz "<decimal>";
    solar_north "<decimal>";
    solar_northeast "<decimal>";
    solar_east "<decimal>";
    solar_southeast "<decimal>";
    solar_south "<decimal>";
    solar_southwest "<decimal>";
    solar_west "<decimal>";
    solar_northwest "<decimal>";
    solar_raw "<decimal> W/sf";
    ground_reflectivity "<decimal> pu";
    reader "<string>";
    forecast "<string>";
    cloud_model "{CUMULUS,NONE}";
    cloud_opacity "<decimal> pu";
    opq_sky_cov "<decimal> pu";
    cloud_speed_factor "<decimal> pu";
    solar_cloud_direct "<decimal> W/sf";
    solar_cloud_diffuse "<decimal> W/sf";
    solar_cloud_global "<decimal> W/sf";
    cloud_alpha "<decimal> pu";
    cloud_num_layers "<decimal> pu";
    cloud_aerosol_transmissivity "<decimal> pu";
  }
~~~

# Description

TODO

## Properties

### `solar_elevation`

~~~
  double solar_elevation[rad];
~~~

Solar elevation angle in radians

### `solar_azimuth`

~~~
  double solar_azimuth[rad];
~~~

Solar azimuth angle in radians

### `solar_zenith`

~~~
  double solar_zenith[rad];
~~~

Solar zenith angle in radians

### `city`

~~~
  char32 city;
~~~

Weather data city name

### `tmyfile`

~~~
  char1024 tmyfile;
~~~

Weather data file name

### `temperature`

~~~
  double temperature[degF];
~~~

Current temperature

### `humidity`

~~~
  double humidity[pu];
~~~

Current humidity

### `solar_flux`

~~~
  double solar_flux[W/sf];
~~~

Current solar irradiance (9 orientiations)

### `solar_direct`

~~~
  double solar_direct[W/sf];
~~~

Solar direct irradiance

### `solar_diffuse`

~~~
  double solar_diffuse[W/sf];
~~~

Solar diffuse irradiance

### `solar_global`

~~~
  double solar_global[W/sf];
~~~

Solar global flux irradiance

### `extraterrestrial_global_horizontal`

~~~
  double extraterrestrial_global_horizontal[W/sf];
~~~

Solar global extraterrestrial irradiance

### `extraterrestrial_direct_normal`

~~~
  double extraterrestrial_direct_normal[W/sf];
~~~

Solar direct normal extraterrestrial irradiance

### `pressure`

~~~
  double pressure[mbar];
~~~

Current air pressure

### `wind_speed`

~~~
  double wind_speed[m/s];
~~~

Current wind speed

### `wind_dir`

~~~
  double wind_dir[rad];
~~~

Current direction in radians

### `wind_gust`

~~~
  double wind_gust[m/s];
~~~

Current wind gusts

### `record.low`

~~~
  double record.low[degF];
~~~

Record low temperature observed

### `record.low_day`

~~~
  int32 record.low_day;
~~~

Day of year for record low observation

### `record.high`

~~~
  double record.high[degF];
~~~

Record high temperature observed

### `record.high_day`

~~~
  int32 record.high_day;
~~~

Day of year for record high observation

### `record.solar`

~~~
  double record.solar[W/sf];
~~~

Record high solar irradiance observed

### `rainfall`

~~~
  double rainfall[in/h];
~~~

Rainfall observed

### `snowdepth`

~~~
  double snowdepth[in];
~~~

Snow depth observed

### `interpolate`

~~~
  enumeration {QUADRATIC, LINEAR, NONE} interpolate;
~~~

The interpolation mode used on the climate data

### `solar_horiz`

~~~
  double solar_horiz;
~~~

TODO

### `solar_north`

~~~
  double solar_north;
~~~

TODO

### `solar_northeast`

~~~
  double solar_northeast;
~~~

TODO

### `solar_east`

~~~
  double solar_east;
~~~

TODO

### `solar_southeast`

~~~
  double solar_southeast;
~~~

TODO

### `solar_south`

~~~
  double solar_south;
~~~

TODO

### `solar_southwest`

~~~
  double solar_southwest;
~~~

TODO

### `solar_west`

~~~
  double solar_west;
~~~

TODO

### `solar_northwest`

~~~
  double solar_northwest;
~~~

TODO

### `solar_raw`

~~~
  double solar_raw[W/sf];
~~~

TODO

### `ground_reflectivity`

~~~
  double ground_reflectivity[pu];
~~~

Ground reflectivity observed

### `reader`

~~~
  object reader;
~~~

Weather reader object

### `forecast`

~~~
  char1024 forecast;
~~~

Forecasting specifications

### `cloud_model`

~~~
  enumeration {CUMULUS, NONE} cloud_model;
~~~

The cloud model to use

### `cloud_opacity`

~~~
  double cloud_opacity[pu];
~~~

Cloud opacity factor

### `opq_sky_cov`

~~~
  double opq_sky_cov[pu];
~~~

Cloud sky coverage factor

### `cloud_speed_factor`

~~~
  double cloud_speed_factor[pu];
~~~

Cloud speed factor

### `solar_cloud_direct`

~~~
  double solar_cloud_direct[W/sf];
~~~

Cloud direct irradiance

### `solar_cloud_diffuse`

~~~
  double solar_cloud_diffuse[W/sf];
~~~

Cloud diffuse irradiance

### `solar_cloud_global`

~~~
  double solar_cloud_global[W/sf];
~~~

Cloud global irradiance

### `cloud_alpha`

~~~
  double cloud_alpha[pu];
~~~

Cloud alpha

### `cloud_num_layers`

~~~
  double cloud_num_layers[pu];
~~~

Number of cloud layers

### `cloud_aerosol_transmissivity`

~~~
  double cloud_aerosol_transmissivity[pu];
~~~

Cloud aerosal transmissivity

# Example

~~~
  object climate {
    solar_elevation "0.0";
    solar_azimuth "0.0";
    solar_zenith "0.0";
    city "";
    tmyfile "";
    temperature "59.0 degF";
    humidity "75%";
    solar_flux "0.0";
    solar_direct "0.0";
    solar_diffuse "0.0";
    solar_global "0.0";
    extraterrestrial_global_horizontal "0.0";
    extraterrestrial_direct_normal "1367 W/m^2";
    pressure "1013.25 mbar";
    wind_speed "0.0";
    wind_dir "0.0";
    wind_gust "0.0";
    record.low "0.0";
    record.low_day "0";
    record.high "0.0";
    record.high_day "0";
    record.solar "0.0";
    rainfall "0.0";
    snowdepth "0.0";
    interpolate "NONE";
    solar_horiz "0.0";
    solar_north "0.0";
    solar_northeast "0.0";
    solar_east "0.0";
    solar_southeast "0.0";
    solar_south "0.0";
    solar_southwest "0.0";
    solar_west "0.0";
    solar_northwest "0.0";
    solar_raw "0.0";
    ground_reflectivity "0.3 pu";
    forecast "";
    cloud_model "NONE";
    cloud_opacity "1.0 pu";
    opq_sky_cov "0.0";
    cloud_speed_factor "1.0 pu";
    solar_cloud_direct "0.0";
    solar_cloud_diffuse "0.0";
    solar_cloud_global "0.0";
    cloud_alpha "400 pu";
    cloud_num_layers "40 pu";
    cloud_aerosol_transmissivity "0.95 pu";
  }
~~~

# See also

* [[/Module/Climate]]

