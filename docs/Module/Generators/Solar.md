[[/Module/Generators/Solar]] -- Solar panel

# Synopsis

~~~
module generators;
class solar {
     enumeration {SUPPLY_DRIVEN=5, CONSTANT_PF=4, CONSTANT_PQ=2, CONSTANT_V=1, UNKNOWN=0} generator_mode;
     enumeration {ONLINE=2, OFFLINE=1} generator_status;
     enumeration {CONCENTRATOR=5, THIN_FILM_GA_AS=4, AMORPHOUS_SILICON=3, MULTI_CRYSTAL_SILICON=2, SINGLE_CRYSTAL_SILICON=1} panel_type;
     enumeration {DC=1, AC=2} power_type;
     enumeration {GROUND_MOUNTED=2, ROOF_MOUNTED=1} INSTALLATION_TYPE;
     enumeration {DEFAULT=1, SOLPOS=2} SOLAR_TILT_MODEL; New in 2.3! 
     enumeration {DEFAULT=1, FLATPLATE=2} SOLAR_POWER_MODEL; New in 2.3! 
     double a_coeff; New in 2.3! 
     double b_coeff; New in 2.3! 
     double dT_coeff; New in 2.3! 
     double T_coeff[%/degC]; New in 2.3! 
     double NOCT[degF];
     double Tmodule[degF];
     double Tambient[degF];
     double wind_speed[mph];
     double ambient_temeprature[degF];
     double Insolation[W/sf];
     double Rinternal[Ohm];
     double Rated_Insolation[W/sf];
     double Pmax_temp_coeff;
     double Voc_temp_coeff;
     complex V_Max[V];
     complex Voc_Max[V];
     complex Voc[V];
     double efficiency[unit];
     double area[sf];
     double soiling[pu]; New in 2.3! 
     double derating[pu]; New in 2.3! 
     double Rated_kVA[kVA];
     complex P_Out[kW];
     complex V_Out[V];
     complex I_Out[A];
     complex VA_Out[VA];
     object weather;
     double shading_factor[pu];  New in 2.3! 
     double tilt_angle[deg]; New in 2.3! 
     double orientation_azimuth[deg]; New in 2.3! 
     bool latitude_angle_fix; New in 2.3! 
     enumeration {DEFAULT=0,FIXED_AXIS=1,ONE_AXIS=2,TWO_AXIS=3,AZIMUTH_AXIS=4}orientation; New in 2.3! 
     set {S=5, N=4, C=3, B=2, A=1} phases;
}
~~~

# Description

A solar panel (also known as solar module or photovoltaic module/panel) is an assembly of solar cells. Solar panels must be connected via a parent inverter.

## Properties

### generator_mode	

~~~
enumeration (UNKNOWN,CONSTANT_V,CONSTANT_PQ,CONSTANT_PF,SUPPLY_DRIVEN) generator_mode
~~~
Currently solar must operate in SUPPLY_DRIVEN.

### generator_status	

~~~
enumeration	(ONLINE, OFFLINE) generator_status
~~~

### panel_type	

~~~
enumeration (SINGLE_CRYSTAL_SILICON, MULTI_CRYSTAL_SILICON, AMORPHOUS_SILICON, THIN_FILM_GA_AS, CONCENTRATOR) panel_type
~~~

Uses pre-defined panel technologies.(SINGLE_CRYSTAL_SILICON, MULTI_CRYSTAL_SILICON, AMORPHOUS_SILICON, THIN_FILM_GA_AS, CONCENTRATOR)

### power_type	
~~~
enumeration	(AC, DC) power_type
~~~
Defines whether the connection is AC or DC. Currently not used.


INSTALLATION_TYPE	enumeration	none	(ROOF_MOUNTED, GROUND_MOUNTED) Currently not used.
SOLAR_TILT_MODEL	enumeration	none	(DEFAULT, SOLPOS) Defines the tilt model to utilize for tilted array calculations.
SOLAR_POWER_MODEL	enumeration	none	(DEFAULT, FLATPLATE) Defines if the PV array output efficiency should be adjusted for temperatures of the cells using a simple efficiency method, or the SAM simple flat plate efficiency model. New in 2.3! 
a_coeff	double	none	a coefficient for temperature correction forumula New in 2.3! 
b_coeff	double	none	b coefficient for temperature correction forumula New in 2.3! 
dT_coeff	double	%/degC	Temperature difference coefficient for temperature correction forumula New in 2.3! 
T_coeff	double	none	Maximum power temperature coefficient for temperature correction forumula New in 2.3! 
NOCT	double	degF	Nominal operating cell temperature.
Tmodule	double	degF	Calculated internal temperature of the PV module.
Tambient	double	degF	Outside air temperature.
wind_speed	double	mph	Outside wind speed. Currently not used.
ambient_temperature	double	degF	Current ambient temperature of air
Insolation	double	W/sf	Solar radiation incident upon the solar panel.
Rinternal	double	Ohm	Currently not used.
Rated_Insolation	double	W/sf	Insolation level that the cell is rated for.
Pmax_temp_coeff	double		Coefficient for the effects of temperature changes on the actual power output.
Voc_temp_coeff	double		Coefficient for the effects of temperature changes on the DC terminal voltage.
V_Max	complex	V	Defines the maximum operating voltage of the PV module.
Voc_Max	complex	V	Voc max of the solar module
Voc	complex	V	Defines the open circuit voltage as specified by the PV manufacturer.
efficiency	double	unit	Defines the efficiency of power conversion from the solar insolation to DC power.
area	double	sf	Defines the surface area of the solar module.
soiling	double	pu	Soiling of the array factor - representing dirt on the array.  New in 2.3! 
derating	double	pu	Panel derating to account for manufacturing variances.  New in 2.3! 
Rated kVA	double	kVA	Currently not used.
P_Out	complex	kW	Currently not used.
V_Out	complex	V	DC voltage passed to the inverter object
I_Out	complex	A	DC current passed to the inverter object
VA_Out	complex	VA	Actual power delivered to the inverter
weather	object	n/a	Reference to a climate object from which temperature, humidity, and solar flux are collected
shading_factor	double	pu	Shading factor for scaling solar power to the array  New in 2.3! 
tilt_angle	double	deg	Tilt angle of PV array  New in 2.3! 
orientation_azimuth	double	deg	Facing direction of the PV array  New in 2.3! 
latitude_angle_fix	bool	n/a	Fix tilt angle to installation latitude value (latitude comes from climate data) New in 2.3! 
orientation	enumeration	n/a	Type of panel orientation. Types DEFAULT and FIXED_AXIS are currently implemented  New in 2.3! 
phases	set	n/a	(A,B,C,N,S) Currently not used.

# Example

A minimal model could be created via:

object solar {
    generator_mode SUPPLY_DRIVEN;
    generator_status ONLINE;
    panel_type SINGLE_CRYSTAL_SILICON;
    efficiency 0.2;
    parent inverter1;
    area 2500;
}

# See also

* [[/Module/Generators]]
* [[/Module/Generators/Inverter]]
