[[/Module/Residential/Apartment]] - Large multi-family residential building

# Synopsis

GLM:

~~~
object apartment 
{
	parent "<meter-name>";
	number_of_storeys "<floors>";
	circulation_indoor {TRUE,FALSE};
	circulation_elevator_count "<count>";
	central_plant_type {NONE,STEAM,HOTWATER,FORCEDAIR};
	central_cooling {TRUE,FALSE};
	parking_size "<total-parking>";
	parking_charger_count "<total-chargers>";
	parking_charger_capacity "<charger-power> kW";
	parking_charger_active "<active-chargers>";
	unit_count "<unit-count>";
	unit_size "<floor-area> sf";
	unit_perimeter_conductance "<conductance> Btu/degF/h";
	unit_thermal_mass "<thermal-mass> Btu/degF";
	unit_mass_conductance "<conductance> Btu/degF/h";
	unit_heating_type {NONE,BASEBOARD,RESISTIVE,HEATPUMP};
	unit_cooling_type {NONE,WINDOW_AC,UNITARY_AC,HEATPUMP};
	unit_appliances {NONE|DISHWASHER|REFRIGERATOR|CLOTHESWASHER|DRYER};
}
~~~

# Description

## Parameters

## Model

# See also