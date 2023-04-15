[[/Module/Powerflow/Ductbank]] -- Duct bank for underground cable

# Synopsis

~~~
module powerflow 
{
	double ground_temperature[degC];
	double warning_cable_temperature[degC];
	double alert_cable_temperature[degC];
	char1024 ductbank_configuration_file;
	double air_effective_conductivity[W/K*m];
}
class ductbank {
	char1024 configuration; // ductbank configuration name
	char256 temperature_source; // source for surface temperature
	double surface_temperature[degC]; // ground temperature at the surface of fill
	int32 channels; // number of channels in duct
	double fill_R[K/W]; // fill soil thermal resistance
	double duct_R[K/W]; // duct thermal resistance
	double air_R[K/W]; // air gap thermal resistance
	double insulation_R[K/W]; // cable insulation thermal
	double duct_area[m^2]; // cross sectional area of interior air space
	double heatgain[W/m]; // (OUTPUT) cable heat per unit length
	double cable_temperature[degC]; // (OUTPUT) cable temperature in duct
	double peak_temperature[degC]; // (OUTPUT) peak cable temperature in duct
	enumeration {UNKNOWN=3, ALERT=2, WARNING=1, OK=0} cable_status; // (OUTPUT) cable status
}
~~~

# Description

The `ductbank` object models the temperature of an underground cable duct. Heat gain to duct originates with the losses on underground cables associated with the duct using the `ductbank` property of `underground_line` objects. The insulation thermal resistance is taken from the `underground_line_configuration` object's `insulation_R` property.

The default ground temperature is specified using the module global `ground_temperature`. This value will be used if a duct bank object's ground temperature is not specified. If the `temperature_source` is specified, then the `surface_temperature` is taken from the specified object property.

Cable temperatures are calculated based on the heat loss to the ground from the duct shell through airgap and the cable insulation.  R-values duct and fill are either explicitly specified or calculated based on the configuration name, which is looked up in the `ductbank_configurations.csv` data file installed in the GridLAB-D `share` (as specified by the `GLD_ETC` environment variable). If a local copy of the file is found, that version is loaded instead.  The name of the configuration file may be specified using the `ductbank_configuration_file` module global.

Cable temperatures are compared to the module globals `warning_cable_temperature` and `alert_cable_temperature` and warning messages are displayed accordingly. In addition the variable `temperature_status` is updated.

# See also

* [[/Module/Powerflow/Underground_line]]
* [[/Module/Powerflow/Underground_line_configuration]]
