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
     enumeration {DEFAULT=1, SOLPOS=2} SOLAR_TILT_MODEL; 
     enumeration {DEFAULT=1, FLATPLATE=2} SOLAR_POWER_MODEL; 
     double a_coeff;
     double b_coeff;
     double dT_coeff;
     double T_coeff[%/degC];
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
     double soiling[pu];
     double derating[pu];
     double Rated_kVA[kVA];
     complex P_Out[kW];
     complex V_Out[V];
     complex I_Out[A];
     complex VA_Out[VA];
     object weather;
     double shading_factor[pu];
     double tilt_degree[deg];
     double orientation_azimuth[deg]; 
     bool latitude_angle_fix; 
     enumeration {DEFAULT=0,FIXED_AXIS=1,ONE_AXIS=2,TWO_AXIS=3,AZIMUTH_AXIS=4}orientation;  
     set {S=5, N=4, C=3, B=2, A=1} phases;
}
~~~

# Description

A solar panel (also known as solar module or photovoltaic module/panel) is an assembly of solar cells. Solar panels must be connected via a parent inverter.

## Properties

### generator_mode	

~~~
enumeration {SUPPLY_DRIVEN=5, CONSTANT_PF=4, CONSTANT_PQ=2, CONSTANT_V=1, UNKNOWN=0} generator_mode
~~~
Currently solar must operate in SUPPLY_DRIVEN.

### generator_status	

~~~
enumeration	{ONLINE=2, OFFLINE=1} generator_status
~~~

### panel_type	

~~~
enumeration    {CONCENTRATOR=5, THIN_FILM_GA_AS=4, AMORPHOUS_SILICON=3, MULTI_CRYSTAL_SILICON=2, SINGLE_CRYSTAL_SILICON=1} panel_type
~~~

Uses pre-defined panel technologies. Types supported: SINGLE_CRYSTAL_SILICON, MULTI_CRYSTAL_SILICON, AMORPHOUS_SILICON, THIN_FILM_GA_AS, CONCENTRATOR

### power_type	
~~~
enumeration	{DC=1, AC=2} power_type
~~~
Defines whether the connection is AC or DC. Currently not used.

### INSTALLATION_TYPE

~~~
enumeration	{GROUND_MOUNTED=2, ROOF_MOUNTED=1} INSTALLATION_TYPE
~~~
Currently not used.

### SOLAR_TILT_MODEL
~~~
enumeration	{DEFAULT=1, SOLPOS=2} SOLAR_TILT_MODEL
~~~
Defines the tilt model to utilize for tilted array calculations.

### SOLAR_POWER_MODEL
~~~
enumeration	{DEFAULT=1, FLATPLATE=2} SOLAR_POWER_MODEL
~~~
Defines if the PV array output efficiency should be adjusted for temperatures of the cells using a simple efficiency method, or the SAM simple flat plate efficiency model. 
### a_coeff
~~~
double    a_coeff
~~~
coefficient for temperature correction forumula.
### b_coeff
~~~
double    b_coeff
~~~
coefficient for temperature correction forumula.
### dT_coeff
~~~
double	dT_coeff[%/degC]	
~~~
Temperature difference coefficient for temperature correction forumula. 
### T_coeff
~~~
double	T_coeff	 
~~~
Maximum power temperature coefficient for temperature correction forumula.
### NOCT
~~~
double	NOCT[degF]	
~~~
Nominal operating cell temperature.
### Tmodule
~~~
double	Tmodule[degF]	
~~~
Calculated internal temperature of the PV module.
### Tambient
~~~
double	Tambient[degF]	
~~~
Outside air temperature.
### wind_speed	
~~~
double	wind_speed[mph]	
~~~
Outside wind speed. Currently not used.
### ambient_temperature
~~~
double	ambient_temperature[degF]	
~~~
Current ambient temperature of air.
### Insolation	
~~~
double    Insolation[W/sf]	
~~~
Solar radiation incident upon the solar panel.
### Rinternal
~~~
double	Rinternal[Ohm]	
~~~
Currently not used.
### Rated_Insolation
~~~
double	Rated_Insolation[W/sf]	
~~~
Insolation level that the cell is rated for.
### Pmax_temp_coeff
~~~
double    Pmax_temp_coeff	
~~~
Coefficient for the effects of temperature changes on the actual power output.
### Voc_temp_coeff
~~~
double	Voc_temp_coeff
~~~
Coefficient for the effects of temperature changes on the DC terminal voltage.
### V_Max	
~~~
complex   V_Max[V]	
~~~
Defines the maximum operating voltage of the PV module.
### Voc_Max
~~~
complex	Voc_Max[V]	
~~~
Voc max of the solar module.
### Voc
~~~
complex	Voc[V]	
~~~
Defines the open circuit voltage as specified by the PV manufacturer.
### efficiency
~~~
double	efficiency	
~~~
Defines the efficiency of power conversion from the solar insolation to DC power.
### area
~~~
double	area[sf]	
~~~
Defines the surface area of the solar module.
### soiling
~~~
double	soiling[pu]	
~~~
Soiling of the array factor - representing dirt on the array. 
### derating
~~~
double	derating[pu]	
~~~
Panel derating to account for manufacturing variances.
### Rated
~~~
double	Rated[kVA]	
~~~
Currently not used.
### P_Out
~~~
complex	P_Out[kW]	
~~~
Currently not used.
### V_Out
~~~
complex	V_Out[V]	
~~~
DC voltage passed to the inverter object.
### I_Out
~~~
complex	I_Out[A]	
~~~
DC current passed to the inverter object.
### VA_Out
~~~
complex	VA_Out[VA]	
~~~
Actual power delivered to the inverter.
### weather
~~~
object	weather	
~~~
Reference to a climate object from which temperature, humidity, and solar flux are collected.
### shading_factor
~~~
double	shading_factor[pu]	
~~~
Shading factor for scaling solar power to the array. 
### tilt_degree	
~~~
double    tilt_degree[deg]	
~~~
Tilt angle of PV array.
### orientation_azimuth
~~~
double	orientation_azimuth[deg]	
~~~
Facing direction of the PV array.  
### latitude_angle_fix
~~~
bool	latitude_angle_fix	
~~~
Fix tilt angle to installation latitude value (latitude comes from climate data). 
### orientation
~~~
enumeration	{DEFAULT=0,FIXED_AXIS=1,ONE_AXIS=2,TWO_AXIS=3,AZIMUTH_AXIS=4} orientation	
~~~
Type of panel orientation. Types DEFAULT and FIXED_AXIS are currently implemented. 
### phases
~~~
set	{S=5, N=4, C=3, B=2, A=1} phases
~~~
Currently not used.

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
