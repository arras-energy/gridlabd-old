[[/Command/Modhelp]] -- Display structure of a class or all classes in a module

# Synopsis

~~~
bash$ gridlabd --modhelp[=json] module[:class]                                
~~~

# Description

Display structure of a class or all classes in a module. If the `=json` option is included, the output is generated in JSON format.

The comment section includes whether the property is a required input, optional, dynamic, output, or deprecated.

# Example

~~~
gridlabd --modhelp climate:climate
module climate {
	set {QUIET=65536, WARNING=131072, DEBUG=262144, VERBOSE=524288} message_flags; // module message control flags
}
class climate {
	function calculate_solar_radiation_degrees();
	function calculate_solar_radiation_radians();
	function calculate_solar_radiation_shading_degrees();
	function calculate_solar_radiation_shading_radians();
	function calculate_solar_radiation_shading_position_radians();
	function calculate_solpos_radiation_shading_position_radians();
	function calc_solar_ideal_shading_position_radians();
	double solar_elevation[rad]; // (OUTPUT) solar elevation angle in radians
	double solar_azimuth[rad]; // (OUTPUT) solar azimuth angle in radians
	double solar_zenith[rad]; // (OUTPUT) solar zenith angle in radians
	char32 city; // weather data city name
	char1024 tmyfile; // (REQUIRED) weather data file name
	double temperature[degF]; // (OUTPUT) current temperature
	double humidity[pu]; // (OUTPUT) current humidity
	...
}
~~~

# See also

* [[/Command/Example]]
