[[/Module/Powerflow/Ductbank]] -- Duct bank for underground cable

# Synopsis

~~~
module powerflow 
{
	char1024 ductbank_configuration_file;
	double ground_temperature[degC];
	double warning_cable_temperature[degC];
	double alert_cable_temperature[degC];
}
class ductbank {
	char1024 configuration; // ductbank configuration name
	double ground_temperature[degC]; // duct ground temperature
	double duct_R[K*m/W]; // duct thermal resistance per unit duct length
	double airgap_R[K*m/W]; // air gap thermal resistance per unit duct length
	double insulation_R[K*m/W]; // cable insulation thermal resistance per unit duct length
	double section_area[m^2]; // cross sectional area of interior air space
	double heatgain[W/m]; // (OUTPUT) cable heat per unit length
	double duct_temperature[degC]; // (OUTPUT) duct shell temperature
	double air_temperature[degC]; // (OUTPUT) duct air temperature
	double cable_temperature[degC]; // (OUTPUT) duct cable temperature
	enumeration {ALERT=2, WARNING=1, OK=0} cable_status; // (OUTPUT) cable status
}
~~~

# Description

The `ductbank` object models the temperature of an underground cable duct. Heat gain to duct originates with the losses on underground cables associated with the duct using the `ductbank` property of `underground_line` objects. The default ground temperature is specified using the module global `ground_temperature`. This value will be used if a duct bank object's ground temperature is not specified.

Cable temperatures are calculated based on the heat loss to the ground from the duct shell through airgap and the cable insulation.  Lineal R-values per meter of duct are either explicitly specified or calculated based on the configuration name, which is looked up in the `ductbank_configurations.csv` data file installed in the GridLAB-D `share` (as specified by the `GLD_ETC` environment variable). If a local copy of the file is found, that version is loaded instead.  The name of the configuration file may be specified using the `ductbank_configuration_file` module global.

Cable temperatures are compared to the module globals `warning_cable_temperature` and `alert_cable_temperature` and warning messages are displayed accordingly. In addition the variable `temperature_status` is updated.

# See also

* [[/Module/Powerflow/Underground_line]]