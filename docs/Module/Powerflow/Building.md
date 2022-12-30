[[/Module/Powerflow/Building]] - General building load model

# Synopsis

GLM:

~~~
module powerflow {
	char1024 building_defaults;
	char1024 building_loadshapes;
	char1024 building_occupancy;
}
class building {
    parent load;
    // model parameters
	double dt[s]; // timestep to use when modeling building response to inputs
	double TA[degC]; // (OUTPUT) indoor air temperature
	double TM[degC]; // (OUTPUT) building mass temperature
	double M[pu]; // (OUTPUT) system mode per unit system capacity
	double UA[W/K]; // (REQUIRED) conductance from interior air to outdoor air
	double CA[J/K]; // (REQUIRED) heat capacity of indoor air volume
	double UI[W/K]; // (REQUIRED) conductance from building mass to indoor air
	double CM[J/K]; // (REQUIRED) heat capacity of building mass
	double UM[W/K]; // (REQUIRED) conductance of building mass to outdoor air
	double TH[degC]; // (REQUIRED) heating design temperature
	double TC[degC]; // (REQUIRED) cooling design temperature
	double DF[pu]; // system over-design factor
	double QH[W]; // (REQUIRED,OUTPUT) HVAC system capacity
	double QE[W/m^2]; // (REQUIRED) nomimal enduse load capacity
	double QG[W/m^2]; // (REQUIRED) natural gas heat per unit nominal enduse capacity
	double QO[W/unit]; // (REQUIRED) heat gain per occupant
	double QV[W/unit]; // (REQUIRED) ventilation gain per occupant
	double SA[m^2]; // (REQUIRED) building mass area exposed to solar radiation
	double K[pu]; // HVAC mode proportional control gain w.r.t indoor temperature
	double TO[degC]; // outdoor air temperature
	double EU[unit]; // enduse load fraction
	double NG[unit]; // natural gas demand
	double NH[unit]; // building occupants
	double QS[W/m^2]; // insolation
	double TS[degC]; // (REQUIRED) thermostat setpoint
	double PZM[pu]; // (OUTPUT) constant impedance HVAC real power per unit system capacity
	double PPM[pu]; // (OUTPUT) constant power HVAC real power per unit system capacity
	double QPM[pu]; // (OUTPUT) constant power HVAC reactive power per unit system capacity
	double PZE[pu]; // (OUTPUT) constant impedance end-use real power fraction
	double PIE[pu]; // (OUTPUT) constant current end-use real power fraction
	double PPE[pu]; // (OUTPUT) constant power end-use real power fraction
	double QZE[pu]; // (OUTPUT) constant impedance end-use reactive fraction
	double QIE[pu]; // (OUTPUT) constant current end-use reactive fraction
	double QPE[pu]; // (OUTPUT) constant power end-use reactive fraction
	double PPH[pu]; // (OUTPUT) constant power ventilation real power per unit occupancy
	double QPH[pu]; // (OUTPUT) constant power ventilation reactive power per unit occupancy
	double measured_real_power[W]; // (OUTPUT) metered real power demand
	double measured_reactive_power[VAr]; // (OUTPUT) metered reactive power demand
	double measured_real_energy[Wh]; // (OUTPUT) cumulative metered real energy consumption
	double measured_real_energy_delta[Wh]; // (OUTPUT) metered real energy interval consumption
	double measured_reactive_energy[Wh]; // (OUTPUT) cumulative metered reactive energy consumption
	double measured_reactive_energy_delta[Wh]; // (OUTPUT) metered reactive energy interval consumption
	double measured_energy_delta_timestep[s]; // (OUTPUT) energy metering interval
	double measured_demand[W]; // (OUTPUT) maximum metered real power interval demand
	double measured_demand_timestep[s]; // (OUTPUT) maximum power metering interval
	char256 temperature_source; // temperature weather object source property
	char256 solar_source; // solar weather object source property
	char256 cooling_design; // cooling design temperature source property
	char256 heating_design; // heating design temperature source property
	char32 building_type; // building type used to be lookup defaults and enduse loadshapes
	double floor_area[m^2]; // (REQUIRED) building floor area
	double electric_gain_fraction[pu]; // (REQUIRED) fraction of electric end-use heat gain to the building
	double gas_gain_fraction[pu]; // (REQUIRED) fraction of gas end-use heat gain to the building
	double electrification_fraction[pu]; // (REQUIRED) fraction of gas enduses that are converted to electricity
	double electrification_efficiency[pu]; // (REQUIRED) performance of electric end-use relative to gas enduse
	int32 occupancy; // building occupancy
	bool electric_heat; // flag to specified whether heating is electric
}
~~~

