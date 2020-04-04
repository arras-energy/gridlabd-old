[[/Command/Example]] -- Display an example of an instance of the class after init

# Synopsis

~~~
bash$ gridlabd --example module:class                                  
~~~

# Description

Display an example of an instance of the class after init.

The properties are groups according to whether they are required, optional, dynamic, output, or deprecated.

# Example

~~~
bash$ gridlabd --example climate:climate
class climate {
	// required input properties
	tmyfile "";
	// optional input properties
	city "";
	reader "";
	forecast "";
	// output properties
	solar_elevation "0.0";
	solar_azimuth "0.0";
	solar_zenith "0.0";
	temperature "59.0 degF";
	humidity "75%";
	solar_flux "0.0";
	solar_direct "0.0";
	...
}
~~~

# See also

* [[/Command/Modhelp]]