# Description

The `building` object implements a general building load model as an integrated powerflow load object, meter, and thermal model.  The thermal model is based on a two-zone air/mass model with a general building HVAC system. The model computes the HVAC system duty-cycle required to maintain steady state given the system capacity.

## Model parameters

* `dt` - Simulation timestep stored in `s`. The default value is 1 hour.
* `UA` - Conductance from interior air to outdoor air in `W/K`. See [#Composition].
* `CA` - Heat capacity of indoor air volume in `J/K`. See [#Composition].
* `UI` - Conductance from building mass to indoor air in `W/K`. See [#Composition].
* `CM` - Heat capacity of building mass in `J/K`. See [#Composition].
* `UM` - Conductance of building mass to outdoor air in `W/K`. See [#Composition].

## Design parameters

* `building_type` - The building type used for occupancy, loadshape, and composition data lookups. May be omitted, in which case no lookups are performed.
* `TH` - Heating design temperature (degC). See [#Weather].
* `TC` - Cooling design temperature (degC). See [#Weather].
* `DF` - HVAC system over-design factor (pu). See [#Composition].
* `QH` - HVAC system capacity (W). See [#Autosizing].
* `QE` - Enduse loads (W). See [#Composition].
* `QG` - Gas load heat gains (W/kg). See [#Composition].
* `QO` - Occupancy heat gain in `W/person`.  See [#Occupancy].
* `QV` - Ventilation heat gain (W/person).. See [#Composition].
* `SA` - Mass area exposed to solar radiation (m^2). See [#Composition].
* `floor_area` - Building floor area (m^2).
* `electric_gain_fraction` - Fraction of electric end-use that go to building heat gains.
* `gas_gain_fraction` - Fraction of gas end-use that go to building heat gains.
* `electrification_fraction` - Fraction of gas end-use that are electrified.
* `electrification_efficiency` - Performance of electric end-use relative to gas enduse.
* `electric_heat` - Flag whether heating is from electric.
* `K` - Control feedback gain (only used for dynamic models). See [#Composition].

## State variables

* `TA` - Indoor air temperature in `degC`.
* `TM` - Building mass temperature in `degC`.
* `M` - System mode in `pu` of system capacity `QH`. Note that $M<0$ when cooling and $0 \le |M| \le 1$.

## Inputs

* `TO` - Outdoor air temperature in `degC`. See [#Weather].
* `EU` - Electric end-use power density in `W/m^2`. See [#Loadshape].
* `NG` - Fraction end-use power density in `W/m^2`. See [#Loadshape].
* `NH` - Fraction of nominal occupancy in `pu`.
* `QS` - Solar irradiance `W/m^2`. See [#Weather].
* `TS` - Indoor temperature setpoint `degC`.
* `occupancy` - Nominal building occupancy.
* `temperature_source` - Source for outdoor air temperature. See [#Weather].
* `solar_source` - Source for solar irradiance. See [#Weather].
* `cooling_design` - Source for cooling design temperature. See [#Weather].
* `heading_design` - Source for heating design temperature. See [#Weather].

## Outputs

* `PZM` - constant impedance HVAC real power (W). See [#Composition].
* `PPM` - constant power HVAC real power (W). See [#Composition].
* `QPM` - constant power HVAC reactive power (VAr). See [#Composition].
* `PZE` - constant impedance end-use real power (W). See [#Composition].
* `PIE` - constant current end-use real power (W). See [#Composition].
* `PPE` - constant power end-use real power (W). See [#Composition].
* `QZE` - constant impedance end-use reactive power (VAr). See [#Composition].
* `QIE` - constant current end-use reactive power (VAr). See [#Composition].
* `QPE` - constant power end-use reactive power (VAr). See [#Composition].
* `PPH` - constant power ventilation real power (W/person). See [#Composition].
* `QPH` - constant power ventilation reactive power (VAr/person). See [#Composition].

## Meter

* `measured_real_power` - Real power mearurement at the meter in `W`.
* `measured_reactive_power` - Reactive power measurement at the meter in `VAr`.
* `measured_real_energy` - Real energy measurement at the meter in `Wh`.
* `measured_real_energy_delta` - Real energy delta measurement at the meter in `Wh`.
* `measured_reactive_energy` - Reactive energy measurement at the meter in `VAhr`.
* `measured_reactive_energy_delta` - Reactive energy delta measurement at the meter in `VAhr`.
* `measured_energy_delta_timestep` - Delta measurement interval timestep in `s`.
* `measured_demand` - Peak power measurement in last demand interval in `W`.
* `measured_demand_timestep` - Peak power measurement demand interval in `s`.

# Weather

Any object can be used as a weather source by specifying the `temperature_source`, `source_source`, `heating_design`, and `cooling_design` properties.  For example, using a TMY3 input file you can specify the climate object's `temperature`, `solar_global`, `record.low`, and `record.high` property, i.e.,

~~~
#weather get CA-San_Francisco_Intl_Ap.tmy3
#input "CA-San_Francisco_Intl_Ap.tmy3" --property=name=weather
object building
{
	temperature_source "weather.temperature";
	solar_source "weather.solar_global";
	heating_design "weather.record.low";
	cooling_design "weather.record.high";
    // ...
}
~~~

# Occupancy

Building occupancy data is provided by the `building_occupancy.csv` data file.  You may specify your own occupancy data by setting the module global `building_occupancy` to your file's pathname and specifying the `building_type` property in the building object.  The file must be formatted similarly to the default file:

| building_type | daytype | start | stop | occupied | unoccupied |
| ------------- | ------- | ----- | ---- | -------- | ---------- |
| APARTMENT | WEEKDAY | 8 | 18 | 0.2 | 1.0 |
| APARTMENT | WEEKEND | 0 | 24 | 1.0 | 0.0 |
| CONDO | WEEKDAY | 8 | 18 | 0.2 | 1.0 |
| CONDO | WEEKEND | 0 | 24 | 1.0 | 0.0 |
| EDUCATION | WEEKDAY | 7 | 16 | 1.0 | 0.0 |
| EDUCATION | WEEKEND | 0 | 24 | 0.0 | 0.0 |
| GROCERY | WEEKDAY | 8 | 20 | 1.0 | 0.0 |
| GROCERY | WEEKEND | 8 | 20 | 1.0 | 0.0 |
| HOUSE | WEEKDAY | 8 | 18 | 0.2 | 1.0 |
| HOUSE | WEEKEND | 0 | 24 | 1.0 | 0.0 |
| HEALTHCARE | WEEKDAY | 6 | 22 | 1.0 | 0.5 |
| HEALTHCARE | WEEKEND | 6 | 22 | 1.0 | 0.5 |
| LARGEOFFICE | WEEKDAY | 9 | 17 | 1.0 | 0.0 |
| LARGEOFFICE | WEEKEND | 0 | 24 | 0.1 | 0.0 |
| LODGING | WEEKDAY | 6 | 8 | 1.0 | 0.5 |
| LODGING | WEEKEND | 5 | 9 | 1.0 | 0.5 |
| RESTAURANT | WEEKDAY | 10 | 20 | 1.0 | 0.0 |
| RESTAURANT | WEEKEND | 10 | 20 | 1.0 | 0.0 |
| RETAIL | WEEKDAY | 10 | 18 | 1.0 | 0.0 |
| RETAIL | WEEKEND | 10 | 18 | 1.0 | 0.0 |
| SMALLOFFICE | WEEKDAY | 9 | 17 | 1.0 | 0.0 |
| SMALLOFFICE | WEEKEND | 0 | 24 | 0.1 | 0.0 |
| TOWNHOUSE | WEEKDAY | 8 | 18 | 0.2 | 1.0 |
| TOWNHOUSE | WEEKEND | 0 | 24 | 1.0 | 0.0 |

The `building_type` may be set to any building type you wish to define.  The `daytype` must be either `WEEKDAY` or `WEEKEND`. The `start` and `stop` hours must be in the range $(0-23)$ and `start` must be less than `stop`.  The `occupied` values are used when the hour is between `start` inclusive and `stop` exclusive. The `unoccupied` values are used otherwise.  If you need to define night-time occupancy, keep in the hours in the proper order and exchange the occupancy values.

# Loadshape

Building end-use load shapes are provided by the `building_loadshapes.csv` data file.  You may specify your own end-use load shape data by setting the module global `building_loadshapes` to your file's pathname and specifying the `building_type` property in the building object.  The file must be formatted similarly to the default file:

| building_type | season | fuel | daytype | hour | load |
| ------------- | ------ | ---- | ------- | ---- | ---- |
| EDUCATION | WINTER | ELECTRIC | WEEKEND | 0 | 0.4969 |
| EDUCATION | WINTER | ELECTRIC | WEEKEND | 1 | 0.5004 |
| EDUCATION | WINTER | ELECTRIC | WEEKEND | 2 | 0.504 |

The `building_type` may be set to any building type you wish to define.  The season must be one of `WINTER`, `SPRING`, `SUMMER`, or `FALL`. The `fuel` type must be either `ELECTRIC` or `GAS`. The `daytype` must be either `WEEKDAY` or `WEEKEND`. The `hour` must be in the range $(0,23)$. The `load` is specified in `W/m^2` of floor area, and must be non-negative.

# Composition

Building design and load compositions can be used using the building type. Design and composition values are looked up according to the `building_type` property when it is set. The building composition file is specified by the module global `building_defaults`. You may specify your own building design defaults by setting the `building_defaults` module global to your file's pathname.  The file must be formatted similarly to the default file:

| building_type | UA | CA | UI | CM | UM | DF | K | QE | QG | QO | QV | SA | TS | PZM | PPM | QPM | PZE | PIE | PPE | QZE | QIE | QPE | PPH | QPH |
| ------------- | -- | -- | -- | -- | -- | -- | - | -- | -- | -- | -- | -- | -- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | 
| APARTMENT | 300.0 | 2E+06 | 5000.0 | 8E+06 | 600.0 | 2.0 | 1.0 | 1E+04 | 1E+03 | 1.2E+03 | 400.0 | 15.0 | 22.0 | 0.0 | 0.3 | 0.03 | 0.5 | 0.0 | 0.5 | 0.05 | 0.0 | 0.05 | 0.06 | 0.01 |
| CONDO | 300.0 | 2E+06 | 5000.0 | 8E+06 | 600.0 | 2.0 | 1.0 | 1E+04 | 1E+03 | 1.2E+03 | 400.0 | 15.0 | 22.0 | 0.0 | 0.3 | 0.03 | 0.5 | 0.0 | 0.5 | 0.05 | 0.0 | 0.05 | 0.06 | 0.01 |
| EDUCATION | 300.0 | 2E+06 | 5000.0 | 8E+06 | 600.0 | 2.0 | 1.0 | 1E+04 | 1E+03 | 1.2E+03 | 400.0 | 15.0 | 22.0 | 0.0 | 0.3 | 0.03 | 0.5 | 0.0 | 0.5 | 0.05 | 0.0 | 0.05 | 0.06 | 0.01 |
| GROCERY | 300.0 | 2E+06 | 5000.0 | 8E+06 | 600.0 | 2.0 | 1.0 | 1E+04 | 1E+03 | 1.2E+03 | 400.0 | 15.0 | 22.0 | 0.0 | 0.3 | 0.03 | 0.5 | 0.0 | 0.5 | 0.05 | 0.0 | 0.05 | 0.06 | 0.01 |
| HOUSE | 300.0 | 2E+06 | 5000.0 | 8E+06 | 600.0 | 2.0 | 1.0 | 1E+04 | 1E+03 | 1.2E+03 | 400.0 | 15.0 | 22.0 | 0.0 | 0.3 | 0.03 | 0.5 | 0.0 | 0.5 | 0.05 | 0.0 | 0.05 | 0.06 | 0.01 |
| HEALTHCARE | 300.0 | 2E+06 | 5000.0 | 8E+06 | 600.0 | 2.0 | 1.0 | 1E+04 | 1E+03 | 1.2E+03 | 400.0 | 15.0 | 22.0 | 0.0 | 0.3 | 0.03 | 0.5 | 0.0 | 0.5 | 0.05 | 0.0 | 0.05 | 0.06 | 0.01 |
| LARGEOFFICE | 300.0 | 2E+06 | 5000.0 | 8E+06 | 600.0 | 2.0 | 1.0 | 1E+04 | 1E+03 | 1.2E+03 | 400.0 | 15.0 | 22.0 | 0.0 | 0.3 | 0.03 | 0.5 | 0.0 | 0.5 | 0.05 | 0.0 | 0.05 | 0.06 | 0.01 |
| LODGING | 300.0 | 2E+06 | 5000.0 | 8E+06 | 600.0 | 2.0 | 1.0 | 1E+04 | 1E+03 | 1.2E+03 | 400.0 | 15.0 | 22.0 | 0.0 | 0.3 | 0.03 | 0.5 | 0.0 | 0.5 | 0.05 | 0.0 | 0.05 | 0.06 | 0.01 |
| RESTAURANT | 300.0 | 2E+06 | 5000.0 | 8E+06 | 600.0 | 2.0 | 1.0 | 1E+04 | 1E+03 | 1.2E+03 | 400.0 | 15.0 | 22.0 | 0.0 | 0.3 | 0.03 | 0.5 | 0.0 | 0.5 | 0.05 | 0.0 | 0.05 | 0.06 | 0.01 |
| RETAIL | 300.0 | 2E+06 | 5000.0 | 8E+06 | 600.0 | 2.0 | 1.0 | 1E+04 | 1E+03 | 1.2E+03 | 400.0 | 15.0 | 22.0 | 0.0 | 0.3 | 0.03 | 0.5 | 0.0 | 0.5 | 0.05 | 0.0 | 0.05 | 0.06 | 0.01 |
| SMALLOFFICE | 300.0 | 2E+06 | 5000.0 | 8E+06 | 600.0 | 2.0 | 1.0 | 1E+04 | 1E+03 | 1.2E+03 | 400.0 | 15.0 | 22.0 | 0.0 | 0.3 | 0.03 | 0.5 | 0.0 | 0.5 | 0.05 | 0.0 | 0.05 | 0.06 | 0.01 |
| TOWNHOUSE | 300.0 | 2E+06 | 5000.0 | 8E+06 | 600.0 | 2.0 | 1.0 | 1E+04 | 1E+03 | 1.2E+03 | 400.0 | 15.0 | 22.0 | 0.0 | 0.3 | 0.03 | 0.5 | 0.0 | 0.5 | 0.05 | 0.0 | 0.05 | 0.06 | 0.01 |

The `building_type` may be set to any building type you wish to define.  The remaining values must be set in the units used by the `building`'s properties. 

# See also

* [[/Module/Powerflow/Load]]
