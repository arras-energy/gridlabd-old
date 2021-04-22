# Model documentation of gldcore/converters/autotest/test_markdown.json

| Application | Version |
| ----------- | ------- |
| gridlabd | 4.2.13 |

## Modules

| Name | Version |
| ---- | ------- |
| assert | 4.2 |
| climate | 4.2 |
| commercial | 4.2 |
| generators | 4.2 |
| industrial | 4.2 |
| influxdb | 4.2 |
| market | 4.2 |
| optimize | 4.2 |
| powerflow | 4.2 |
| reliability | 4.2 |
| residential | 4.2 |
| revenue | 4.2 |
| tape | 4.2 |

## Classes

### assert

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| status | enumeration |  | PUBLIC |  | ERROR FALSE TRUE INIT NONE | TRUE | desired outcome of assert test |
| target | char1024 |  | PUBLIC | REQUIRED |  |  | the target property to test |
| part | char32 |  | PUBLIC |  |  |  | the target property part to test |
| relation | enumeration |  | PUBLIC | REQUIRED | outside inside != >= > <= < == |  | the relation to use for the test |
| value | char1024 |  | PUBLIC | REQUIRED |  |  | the value to compare with for binary tests |
| within | char1024 |  | PUBLIC |  |  |  | the bounds within which the value must bed compared |
| lower | char1024 |  | PUBLIC |  |  |  | the lower bound to compare with for interval tests |
| upper | char1024 |  | PUBLIC |  |  |  | the upper bound to compare with for interval tests |
| group | char1024 |  | PUBLIC |  |  |  | a target group specification to use instead of parent object |
| start | timestamp |  | PUBLIC |  |  | INIT | time to start assertion |
| stop | timestamp |  | PUBLIC |  |  | NEVER | time to stop assertion |
| hold | double | s | PUBLIC |  |  |  | a duration over which the assertion must be violated before failing |

### double_assert

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| status | enumeration |  | PUBLIC |  | ASSERT_NONE ASSERT_FALSE ASSERT_TRUE | ASSERT_TRUE | Conditions for the assert checks |
| once | enumeration |  | PUBLIC |  | ONCE_DONE ONCE_TRUE ONCE_FALSE |  | Conditions for a single assert check |
| within_mode | enumeration |  | PUBLIC |  | WITHIN_RATIO WITHIN_VALUE |  | Method of applying tolerance |
| value | double |  | PUBLIC |  |  |  | Value to assert |
| within | double |  | PUBLIC |  |  |  | Tolerance for a successful assert |
| target | char1024 |  | PUBLIC |  |  |  | Property to perform the assert upon |

### complex_assert

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| status | enumeration |  | PUBLIC |  | ASSERT_NONE ASSERT_FALSE ASSERT_TRUE | ASSERT_TRUE | Conditions for the assert checks |
| once | enumeration |  | PUBLIC |  | ONCE_DONE ONCE_TRUE ONCE_FALSE |  | Conditions for a single assert check |
| operation | enumeration |  | PUBLIC |  | ANGLE MAGNITUDE IMAGINARY REAL FULL |  | Complex operation for the comparison |
| value | complex |  | PUBLIC |  |  |  | Value to assert |
| within | double |  | PUBLIC |  |  |  | Tolerance for a successful assert |
| target | char1024 |  | PUBLIC |  |  |  | Property to perform the assert upon |

### enum_assert

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| status | enumeration |  | PUBLIC |  | ASSERT_NONE ASSERT_FALSE ASSERT_TRUE | ASSERT_TRUE | Conditions for the assert checks |
| value | int32 |  | PUBLIC |  |  |  | Value to assert |
| target | char1024 |  | PUBLIC |  |  |  | Property to perform the assert upon |

### int_assert

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| status | enumeration |  | PUBLIC |  | ASSERT_NONE ASSERT_FALSE ASSERT_TRUE | ASSERT_TRUE |  |
| once | enumeration |  | PUBLIC |  | ONCE_DONE ONCE_TRUE ONCE_FALSE |  |  |
| within_mode | enumeration |  | PUBLIC |  | WITHIN_RATIO WITHIN_VALUE |  |  |
| value | int32 |  | PUBLIC |  |  |  |  |
| within | int32 |  | PUBLIC |  |  |  |  |
| target | char1024 |  | PUBLIC |  |  |  |  |

### climate

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| solar_elevation | double | rad | PUBLIC | OUTPUT |  |  | solar elevation angle in radians |
| solar_azimuth | double | rad | PUBLIC | OUTPUT |  |  | solar azimuth angle in radians |
| solar_zenith | double | rad | PUBLIC | OUTPUT |  |  | solar zenith angle in radians |
| city | char32 |  | PUBLIC |  |  |  | weather data city name |
| tmyfile | char1024 |  | PUBLIC | REQUIRED |  |  | weather data file name |
| temperature | double | degF | PUBLIC | OUTPUT |  | 59.0 degF | current temperature |
| humidity | double | pu | PUBLIC | OUTPUT |  | 75% | current humidity |
| solar_flux | double | W/sf | PUBLIC | OUTPUT |  |  | current solar irradiance (9 orientiations) |
| solar_direct | double | W/sf | PUBLIC | OUTPUT |  |  | solar direct irradiance |
| solar_diffuse | double | W/sf | PUBLIC | OUTPUT |  |  | solar diffuse irradiance |
| solar_global | double | W/sf | PUBLIC | OUTPUT |  |  | solar global flux irradiance |
| extraterrestrial_global_horizontal | double | W/sf | PUBLIC | OUTPUT |  |  | solar global extraterrestrial irradiance |
| extraterrestrial_direct_normal | double | W/sf | PUBLIC | OUTPUT |  | 1367 W/m^2 | solar direct normal extraterrestrial irradiance |
| pressure | double | mbar | PUBLIC | OUTPUT |  | 1013.25 mbar | current air pressure |
| wind_speed | double | m/s | PUBLIC | OUTPUT |  |  | current wind speed |
| wind_dir | double | rad | PUBLIC | OUTPUT |  |  | current direction in radians |
| wind_gust | double | m/s | PUBLIC | OUTPUT |  |  | current wind gusts |
| record.low | double | degF | PUBLIC | OUTPUT |  |  | record low temperature observed |
| record.low_day | int32 |  | PUBLIC | OUTPUT |  |  | day of year for record low observation |
| record.high | double | degF | PUBLIC | OUTPUT |  |  | record high temperature observed |
| record.high_day | int32 |  | PUBLIC | OUTPUT |  |  | day of year for record high observation |
| record.solar | double | W/sf | PUBLIC | OUTPUT |  |  | record high solar irradiance observed |
| rainfall | double | in/h | PUBLIC | OUTPUT |  |  | rainfall observed |
| snowdepth | double | in | PUBLIC | OUTPUT |  |  | snow depth observed |
| interpolate | enumeration |  | PUBLIC | OUTPUT | QUADRATIC LINEAR NONE | NONE | the interpolation mode used on the climate data |
| solar_horiz | double |  | PUBLIC | OUTPUT |  |  | solar flux on horizontal surface |
| solar_north | double |  | PUBLIC | OUTPUT |  |  | solar flux on north surface |
| solar_northeast | double |  | PUBLIC | OUTPUT |  |  | solar flux on northeast surface |
| solar_east | double |  | PUBLIC | OUTPUT |  |  | solar flux on east surface |
| solar_southeast | double |  | PUBLIC | OUTPUT |  |  | solar flux on southeast surface |
| solar_south | double |  | PUBLIC | OUTPUT |  |  | solar flux on south surface |
| solar_southwest | double |  | PUBLIC | OUTPUT |  |  | solar flux on southwest surface |
| solar_west | double |  | PUBLIC | OUTPUT |  |  | solar flux on west surface |
| solar_northwest | double |  | PUBLIC | OUTPUT |  |  | solar flux on northwest surface |
| solar_raw | double | W/sf | PUBLIC | OUTPUT |  |  | raw solar flux |
| ground_reflectivity | double | pu | PUBLIC | OUTPUT |  | 0.3 pu | ground reflectivity observed |
| reader | object |  | PUBLIC |  |  |  | weather reader object |
| forecast | char1024 |  | PUBLIC |  |  |  | forecasting specifications |
| cloud_model | enumeration |  | PUBLIC | OUTPUT | CUMULUS NONE | NONE | the cloud model to use |
| cloud_opacity | double | pu | PUBLIC | OUTPUT |  | 1.0 pu | cloud opacity factor |
| opq_sky_cov | double | pu | PUBLIC | OUTPUT |  |  | cloud sky coverage factor |
| cloud_speed_factor | double | pu | PUBLIC | OUTPUT |  | 1.0 pu | cloud speed factor |
| solar_cloud_direct | double | W/sf | PUBLIC | OUTPUT |  |  | cloud direct irradiance |
| solar_cloud_diffuse | double | W/sf | PUBLIC | OUTPUT |  |  | cloud diffuse irradiance |
| solar_cloud_global | double | W/sf | PUBLIC | OUTPUT |  |  | cloud global irradiance |
| cloud_alpha | double | pu | PUBLIC | OUTPUT |  | 400 pu | cloud alpha |
| cloud_num_layers | double | pu | PUBLIC | OUTPUT |  | 40 pu | number of cloud layers |
| cloud_aerosol_transmissivity | double | pu | PUBLIC | OUTPUT |  | 0.95 pu | cloud aerosal transmissivity |
| heat_index | double | degF | PUBLIC | OUTPUT |  |  | heat index based on temperature and humidity |

### weather

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| temperature | double | degF | PUBLIC | OUTPUT |  | 0.0 degF | dry-bulb air temperature |
| humidity | double | % | PUBLIC | OUTPUT |  | 0% | relative humidity |
| solar_dir | double | W/sf | PUBLIC | DEPRECATED|OUTPUT |  | 0.0 W/sf | direct solar flux |
| solar_direct | double | W/sf | PUBLIC |  |  | 0.0 W/sf | direct solar flux |
| solar_diff | double | W/sf | PUBLIC | DEPRECATED|OUTPUT |  | 0.0 W/sf | diffuse solar flux |
| solar_diffuse | double | W/sf | PUBLIC | OUTPUT |  | 0.0 W/sf | diffuse solar flux |
| solar_global | double | W/sf | PUBLIC | OUTPUT |  | 0.0 W/sf | global solar flux |
| global_horizontal_extra | double | W/sf | PUBLIC | OUTPUT |  | 0.0 W/sf | horizontal surface solar flux |
| wind_speed | double | mph | PUBLIC | OUTPUT |  | 0.0 mph | wind speed |
| wind_dir | double | deg | PUBLIC | OUTPUT |  | 0.0 deg | wind direction |
| opq_sky_cov | double | pu | PUBLIC | OUTPUT |  | 0.0 pu | opaque sky cover |
| tot_sky_cov | double | pu | PUBLIC | OUTPUT |  | 0.0 pu | total sky cover |
| rainfall | double | in/h | PUBLIC | OUTPUT |  | 0.0 in/h | rainfall rate |
| snowdepth | double | in | PUBLIC | OUTPUT |  | 0.0 in | depth of snow on the ground |
| pressure | double | mbar | PUBLIC | OUTPUT |  | 0.0 mbar | air pressure |
| month | int16 |  | PUBLIC | OUTPUT |  |  | current month |
| day | int16 |  | PUBLIC | OUTPUT |  |  | current day |
| hour | int16 |  | PUBLIC | OUTPUT |  |  | current hour |
| minute | int16 |  | PUBLIC | OUTPUT |  |  | current minute |
| second | int16 |  | PUBLIC | OUTPUT |  |  | current second |

### csv_reader

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| index | int32 |  | REFERENCE |  |  |  | reader line index |
| city_name | char32 |  | PUBLIC |  |  |  | name of city |
| state_name | char32 |  | PUBLIC |  |  |  | name of state/province |
| lat_deg | double |  | PUBLIC |  |  |  | latitude degrees |
| lat_min | double |  | PUBLIC |  |  |  | latitude minutes |
| long_deg | double |  | PUBLIC |  |  |  | longitude degrees |
| long_min | double |  | PUBLIC |  |  |  | longitude minutes |
| low_temp | double |  | REFERENCE |  |  |  | lowest temperature |
| high_temp | double |  | REFERENCE |  |  |  | highest temperature |
| peak_solar | double |  | REFERENCE |  |  |  | peak solar flux |
| elevation | int32 |  | PUBLIC |  |  |  | elevation |
| status | enumeration |  | REFERENCE |  | ERROR OPEN INIT |  | reader status |
| timefmt | char32 |  | PUBLIC |  |  |  | time format |
| timezone | char32 |  | PUBLIC |  |  |  | timezone |
| timezone_offset | double |  | PUBLIC |  |  |  | timezone offset |
| columns | char256 |  | PUBLIC |  |  |  | column names |
| filename | char256 |  | PUBLIC |  |  |  | filename |

### office

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| floor_area | double | sf | PUBLIC |  |  |  |  |
| floor_height | double | ft | PUBLIC |  |  |  |  |
| exterior_ua | double | Btu/degF/h | PUBLIC |  |  |  |  |
| interior_ua | double | Btu/degF/h | PUBLIC |  |  |  |  |
| interior_mass | double | Btu/degF | PUBLIC |  |  |  |  |
| glazing | double | sf | PUBLIC |  |  |  |  |
| glazing.north | double | sf | PUBLIC |  |  |  |  |
| glazing.northeast | double | sf | PUBLIC |  |  |  |  |
| glazing.east | double | sf | PUBLIC |  |  |  |  |
| glazing.southeast | double | sf | PUBLIC |  |  |  |  |
| glazing.south | double | sf | PUBLIC |  |  |  |  |
| glazing.southwest | double | sf | PUBLIC |  |  |  |  |
| glazing.west | double | sf | PUBLIC |  |  |  |  |
| glazing.northwest | double | sf | PUBLIC |  |  |  |  |
| glazing.horizontal | double | sf | PUBLIC |  |  |  |  |
| glazing.coefficient | double | pu | PUBLIC |  |  |  |  |
| occupancy | double |  | PUBLIC |  |  |  |  |
| occupants | double |  | PUBLIC |  |  |  |  |
| schedule | char256 |  | PUBLIC |  |  |  |  |
| air_temperature | double | degF | PUBLIC |  |  |  |  |
| mass_temperature | double | degF | PUBLIC |  |  |  |  |
| temperature_change | double | degF/h | PUBLIC |  |  |  |  |
| outdoor_temperature | double | degF | PUBLIC |  |  |  |  |
| Qh | double | Btu/h | PUBLIC |  |  |  |  |
| Qs | double | Btu/h | PUBLIC |  |  |  |  |
| Qi | double | Btu/h | PUBLIC |  |  |  |  |
| Qz | double | Btu/h | PUBLIC |  |  |  |  |
| hvac_mode | enumeration |  | PUBLIC |  | OFF VENT ECON COOL AUX HEAT |  |  |
| hvac.cooling.balance_temperature | double | degF | PUBLIC |  |  |  |  |
| hvac.cooling.capacity | double | Btu/h | PUBLIC |  |  |  |  |
| hvac.cooling.capacity_perF | double | Btu/degF/h | PUBLIC |  |  |  |  |
| hvac.cooling.design_temperature | double | degF | PUBLIC |  |  |  |  |
| hvac.cooling.efficiency | double | pu | PUBLIC |  |  |  |  |
| hvac.cooling.cop | double | pu | PUBLIC |  |  |  |  |
| hvac.heating.balance_temperature | double | degF | PUBLIC |  |  |  |  |
| hvac.heating.capacity | double | Btu/h | PUBLIC |  |  |  |  |
| hvac.heating.capacity_perF | double | Btu/degF/h | PUBLIC |  |  |  |  |
| hvac.heating.design_temperature | double | degF | PUBLIC |  |  |  |  |
| hvac.heating.efficiency | double | pu | PUBLIC |  |  |  |  |
| hvac.heating.cop | double | pu | PUBLIC |  |  |  |  |
| lights.capacity | double | kW | PUBLIC |  |  |  |  |
| lights.fraction | double | pu | PUBLIC |  |  |  |  |
| plugs.capacity | double | kW | PUBLIC |  |  |  |  |
| plugs.fraction | double | pu | PUBLIC |  |  |  |  |
| demand | complex | kW | PUBLIC |  |  |  |  |
| total_load | complex | kW | PUBLIC |  |  |  |  |
| energy | complex | kWh | PUBLIC |  |  |  |  |
| power_factor | double |  | PUBLIC |  |  |  |  |
| power | complex | kW | PUBLIC |  |  |  |  |
| current | complex | A | PUBLIC |  |  |  |  |
| admittance | complex | 1/Ohm | PUBLIC |  |  |  |  |
| hvac.demand | complex | kW | PUBLIC |  |  |  |  |
| hvac.load | complex | kW | PUBLIC |  |  |  |  |
| hvac.energy | complex | kWh | PUBLIC |  |  |  |  |
| hvac.power_factor | double |  | PUBLIC |  |  |  |  |
| lights.demand | complex | kW | PUBLIC |  |  |  |  |
| lights.load | complex | kW | PUBLIC |  |  |  |  |
| lights.energy | complex | kWh | PUBLIC |  |  |  |  |
| lights.power_factor | double |  | PUBLIC |  |  |  |  |
| lights.heatgain_fraction | double |  | PUBLIC |  |  |  |  |
| lights.heatgain | double | kW | PUBLIC |  |  |  |  |
| plugs.demand | complex | kW | PUBLIC |  |  |  |  |
| plugs.load | complex | kW | PUBLIC |  |  |  |  |
| plugs.energy | complex | kWh | PUBLIC |  |  |  |  |
| plugs.power_factor | double |  | PUBLIC |  |  |  |  |
| plugs.heatgain_fraction | double |  | PUBLIC |  |  |  |  |
| plugs.heatgain | double | kW | PUBLIC |  |  |  |  |
| cooling_setpoint | double | degF | PUBLIC |  |  |  |  |
| heating_setpoint | double | degF | PUBLIC |  |  |  |  |
| thermostat_deadband | double | degF | PUBLIC |  |  |  |  |
| control.ventilation_fraction | double |  | PUBLIC |  |  |  |  |
| control.lighting_fraction | double |  | PUBLIC |  |  |  |  |
| ACH | double |  | PUBLIC |  |  |  |  |

### multizone

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| from | object |  | PUBLIC |  |  |  |  |
| to | object |  | PUBLIC |  |  |  |  |
| ua | double |  | PUBLIC |  |  |  |  |

### ceus

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| floor_area | double | sf | PUBLIC | REQUIRED |  |  | building floor area |
| filename | method |  | PUBLIC | REQUIRED |  |  | CEUS data file |
| composition | method |  | PUBLIC | REQUIRED |  |  | load composition specification |
| total_real_power | double | W | PUBLIC | OUTPUT |  |  | total real power |
| total_reactive_power | double | W | PUBLIC | OUTPUT |  |  | total reactive power |
| total_power_A | complex | W | PUBLIC | OUTPUT |  |  | total complex power on phase A |
| total_power_B | complex | W | PUBLIC | OUTPUT |  |  | total complex power on phase B |
| total_power_C | complex | W | PUBLIC | OUTPUT |  |  | total complex power on phase C |
| weather | object |  | PUBLIC |  |  |  | weather object for temperature and solar gain |
| tariff | object |  | PUBLIC |  |  |  | tariff object for energy price sensitivity |

### parking

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| weather | object |  | PUBLIC |  |  |  | weather object for lighting data |
| lighting_nightonly | bool |  | PUBLIC |  |  | TRUE | lighting is used only at night (requires weather) |
| lighting_capacity | complex |  | PUBLIC |  |  | 0 | installed capacity for lighting |
| charger_installed | int16 |  | PUBLIC |  |  | 0 | number of chargers installed |
| charger_active | double |  | PUBLIC |  |  | 0 | number of chargers active |
| charger_unit_power | complex |  | PUBLIC |  |  | 0 | power demand for a single charger |
| lighting_power | complex |  | PUBLIC |  |  | 0 | power demand for lighting |
| ventilation_power | complex |  | PUBLIC |  |  | 0 | power demand for ventilation |
| charger_power | complex |  | PUBLIC |  |  | 0 | power demand for active chargers |
| total_power | complex |  | PUBLIC |  |  | 0 | power total demand for parking structure |

### diesel_dg

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| Gen_mode | enumeration |  | PUBLIC |  | CONSTANTP CONSTANTPQ CONSTANTE UNKNOWN |  |  |
| Gen_status | enumeration |  | PUBLIC |  | ONLINE OFFLINE UNKNOWN |  |  |
| Gen_type | enumeration |  | PUBLIC |  | DYN_SYNCHRONOUS SYNCHRONOUS INDUCTION |  | Dynamics-capable implementation of synchronous diesel generator |
| pf | double |  | PUBLIC |  |  |  | desired power factor |
| GenElecEff | double |  | PUBLIC |  |  |  | calculated electrical efficiency of generator |
| TotalOutputPow | complex | VA | PUBLIC |  |  |  | total complex power generated |
| TotalRealPow | double | W | PUBLIC |  |  |  | total real power generated |
| TotalReacPow | double | VAr | PUBLIC |  |  |  | total reactive power generated |
| speed | double | 1/min | PUBLIC |  |  |  | speed of an engine |
| cylinders | double |  | PUBLIC |  |  |  | Total number of cylinders in a diesel engine |
| stroke | double |  | PUBLIC |  |  |  | category of internal combustion engines |
| torque | double | N | PUBLIC |  |  |  | Net brake load |
| pressure | double | N/m^2 | PUBLIC |  |  |  |  |
| time_operation | double | min | PUBLIC |  |  |  |  |
| fuel | double | kg | PUBLIC |  |  |  | fuel consumption |
| w_coolingwater | double | kg | PUBLIC |  |  |  | weight of cooling water supplied per minute |
| inlet_temperature | double | degC | PUBLIC |  |  |  | Inlet temperature of cooling water in degC |
| outlet_temperature | double | degC | PUBLIC |  |  |  | outlet temperature of cooling water in degC |
| air_fuel | double | kg | PUBLIC |  |  |  | Air used per kg fuel |
| room_temperature | double | degC | PUBLIC |  |  |  | Room temperature in degC |
| exhaust_temperature | double | degC | PUBLIC |  |  |  | exhaust gas temperature in degC |
| cylinder_length | double | m | PUBLIC |  |  |  | length of the cylinder, used in efficiency calculations |
| cylinder_radius | double | m | PUBLIC |  |  |  | inner radius of cylinder, used in efficiency calculations |
| brake_diameter | double | m | PUBLIC |  |  |  | diameter of brake, used in efficiency calculations |
| calotific_fuel | double | kJ/kg | PUBLIC |  |  |  | calorific value of fuel |
| steam_exhaust | double | kg | PUBLIC |  |  |  | steam formed per kg of fuel in the exhaust |
| specific_heat_steam | double | kJ/kg/K | PUBLIC |  |  |  | specific heat of steam in exhaust |
| specific_heat_dry | double | kJ/kg/K | PUBLIC |  |  |  | specific heat of dry exhaust gases |
| indicated_hp | double | W | PUBLIC |  |  |  | Indicated horse power is the power developed inside the cylinder |
| brake_hp | double | W | PUBLIC |  |  |  | brake horse power is the output of the engine at the shaft measured by a dynamometer |
| thermal_efficiency | double |  | PUBLIC |  |  |  | thermal efficiency or mechanical efiiciency of the engine is efined as bp/ip |
| energy_supplied | double | kJ | PUBLIC |  |  |  | energy supplied during the trail |
| heat_equivalent_ip | double | kJ | PUBLIC |  |  |  | heat equivalent of IP in a given time of operation |
| energy_coolingwater | double | kJ | PUBLIC |  |  |  | energy carried away by cooling water |
| mass_exhaustgas | double | kg | PUBLIC |  |  |  | mass of dry exhaust gas |
| energy_exhaustgas | double | kJ | PUBLIC |  |  |  | energy carried away by dry exhaust gases |
| energy_steam | double | kJ | PUBLIC |  |  |  | energy carried away by steam |
| total_energy_exhaustgas | double | kJ | PUBLIC |  |  |  | total energy carried away by dry exhaust gases is the sum of energy carried away bt steam and energy carried away by dry exhaust gases |
| unaccounted_energyloss | double | kJ | PUBLIC |  |  |  | unaccounted for energy loss |
| Pconv | double | kW | PUBLIC |  |  |  | Converted power = Mechanical input - (F & W loasses + Stray losses + Core losses) |
| Rated_V | double | V | PUBLIC |  |  |  | nominal line-line voltage in Volts |
| Rated_VA | double | VA | PUBLIC |  |  |  | nominal capacity in VA |
| power_out_A | complex | VA | PUBLIC |  |  |  | Output power of phase A |
| power_out_B | complex | VA | PUBLIC |  |  |  | Output power of phase B |
| power_out_C | complex | VA | PUBLIC |  |  |  | Output power of phase C |
| Rs | double |  | PUBLIC |  |  |  | internal transient resistance in p.u. |
| Xs | double |  | PUBLIC |  |  |  | internal transient impedance in p.u. |
| Rg | double |  | PUBLIC |  |  |  | grounding resistance in p.u. |
| Xg | double |  | PUBLIC |  |  |  | grounding impedance in p.u. |
| voltage_A | complex | V | PUBLIC |  |  |  | voltage at generator terminal, phase A |
| voltage_B | complex | V | PUBLIC |  |  |  | voltage at generator terminal, phase B |
| voltage_C | complex | V | PUBLIC |  |  |  | voltage at generator terminal, phase C |
| current_A | complex | A | PUBLIC |  |  |  | current generated at generator terminal, phase A |
| current_B | complex | A | PUBLIC |  |  |  | current generated at generator terminal, phase B |
| current_C | complex | A | PUBLIC |  |  |  | current generated at generator terminal, phase C |
| EfA | complex | V | PUBLIC |  |  |  | induced voltage on phase A |
| EfB | complex | V | PUBLIC |  |  |  | induced voltage on phase B |
| EfC | complex | V | PUBLIC |  |  |  | induced voltage on phase C |
| omega_ref | double | rad/s | PUBLIC |  |  |  | Reference frequency of generator (rad/s) |
| inertia | double |  | PUBLIC |  |  |  | Inertial constant (H) of generator |
| damping | double |  | PUBLIC |  |  |  | Damping constant (D) of generator |
| number_poles | double |  | PUBLIC |  |  |  | Number of poles in the generator |
| Ra | double | pu | PUBLIC |  |  |  | Stator resistance (p.u.) |
| Xd | double | pu | PUBLIC |  |  |  | d-axis reactance (p.u.) |
| Xq | double | pu | PUBLIC |  |  |  | q-axis reactance (p.u.) |
| Xdp | double | pu | PUBLIC |  |  |  | d-axis transient reactance (p.u.) |
| Xqp | double | pu | PUBLIC |  |  |  | q-axis transient reactance (p.u.) |
| Xdpp | double | pu | PUBLIC |  |  |  | d-axis subtransient reactance (p.u.) |
| Xqpp | double | pu | PUBLIC |  |  |  | q-axis subtransient reactance (p.u.) |
| Xl | double | pu | PUBLIC |  |  |  | Leakage reactance (p.u.) |
| Tdp | double | s | PUBLIC |  |  |  | d-axis short circuit time constant (s) |
| Tdop | double | s | PUBLIC |  |  |  | d-axis open circuit time constant (s) |
| Tqop | double | s | PUBLIC |  |  |  | q-axis open circuit time constant (s) |
| Tdopp | double | s | PUBLIC |  |  |  | d-axis open circuit subtransient time constant (s) |
| Tqopp | double | s | PUBLIC |  |  |  | q-axis open circuit subtransient time constant (s) |
| Ta | double | s | PUBLIC |  |  |  | Armature short-circuit time constant (s) |
| X0 | complex | pu | PUBLIC |  |  |  | Zero sequence impedance (p.u.) |
| X2 | complex | pu | PUBLIC |  |  |  | Negative sequence impedance (p.u.) |
| rotor_speed_convergence | double | rad | PUBLIC |  |  |  | Convergence criterion on rotor speed used to determine when to exit deltamode |
| voltage_convergence | double | V | PUBLIC |  |  |  | Convergence criterion for voltage changes (if exciter present) to determine when to exit deltamode |
| rotor_speed_convergence_enabled | bool |  | PUBLIC |  |  |  | Uses rotor_speed_convergence to determine if an exit of deltamode is needed |
| voltage_magnitude_convergence_enabled | bool |  | PUBLIC |  |  |  | Uses voltage_convergence to determine if an exit of deltamode is needed - only works if an exciter is present |
| rotor_angle | double | rad | PUBLIC |  |  |  | rotor angle state variable |
| rotor_speed | double | rad/s | PUBLIC |  |  |  | machine speed state variable |
| field_voltage | double | pu | PUBLIC |  |  |  | machine field voltage state variable |
| flux1d | double | pu | PUBLIC |  |  |  | machine transient flux on d-axis state variable |
| flux2q | double | pu | PUBLIC |  |  |  | machine subtransient flux on q-axis state variable |
| EpRotated | complex | pu | PUBLIC |  |  |  | d-q rotated E-prime internal voltage state variable |
| VintRotated | complex | pu | PUBLIC |  |  |  | d-q rotated Vint voltage state variable |
| Eint_A | complex | V | PUBLIC |  |  |  | Unrotated, unsequenced phase A internal voltage |
| Eint_B | complex | V | PUBLIC |  |  |  | Unrotated, unsequenced phase B internal voltage |
| Eint_C | complex | V | PUBLIC |  |  |  | Unrotated, unsequenced phase C internal voltage |
| Irotated | complex | pu | PUBLIC |  |  |  | d-q rotated sequence current state variable |
| pwr_electric | complex | VA | PUBLIC |  |  |  | Current electrical output of machine |
| pwr_mech | double | W | PUBLIC |  |  |  | Current mechanical output of machine |
| torque_mech | double | N*m | PUBLIC |  |  |  | Current mechanical torque of machine |
| torque_elec | double | N*m | PUBLIC |  |  |  | Current electrical torque output of machine |
| wref | double | pu | PUBLIC |  |  |  | wref input to governor controls (per-unit) |
| vset | double | pu | PUBLIC |  |  |  | vset input to AVR controls (per-unit) |
| Pref | double | pu | PUBLIC |  |  |  | Pref input to governor controls (per-unit), if supported |
| Qref | double | pu | PUBLIC |  |  |  | Qref input to govornor or AVR controls (per-unit), if supported |
| Exciter_type | enumeration |  | PUBLIC |  | SEXS NO_EXC |  | Simplified Excitation System |
| KA | double | pu | PUBLIC |  |  |  | Exciter gain (p.u.) |
| TA | double | s | PUBLIC |  |  |  | Exciter time constant (seconds) |
| TB | double | s | PUBLIC |  |  |  | Exciter transient gain reduction time constant (seconds) |
| TC | double | s | PUBLIC |  |  |  | Exciter transient gain reduction time constant (seconds) |
| EMAX | double | pu | PUBLIC |  |  |  | Exciter upper limit (p.u.) |
| EMIN | double | pu | PUBLIC |  |  |  | Exciter lower limit (p.u.) |
| Vterm_max | double | pu | PUBLIC |  |  |  | Upper voltage limit for super-second (p.u.) |
| Vterm_min | double | pu | PUBLIC |  |  |  | Lower voltage limit for super-second (p.u.) |
| bias | double |  | PUBLIC |  |  |  | Exciter bias state variable |
| xe | double |  | PUBLIC |  |  |  | Exciter state variable |
| xb | double |  | PUBLIC |  |  |  | Exciter state variable |
| x_cvr1 | double |  | PUBLIC |  |  |  | Exciter state variable |
| x_cvr2 | double |  | PUBLIC |  |  |  | Exciter state variable |
| Vref | double |  | PUBLIC |  |  |  | Exciter CVR control voltage reference value |
| CVR_mode | enumeration |  | PUBLIC |  | Feedback HighOrder |  | First order control mode with feedback loop |
| P_CONSTANT_ki | double |  | PUBLIC |  |  |  | parameter of the integration control for constant P mode |
| P_CONSTANT_kp | double |  | PUBLIC |  |  |  | parameter of the proportional control for constant P mode |
| Exciter_Q_constant_mode | bool |  | PUBLIC |  |  |  | True if the generator is operating under constant Q mode |
| Exciter_Q_constant_ki | double |  | PUBLIC |  |  |  | parameter of the integration control for constant Q mode |
| Exciter_Q_constant_kp | double |  | PUBLIC |  |  |  | parameter of the propotional control for constant Q mode |
| P_CONSTANT_Pref | double | pu | PUBLIC |  |  |  | Pref input to governor controls (per-unit), if supported |
| Exciter_Q_constant_Qref | double | pu | PUBLIC |  |  |  | Qref input to govornor or AVR controls (per-unit), if supported |
| CVR_enabled | bool |  | PUBLIC |  |  |  | True if the CVR control is enabled in the exciter |
| CVR_ki_cvr | double |  | PUBLIC |  |  |  | parameter of the integration control for CVR control |
| CVR_kp_cvr | double |  | PUBLIC |  |  |  | parameter of the proportional control for CVR control |
| CVR_kd_cvr | double |  | PUBLIC |  |  |  | parameter of the deviation control for CVR control |
| CVR_kt_cvr | double |  | PUBLIC |  |  |  | parameter of the gain in feedback loop for CVR control |
| CVR_kw_cvr | double |  | PUBLIC |  |  |  | parameter of the gain in feedback loop for CVR control |
| CVR_PI | bool |  | PUBLIC |  |  |  | True if the PI controller is implemented in CVR control |
| CVR_PID | bool |  | PUBLIC |  |  |  | True if the PID controller is implemented in CVR control |
| vset_EMAX | double |  | PUBLIC |  |  |  | Maximum Vset limit |
| vset_EMIN | double |  | PUBLIC |  |  |  | Minimum Vset limit |
| CVR_Kd1 | double |  | PUBLIC |  |  |  | parameter of the second order transfer function for CVR control |
| CVR_Kd2 | double |  | PUBLIC |  |  |  | parameter of the second order transfer function for CVR control |
| CVR_Kd3 | double |  | PUBLIC |  |  |  | parameter of the second order transfer function for CVR control |
| CVR_Kn1 | double |  | PUBLIC |  |  |  | parameter of the second order transfer function for CVR control |
| CVR_Kn2 | double |  | PUBLIC |  |  |  | parameter of the second order transfer function for CVR control |
| vset_delta_MAX | double |  | PUBLIC |  |  |  | Maximum delta Vset limit |
| vset_delta_MIN | double |  | PUBLIC |  |  |  | Minimum delta Vset limit |
| vadd | double |  | PUBLIC |  |  |  | Delta Vset |
| vadd_a | double |  | PUBLIC |  |  |  | Delta Vset before going into bound check |
| Governor_type | enumeration |  | PUBLIC |  | P_CONSTANT GGOV1 GGOV1_OLD GAST DEGOV1 NO_GOV |  | P_CONSTANT mode Governor Model |
| DEGOV1_R | double | pu | PUBLIC |  |  |  | Governor droop constant (p.u.) |
| DEGOV1_T1 | double | s | PUBLIC |  |  |  | Governor electric control box time constant (s) |
| DEGOV1_T2 | double | s | PUBLIC |  |  |  | Governor electric control box time constant (s) |
| DEGOV1_T3 | double | s | PUBLIC |  |  |  | Governor electric control box time constant (s) |
| DEGOV1_T4 | double | s | PUBLIC |  |  |  | Governor actuator time constant (s) |
| DEGOV1_T5 | double | s | PUBLIC |  |  |  | Governor actuator time constant (s) |
| DEGOV1_T6 | double | s | PUBLIC |  |  |  | Governor actuator time constant (s) |
| DEGOV1_K | double | pu | PUBLIC |  |  |  | Governor actuator gain |
| DEGOV1_TMAX | double | pu | PUBLIC |  |  |  | Governor actuator upper limit (p.u.) |
| DEGOV1_TMIN | double | pu | PUBLIC |  |  |  | Governor actuator lower limit (p.u.) |
| DEGOV1_TD | double | s | PUBLIC |  |  |  | Governor combustion delay (s) |
| DEGOV1_x1 | double |  | PUBLIC |  |  |  | Governor electric box state variable |
| DEGOV1_x2 | double |  | PUBLIC |  |  |  | Governor electric box state variable |
| DEGOV1_x4 | double |  | PUBLIC |  |  |  | Governor electric box state variable |
| DEGOV1_x5 | double |  | PUBLIC |  |  |  | Governor electric box state variable |
| DEGOV1_x6 | double |  | PUBLIC |  |  |  | Governor electric box state variable |
| DEGOV1_throttle | double |  | PUBLIC |  |  |  | Governor electric box state variable |
| GAST_R | double | pu | PUBLIC |  |  |  | Governor droop constant (p.u.) |
| GAST_T1 | double | s | PUBLIC |  |  |  | Governor electric control box time constant (s) |
| GAST_T2 | double | s | PUBLIC |  |  |  | Governor electric control box time constant (s) |
| GAST_T3 | double | s | PUBLIC |  |  |  | Governor temperature limiter time constant (s) |
| GAST_AT | double | s | PUBLIC |  |  |  | Governor Ambient Temperature load limit (units) |
| GAST_KT | double | pu | PUBLIC |  |  |  | Governor temperature control loop gain |
| GAST_VMAX | double | pu | PUBLIC |  |  |  | Governor actuator upper limit (p.u.) |
| GAST_VMIN | double | pu | PUBLIC |  |  |  | Governor actuator lower limit (p.u.) |
| GAST_x1 | double |  | PUBLIC |  |  |  | Governor electric box state variable |
| GAST_x2 | double |  | PUBLIC |  |  |  | Governor electric box state variable |
| GAST_x3 | double |  | PUBLIC |  |  |  | Governor electric box state variable |
| GAST_throttle | double |  | PUBLIC |  |  |  | Governor electric box state variable |
| GGOV1_R | double | pu | PUBLIC |  |  |  | Permanent droop, p.u. |
| GGOV1_Rselect | int32 |  | PUBLIC |  |  |  | Feedback signal for droop, = 1 selected electrical power, = 0 none (isochronous governor), = -1 fuel valve stroke ( true stroke),= -2 governor output ( requested stroke) |
| GGOV1_Tpelec | double | s | PUBLIC |  |  |  | Electrical power transducer time constant, sec. (>0.) |
| GGOV1_maxerr | double |  | PUBLIC |  |  |  | Maximum value for speed error signal |
| GGOV1_minerr | double |  | PUBLIC |  |  |  | Minimum value for speed error signal |
| GGOV1_Kpgov | double |  | PUBLIC |  |  |  | Governor proportional gain |
| GGOV1_Kigov | double |  | PUBLIC |  |  |  | Governor integral gain |
| GGOV1_Kdgov | double |  | PUBLIC |  |  |  | Governor derivative gain |
| GGOV1_Tdgov | double | s | PUBLIC |  |  |  | Governor derivative controller time constant, sec. |
| GGOV1_vmax | double |  | PUBLIC |  |  |  | Maximum valve position limit |
| GGOV1_vmin | double |  | PUBLIC |  |  |  | Minimum valve position limit |
| GGOV1_Tact | double |  | PUBLIC |  |  |  | Actuator time constant |
| GGOV1_Kturb | double |  | PUBLIC |  |  |  | Turbine gain (>0.) |
| GGOV1_wfnl | double | pu | PUBLIC |  |  |  | No load fuel flow, p.u |
| GGOV1_Tb | double | s | PUBLIC |  |  |  | Turbine lag time constant, sec. (>0.) |
| GGOV1_Tc | double | s | PUBLIC |  |  |  | Turbine lead time constant, sec. |
| GGOV1_Fuel_lag | int32 |  | PUBLIC |  |  |  | Switch for fuel source characteristic, = 0 for fuel flow independent of speed, = 1 fuel flow proportional to speed |
| GGOV1_Teng | double |  | PUBLIC |  |  |  | Transport lag time constant for diesel engine |
| GGOV1_Tfload | double |  | PUBLIC |  |  |  | Load Limiter time constant, sec. (>0.) |
| GGOV1_Kpload | double |  | PUBLIC |  |  |  | Load limiter proportional gain for PI controller |
| GGOV1_Kiload | double |  | PUBLIC |  |  |  | Load limiter integral gain for PI controller |
| GGOV1_Ldref | double | pu | PUBLIC |  |  |  | Load limiter reference value p.u. |
| GGOV1_Dm | double | pu | PUBLIC |  |  |  | Speed sensitivity coefficient, p.u. |
| GGOV1_ropen | double | pu/s | PUBLIC |  |  |  | Maximum valve opening rate, p.u./sec. |
| GGOV1_rclose | double | pu/s | PUBLIC |  |  |  | Minimum valve closing rate, p.u./sec. |
| GGOV1_Kimw | double |  | PUBLIC |  |  |  | Power controller (reset) gain |
| GGOV1_Pmwset | double | MW | PUBLIC |  |  |  | Power controller setpoint, MW |
| GGOV1_aset | double | pu/s | PUBLIC |  |  |  | Acceleration limiter setpoint, p.u. / sec. |
| GGOV1_Ka | double |  | PUBLIC |  |  |  | Acceleration limiter Gain |
| GGOV1_Ta | double | s | PUBLIC |  |  |  | Acceleration limiter time constant, sec. (>0.) |
| GGOV1_db | double |  | PUBLIC |  |  |  | Speed governor dead band |
| GGOV1_Tsa | double | s | PUBLIC |  |  |  | Temperature detection lead time constant, sec. |
| GGOV1_Tsb | double | s | PUBLIC |  |  |  | Temperature detection lag time constant, sec. |
| GGOV1_Load_Limit_enable | bool |  | PUBLIC |  |  |  | Enables/disables load limiter (fsrt) of low-value-select |
| GGOV1_Acceleration_Limit_enable | bool |  | PUBLIC |  |  |  | Enables/disables acceleration limiter (fsra) of low-value-select |
| GGOV1_PID_enable | bool |  | PUBLIC |  |  |  | Enables/disables PID controller (fsrn) of low-value-select |
| GGOV1_fsrt | double |  | PUBLIC |  |  |  | Load limiter block input to low-value-select |
| GGOV1_fsra | double |  | PUBLIC |  |  |  | Acceleration limiter block input to low-value-select |
| GGOV1_fsrn | double |  | PUBLIC |  |  |  | PID block input to low-value-select |
| GGOV1_speed_error | double | pu | PUBLIC |  |  |  | Speed difference in per-unit for input to PID controller |
| GGOV1_x1 | double |  | PUBLIC |  |  |  |  |
| GGOV1_x2 | double |  | PUBLIC |  |  |  |  |
| GGOV1_x2a | double |  | PUBLIC |  |  |  |  |
| GGOV1_x3 | double |  | PUBLIC |  |  |  |  |
| GGOV1_x3a | double |  | PUBLIC |  |  |  |  |
| GGOV1_x4 | double |  | PUBLIC |  |  |  |  |
| GGOV1_x4a | double |  | PUBLIC |  |  |  |  |
| GGOV1_x4b | double |  | PUBLIC |  |  |  |  |
| GGOV1_x5 | double |  | PUBLIC |  |  |  |  |
| GGOV1_x5a | double |  | PUBLIC |  |  |  |  |
| GGOV1_x5b | double |  | PUBLIC |  |  |  |  |
| GGOV1_x6 | double |  | PUBLIC |  |  |  |  |
| GGOV1_x7 | double |  | PUBLIC |  |  |  |  |
| GGOV1_x7a | double |  | PUBLIC |  |  |  |  |
| GGOV1_x8 | double |  | PUBLIC |  |  |  |  |
| GGOV1_x8a | double |  | PUBLIC |  |  |  |  |
| GGOV1_x9 | double |  | PUBLIC |  |  |  |  |
| GGOV1_x9a | double |  | PUBLIC |  |  |  |  |
| GGOV1_x10 | double |  | PUBLIC |  |  |  |  |
| GGOV1_x10a | double |  | PUBLIC |  |  |  |  |
| GGOV1_x10b | double |  | PUBLIC |  |  |  |  |
| GGOV1_ValveStroke | double |  | PUBLIC |  |  |  |  |
| GGOV1_FuelFlow | double |  | PUBLIC |  |  |  |  |
| GGOV1_GovOutPut | double |  | PUBLIC |  |  |  |  |
| GGOV1_RselectValue | double |  | PUBLIC |  |  |  |  |
| GGOV1_fsrtNoLim | double |  | PUBLIC |  |  |  |  |
| GGOV1_err2 | double |  | PUBLIC |  |  |  |  |
| GGOV1_err2a | double |  | PUBLIC |  |  |  |  |
| GGOV1_err3 | double |  | PUBLIC |  |  |  |  |
| GGOV1_err4 | double |  | PUBLIC |  |  |  |  |
| GGOV1_err7 | double |  | PUBLIC |  |  |  |  |
| GGOV1_LowValSelect1 | double |  | PUBLIC |  |  |  |  |
| GGOV1_LowValSelect | double |  | PUBLIC |  |  |  |  |
| P_CONSTANT_Tpelec | double | s | PUBLIC |  |  |  | Electrical power transducer time constant, sec. (>0.) |
| P_CONSTANT_Tact | double |  | PUBLIC |  |  |  | Actuator time constant |
| P_CONSTANT_Kturb | double |  | PUBLIC |  |  |  | Turbine gain (>0.) |
| P_CONSTANT_wfnl | double | pu | PUBLIC |  |  |  | No load fuel flow, p.u |
| P_CONSTANT_Tb | double | s | PUBLIC |  |  |  | Turbine lag time constant, sec. (>0.) |
| P_CONSTANT_Tc | double | s | PUBLIC |  |  |  | Turbine lead time constant, sec. |
| P_CONSTANT_Teng | double |  | PUBLIC |  |  |  | Transport lag time constant for diesel engine |
| P_CONSTANT_ropen | double | pu/s | PUBLIC |  |  |  | Maximum valve opening rate, p.u./sec. |
| P_CONSTANT_rclose | double | pu/s | PUBLIC |  |  |  | Minimum valve closing rate, p.u./sec. |
| P_CONSTANT_Kimw | double |  | PUBLIC |  |  |  | Power controller (reset) gain |
| P_CONSTANT_x1 | double |  | PUBLIC |  |  |  |  |
| P_CONSTANT_x4 | double |  | PUBLIC |  |  |  |  |
| P_CONSTANT_x4a | double |  | PUBLIC |  |  |  |  |
| P_CONSTANT_x4b | double |  | PUBLIC |  |  |  |  |
| P_CONSTANT_x5 | double |  | PUBLIC |  |  |  |  |
| P_CONSTANT_x5a | double |  | PUBLIC |  |  |  |  |
| P_CONSTANT_x5b | double |  | PUBLIC |  |  |  |  |
| P_CONSTANT_x_Pconstant | double |  | PUBLIC |  |  |  |  |
| P_CONSTANT_err4 | double |  | PUBLIC |  |  |  |  |
| P_CONSTANT_ValveStroke | double |  | PUBLIC |  |  |  |  |
| P_CONSTANT_FuelFlow | double |  | PUBLIC |  |  |  |  |
| P_CONSTANT_GovOutPut | double |  | PUBLIC |  |  |  |  |
| fuelEmissionCal | bool |  | PUBLIC |  |  |  | Boolean value indicating whether fuel and emission calculations are used or not |
| outputEnergy | double |  | PUBLIC |  |  |  | Total energy(kWh) output from the generator |
| FuelUse | double |  | PUBLIC |  |  |  | Total fuel usage (gal) based on kW power output |
| efficiency | double |  | PUBLIC |  |  |  | Total energy output per fuel usage (kWh/gal) |
| CO2_emission | double |  | PUBLIC |  |  |  | Total CO2 emissions (lbs) based on fule usage |
| SOx_emission | double |  | PUBLIC |  |  |  | Total SOx emissions (lbs) based on fule usage |
| NOx_emission | double |  | PUBLIC |  |  |  | Total NOx emissions (lbs) based on fule usage |
| PM10_emission | double |  | PUBLIC |  |  |  | Total PM-10 emissions (lbs) based on fule usage |
| frequency_deviation | double |  | PUBLIC |  |  |  | Frequency deviation of diesel_dg |
| frequency_deviation_energy | double |  | PUBLIC |  |  |  | Frequency deviation accumulation of diesel_dg |
| frequency_deviation_max | double |  | PUBLIC |  |  |  | Frequency deviation of diesel_dg |
| realPowerChange | double |  | PUBLIC |  |  |  | Real power output change of diesel_dg |
| ratio_f_p | double |  | PUBLIC |  |  |  | Ratio of frequency deviation to real power output change of diesel_dg |
| phases | set |  | PUBLIC | CHARSET | S N C B A |  | Specifies which phases to connect to - currently not supported and assumes three-phase connection |

### windturb_dg

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| Gen_status | enumeration |  | PUBLIC |  | ONLINE OFFLINE |  | Generator is currently available to supply power |
| Gen_type | enumeration |  | PUBLIC |  | SYNCHRONOUS INDUCTION |  | Standard synchronous generator; is also used to 'fake' a doubly-fed induction generator for now |
| Gen_mode | enumeration |  | PUBLIC |  | CONSTANTPQ CONSTANTP CONSTANTE |  | Maintains the real and reactive output at the terminals - currently unsupported |
| Turbine_Model | enumeration |  | PUBLIC |  | BERGEY_10kW GE_25MW VESTAS_V82 USER_DEFINED GENERIC_IND_LARGE GENERIC_IND_MID GENERIC_IND_SMALL GENERIC_SYNCH_LARGE GENERIC_SYNCH_MID GENERIC_SYNCH_SMALL |  | Sets all defaults to represent the power output of a Bergey 10kW turbine |
| turbine_height | double | m | PUBLIC |  |  |  | Describes the height of the wind turbine hub above the ground |
| roughness_length_factor | double |  | PUBLIC |  |  |  | European Wind Atlas unitless correction factor for adjusting wind speed at various heights above ground and terrain types, default=0.055 |
| blade_diam | double | m | PUBLIC |  |  |  | Diameter of blades |
| blade_diameter | double | m | PUBLIC |  |  |  | Diameter of blades |
| cut_in_ws | double | m/s | PUBLIC |  |  |  | Minimum wind speed for generator operation |
| cut_out_ws | double | m/s | PUBLIC |  |  |  | Maximum wind speed for generator operation |
| ws_rated | double | m/s | PUBLIC |  |  |  | Rated wind speed for generator operation |
| ws_maxcp | double | m/s | PUBLIC |  |  |  | Wind speed at which generator reaches maximum Cp |
| Cp_max | double | pu | PUBLIC |  |  |  | Maximum coefficient of performance |
| Cp_rated | double | pu | PUBLIC |  |  |  | Rated coefficient of performance |
| Cp | double | pu | PUBLIC |  |  |  | Calculated coefficient of performance |
| Rated_VA | double | VA | PUBLIC |  |  |  | Rated generator power output |
| Rated_V | double | V | PUBLIC |  |  |  | Rated generator terminal voltage |
| Pconv | double | W | PUBLIC |  |  |  | Amount of electrical power converted from mechanical power delivered |
| P_converted | double | W | PUBLIC |  |  |  | Amount of electrical power converted from mechanical power delivered |
| GenElecEff | double | % | PUBLIC |  |  |  | Calculated generator electrical efficiency |
| generator_efficiency | double | % | PUBLIC |  |  |  | Calculated generator electrical efficiency |
| TotalRealPow | double | W | PUBLIC |  |  |  | Total real power output |
| total_real_power | double | W | PUBLIC |  |  |  | Total real power output |
| TotalReacPow | double | VA | PUBLIC |  |  |  | Total reactive power output |
| total_reactive_power | double | VA | PUBLIC |  |  |  | Total reactive power output |
| power_A | complex | VA | PUBLIC |  |  |  | Total complex power injected on phase A |
| power_B | complex | VA | PUBLIC |  |  |  | Total complex power injected on phase B |
| power_C | complex | VA | PUBLIC |  |  |  | Total complex power injected on phase C |
| WSadj | double | m/s | PUBLIC |  |  |  | Speed of wind at hub height |
| wind_speed_adjusted | double | m/s | PUBLIC |  |  |  | Speed of wind at hub height |
| Wind_Speed | double | m/s | PUBLIC |  |  |  | Wind speed at 5-15m level (typical measurement height) |
| wind_speed | double | m/s | PUBLIC |  |  |  | Wind speed at 5-15m level (typical measurement height) |
| air_density | double | kg/m^3 | PUBLIC |  |  |  | Estimated air density |
| R_stator | double | pu*Ohm | PUBLIC |  |  |  | Induction generator primary stator resistance in p.u. |
| X_stator | double | pu*Ohm | PUBLIC |  |  |  | Induction generator primary stator reactance in p.u. |
| R_rotor | double | pu*Ohm | PUBLIC |  |  |  | Induction generator primary rotor resistance in p.u. |
| X_rotor | double | pu*Ohm | PUBLIC |  |  |  | Induction generator primary rotor reactance in p.u. |
| R_core | double | pu*Ohm | PUBLIC |  |  |  | Induction generator primary core resistance in p.u. |
| X_magnetic | double | pu*Ohm | PUBLIC |  |  |  | Induction generator primary core reactance in p.u. |
| Max_Vrotor | double | pu*V | PUBLIC |  |  |  | Induction generator maximum induced rotor voltage in p.u., e.g. 1.2 |
| Min_Vrotor | double | pu*V | PUBLIC |  |  |  | Induction generator minimum induced rotor voltage in p.u., e.g. 0.8 |
| Rs | double | pu*Ohm | PUBLIC |  |  |  | Synchronous generator primary stator resistance in p.u. |
| Xs | double | pu*Ohm | PUBLIC |  |  |  | Synchronous generator primary stator reactance in p.u. |
| Rg | double | pu*Ohm | PUBLIC |  |  |  | Synchronous generator grounding resistance in p.u. |
| Xg | double | pu*Ohm | PUBLIC |  |  |  | Synchronous generator grounding reactance in p.u. |
| Max_Ef | double | pu*V | PUBLIC |  |  |  | Synchronous generator maximum induced rotor voltage in p.u., e.g. 0.8 |
| Min_Ef | double | pu*V | PUBLIC |  |  |  | Synchronous generator minimum induced rotor voltage in p.u., e.g. 0.8 |
| pf | double | pu | PUBLIC |  |  |  | Desired power factor in CONSTANTP mode (can be modified over time) |
| power_factor | double | pu | PUBLIC |  |  |  | Desired power factor in CONSTANTP mode (can be modified over time) |
| voltage_A | complex | V | PUBLIC |  |  |  | Terminal voltage on phase A |
| voltage_B | complex | V | PUBLIC |  |  |  | Terminal voltage on phase B |
| voltage_C | complex | V | PUBLIC |  |  |  | Terminal voltage on phase C |
| current_A | complex | A | PUBLIC |  |  |  | Calculated terminal current on phase A |
| current_B | complex | A | PUBLIC |  |  |  | Calculated terminal current on phase B |
| current_C | complex | A | PUBLIC |  |  |  | Calculated terminal current on phase C |
| EfA | complex | V | PUBLIC |  |  |  | Synchronous generator induced voltage on phase A |
| EfB | complex | V | PUBLIC |  |  |  | Synchronous generator induced voltage on phase B |
| EfC | complex | V | PUBLIC |  |  |  | Synchronous generator induced voltage on phase C |
| Vrotor_A | complex | V | PUBLIC |  |  |  | Induction generator induced voltage on phase A in p.u. |
| Vrotor_B | complex | V | PUBLIC |  |  |  | Induction generator induced voltage on phase B in p.u. |
| Vrotor_C | complex | V | PUBLIC |  |  |  | Induction generator induced voltage on phase C in p.u. |
| Irotor_A | complex | V | PUBLIC |  |  |  | Induction generator induced current on phase A in p.u. |
| Irotor_B | complex | V | PUBLIC |  |  |  | Induction generator induced current on phase B in p.u. |
| Irotor_C | complex | V | PUBLIC |  |  |  | Induction generator induced current on phase C in p.u. |
| phases | set |  | PUBLIC | CHARSET | S N C B A |  | Specifies which phases to connect to - currently not supported and assumes three-phase connection |

### power_electronics

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| generator_mode | enumeration |  | PUBLIC |  | SUPPLY_DRIVEN CONSTANT_PF CONSTANT_PQ CONSTANT_V UNKNOWN |  |  |
| generator_status | enumeration |  | PUBLIC |  | ONLINE OFFLINE |  |  |
| converter_type | enumeration |  | PUBLIC |  | CURRENT_SOURCED VOLTAGE_SOURCED |  |  |
| switch_type | enumeration |  | PUBLIC |  | DARLINGTON IBJT JFET SCR MOSFET BJT IDEAL_SWITCH |  |  |
| filter_type | enumeration |  | PUBLIC |  | BAND_PASS BAND_STOP HIGH_PASS LOW_PASS |  |  |
| filter_implementation | enumeration |  | PUBLIC |  | PARALLEL_RESONANT SERIES_RESONANT INDUCTIVE CAPACITVE IDEAL_FILTER |  |  |
| filter_frequency | enumeration |  | PUBLIC |  | F240HZ F180HZ F120HZ |  |  |
| power_type | enumeration |  | PUBLIC |  | DC AC |  |  |
| Rated_kW | double | kW | PUBLIC |  |  |  |  |
| Max_P | double | kW | PUBLIC |  |  |  |  |
| Min_P | double | kW | PUBLIC |  |  |  |  |
| Rated_kVA | double | kVA | PUBLIC |  |  |  |  |
| Rated_kV | double | kV | PUBLIC |  |  |  |  |
| phases | set |  | PUBLIC | CHARSET | S N C B A |  |  |

### energy_storage

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| generator_mode | enumeration |  | PUBLIC |  | SUPPLY_DRIVEN CONSTANT_PF CONSTANT_PQ CONSTANT_V UNKNOWN |  |  |
| generator_status | enumeration |  | PUBLIC |  | ONLINE OFFLINE |  |  |
| power_type | enumeration |  | PUBLIC |  | DC AC |  |  |
| Rinternal | double |  | PUBLIC |  |  |  |  |
| V_Max | double | V | PUBLIC |  |  |  |  |
| I_Max | complex | A | PUBLIC |  |  |  |  |
| E_Max | double |  | PUBLIC |  |  |  |  |
| Energy | double |  | PUBLIC |  |  |  |  |
| efficiency | double |  | PUBLIC |  |  |  |  |
| Rated_kVA | double | kVA | PUBLIC |  |  |  |  |
| V_Out | complex | V | PUBLIC |  |  |  |  |
| I_Out | complex | A | PUBLIC |  |  |  |  |
| VA_Out | complex | VA | PUBLIC |  |  |  |  |
| V_In | complex | V | PUBLIC |  |  |  |  |
| I_In | complex | A | PUBLIC |  |  |  |  |
| V_Internal | complex | V | PUBLIC |  |  |  |  |
| I_Internal | complex | A | PUBLIC |  |  |  |  |
| I_Prev | complex | A | PUBLIC |  |  |  |  |
| phases | set |  | PUBLIC | CHARSET | S N C B A |  |  |

### inverter

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| inverter_type | enumeration |  | PUBLIC |  | FOUR_QUADRANT PWM TWELVE_PULSE SIX_PULSE TWO_PULSE |  | LEGACY MODEL: Sets efficiencies and other parameters; if using four_quadrant_control_mode, set this to FOUR_QUADRANT |
| four_quadrant_control_mode | enumeration |  | PUBLIC |  | VOLTAGE_SOURCE GROUP_LOAD_FOLLOWING LOAD_FOLLOWING VOLT_VAR_FREQ_PWR VOLT_VAR CONSTANT_PF CONSTANT_PQ NONE |  | FOUR QUADRANT MODEL: Activates various control modes |
| pf_reg | enumeration |  | PUBLIC |  | EXCLUDED INCLUDED_ALT INCLUDED |  | Activate (or not) power factor regulation in four_quadrant_control_mode |
| generator_status | enumeration |  | PUBLIC |  | ONLINE OFFLINE |  | describes whether the generator is online or offline |
| generator_mode | enumeration |  | PUBLIC |  | SUPPLY_DRIVEN CONSTANT_PF CONSTANT_PQ CONSTANT_V UNKNOWN |  | LEGACY MODEL: Selects generator control mode when using legacy model; in non-legacy models, this should be SUPPLY_DRIVEN. |
| inverter_convergence_criterion | double |  | PUBLIC |  |  |  | The maximum change in error threshold for exitting deltamode. |
| V_In | complex | V | PUBLIC |  |  |  | DC voltage |
| I_In | complex | A | PUBLIC |  |  |  | DC current |
| VA_In | complex | VA | PUBLIC |  |  |  | DC power |
| VA_Out | complex | VA | PUBLIC |  |  |  | AC power |
| Vdc | double | V | PUBLIC |  |  |  | LEGACY MODEL: DC voltage |
| phaseA_V_Out | complex | V | PUBLIC |  |  |  | AC voltage on A phase in three-phase system; 240-V connection on a triplex system |
| phaseB_V_Out | complex | V | PUBLIC |  |  |  | AC voltage on B phase in three-phase system |
| phaseC_V_Out | complex | V | PUBLIC |  |  |  | AC voltage on C phase in three-phase system |
| phaseA_I_Out | complex | V | PUBLIC |  |  |  | AC current on A phase in three-phase system; 240-V connection on a triplex system |
| phaseB_I_Out | complex | V | PUBLIC |  |  |  | AC current on B phase in three-phase system |
| phaseC_I_Out | complex | V | PUBLIC |  |  |  | AC current on C phase in three-phase system |
| power_A | complex | VA | PUBLIC |  |  |  | AC power on A phase in three-phase system; 240-V connection on a triplex system |
| power_B | complex | VA | PUBLIC |  |  |  | AC power on B phase in three-phase system |
| power_C | complex | VA | PUBLIC |  |  |  | AC power on C phase in three-phase system |
| curr_VA_out_A | complex | VA | PUBLIC |  |  |  | AC power on A phase in three-phase system; 240-V connection on a triplex system |
| curr_VA_out_B | complex | VA | PUBLIC |  |  |  | AC power on B phase in three-phase system |
| curr_VA_out_C | complex | VA | PUBLIC |  |  |  | AC power on C phase in three-phase system |
| prev_VA_out_A | complex | VA | PUBLIC |  |  |  | AC power on A phase in three-phase system; 240-V connection on a triplex system |
| prev_VA_out_B | complex | VA | PUBLIC |  |  |  | AC power on B phase in three-phase system |
| prev_VA_out_C | complex | VA | PUBLIC |  |  |  | AC power on C phase in three-phase system |
| P_Out | double | VA | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Scheduled real power out in CONSTANT_PQ control mode |
| Q_Out | double | VAr | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Schedule reactive power out in CONSTANT_PQ control mode |
| power_in | double | W | PUBLIC |  |  |  | LEGACY MODEL: No longer used |
| rated_power | double | VA | PUBLIC |  |  |  | FOUR QUADRANT MODEL: The rated power of the inverter |
| rated_battery_power | double | W | PUBLIC |  |  |  | FOUR QUADRANT MODEL: The rated power of battery when battery is attached |
| inverter_efficiency | double |  | PUBLIC |  |  |  | FOUR QUADRANT MODEL: The efficiency of the inverter |
| battery_soc | double | pu | PUBLIC |  |  |  | FOUR QUADRANT MODEL: The state of charge of an attached battery |
| soc_reserve | double | pu | PUBLIC |  |  |  | FOUR QUADRANT MODEL: The reserve state of charge of an attached battery for islanding cases |
| power_factor | double | unit | PUBLIC |  |  |  | FOUR QUADRANT MODEL: The power factor used for CONSTANT_PF control mode |
| islanded_state | bool |  | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Boolean used to let control modes to act under island conditions |
| nominal_frequency | double | Hz | PUBLIC |  |  |  |  |
| Pref | double |  | PUBLIC |  |  |  | DELTAMODE: The real power reference. |
| Qref | double |  | PUBLIC |  |  |  | DELTAMODE: The reactive power reference. |
| kpd | double |  | PUBLIC |  |  |  | DELTAMODE: The d-axis integration gain for the current modulation PI controller. |
| kpq | double |  | PUBLIC |  |  |  | DELTAMODE: The q-axis integration gain for the current modulation PI controller. |
| kid | double |  | PUBLIC |  |  |  | DELTAMODE: The d-axis proportional gain for the current modulation PI controller. |
| kiq | double |  | PUBLIC |  |  |  | DELTAMODE: The q-axis proportional gain for the current modulation PI controller. |
| kdd | double |  | PUBLIC |  |  |  | DELTAMODE: The d-axis differentiator gain for the current modulation PID controller |
| kdq | double |  | PUBLIC |  |  |  | DELTAMODE: The q-axis differentiator gain for the current modulation PID controller |
| epA | double |  | PUBLIC |  |  |  | DELTAMODE: The real current error for phase A or triplex phase. |
| epB | double |  | PUBLIC |  |  |  | DELTAMODE: The real current error for phase B. |
| epC | double |  | PUBLIC |  |  |  | DELTAMODE: The real current error for phase C. |
| eqA | double |  | PUBLIC |  |  |  | DELTAMODE: The reactive current error for phase A or triplex phase. |
| eqB | double |  | PUBLIC |  |  |  | DELTAMODE: The reactive current error for phase B. |
| eqC | double |  | PUBLIC |  |  |  | DELTAMODE: The reactive current error for phase C. |
| delta_epA | double |  | PUBLIC |  |  |  | DELTAMODE: The change in real current error for phase A or triplex phase. |
| delta_epB | double |  | PUBLIC |  |  |  | DELTAMODE: The change in real current error for phase B. |
| delta_epC | double |  | PUBLIC |  |  |  | DELTAMODE: The change in real current error for phase C. |
| delta_eqA | double |  | PUBLIC |  |  |  | DELTAMODE: The change in reactive current error for phase A or triplex phase. |
| delta_eqB | double |  | PUBLIC |  |  |  | DELTAMODE: The change in reactive current error for phase B. |
| delta_eqC | double |  | PUBLIC |  |  |  | DELTAMODE: The change in reactive current error for phase C. |
| mdA | double |  | PUBLIC |  |  |  | DELTAMODE: The d-axis current modulation for phase A or triplex phase. |
| mdB | double |  | PUBLIC |  |  |  | DELTAMODE: The d-axis current modulation for phase B. |
| mdC | double |  | PUBLIC |  |  |  | DELTAMODE: The d-axis current modulation for phase C. |
| mqA | double |  | PUBLIC |  |  |  | DELTAMODE: The q-axis current modulation for phase A or triplex phase. |
| mqB | double |  | PUBLIC |  |  |  | DELTAMODE: The q-axis current modulation for phase B. |
| mqC | double |  | PUBLIC |  |  |  | DELTAMODE: The q-axis current modulation for phase C. |
| delta_mdA | double |  | PUBLIC |  |  |  | DELTAMODE: The change in d-axis current modulation for phase A or triplex phase. |
| delta_mdB | double |  | PUBLIC |  |  |  | DELTAMODE: The change in d-axis current modulation for phase B. |
| delta_mdC | double |  | PUBLIC |  |  |  | DELTAMODE: The change in d-axis current modulation for phase C. |
| delta_mqA | double |  | PUBLIC |  |  |  | DELTAMODE: The change in q-axis current modulation for phase A or triplex phase. |
| delta_mqB | double |  | PUBLIC |  |  |  | DELTAMODE: The change in q-axis current modulation for phase B. |
| delta_mqC | double |  | PUBLIC |  |  |  | DELTAMODE: The change in q-axis current modulation for phase C. |
| IdqA | complex |  | PUBLIC |  |  |  | DELTAMODE: The dq-axis current for phase A or triplex phase. |
| IdqB | complex |  | PUBLIC |  |  |  | DELTAMODE: The dq-axis current for phase B. |
| IdqC | complex |  | PUBLIC |  |  |  | DELTAMODE: The dq-axis current for phase C. |
| Tfreq_delay | double |  | PUBLIC |  |  |  | DELTAMODE: The time constant for delayed frequency seen by the inverter |
| inverter_droop_fp | bool |  | PUBLIC |  |  |  | DELTAMODE: Boolean used to indicate whether inverter f/p droop is included or not |
| R_fp | double |  | PUBLIC |  |  |  | DELTAMODE: The droop parameter of the f/p droop |
| Tvol_delay | double |  | PUBLIC |  |  |  | DELTAMODE: The time constant for delayed voltage seen by the inverter |
| inverter_droop_vq | bool |  | PUBLIC |  |  |  | DELTAMODE: Boolean used to indicate whether inverter q/v droop is included or not |
| R_vq | double |  | PUBLIC |  |  |  | DELTAMODE: The droop parameter of the v/q droop |
| Tp_delay | double |  | PUBLIC |  |  |  | DELTAMODE: The time constant for delayed real power seen by the VSI droop controller |
| Tq_delay | double |  | PUBLIC |  |  |  | DELTAMODE: The time constant for delayed reactive power seen by the VSI droop controller |
| VSI_Rfilter | complex | pu | PUBLIC |  |  |  | VSI filter resistance (p.u.) |
| VSI_Xfilter | complex | pu | PUBLIC |  |  |  | VSI filter inductance (p.u.) |
| VSI_mode | enumeration |  | PUBLIC |  | VSI_DROOP VSI_ISOCHRONOUS |  | VSI MODEL: Selects VSI mode for either isochronous or droop one |
| VSI_freq | double |  | PUBLIC |  |  |  | VSI frequency |
| ki_Vterminal | double |  | PUBLIC |  |  |  | DELTAMODE: The integrator gain for the VSI terminal voltage modulation |
| kp_Vterminal | double |  | PUBLIC |  |  |  | DELTAMODE: The proportional gain for the VSI terminal voltage modulation |
| enable_ramp_rates_real | bool |  | PUBLIC |  |  |  | DELTAMODE: Boolean used to indicate whether inverter ramp rate is enforced or not |
| max_ramp_up_real | double | W/s | PUBLIC |  |  |  | DELTAMODE: The real power ramp up rate limit |
| max_ramp_down_real | double | W/s | PUBLIC |  |  |  | DELTAMODE: The real power ramp down rate limit |
| enable_ramp_rates_reactive | bool |  | PUBLIC |  |  |  | DELTAMODE: Boolean used to indicate whether inverter ramp rate is enforced or not |
| max_ramp_up_reactive | double | VAr/s | PUBLIC |  |  |  | DELTAMODE: The reactive power ramp up rate limit |
| max_ramp_down_reactive | double | VAr/s | PUBLIC |  |  |  | DELTAMODE: The reactive power ramp down rate limit |
| dynamic_model_mode | enumeration |  | PUBLIC |  | PI PID |  | DELTAMODE: Underlying model to use for deltamode control |
| enable_1547_checks | bool |  | PUBLIC |  |  |  | DELTAMODE: Enable IEEE 1547-2003 disconnect checking |
| reconnect_time | double | s | PUBLIC |  |  |  | DELTAMODE: Time delay after IEEE 1547-2003 violation clears before resuming generation |
| inverter_1547_status | bool |  | PUBLIC |  |  |  | DELTAMODE: Indicator if the inverter is curtailed due to a 1547 violation or not |
| IEEE_1547_version | enumeration |  | PUBLIC |  | IEEE1547A IEEE1547 NONE |  | DELTAMODE: Version of IEEE 1547 to use to populate defaults |
| over_freq_high_cutout | double | Hz | PUBLIC |  |  |  | DELTAMODE: OF2 set point for IEEE 1547a |
| over_freq_high_disconenct_time | double | s | PUBLIC |  |  |  | DELTAMODE: OF2 clearing time for IEEE1547a |
| over_freq_low_cutout | double | Hz | PUBLIC |  |  |  | DELTAMODE: OF1 set point for IEEE 1547a |
| over_freq_low_disconenct_time | double | s | PUBLIC |  |  |  | DELTAMODE: OF1 clearing time for IEEE 1547a |
| under_freq_high_cutout | double | Hz | PUBLIC |  |  |  | DELTAMODE: UF2 set point for IEEE 1547a |
| under_freq_high_disconenct_time | double | s | PUBLIC |  |  |  | DELTAMODE: UF2 clearing time for IEEE1547a |
| under_freq_low_cutout | double | Hz | PUBLIC |  |  |  | DELTAMODE: UF1 set point for IEEE 1547a |
| under_freq_low_disconenct_time | double | s | PUBLIC |  |  |  | DELTAMODE: UF1 clearing time for IEEE 1547a |
| under_voltage_low_cutout | double | pu | PUBLIC |  |  |  | Lowest voltage threshold for undervoltage |
| under_voltage_middle_cutout | double | pu | PUBLIC |  |  |  | Middle-lowest voltage threshold for undervoltage |
| under_voltage_high_cutout | double | pu | PUBLIC |  |  |  | High value of low voltage threshold for undervoltage |
| over_voltage_low_cutout | double | pu | PUBLIC |  |  |  | Lowest voltage value for overvoltage |
| over_voltage_high_cutout | double | pu | PUBLIC |  |  |  | High voltage value for overvoltage |
| under_voltage_low_disconnect_time | double | s | PUBLIC |  |  |  | Lowest voltage clearing time for undervoltage |
| under_voltage_middle_disconnect_time | double | s | PUBLIC |  |  |  | Middle-lowest voltage clearing time for undervoltage |
| under_voltage_high_disconnect_time | double | s | PUBLIC |  |  |  | Highest voltage clearing time for undervoltage |
| over_voltage_low_disconnect_time | double | s | PUBLIC |  |  |  | Lowest voltage clearing time for overvoltage |
| over_voltage_high_disconnect_time | double | s | PUBLIC |  |  |  | Highest voltage clearing time for overvoltage |
| IEEE_1547_trip_method | enumeration |  | PUBLIC |  | OVER_VOLTAGE_HIGH OVER_VOLTAGE_LOW UNDER_VOLTAGE_HIGH UNDER_VOLTAGE_MID UNDER_VOLTAGE_LOW UNDER_FREQUENCY_LOW UNDER_FREQUENCY_HIGH OVER_FREQUENCY_LOW OVER_FREQUENCY_HIGH NONE |  | High over-voltage level trip |
| phases | set |  | PUBLIC | CHARSET | S N C B A |  | The phases the inverter is attached to |
| use_multipoint_efficiency | bool |  | PUBLIC |  |  |  | FOUR QUADRANT MODEL: boolean to used the multipoint efficiency curve for the inverter when solar is attached |
| inverter_manufacturer | enumeration |  | PUBLIC |  | XANTREX SMA FRONIUS NONE |  | MULTIPOINT EFFICIENCY MODEL: the manufacturer of the inverter to setup up pre-existing efficiency curves |
| maximum_dc_power | double |  | PUBLIC |  |  |  | MULTIPOINT EFFICIENCY MODEL: the maximum dc power point for the efficiency curve |
| maximum_dc_voltage | double |  | PUBLIC |  |  |  | MULTIPOINT EFFICIENCY MODEL: the maximum dc voltage point for the efficiency curve |
| minimum_dc_power | double |  | PUBLIC |  |  |  | MULTIPOINT EFFICIENCY MODEL: the minimum dc power point for the efficiency curve |
| c_0 | double |  | PUBLIC |  |  |  | MULTIPOINT EFFICIENCY MODEL: the first coefficient in the efficienty curve |
| c_1 | double |  | PUBLIC |  |  |  | MULTIPOINT EFFICIENCY MODEL: the second coefficient in the efficienty curve |
| c_2 | double |  | PUBLIC |  |  |  | MULTIPOINT EFFICIENCY MODEL: the third coefficient in the efficienty curve |
| c_3 | double |  | PUBLIC |  |  |  | MULTIPOINT EFFICIENCY MODEL: the fourth coefficient in the efficienty curve |
| sense_object | object |  | PUBLIC |  |  |  | FOUR QUADRANT MODEL: name of the object the inverter is trying to mitigate the load on (node/link) in LOAD_FOLLOWING |
| max_charge_rate | double | W | PUBLIC |  |  |  | FOUR QUADRANT MODEL: maximum rate the battery can be charged in LOAD_FOLLOWING |
| max_discharge_rate | double | W | PUBLIC |  |  |  | FOUR QUADRANT MODEL: maximum rate the battery can be discharged in LOAD_FOLLOWING |
| charge_on_threshold | double | W | PUBLIC |  |  |  | FOUR QUADRANT MODEL: power level at which the inverter should try charging the battery in LOAD_FOLLOWING |
| charge_off_threshold | double | W | PUBLIC |  |  |  | FOUR QUADRANT MODEL: power level at which the inverter should cease charging the battery in LOAD_FOLLOWING |
| discharge_on_threshold | double | W | PUBLIC |  |  |  | FOUR QUADRANT MODEL: power level at which the inverter should try discharging the battery in LOAD_FOLLOWING |
| discharge_off_threshold | double | W | PUBLIC |  |  |  | FOUR QUADRANT MODEL: power level at which the inverter should cease discharging the battery in LOAD_FOLLOWING |
| excess_input_power | double | W | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Excess power at the input of the inverter that is otherwise just lost, or could be shunted to a battery |
| charge_lockout_time | double | s | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Lockout time when a charging operation occurs before another LOAD_FOLLOWING dispatch operation can occur |
| discharge_lockout_time | double | s | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Lockout time when a discharging operation occurs before another LOAD_FOLLOWING dispatch operation can occur |
| pf_reg_activate | double |  | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Lowest acceptable power-factor level below which power-factor regulation will activate. |
| pf_reg_deactivate | double |  | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Lowest acceptable power-factor above which no power-factor regulation is needed. |
| pf_target | double |  | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Desired power-factor to maintain (signed) positive is inductive |
| pf_reg_high | double |  | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Upper limit for power-factor - if exceeds, go full reverse reactive |
| pf_reg_low | double |  | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Lower limit for power-factor - if exceeds, stop regulating - pf_target_var is below this |
| pf_reg_activate_lockout_time | double | s | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Mandatory pause between the deactivation of power-factor regulation and it reactivation |
| disable_volt_var_if_no_input_power | bool |  | PUBLIC |  |  |  |  |
| delay_time | double | s | PUBLIC |  |  |  |  |
| max_var_slew_rate | double | VAr/s | PUBLIC |  |  |  |  |
| max_pwr_slew_rate | double | W/s | PUBLIC |  |  |  |  |
| volt_var_sched | char1024 |  | PUBLIC |  |  |  |  |
| freq_pwr_sched | char1024 |  | PUBLIC |  |  |  |  |
| charge_threshold | double | W | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Level at which all inverters in the group will begin charging attached batteries. Regulated minimum load level. |
| discharge_threshold | double | W | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Level at which all inverters in the group will begin discharging attached batteries. Regulated maximum load level. |
| group_max_charge_rate | double | W | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Sum of the charge rates of the batteries involved in the group load-following. |
| group_max_discharge_rate | double | W | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Sum of the discharge rates of the batteries involved in the group load-following. |
| group_rated_power | double | W | PUBLIC |  |  |  | FOUR QUADRANT MODEL: Sum of the inverter power ratings of the inverters involved in the group power-factor regulation. |
| V_base | double | V | PUBLIC |  |  |  | FOUR QUADRANT MODEL: The base voltage on the grid side of the inverter. Used in VOLT_VAR control mode. |
| V1 | double | pu | PUBLIC |  |  |  | FOUR QUADRANT MODEL: voltage point 1 in volt/var curve. Used in VOLT_VAR control mode. |
| Q1 | double | pu | PUBLIC |  |  |  | FOUR QUADRANT MODEL: VAR point 1 in volt/var curve. Used in VOLT_VAR control mode. |
| V2 | double | pu | PUBLIC |  |  |  | FOUR QUADRANT MODEL: voltage point 2 in volt/var curve. Used in VOLT_VAR control mode. |
| Q2 | double | pu | PUBLIC |  |  |  | FOUR QUADRANT MODEL: VAR point 2 in volt/var curve. Used in VOLT_VAR control mode. |
| V3 | double | pu | PUBLIC |  |  |  | FOUR QUADRANT MODEL: voltage point 3 in volt/var curve. Used in VOLT_VAR control mode. |
| Q3 | double | pu | PUBLIC |  |  |  | FOUR QUADRANT MODEL: VAR point 3 in volt/var curve. Used in VOLT_VAR control mode. |
| V4 | double | pu | PUBLIC |  |  |  | FOUR QUADRANT MODEL: voltage point 4 in volt/var curve. Used in VOLT_VAR control mode. |
| Q4 | double | pu | PUBLIC |  |  |  | FOUR QUADRANT MODEL: VAR point 4 in volt/var curve. Used in VOLT_VAR control mode. |
| volt_var_control_lockout | double | s | PUBLIC |  |  |  | FOUR QUADRANT QUADRANT MODEL: the lockout time between volt/var actions. |

### dc_dc_converter

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| dc_dc_converter_type | enumeration |  | PUBLIC |  | BUCK_BOOST BOOST BUCK |  |  |
| generator_mode | enumeration |  | PUBLIC |  | SUPPLY_DRIVEN CONSTANT_PF CONSTANT_PQ CONSTANT_V UNKNOWN |  |  |
| V_Out | complex | V | PUBLIC |  |  |  |  |
| I_Out | complex | A | PUBLIC |  |  |  |  |
| Vdc | complex | V | PUBLIC |  |  |  |  |
| VA_Out | complex | VA | PUBLIC |  |  |  |  |
| P_Out | double |  | PUBLIC |  |  |  |  |
| Q_Out | double |  | PUBLIC |  |  |  |  |
| service_ratio | double |  | PUBLIC |  |  |  |  |
| V_In | complex | V | PUBLIC |  |  |  |  |
| I_In | complex | A | PUBLIC |  |  |  |  |
| VA_In | complex | VA | PUBLIC |  |  |  |  |
| phases | set |  | PUBLIC | CHARSET | S N C B A |  |  |

### rectifier

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| rectifier_type | enumeration |  | PUBLIC |  | TWELVE_PULSE SIX_PULSE THREE_PULSE TWO_PULSE ONE_PULSE |  |  |
| generator_mode | enumeration |  | PUBLIC |  | SUPPLY_DRIVEN CONSTANT_PF CONSTANT_PQ CONSTANT_V UNKNOWN |  |  |
| V_Out | complex | V | PUBLIC |  |  |  |  |
| V_Rated | double | V | PUBLIC |  |  |  |  |
| I_Out | complex | A | PUBLIC |  |  |  |  |
| VA_Out | complex | VA | PUBLIC |  |  |  |  |
| P_Out | double |  | PUBLIC |  |  |  |  |
| Q_Out | double |  | PUBLIC |  |  |  |  |
| Vdc | complex | V | PUBLIC |  |  |  |  |
| voltage_A | complex | V | PUBLIC |  |  |  |  |
| voltage_B | complex | V | PUBLIC |  |  |  |  |
| voltage_C | complex | V | PUBLIC |  |  |  |  |
| current_A | complex | V | PUBLIC |  |  |  |  |
| current_B | complex | V | PUBLIC |  |  |  |  |
| current_C | complex | V | PUBLIC |  |  |  |  |
| power_A_In | complex | VA | PUBLIC |  |  |  |  |
| power_B_In | complex | VA | PUBLIC |  |  |  |  |
| power_C_In | complex | VA | PUBLIC |  |  |  |  |
| phases | set |  | PUBLIC | CHARSET | S N C B A |  |  |

### microturbine

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| generator_mode | enumeration |  | PUBLIC |  | SUPPLY_DRIVEN CONSTANT_PF CONSTANT_PQ CONSTANT_V UNKNOWN |  |  |
| generator_status | enumeration |  | PUBLIC |  | ONLINE OFFLINE |  |  |
| power_type | enumeration |  | PUBLIC |  | DC AC |  |  |
| Rinternal | double |  | PUBLIC |  |  |  |  |
| Rload | double |  | PUBLIC |  |  |  |  |
| V_Max | double | V | PUBLIC |  |  |  |  |
| I_Max | complex | A | PUBLIC |  |  |  |  |
| frequency | double | Hz | PUBLIC |  |  |  |  |
| Max_Frequency | double | Hz | PUBLIC |  |  |  |  |
| Min_Frequency | double | Hz | PUBLIC |  |  |  |  |
| Fuel_Used | double | kVA | PUBLIC |  |  |  |  |
| Heat_Out | double | kVA | PUBLIC |  |  |  |  |
| KV | double |  | PUBLIC |  |  |  |  |
| Power_Angle | double |  | PUBLIC |  |  |  |  |
| Max_P | double | kW | PUBLIC |  |  |  |  |
| Min_P | double | kW | PUBLIC |  |  |  |  |
| phaseA_V_Out | complex | kV | PUBLIC |  |  |  |  |
| phaseB_V_Out | complex | kV | PUBLIC |  |  |  |  |
| phaseC_V_Out | complex | kV | PUBLIC |  |  |  |  |
| phaseA_I_Out | complex | A | PUBLIC |  |  |  |  |
| phaseB_I_Out | complex | A | PUBLIC |  |  |  |  |
| phaseC_I_Out | complex | A | PUBLIC |  |  |  |  |
| power_A_Out | complex |  | PUBLIC |  |  |  |  |
| power_B_Out | complex |  | PUBLIC |  |  |  |  |
| power_C_Out | complex |  | PUBLIC |  |  |  |  |
| VA_Out | complex |  | PUBLIC |  |  |  |  |
| pf_Out | double |  | PUBLIC |  |  |  |  |
| E_A_Internal | complex |  | PUBLIC |  |  |  |  |
| E_B_Internal | complex |  | PUBLIC |  |  |  |  |
| E_C_Internal | complex |  | PUBLIC |  |  |  |  |
| efficiency | double |  | PUBLIC |  |  |  |  |
| Rated_kVA | double | kVA | PUBLIC |  |  |  |  |
| phases | set |  | PUBLIC | CHARSET | S N C B A |  |  |

### battery

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| generator_mode | enumeration |  | PUBLIC |  | POWER_VOLTAGE_HYBRID VOLTAGE_CONTROLLED POWER_DRIVEN SUPPLY_DRIVEN CONSTANT_PF CONSTANT_PQ CONSTANT_V UNKNOWN | UNKNOWN | LEGACY MODEL: Selects generator control mode when using legacy model; in non-legacy models, this should be SUPPLY_DRIVEN. |
| additional_controls | enumeration |  | PUBLIC |  | LINEAR_TEMPERATURE NONE | NONE | LEGACY MODEL: In conjunction with POWER_DRIVEN, VOLTAGE_CONTROLLED, and POWER_VOLTAGE_HYBRID, this will activate control set points that adjust with temperature |
| generator_status | enumeration |  | PUBLIC |  | ONLINE OFFLINE | ONLINE | describes whether the generator is online or offline |
| rfb_size | enumeration |  | PUBLIC |  | LARGE MED_HIGH_ENERGY MED_COMMERCIAL SMALL HOUSEHOLD |  | Default settings for certain sizes of batteries |
| power_type | enumeration |  | PUBLIC |  | DC AC |  | LEGACY MODEL: Not currently used |
| battery_state | enumeration |  | PUBLIC |  | CONFLICTED EMPTY FULL WAITING DISCHARGING CHARGING |  | Describes the current state of the battery |
| number_battery_state_changes | double |  | PUBLIC |  |  |  | LEGACY MODEL: Number of times battery switches between charging and discharging |
| monitored_power | double | W | PUBLIC |  |  |  | LEGACY MODEL: output only; power output value of parent meter when performing load following modes (POWER_DRIVEN) |
| power_set_high | double | W | PUBLIC |  |  |  | LEGACY MODEL: high set point of dead band for load following (POWER_DRIVEN) |
| power_set_low | double | W | PUBLIC |  |  |  | LEGACY MODEL: low set point of dead band for load following (POWER_DRIVEN) |
| power_set_high_highT | double | W | PUBLIC |  |  |  | LEGACY MODEL: high set point of dead band for load following at higher temperatures (POWER_DRIVEN + LINEAR_TEMPERATURE) |
| power_set_low_highT | double | W | PUBLIC |  |  |  | LEGACY MODEL: low set point of dead band for load following at higher temperatures (POWER_DRIVEN + LINEAR_TEMPERATURE) |
| check_power_low | double | W | PUBLIC |  |  |  | LEGACY MODEL: high set point of dead band for load following at lower temperatures (POWER_DRIVEN + LINEAR_TEMPERATURE) |
| check_power_high | double | W | PUBLIC |  |  |  | LEGACY MODEL: low set point of dead band for load following at lower temperatures (POWER_DRIVEN + LINEAR_TEMPERATURE) |
| voltage_set_high | double | V | PUBLIC |  |  |  | LEGACY MODEL: high set point for voltage control |
| voltage_set_low | double | V | PUBLIC |  |  |  | LEGACY MODEL: low set point for voltage control |
| deadband | double | V | PUBLIC |  |  | 0 V | LEGACY MODEL: voltage deadband |
| sensitivity | double |  | PUBLIC |  |  |  | LEGACY MODEL: describes how sensitive the control is to temperature excursions; defines slope of linear control |
| high_temperature | double | degF | PUBLIC |  |  | 100 degF | LEGACY MODEL: high temperature of linear control; defines slope |
| midpoint_temperature | double | degF | PUBLIC |  |  | 50 degF | LEGACY MODEL: midpoint temperature of linear control; defines slope |
| low_temperature | double | degF | PUBLIC |  |  | 0 degF | LEGACY MODEL: low temperature of linear control; defines slope |
| scheduled_power | double | W | PUBLIC |  |  |  | LEGACY MODEL: real power output of battery/inverter system |
| Rinternal | double | Ohm | PUBLIC |  |  | 10 Ohm | LEGACY MODEL: the internal resistance of the battery. |
| V_Max | double | V | PUBLIC |  |  | 0 V | LEGACY MODEL: the maximum terminal voltage of the battery. |
| I_Max | complex | A | PUBLIC |  |  | 0 A | LEGACY MODEL: the maximum current output of the battery. |
| E_Max | double | Wh | PUBLIC |  |  | 0 Wh | LEGACY MODEL: the maximum capacity of the battery. |
| P_Max | double | W | PUBLIC |  |  | 0 W | LEGACY MODEL: the maximum power output of the battery. |
| power_factor | double |  | PUBLIC |  |  |  | LEGACY MODEL: the power factor output of the battery. |
| Energy | double | Wh | PUBLIC |  |  | -1 | LEGACY MODEL: the available capacity of the battery. |
| efficiency | double | unit | PUBLIC |  |  | 0 unit | LEGACY MODEL: the efficiency of the battery. |
| base_efficiency | double | unit | PUBLIC |  |  | 0 unit | LEGACY MODEL: the efficiency of the battery at rated voltaged and current. |
| parasitic_power_draw | double | W | PUBLIC |  |  | 0 W | LEGACY MODEL: the parasytic power draw of the battery when idle. |
| Rated_kVA | double | kVA | PUBLIC | DEPRECATED |  | 1 kVA | LEGACY MODEL: the rated power of the battery. |
| V_Out | complex | V | PUBLIC |  |  |  | LEGACY MODEL: the AC voltage at the terminals of the battery. |
| I_Out | complex | A | PUBLIC |  |  |  | LEGACY MODEL: the AC current output of the battery. |
| VA_Out | complex | VA | PUBLIC |  |  |  | LEGACY MODEL: the power output of the battery. |
| V_In | complex | V | PUBLIC |  |  |  | LEGACY MODEL: the voltage at the terminals of the battery. |
| I_In | complex | A | PUBLIC |  |  |  | LEGACY MODEL: the current flowing into the battery of the battery. |
| V_Internal | complex | V | PUBLIC |  |  |  | LEGACY MODEL: the internal voltage of the battery. |
| I_Internal | complex | A | PUBLIC |  |  |  | LEGACY MODEL: the internal current of the battery. |
| I_Prev | complex | A | PUBLIC |  |  |  | LEGACY MODEL: the previous current output of the battery. |
| power_transferred | double |  | PUBLIC |  |  |  | LEGACY MODEL: the power output of the battery. |
| use_internal_battery_model | bool |  | PUBLIC |  |  | FALSE | Enables the INTERNAL BATTERY MODEL. |
| battery_type | enumeration |  | PUBLIC |  | LEAD_ACID LI_ION UNKNOWN |  | INTERNAL BATTERY MODEL: the type of the battery. Used to determine the soc vs voltage curve. |
| nominal_voltage | double | V | PUBLIC |  |  |  | INTERNAL BATTERY MODEL: the rated DC voltage at the terminals of the battery. |
| rated_power | double | W | PUBLIC |  |  |  | INTERNAL BATTERY MODEL: the rated power output of the battery. |
| battery_capacity | double | Wh | PUBLIC |  |  |  | INTERNAL BATTERY MODEL: the rated battery capacity of the battery. |
| round_trip_efficiency | double | pu | PUBLIC |  |  |  | INTERNAL BATTERY MODEL: the round trip efficiency of the battery according to a full discharge/charge cycle. |
| state_of_charge | double | pu | PUBLIC |  |  | -1 pu | INTERNAL BATTERY MODEL: the current state of charge of the battery. |
| battery_load | double | W | PUBLIC |  |  |  | INTERNAL BATTERY MODEL: the current power output of the battery. |
| reserve_state_of_charge | double | pu | PUBLIC |  |  | 0 pu | INTERNAL BATTERY MODEL: the reserve state of charge the battery can reach. |

### solar

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| generator_mode | enumeration |  | PUBLIC |  | SUPPLY_DRIVEN CONSTANT_PF CONSTANT_PQ CONSTANT_V UNKNOWN |  |  |
| generator_status | enumeration |  | PUBLIC |  | ONLINE OFFLINE |  |  |
| panel_type | enumeration |  | PUBLIC |  | CONCENTRATOR THIN_FILM_GA_AS AMORPHOUS_SILICON MULTI_CRYSTAL_SILICON SINGLE_CRYSTAL_SILICON |  |  |
| power_type | enumeration |  | PUBLIC |  | DC AC |  |  |
| INSTALLATION_TYPE | enumeration |  | PUBLIC |  | GROUND_MOUNTED ROOF_MOUNTED |  |  |
| SOLAR_TILT_MODEL | enumeration |  | PUBLIC |  | PLAYERVALUE SOLPOS DEFAULT | DEFAULT | solar tilt model used to compute insolation values |
| SOLAR_POWER_MODEL | enumeration |  | PUBLIC |  | FLATPLATE DEFAULT | DEFAULT |  |
| a_coeff | double |  | PUBLIC |  |  | -2.81 | a coefficient for module temperature correction formula |
| b_coeff | double | s/m | PUBLIC |  |  | -0.0455 s/m | b coefficient for module temperature correction formula |
| dT_coeff | double | m^2*degC/kW | PUBLIC |  |  | 0 m^2*degC/kW | Temperature difference coefficient for module temperature correction formula |
| T_coeff | double | %/degC | PUBLIC |  |  | -0.5 %/degC | Maximum power temperature coefficient for module temperature correction formula |
| NOCT | double | degF | PUBLIC |  |  | 118.4 degF | Nominal operating cell temperature NOCT in deg F |
| Tmodule | double | degF | PUBLIC |  |  |  | Temperature of PV module |
| Tambient | double | degC | PUBLIC |  |  | 25 degC | Ambient temperature for cell efficiency calculations |
| wind_speed | double | mph | PUBLIC |  |  | 0 mph | Wind speed |
| ambient_temperature | double | degF | PUBLIC |  |  | 77 degF | Current ambient temperature of air from climate data |
| Insolation | double | W/sf | PUBLIC |  |  | 0 W/sf |  |
| Rinternal | double | Ohm | PUBLIC |  |  | 0.05 Ohm |  |
| Rated_Insolation | double | W/sf | PUBLIC |  |  | 92.902 W/sf |  |
| Pmax_temp_coeff | double |  | PUBLIC |  |  |  | temperature coefficient of rated power in %/degC |
| Voc_temp_coeff | double |  | PUBLIC |  |  |  |  |
| V_Max | complex | V | PUBLIC |  |  | 27.1+0i | Vmax of solar module found on specs |
| Voc_Max | complex | V | PUBLIC |  |  | 34+0i | Voc max of solar module |
| Voc | complex | V | PUBLIC |  |  | 34+0i |  |
| efficiency | double | unit | PUBLIC |  |  |  |  |
| area | double | sf | PUBLIC |  |  |  | solar panel area |
| soiling | double | pu | PUBLIC |  |  | 0.95 pu | Soiling of array factor - representing dirt on array |
| derating | double | pu | PUBLIC |  |  | 0.95 pu | Panel derating to account for manufacturing variances |
| Tcell | double | degC | PUBLIC |  |  | 69.8 degF |  |
| Rated_kVA | double | kVA | PUBLIC | DEPRECATED |  |  | This variable has issues with inconsistent handling in the code, so we will deprecate this in the future (VA maps to kVA, for example). |
| rated_power | double | W | PUBLIC |  |  |  | Used to define the size of the solar panel in power rather than square footage. |
| P_Out | complex | kW | PUBLIC |  |  |  |  |
| V_Out | complex | V | PUBLIC |  |  |  |  |
| I_Out | complex | A | PUBLIC |  |  |  |  |
| VA_Out | complex | VA | PUBLIC |  |  |  |  |
| weather | object |  | PUBLIC |  |  |  |  |
| shading_factor | double | pu | PUBLIC |  |  | 1 pu | Shading factor for scaling solar power to the array |
| tilt_angle | double | deg | PUBLIC |  |  | 45 deg | Tilt angle of PV array |
| orientation_azimuth | double | deg | PUBLIC |  |  | 180 deg | Facing direction of the PV array |
| latitude_angle_fix | bool |  | PUBLIC |  |  | FALSE | Fix tilt angle to installation latitude value |
| orientation | enumeration |  | PUBLIC |  | FIXED_AXIS DEFAULT | DEFAULT |  |
| phases | set |  | PUBLIC | RECALC|CHARSET|EXTENDED|DEPRECATED|REQUIRED|OUTPUT|DYNAMIC | S N C B A |  |  |

### central_dg_control

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| controlled_dgs | char32 |  | PUBLIC |  |  |  | the group ID of the dg objects the controller controls. |
| feederhead_meter | object |  | PUBLIC |  |  |  | the name of the meter. |
| control_mode_0 | enumeration |  | PUBLIC |  | PEAK_SHAVING CONSTANT_PF NO_CONTROL |  |  |
| control_mode_1 | enumeration |  | PUBLIC |  | PEAK_SHAVING CONSTANT_PF NO_CONTROL |  |  |
| control_mode_2 | enumeration |  | PUBLIC |  | PEAK_SHAVING CONSTANT_PF NO_CONTROL |  |  |
| control_mode_3 | enumeration |  | PUBLIC |  | PEAK_SHAVING CONSTANT_PF NO_CONTROL |  |  |
| peak_S | double | W | PUBLIC |  |  |  |  |
| pf_low | double | unit | PUBLIC |  |  |  |  |
| pf_high | double | unit | PUBLIC |  |  |  |  |

### controller_dg

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| groupid_dg | char32 |  | PUBLIC |  |  |  | the group ID of the dg objects the controller controls. |
| ki | double |  | PUBLIC |  |  |  | parameter of the propotional control |
| kp | double |  | PUBLIC |  |  |  | parameter of the integral control |
| gain | double |  | PUBLIC |  |  |  | Gain of the controller |
| ki_QV | double |  | PUBLIC |  |  |  | parameter of the propotional control for secondary voltage control |
| kp_QV | double |  | PUBLIC |  |  |  | parameter of the integral control for secondary voltage control |
| gain_QV | double |  | PUBLIC |  |  |  | Gain of the controller for secondary voltage control |

### industrial

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| naics_code | int32 |  | PUBLIC | REQUIRED |  | 0 | NAICS facility code |
| power_pf | double |  | PUBLIC |  |  | 1.0 | power factor for constant power load |
| impedance_pf | double |  | PUBLIC |  |  | 1.0 | power factor for constant impedance load |
| operating_factor | double | pu | PUBLIC |  |  |  | fraction of facility power capacity that is operating |
| operating_capacity | double | MW | PUBLIC |  |  |  | facility power when operating at full capacity |
| total_power | double | W | PUBLIC | OUTPUT |  | 0 MW | total facility power demand |

### database

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| username | char32 |  | PUBLIC |  |  |  | user name |
| password | char32 |  | PUBLIC |  |  |  | user password |
| hostname | char256 |  | PUBLIC |  |  |  | server hostname |
| port | int32 |  | PUBLIC |  |  |  | TCP port number |
| database | char256 |  | PUBLIC |  |  |  | database name |
| options | set |  | PUBLIC |  | OVERWRITE NEWDB NOCREATE SHOWQUERY NONE |  | destroy existing files before output dump/backup results (risky) |
| sync_interval | double | s | PUBLIC |  |  |  | interval at which on_sync is called |
| tz_offset | int32 |  | PUBLIC |  |  |  | timezone offset used by timestamp in the database |
| uses_dst | bool |  | PUBLIC |  |  |  | timestamps in database include summer time offsets |
| logname | char32 |  | PUBLIC |  |  |  | name of log table |
| logtag | method |  | PUBLIC |  |  |  | property tag add method |

### recorder

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| trigger | char32 |  | PUBLIC |  |  |  | condition to trigger recorder |
| file | char1024 |  | PUBLIC | REQUIRED |  |  | file in which to record data |
| filetype | char8 |  | PUBLIC |  |  |  | file type to use when storing data |
| mode | char32 |  | PUBLIC |  |  | file | recorder operating mode |
| multifile | char1024 |  | PUBLIC |  |  |  |  |
| limit | int32 |  | PUBLIC |  |  | 0 | limit on number of rows to record (0=none) |
| plotcommands | char1024 |  | PUBLIC |  |  |  |  |
| xdata | char32 |  | PUBLIC |  |  |  |  |
| columns | char32 |  | PUBLIC |  |  |  |  |
| flush | int32 |  | PUBLIC |  |  | -1 | row flush mode (-1=when full, 0=every row, >0=interval in seconds) |
| format | bool |  | PUBLIC |  |  | 0 |  |
| interval | double | s | PUBLIC |  |  | -1 s | sampling interval |
| strftime_format | char256 |  | PUBLIC |  |  |  | date/time format |
| property | method |  | PUBLIC | REQUIRED |  |  | list of properties to sample |
| output | enumeration |  | PUBLIC |  | SVG PNG PDF JPG GIF EPS SCREEN |  |  |
| header_units | enumeration |  | PUBLIC |  | NONE ALL DEFAULT | DEFAULT | header option for output of units |
| line_units | enumeration |  | PUBLIC |  | NONE ALL DEFAULT | DEFAULT | inline option for output of units |

### auction

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| unit | char32 |  | PUBLIC |  |  |  | unit of quantity |
| period | double | s | PUBLIC |  |  |  | interval of time between market clearings |
| latency | double | s | PUBLIC |  |  |  | latency between market clearing and delivery |
| market_id | int64 |  | REFERENCE |  |  |  | unique identifier of market clearing |
| network | object |  | PUBLIC |  |  |  | the comm network used by object to talk to the market (if any) |
| verbose | bool |  | PUBLIC |  |  |  | enable verbose auction operations |
| linkref | object |  | PUBLIC | DEPRECATED |  |  | reference to link object that has demand as power_out (only used when not all loads are bidding) |
| pricecap | double |  | PUBLIC | DEPRECATED |  |  | the maximum price (magnitude) allowed |
| price_cap | double |  | PUBLIC |  |  |  | the maximum price (magnitude) allowed |
| special_mode | enumeration |  | PUBLIC |  | BUYERS_ONLY SELLERS_ONLY NONE |  |  |
| statistic_mode | enumeration |  | PUBLIC |  | OFF ON |  |  |
| fixed_price | double |  | PUBLIC |  |  |  |  |
| fixed_quantity | double |  | PUBLIC |  |  |  |  |
| capacity_reference_object | object |  | PUBLIC |  |  |  |  |
| capacity_reference_property | char32 |  | PUBLIC |  |  |  |  |
| capacity_reference_bid_price | double |  | PUBLIC |  |  |  |  |
| max_capacity_reference_bid_quantity | double |  | PUBLIC |  |  |  |  |
| capacity_reference_bid_quantity | double |  | PUBLIC |  |  |  |  |
| init_price | double |  | PUBLIC |  |  |  |  |
| init_stdev | double |  | PUBLIC |  |  |  |  |
| future_mean_price | double |  | PUBLIC |  |  |  |  |
| use_future_mean_price | bool |  | PUBLIC |  |  |  |  |
| current_market.start_time | timestamp |  | PUBLIC |  |  |  |  |
| current_market.end_time | timestamp |  | PUBLIC |  |  |  |  |
| current_market.clearing_price | double | $ | PUBLIC |  |  |  |  |
| current_market.clearing_quantity | double |  | PUBLIC |  |  |  |  |
| current_market.clearing_type | enumeration |  | PUBLIC |  | NULL FAILURE EXACT MARGINAL_PRICE MARGINAL_BUYER MARGINAL_SELLER |  |  |
| current_market.marginal_quantity_load | double |  | PUBLIC |  |  |  |  |
| current_market.marginal_quantity | double |  | PUBLIC |  |  |  |  |
| current_market.marginal_quantity_bid | double |  | PUBLIC |  |  |  |  |
| current_market.marginal_quantity_frac | double |  | PUBLIC |  |  |  |  |
| current_market.seller_total_quantity | double |  | PUBLIC |  |  |  |  |
| current_market.buyer_total_quantity | double |  | PUBLIC |  |  |  |  |
| current_market.seller_min_price | double |  | PUBLIC |  |  |  |  |
| current_market.buyer_total_unrep | double |  | PUBLIC |  |  |  |  |
| current_market.cap_ref_unrep | double |  | PUBLIC |  |  |  |  |
| next_market.start_time | timestamp |  | PUBLIC |  |  |  |  |
| next_market.end_time | timestamp |  | PUBLIC |  |  |  |  |
| next_market.clearing_price | double | $ | PUBLIC |  |  |  |  |
| next_market.clearing_quantity | double |  | PUBLIC |  |  |  |  |
| next_market.clearing_type | enumeration |  | PUBLIC |  | NULL FAILURE EXACT MARGINAL_PRICE MARGINAL_BUYER MARGINAL_SELLER |  |  |
| next_market.marginal_quantity_load | double |  | PUBLIC |  |  |  |  |
| next_market.marginal_quantity_bid | double |  | PUBLIC |  |  |  |  |
| next_market.marginal_quantity_frac | double |  | PUBLIC |  |  |  |  |
| next_market.seller_total_quantity | double |  | PUBLIC |  |  |  |  |
| next_market.buyer_total_quantity | double |  | PUBLIC |  |  |  |  |
| next_market.seller_min_price | double |  | PUBLIC |  |  |  |  |
| next_market.cap_ref_unrep | double |  | PUBLIC |  |  |  |  |
| past_market.start_time | timestamp |  | PUBLIC |  |  |  |  |
| past_market.end_time | timestamp |  | PUBLIC |  |  |  |  |
| past_market.clearing_price | double | $ | PUBLIC |  |  |  |  |
| past_market.clearing_quantity | double |  | PUBLIC |  |  |  |  |
| past_market.clearing_type | enumeration |  | PUBLIC |  | NULL FAILURE EXACT MARGINAL_PRICE MARGINAL_BUYER MARGINAL_SELLER |  |  |
| past_market.marginal_quantity_load | double |  | PUBLIC |  |  |  |  |
| past_market.marginal_quantity_bid | double |  | PUBLIC |  |  |  |  |
| past_market.marginal_quantity_frac | double |  | PUBLIC |  |  |  |  |
| past_market.seller_total_quantity | double |  | PUBLIC |  |  |  |  |
| past_market.buyer_total_quantity | double |  | PUBLIC |  |  |  |  |
| past_market.seller_min_price | double |  | PUBLIC |  |  |  |  |
| past_market.cap_ref_unrep | double |  | PUBLIC |  |  |  |  |
| margin_mode | enumeration |  | PUBLIC |  | PROB DENY NORMAL |  |  |
| warmup | int32 |  | PUBLIC |  |  |  |  |
| ignore_failed_market | enumeration |  | PUBLIC |  | TRUE FALSE |  |  |
| ignore_pricecap | enumeration |  | PUBLIC |  | TRUE FALSE |  |  |
| transaction_log_file | char256 |  | PUBLIC |  |  |  |  |
| transaction_log_limit | int32 |  | PUBLIC |  |  |  |  |
| curve_log_file | char256 |  | PUBLIC |  |  |  |  |
| curve_log_limit | int32 |  | PUBLIC |  |  |  |  |
| curve_log_info | enumeration |  | PUBLIC |  | EXTRA NORMAL |  |  |

### controller

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| simple_mode | enumeration |  | PUBLIC |  | DOUBLE_RAMP WATERHEATER HOUSE_PRECOOL HOUSE_PREHEAT HOUSE_COOL HOUSE_HEAT NONE |  |  |
| bid_mode | enumeration |  | PUBLIC |  | PROXY OFF ON |  |  |
| use_override | enumeration |  | PUBLIC |  | ON OFF |  |  |
| ramp_low | double | degF | PUBLIC |  |  |  | the comfort response below the setpoint |
| ramp_high | double | degF | PUBLIC |  |  |  | the comfort response above the setpoint |
| range_low | double |  | PUBLIC |  |  |  | the setpoint limit on the low side |
| range_high | double |  | PUBLIC |  |  |  | the setpoint limit on the high side |
| target | char32 |  | PUBLIC |  |  |  | the observed property (e.g., air temperature) |
| setpoint | char32 |  | PUBLIC |  |  |  | the controlled property (e.g., heating setpoint) |
| demand | char32 |  | PUBLIC |  |  |  | the controlled load when on |
| load | char32 |  | PUBLIC |  |  |  | the current controlled load |
| total | char32 |  | PUBLIC |  |  |  | the uncontrolled load (if any) |
| market | char32 |  | PUBLIC |  |  |  | the market to bid into |
| state | char32 |  | PUBLIC |  |  |  | the state property of the controlled load |
| avg_target | char32 |  | PUBLIC |  |  |  |  |
| std_target | char32 |  | PUBLIC |  |  |  |  |
| bid_price | double |  | REFERENCE |  |  |  | the bid price |
| bid_quantity | double |  | REFERENCE |  |  |  | the bid quantity |
| set_temp | double | degF | REFERENCE |  |  |  | the reset value |
| base_setpoint | double | degF | PUBLIC |  |  |  |  |
| market_price | double |  | PUBLIC |  |  |  | the current market clearing price seen by the controller. |
| period | double | s | PUBLIC |  |  |  | interval of time between market clearings |
| control_mode | enumeration |  | PUBLIC |  | DEV_LEVEL DOUBLE_RAMP RAMP |  |  |
| resolve_mode | enumeration |  | PUBLIC |  | SLIDING DEADBAND |  |  |
| slider_setting | double |  | PUBLIC |  |  |  |  |
| slider_setting_heat | double |  | PUBLIC |  |  |  |  |
| slider_setting_cool | double |  | PUBLIC |  |  |  |  |
| override | char32 |  | PUBLIC |  |  |  |  |
| heating_range_high | double | degF | PUBLIC |  |  |  |  |
| heating_range_low | double | degF | PUBLIC |  |  |  |  |
| heating_ramp_high | double |  | PUBLIC |  |  |  |  |
| heating_ramp_low | double |  | PUBLIC |  |  |  |  |
| cooling_range_high | double | degF | PUBLIC |  |  |  |  |
| cooling_range_low | double | degF | PUBLIC |  |  |  |  |
| cooling_ramp_high | double |  | PUBLIC |  |  |  |  |
| cooling_ramp_low | double |  | PUBLIC |  |  |  |  |
| heating_base_setpoint | double | degF | PUBLIC |  |  |  |  |
| cooling_base_setpoint | double | degF | PUBLIC |  |  |  |  |
| deadband | char32 |  | PUBLIC |  |  |  |  |
| heating_setpoint | char32 |  | PUBLIC |  |  |  |  |
| heating_demand | char32 |  | PUBLIC |  |  |  |  |
| cooling_setpoint | char32 |  | PUBLIC |  |  |  |  |
| cooling_demand | char32 |  | PUBLIC |  |  |  |  |
| sliding_time_delay | double | s | PUBLIC |  |  |  | time interval desired for the sliding resolve mode to change from cooling or heating to off |
| use_predictive_bidding | bool |  | PUBLIC |  |  |  |  |
| device_actively_engaged | double |  | PUBLIC |  |  |  |  |
| cleared_market | int32 |  | PUBLIC |  |  |  |  |
| locked | int32 |  | PUBLIC |  |  |  |  |
| p_ON | double |  | PUBLIC |  |  |  |  |
| p_OFF | double |  | PUBLIC |  |  |  |  |
| p_ONLOCK | double |  | PUBLIC |  |  |  |  |
| p_OFFLOCK | double |  | PUBLIC |  |  |  |  |
| delta_u | double |  | PUBLIC |  |  |  |  |
| regulation_market_on | char32 |  | PUBLIC |  |  |  | the willing to change state from ON->OFF market to bid into for regulation case |
| regulation_market_off | char32 |  | PUBLIC |  |  |  | the willing to change state from OFF->ON market to bid into for regulation case |
| fast_regulation_signal | double | s | PUBLIC |  |  |  | the regulation signal that the controller compares against (i.e., how often is there a control action |
| market_price_on | double |  | PUBLIC |  |  |  | the current market clearing price seen by the controller in ON->OFF regulation market |
| market_price_off | double |  | PUBLIC |  |  |  | the current market clearing price seen by the controller  in OFF->ON regulation market |
| period_on | double | s | PUBLIC |  |  |  | interval of time between market clearings in ON->OFF regulation market |
| period_off | double | s | PUBLIC |  |  |  | interval of time between market clearings in  OFF->ON regulation market |
| regulation_period | int32 |  | PUBLIC |  |  |  | fast regulation signal period |
| r1 | double |  | PUBLIC |  |  |  | temporary diagnostic variable |
| mu0 | double |  | PUBLIC |  |  |  | temporary diagnostic variable |
| mu1 | double |  | PUBLIC |  |  |  | temporary diagnostic variable |
| average_target | char32 |  | PUBLIC |  |  |  |  |
| standard_deviation_target | char32 |  | PUBLIC |  |  |  |  |
| bid_id | int64 |  | PUBLIC |  |  |  |  |
| bid_delay | int32 |  | PUBLIC |  |  |  |  |
| thermostat_state | char32 |  | PUBLIC |  |  |  | The name of the thermostat state property within the object being controlled |
| proxy_average | double |  | PUBLIC |  |  |  |  |
| proxy_standard_deviation | double |  | PUBLIC |  |  |  |  |
| proxy_market_id | int64 |  | PUBLIC |  |  |  |  |
| proxy_market2_id | int64 |  | PUBLIC |  |  |  |  |
| proxy_clear_price | double | $ | PUBLIC |  |  |  |  |
| proxy_clear_price2 | double | $ | PUBLIC |  |  |  |  |
| proxy_price_cap | double |  | PUBLIC |  |  |  |  |
| proxy_price_cap2 | double |  | PUBLIC |  |  |  |  |
| proxy_market_unit | char32 |  | PUBLIC |  |  |  |  |
| proxy_initial_price | double |  | PUBLIC |  |  |  |  |
| proxy_marginal_fraction | double |  | PUBLIC |  |  |  |  |
| proxy_marginal_fraction2 | double |  | PUBLIC |  |  |  |  |
| proxy_clearing_quantity | double |  | PUBLIC |  |  |  |  |
| proxy_clearing_quantity2 | double |  | PUBLIC |  |  |  |  |
| proxy_seller_total_quantity | double |  | PUBLIC |  |  |  |  |
| proxy_seller_total_quantity2 | double |  | PUBLIC |  |  |  |  |
| proxy_margin_mode | enumeration |  | PUBLIC |  | PROB DENY NORMAL |  |  |
| proxy_clearing_type | enumeration |  | PUBLIC |  | NULL FAILURE EXACT MARGINAL_PRICE MARGINAL_BUYER MARGINAL_SELLER |  |  |
| proxy_clearing_type2 | enumeration |  | PUBLIC |  | NULL FAILURE EXACT MARGINAL_PRICE MARGINAL_BUYER MARGINAL_SELLER |  |  |

### stubauction

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| unit | char32 |  | PUBLIC |  |  |  | unit of quantity |
| period | double | s | PUBLIC |  |  |  | interval of time between market clearings |
| last.P | double |  | PUBLIC |  |  |  | last cleared price |
| current_market.clearing_price | double |  | PUBLIC |  |  |  | next cleared price |
| past_market.clearing_price | double |  | PUBLIC |  |  |  | last cleared price |
| next.P | double |  | PUBLIC |  |  |  | next cleared price |
| avg24 | double |  | PUBLIC |  |  |  | daily average of price |
| std24 | double |  | PUBLIC |  |  |  | daily stdev of price |
| avg72 | double |  | PUBLIC |  |  |  | three day price average |
| std72 | double |  | PUBLIC |  |  |  | three day price stdev |
| avg168 | double |  | PUBLIC |  |  |  | weekly average of price |
| std168 | double |  | PUBLIC |  |  |  | weekly stdev of price |
| market_id | int64 |  | REFERENCE |  |  |  | unique identifier of market clearing |
| verbose | bool |  | PUBLIC |  |  |  | enable verbose stubauction operations |
| control_mode | enumeration |  | PUBLIC |  | DISABLED NORMAL |  | the control mode to use for determining average and deviation calculations |

### passive_controller

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| input_state | int32 |  | PUBLIC |  |  |  |  |
| input_setpoint | double |  | PUBLIC |  |  |  |  |
| input_chained | bool |  | PUBLIC |  |  |  |  |
| observation | double |  | REFERENCE |  |  |  | the observed value |
| mean_observation | double |  | REFERENCE |  |  |  | the observed mean value |
| stdev_observation | double |  | REFERENCE |  |  |  | the observed standard deviation value |
| expectation | double |  | REFERENCE |  |  |  | the observed expected value |
| sensitivity | double |  | PUBLIC | DEPRECATED |  |  | the sensitivity of the control actuator to observation deviations |
| period | double | s | PUBLIC |  |  |  | the cycle period for the controller logic |
| expectation_prop | char32 |  | PUBLIC | DEPRECATED |  |  | the name of the property to observe for the expected value |
| expectation_obj | object |  | PUBLIC | DEPRECATED |  |  | the object to watch for the expectation property |
| expectation_property | char32 |  | PUBLIC |  |  |  | the name of the property to observe for the expected value |
| expectation_object | object |  | PUBLIC |  |  |  | the object to watch for the expectation property |
| setpoint_prop | char32 |  | PUBLIC | DEPRECATED |  |  | the name of the setpoint property in the parent object |
| setpoint | char32 |  | PUBLIC |  |  |  | the name of the setpoint property in the parent object |
| state_prop | char32 |  | PUBLIC | DEPRECATED |  |  | the name of the actuator property in the parent object |
| state_property | char32 |  | PUBLIC |  |  |  | the name of the actuator property in the parent object |
| observation_obj | object |  | PUBLIC | DEPRECATED |  |  | the object to observe |
| observation_prop | char32 |  | PUBLIC | DEPRECATED |  |  | the name of the observation property |
| observation_object | object |  | PUBLIC |  |  |  | the object to observe |
| observation_property | char32 |  | PUBLIC |  |  |  | the name of the observation property |
| mean_observation_prop | char32 |  | PUBLIC | DEPRECATED |  |  | the name of the mean observation property |
| stdev_observation_prop | char32 |  | PUBLIC | DEPRECATED |  |  | the name of the standard deviation observation property |
| stdev_observation_property | char32 |  | PUBLIC |  |  |  | the name of the standard deviation observation property |
| cycle_length | int32 |  | PUBLIC | DEPRECATED |  |  | length of time between processing cycles |
| base_setpoint | double |  | PUBLIC |  |  |  | the base setpoint to base control off of |
| critical_day | double |  | PUBLIC |  |  |  | used to switch between TOU and CPP days, 1 is CPP, 0 is TOU |
| two_tier_cpp | bool |  | PUBLIC |  |  |  |  |
| daily_elasticity | double |  | PUBLIC |  |  |  |  |
| sub_elasticity_first_second | double |  | PUBLIC |  |  |  |  |
| sub_elasticity_first_third | double |  | PUBLIC |  |  |  |  |
| second_tier_hours | int32 |  | PUBLIC |  |  |  |  |
| third_tier_hours | int32 |  | PUBLIC |  |  |  |  |
| first_tier_hours | int32 |  | PUBLIC |  |  |  |  |
| first_tier_price | double |  | PUBLIC |  |  |  |  |
| second_tier_price | double |  | PUBLIC |  |  |  |  |
| third_tier_price | double |  | PUBLIC |  |  |  |  |
| old_first_tier_price | double |  | PUBLIC |  |  |  |  |
| old_second_tier_price | double |  | PUBLIC |  |  |  |  |
| old_third_tier_price | double |  | PUBLIC |  |  |  |  |
| Percent_change_in_price | double |  | PUBLIC |  |  |  |  |
| Percent_change_in_peakoffpeak_ratio | double |  | PUBLIC |  |  |  |  |
| Percent_change_in_Criticalpeakoffpeak_ratio | double |  | PUBLIC |  |  |  |  |
| linearize_elasticity | bool |  | PUBLIC |  |  |  |  |
| price_offset | double |  | PUBLIC |  |  |  |  |
| pool_pump_model | bool |  | PUBLIC |  |  |  | Boolean flag for turning on the pool pump version of the DUTYCYCLE control |
| base_duty_cycle | double |  | PUBLIC |  |  |  | This is the duty cycle before modification due to the price signal |
| trigger_time_under_frequency | int32 |  | PUBLIC |  |  |  | Time to stay in triggered off state in seconds |
| trigger_time_over_frequency | int32 |  | PUBLIC |  |  |  | Time to stay in triggered on state in seconds |
| release_time_under_frequency | int32 |  | PUBLIC |  |  |  | Time to stay in released on state in seconds |
| release_time_over_frequency | int32 |  | PUBLIC |  |  |  | Time to stay in released off state in seconds |
| release_point_under_frequency | double |  | PUBLIC |  |  |  | Frequency value for releasing GFA in under frequency mode |
| release_point_over_frequency | double |  | PUBLIC |  |  |  | Frequency value for releasing GFA in over frequency mode |
| trigger_point_under_frequency | double |  | PUBLIC |  |  |  | Frequency value for triggereing GFA in under frequency mode |
| trigger_point_over_frequency | double |  | PUBLIC |  |  |  | Frequency value for triggereing GFA in over frequency mode |
| frequency | double |  | PUBLIC |  |  |  | Frequency value |
| PFC_mode | enumeration |  | PUBLIC |  | OVER_UNDER_FREQUENCY UNDER_FREQUENCY OVER_FREQUENCY |  | operation mode of the primary frequency controller |
| PFC_state | enumeration |  | PUBLIC |  | RELEASED_ON RELEASED_OFF FORCED_ON FORCED_OFF TRIGGERED_ON TRIGGERED_OFF FREE |  | State of the primary frequency controller |
| state_observed | char32 |  | PUBLIC |  |  |  | the name of the observed state property in the parent object |
| power_observed | char32 |  | PUBLIC |  |  |  | the name of the observed state property in the parent object |
| output_observed | int32 |  | PUBLIC |  |  |  |  |
| bid_delay | int32 |  | PUBLIC |  |  |  | time the controller will bid in advance before clearing |
| voltage_lockout | double | % | PUBLIC |  |  |  | lockout primary frequency control if voltage is deviating % from nominal |
| voltage_lockout_time | double | s | PUBLIC |  |  |  | voltage lockout period |
| voltage_lockout_state | int32 |  | PUBLIC |  |  |  | value to determine if water heater is in voltage lockout |
| distribution_type | enumeration |  | PUBLIC |  | UNIFORM EXPONENTIAL NORMAL |  |  |
| comfort_level | double |  | PUBLIC |  |  |  |  |
| range_high | double |  | PUBLIC |  |  |  |  |
| range_low | double |  | PUBLIC |  |  |  |  |
| ramp_high | double |  | PUBLIC |  |  |  |  |
| ramp_low | double |  | PUBLIC |  |  |  |  |
| prob_off | double |  | PUBLIC |  |  |  |  |
| output_state | int32 |  | REFERENCE |  |  |  | the target setpoint given the input observations |
| output_setpoint | double |  | PUBLIC |  |  |  |  |
| control_mode | enumeration |  | PUBLIC |  | PRIMARY_FREQUENCY_CONTROL DIRECT_LOAD_CONTROL ELASTICITY_MODEL PROBABILITY_OFF DUTYCYCLE RAMP NONE |  | the control mode to use for determining controller action |
| dlc_mode | enumeration |  | PUBLIC |  | CYCLING OFF |  | this mode is roughly designed to force cycle an AC unit |
| cycle_length_off | double | s | PUBLIC |  |  |  |  |
| cycle_length_on | double | s | PUBLIC |  |  |  |  |

### double_controller

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| thermostat_mode | enumeration |  | PUBLIC |  | COOL HEAT OFF INVALID |  |  |
| last_mode | enumeration |  | REFERENCE |  | COOL HEAT OFF INVALID |  |  |
| resolve_mode | enumeration |  | PUBLIC |  | STICKY DEADBAND NONE |  |  |
| setup_mode | enumeration |  | PUBLIC |  | HOUSE NONE |  |  |
| bid_mode | enumeration |  | PUBLIC |  | OFF ON |  |  |
| last_mode_timer | int64 |  | PUBLIC |  |  |  |  |
| cool_ramp_low | double |  | PUBLIC |  |  |  |  |
| cool_ramp_high | double |  | PUBLIC |  |  |  |  |
| cool_range_low | double |  | PUBLIC |  |  |  |  |
| cool_range_high | double |  | PUBLIC |  |  |  |  |
| cool_set_base | double |  | PUBLIC |  |  |  |  |
| cool_setpoint | double |  | PUBLIC |  |  |  |  |
| heat_ramp_low | double |  | PUBLIC |  |  |  |  |
| heat_ramp_high | double |  | PUBLIC |  |  |  |  |
| heat_range_low | double |  | PUBLIC |  |  |  |  |
| heat_range_high | double |  | PUBLIC |  |  |  |  |
| heat_set_base | double |  | PUBLIC |  |  |  |  |
| heat_setpoint | double |  | PUBLIC |  |  |  |  |
| temperature_name | char32 |  | PUBLIC |  |  |  |  |
| cool_setpoint_name | char32 |  | PUBLIC |  |  |  |  |
| cool_demand_name | char32 |  | PUBLIC |  |  |  |  |
| heat_setpoint_name | char32 |  | PUBLIC |  |  |  |  |
| heat_demand_name | char32 |  | PUBLIC |  |  |  |  |
| load_name | char32 |  | PUBLIC |  |  |  |  |
| total_load_name | char32 |  | PUBLIC |  |  |  |  |
| deadband_name | char32 |  | PUBLIC |  |  |  |  |
| state_name | char32 |  | PUBLIC |  |  |  |  |
| market | object |  | PUBLIC |  |  |  | the market to bid into |
| market_period | double |  | PUBLIC |  |  |  |  |
| bid_price | double |  | REFERENCE |  |  |  | the bid price |
| bid_quant | double |  | REFERENCE |  |  |  | the bid quantity |
| load | char32 |  | PUBLIC |  |  |  | the current controlled load |
| total | char32 |  | PUBLIC |  |  |  | the uncontrolled load (if any) |
| last_price | double |  | PUBLIC |  |  |  |  |
| temperature | double |  | PUBLIC |  |  |  |  |
| cool_bid | double |  | PUBLIC |  |  |  |  |
| heat_bid | double |  | PUBLIC |  |  |  |  |
| cool_demand | double |  | PUBLIC |  |  |  |  |
| heat_demand | double |  | PUBLIC |  |  |  |  |
| price | double |  | PUBLIC |  |  |  |  |
| avg_price | double |  | PUBLIC |  |  |  |  |
| stdev_price | double |  | PUBLIC |  |  |  |  |

### stub_bidder

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| bid_period | double | s | PUBLIC |  |  |  |  |
| count | int16 |  | PUBLIC |  |  |  |  |
| market | object |  | PUBLIC |  |  |  |  |
| role | enumeration |  | PUBLIC |  | SELLER BUYER |  |  |
| price | double |  | PUBLIC |  |  |  |  |
| quantity | double |  | PUBLIC |  |  |  |  |
| bid_id | int64 |  | PUBLIC |  |  |  |  |

### generator_controller

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| generator_state | enumeration |  | PUBLIC |  | STARTING RUNNING OFF |  | Current generator state |
| amortization_type | enumeration |  | PUBLIC |  | LINEAR_BID LINEAR_COST EXPONENTIAL |  | Amortization compounding method |
| generator_state_number | int32 |  | PUBLIC |  |  |  | Current generator state as numeric value |
| market | object |  | PUBLIC |  |  |  | Market the object will watch and bid into |
| bid_curve | char1024 |  | PUBLIC |  |  |  | Bidding curve text format |
| bid_curve_file | char1024 |  | PUBLIC |  |  |  | Bidding curve file name |
| bid_generator_rating | double | VA | PUBLIC |  |  |  | Size of generator in VA for the bid curve |
| update_curve | bool |  | PUBLIC |  |  |  | Flag to force updating of bidding curve parse |
| is_marginal_gen | bool |  | PUBLIC |  |  |  | Flag to indicate if the generator is a marginal generator |
| generator_rating | double | VA | PUBLIC |  |  |  | Size of generator in VA for the active bid |
| generator_output | double |  | PUBLIC |  |  |  | Current real power output of generator |
| input_unit_base | double | MW | PUBLIC |  |  |  | Base multiplier for generator bid units |
| startup_cost | double | $ | PUBLIC |  |  |  | Cost to start the generator from OFF status |
| shutdown_cost | double | $ | PUBLIC |  |  |  | Cost to shut down the generator prematurely |
| minimum_runtime | double | s | PUBLIC |  |  |  | Minimum time the generator should run to avoid shutdown cost |
| minimum_downtime | double | s | PUBLIC |  |  |  | Minimum down time for the generator before it can bid again |
| capacity_factor | double |  | PUBLIC |  |  |  | Calculation of generator's current capacity factor based on the market period |
| amortization_factor | double | 1/s | PUBLIC |  |  |  | Exponential decay factor in 1/s for shutdown cost repayment |
| bid_delay | double |  | PUBLIC |  |  |  | Time before a market closes to bid |
| generator_attachment | enumeration |  | PUBLIC |  | BUILDING STANDALONE |  | Generator attachment type - determines interactions |
| building_load_curr | double |  | PUBLIC |  |  |  | Present building load value (if BUILDING attachment) |
| building_load_bid | double |  | PUBLIC |  |  |  | Expected building load value for currently bidding market period (if BUILDING attachment) |
| year_runtime_limit | double | h | PUBLIC |  |  |  | Total number of hours the generator can run in a year |
| current_year_runtime | double | h | PUBLIC |  |  |  | Total number of hours generator has run this year |
| runtime_year_end | char1024 |  | PUBLIC |  |  |  | Date and time the generator runtime year resets |
| scaling_factor | double | unit | PUBLIC |  |  |  | scaling factor applied to license premium calculation |
| license_premium | double |  | PUBLIC |  |  |  | current value of the generator license premium calculated |
| hours_in_year | double | h | PUBLIC |  |  |  | Number of hours assumed for the total year |
| op_and_maint_cost | double | $ | PUBLIC |  |  |  | Operation and maintenance cost per runtime year |
| bid_id | int64 |  | PUBLIC |  |  |  |  |

### supervisory_control

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| unit | char32 |  | PUBLIC |  |  |  | unit of quantity |
| period | double | s | PUBLIC |  |  |  | interval of time between market clearings |
| market_id | int32 |  | REFERENCE |  |  |  | unique identifier of market clearing |
| nominal_frequency | double | Hz | PUBLIC |  |  |  | nominal frequency |
| droop | double | % | PUBLIC |  |  |  | droop value for the supervisor |
| frequency_deadband | double | Hz | PUBLIC |  |  |  | frequency deadband for assigning trigger frequencies |
| PFC_mode | enumeration |  | PUBLIC |  | OVER_UNDER_FREQUENCY UNDER_FREQUENCY OVER_FREQUENCY |  | operation mode of the primary frequency controller |
| bid_sort_mode | enumeration |  | PUBLIC |  | VOLTAGE_EXTREMES VOLTAGE_DEVIAION_FROM_NOMINAL POWER_DECREASING POWER_INCREASING NONE |  | Determines how the bids into the market is sorted to contruct the PF curve |

### simple

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| objective | char1024 |  | PUBLIC |  |  |  | Optimization objective value |
| variable | char1024 |  | PUBLIC |  |  |  | Optimization decision variables |
| constraint | char1024 |  | PUBLIC |  |  |  | Optimization constraints |
| delta | double |  | PUBLIC |  |  |  | Change applied to decision variable |
| epsilon | double |  | PUBLIC |  |  |  | Precision of objective value |
| trials | int32 |  | PUBLIC |  |  |  | Limits on number of trials |
| goal | enumeration |  | PUBLIC |  | MAXIMUM MINIMUM EXTREMUM | EXTREMUM | Optimization objective goal |

### powerflow_object

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| phases | set |  | PUBLIC | CHARSET|REQUIRED | A B C D N S G |  | phases to which this object is connected |
| nominal_voltage | double | V | PUBLIC | REQUIRED |  |  | nominal voltage for this object |
| supernode_name | char1024 |  | PUBLIC |  |  |  |  |

### powerflow_library

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |

### node

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| bustype | enumeration |  | PUBLIC |  | SWING_PQ SWING PV PQ | PQ | defines whether the node is a PQ, PV, or SWING node |
| busflags | set |  | PUBLIC |  | ISSOURCE HASSOURCE | HASSOURCE | flag indicates node has a source for voltage, i.e. connects to the swing node |
| reference_bus | object |  | PUBLIC |  |  |  | reference bus from which frequency is defined |
| maximum_voltage_error | double | V | PUBLIC |  |  | 0 V | convergence voltage limit or convergence criteria |
| voltage_A | complex | V | PUBLIC | OUTPUT |  |  | bus voltage, Phase A to ground |
| voltage_B | complex | V | PUBLIC | OUTPUT |  |  | bus voltage, Phase B to ground |
| voltage_C | complex | V | PUBLIC | OUTPUT |  |  | bus voltage, Phase C to ground |
| voltage_AB | complex | V | PUBLIC | OUTPUT |  |  | line voltages, Phase AB |
| voltage_BC | complex | V | PUBLIC | OUTPUT |  |  | line voltages, Phase BC |
| voltage_CA | complex | V | PUBLIC | OUTPUT |  |  | line voltages, Phase CA |
| current_A | complex | A | HIDDEN | OUTPUT |  |  | bus current injection (in = positive), this an accumulator only, not a output or input variable |
| current_B | complex | A | HIDDEN | OUTPUT |  |  | bus current injection (in = positive), this an accumulator only, not a output or input variable |
| current_C | complex | A | HIDDEN | OUTPUT |  |  | bus current injection (in = positive), this an accumulator only, not a output or input variable |
| power_A | complex | VA | HIDDEN | OUTPUT |  |  | bus power injection (in = positive), this an accumulator only, not a output or input variable |
| power_B | complex | VA | HIDDEN | OUTPUT |  |  | bus power injection (in = positive), this an accumulator only, not a output or input variable |
| power_C | complex | VA | HIDDEN | OUTPUT |  |  | bus power injection (in = positive), this an accumulator only, not a output or input variable |
| shunt_A | complex | S | HIDDEN | OUTPUT |  |  | bus shunt admittance, this an accumulator only, not a output or input variable |
| shunt_B | complex | S | HIDDEN | OUTPUT |  |  | bus shunt admittance, this an accumulator only, not a output or input variable |
| shunt_C | complex | S | HIDDEN | OUTPUT |  |  | bus shunt admittance, this an accumulator only, not a output or input variable |
| prerotated_current_A | complex | A | HIDDEN | OUTPUT |  |  | deltamode-functionality - bus current injection (in = positive), but will not be rotated by powerflow for off-nominal frequency, this an accumulator only, not a output or input variable |
| prerotated_current_B | complex | A | HIDDEN | OUTPUT |  |  | deltamode-functionality - bus current injection (in = positive), but will not be rotated by powerflow for off-nominal frequency, this an accumulator only, not a output or input variable |
| prerotated_current_C | complex | A | HIDDEN | OUTPUT |  |  | deltamode-functionality - bus current injection (in = positive), but will not be rotated by powerflow for off-nominal frequency, this an accumulator only, not a output or input variable |
| current_AB | complex | A | HIDDEN | OUTPUT |  |  | bus current delta-connected injection (in = positive), this an accumulator only, not a output or input variable |
| current_BC | complex | A | HIDDEN | OUTPUT |  |  | bus current delta-connected injection (in = positive), this an accumulator only, not a output or input variable |
| current_CA | complex | A | HIDDEN | OUTPUT |  |  | bus current delta-connected injection (in = positive), this an accumulator only, not a output or input variable |
| current_AN | complex | A | HIDDEN | OUTPUT |  |  | bus current wye-connected injection (in = positive), this an accumulator only, not a output or input variable |
| current_BN | complex | A | HIDDEN | OUTPUT |  |  | bus current wye-connected injection (in = positive), this an accumulator only, not a output or input variable |
| current_CN | complex | A | HIDDEN | OUTPUT |  |  | bus current wye-connected injection (in = positive), this an accumulator only, not a output or input variable |
| power_AB | complex | VA | HIDDEN | OUTPUT |  |  | bus power delta-connected injection (in = positive), this an accumulator only, not a output or input variable |
| power_BC | complex | VA | HIDDEN | OUTPUT |  |  | bus power delta-connected injection (in = positive), this an accumulator only, not a output or input variable |
| power_CA | complex | VA | HIDDEN | OUTPUT |  |  | bus power delta-connected injection (in = positive), this an accumulator only, not a output or input variable |
| power_AN | complex | VA | HIDDEN | OUTPUT |  |  | bus power wye-connected injection (in = positive), this an accumulator only, not a output or input variable |
| power_BN | complex | VA | HIDDEN | OUTPUT |  |  | bus power wye-connected injection (in = positive), this an accumulator only, not a output or input variable |
| power_CN | complex | VA | HIDDEN | OUTPUT |  |  | bus power wye-connected injection (in = positive), this an accumulator only, not a output or input variable |
| shunt_AB | complex | S | HIDDEN | OUTPUT |  |  | bus shunt delta-connected admittance, this an accumulator only, not a output or input variable |
| shunt_BC | complex | S | HIDDEN | OUTPUT |  |  | bus shunt delta-connected admittance, this an accumulator only, not a output or input variable |
| shunt_CA | complex | S | HIDDEN | OUTPUT |  |  | bus shunt delta-connected admittance, this an accumulator only, not a output or input variable |
| shunt_AN | complex | S | HIDDEN | OUTPUT |  |  | bus shunt wye-connected admittance, this an accumulator only, not a output or input variable |
| shunt_BN | complex | S | HIDDEN | OUTPUT |  |  | bus shunt wye-connected admittance, this an accumulator only, not a output or input variable |
| shunt_CN | complex | S | HIDDEN | OUTPUT |  |  | bus shunt wye-connected admittance, this an accumulator only, not a output or input variable |
| mean_repair_time | double | s | PUBLIC |  |  | 0 s | Time after a fault clears for the object to be back in service |
| frequency_measure_type | enumeration |  | PUBLIC |  | PLL SIMPLE NONE |  | PLL frequency measurement |
| sfm_Tf | double | s | PUBLIC |  |  |  | Transducer time constant for simplified frequency measurement (seconds) |
| pll_Kp | double | pu | PUBLIC |  |  |  | Proportional gain of PLL frequency measurement |
| pll_Ki | double | pu | PUBLIC |  |  |  | Integration gain of PLL frequency measurement |
| measured_angle_A | double | rad | PUBLIC |  |  |  | bus angle measurement, phase A |
| measured_frequency_A | double | Hz | PUBLIC |  |  |  | frequency measurement, phase A |
| measured_angle_B | double | rad | PUBLIC |  |  |  | bus angle measurement, phase B |
| measured_frequency_B | double | Hz | PUBLIC |  |  |  | frequency measurement, phase B |
| measured_angle_C | double | rad | PUBLIC |  |  |  | bus angle measurement, phase C |
| measured_frequency_C | double | Hz | PUBLIC |  |  |  | frequency measurement, phase C |
| measured_frequency | double | Hz | PUBLIC |  |  |  | frequency measurement - average of present phases |
| service_status | enumeration |  | PUBLIC |  | OUT_OF_SERVICE IN_SERVICE |  | In and out of service flag |
| service_status_double | double |  | PUBLIC |  |  |  | In and out of service flag - type double - will indiscriminately override service_status - useful for schedules |
| previous_uptime | double | min | PUBLIC |  |  |  | Previous time between disconnects of node in minutes |
| current_uptime | double | min | PUBLIC |  |  |  | Current time since last disconnect of node in minutes |
| Norton_dynamic | bool |  | PUBLIC |  |  |  | Flag to indicate a Norton-equivalent connection -- used for generators and deltamode |
| generator_dynamic | bool |  | PUBLIC |  |  |  | Flag to indicate a voltage-sourcing or swing-type generator is present -- used for generators and deltamode |
| GFA_enable | bool |  | PUBLIC |  |  |  | Disable/Enable Grid Friendly Appliance(TM)-type functionality |
| GFA_freq_low_trip | double | Hz | PUBLIC |  |  |  | Low frequency trip point for Grid Friendly Appliance(TM)-type functionality |
| GFA_freq_high_trip | double | Hz | PUBLIC |  |  |  | High frequency trip point for Grid Friendly Appliance(TM)-type functionality |
| GFA_volt_low_trip | double | pu | PUBLIC |  |  |  | Low voltage trip point for Grid Friendly Appliance(TM)-type functionality |
| GFA_volt_high_trip | double | pu | PUBLIC |  |  |  | High voltage trip point for Grid Friendly Appliance(TM)-type functionality |
| GFA_reconnect_time | double | s | PUBLIC |  |  |  | Reconnect time for Grid Friendly Appliance(TM)-type functionality |
| GFA_freq_disconnect_time | double | s | PUBLIC |  |  |  | Frequency violation disconnect time for Grid Friendly Appliance(TM)-type functionality |
| GFA_volt_disconnect_time | double | s | PUBLIC |  |  |  | Voltage violation disconnect time for Grid Friendly Appliance(TM)-type functionality |
| GFA_status | bool |  | PUBLIC |  |  |  | Low frequency trip point for Grid Friendly Appliance(TM)-type functionality |
| GFA_trip_method | enumeration |  | PUBLIC |  | OVER_VOLTAGE UNDER_VOLTAGE OVER_FREQUENCY UNDER_FREQUENCY NONE |  | GFA trip for over-voltage |
| topological_parent | object |  | PUBLIC |  |  |  | topological parent as per GLM configuration |

### link

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| status | enumeration |  | PUBLIC |  | OPEN CLOSED | CLOSED |  |
| from | object |  | PUBLIC | REQUIRED |  |  | from_node - source node |
| to | object |  | PUBLIC | REQUIRED |  |  | to_node - load node |
| power_in | complex | VA | PUBLIC | OUTPUT |  |  | power flow in (w.r.t from node) |
| power_out | complex | VA | PUBLIC | OUTPUT |  |  | power flow out (w.r.t to node) |
| power_out_real | double | W | PUBLIC | OUTPUT |  |  | power flow out (w.r.t to node), real |
| power_losses | complex | VA | PUBLIC | OUTPUT |  |  | power losses |
| power_in_A | complex | VA | PUBLIC | OUTPUT |  |  | power flow in (w.r.t from node), phase A |
| power_in_B | complex | VA | PUBLIC | OUTPUT |  |  | power flow in (w.r.t from node), phase B |
| power_in_C | complex | VA | PUBLIC | OUTPUT |  |  | power flow in (w.r.t from node), phase C |
| power_out_A | complex | VA | PUBLIC | OUTPUT |  |  | power flow out (w.r.t to node), phase A |
| power_out_B | complex | VA | PUBLIC | OUTPUT |  |  | power flow out (w.r.t to node), phase B |
| power_out_C | complex | VA | PUBLIC | OUTPUT |  |  | power flow out (w.r.t to node), phase C |
| power_losses_A | complex | VA | PUBLIC | OUTPUT |  |  | power losses, phase A |
| power_losses_B | complex | VA | PUBLIC | OUTPUT |  |  | power losses, phase B |
| power_losses_C | complex | VA | PUBLIC | OUTPUT |  |  | power losses, phase C |
| current_out_A | complex | A | PUBLIC | OUTPUT |  |  | current flow out of link (w.r.t. to node), phase A |
| current_out_B | complex | A | PUBLIC | OUTPUT |  |  | current flow out of link (w.r.t. to node), phase B |
| current_out_C | complex | A | PUBLIC | OUTPUT |  |  | current flow out of link (w.r.t. to node), phase C |
| current_in_A | complex | A | PUBLIC | OUTPUT |  |  | current flow to link (w.r.t from node), phase A |
| current_in_B | complex | A | PUBLIC | OUTPUT |  |  | current flow to link (w.r.t from node), phase B |
| current_in_C | complex | A | PUBLIC | OUTPUT |  |  | current flow to link (w.r.t from node), phase C |
| fault_current_in_A | complex | A | PUBLIC | OUTPUT |  |  | fault current flowing in, phase A |
| fault_current_in_B | complex | A | PUBLIC | OUTPUT |  |  | fault current flowing in, phase B |
| fault_current_in_C | complex | A | PUBLIC | OUTPUT |  |  | fault current flowing in, phase C |
| fault_current_out_A | complex | A | PUBLIC | OUTPUT |  |  | fault current flowing out, phase A |
| fault_current_out_B | complex | A | PUBLIC | OUTPUT |  |  | fault current flowing out, phase B |
| fault_current_out_C | complex | A | PUBLIC | OUTPUT |  |  | fault current flowing out, phase C |
| fault_voltage_A | complex | A | PUBLIC |  |  |  | fault voltage, phase A |
| fault_voltage_B | complex | A | PUBLIC |  |  |  | fault voltage, phase B |
| fault_voltage_C | complex | A | PUBLIC |  |  |  | fault voltage, phase C |
| flow_direction | set |  | PUBLIC | OUTPUT | CN CR CF BN BR BF AN AR AF UNKNOWN |  | flag used for describing direction of the flow of power |
| mean_repair_time | double | s | PUBLIC |  |  |  | Time after a fault clears for the object to be back in service |
| continuous_rating | double | A | PUBLIC |  |  |  | Continuous rating for this link object (set individual line segments |
| emergency_rating | double | A | PUBLIC |  |  |  | Emergency rating for this link object (set individual line segments |
| inrush_convergence_value | double | V | PUBLIC |  |  |  | Tolerance, as change in line voltage drop between iterations, for deltamode in-rush completion |

### capacitor

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| pt_phase | set |  | PUBLIC | CHARSET | N D C B A |  | Phase(s) that the PT is on, used as measurement points for control |
| phases_connected | set |  | PUBLIC | CHARSET | N D C B A |  | phases capacitor is connected to |
| switchA | enumeration |  | PUBLIC |  | CLOSED OPEN | OPEN | capacitor A switch open or close |
| switchB | enumeration |  | PUBLIC |  | CLOSED OPEN | OPEN | capacitor B switch open or close |
| switchC | enumeration |  | PUBLIC |  | CLOSED OPEN | OPEN | capacitor C switch open or close |
| control | enumeration |  | PUBLIC |  | CURRENT VARVOLT VOLT VAR MANUAL | MANUAL | control operation strategy |
| cap_A_switch_count | double |  | PUBLIC | OUTPUT |  | -1 | number of switch operations on Phase A |
| cap_B_switch_count | double |  | PUBLIC | OUTPUT |  | -1 | number of switch operations on Phase B |
| cap_C_switch_count | double |  | PUBLIC | OUTPUT |  | -1 | number of switch operations on Phase C |
| voltage_set_high | double | V | PUBLIC |  |  |  | Turn off if voltage is above this set point |
| voltage_set_low | double | V | PUBLIC |  |  |  | Turns on if voltage is below this set point |
| VAr_set_high | double | VAr | PUBLIC |  |  |  | high VAR set point for VAR control (turn off) |
| VAr_set_low | double | VAr | PUBLIC |  |  |  | low VAR set point for VAR control (turn on) |
| current_set_low | double | A | PUBLIC |  |  |  | high current set point for current control mode (turn on) |
| current_set_high | double | A | PUBLIC |  |  |  | low current set point for current control mode (turn off) |
| capacitor_A | double | VAr | PUBLIC |  |  |  | Capacitance value for phase A or phase AB |
| capacitor_B | double | VAr | PUBLIC |  |  |  | Capacitance value for phase B or phase BC |
| capacitor_C | double | VAr | PUBLIC |  |  |  | Capacitance value for phase C or phase CA |
| cap_nominal_voltage | double | V | PUBLIC |  |  |  | Nominal voltage for the capacitor. Used for calculation of capacitance value |
| time_delay | double | s | PUBLIC |  |  |  | control time delay |
| dwell_time | double | s | PUBLIC |  |  |  | Time for system to remain constant before a state change will be passed |
| lockout_time | double | s | PUBLIC |  |  |  | Time for capacitor to remain locked out from further switching operations (VARVOLT control) |
| remote_sense | object |  | PUBLIC |  |  |  | Remote object for sensing values used for control schemes |
| remote_sense_B | object |  | PUBLIC |  |  |  | Secondary Remote object for sensing values used for control schemes (VARVOLT uses two) |
| control_level | enumeration |  | PUBLIC |  | INDIVIDUAL BANK | INDIVIDUAL | define bank or individual control |

### fuse

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| phase_A_status | enumeration |  | PUBLIC |  | GOOD BLOWN | GOOD |  |
| phase_B_status | enumeration |  | PUBLIC |  | GOOD BLOWN | GOOD |  |
| phase_C_status | enumeration |  | PUBLIC |  | GOOD BLOWN | GOOD |  |
| repair_dist_type | enumeration |  | PUBLIC |  | EXPONENTIAL NONE | NONE |  |
| current_limit | double | A | PUBLIC |  |  | 9999 A |  |
| mean_replacement_time | double | s | PUBLIC |  |  |  |  |
| fuse_resistance | double | Ohm | PUBLIC |  |  | -1 Ohm | The resistance value of the fuse when it is not blown. |

### meter

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| measured_real_energy | double | Wh | PUBLIC | OUTPUT |  |  | metered real energy consumption, cummalitive |
| measured_real_energy_delta | double | Wh | PUBLIC | OUTPUT |  |  | delta in metered real energy consumption from last specified measured_energy_delta_timestep |
| measured_reactive_energy | double | VAh | PUBLIC | OUTPUT |  |  | metered reactive energy consumption, cummalitive |
| measured_reactive_energy_delta | double | VAh | PUBLIC | OUTPUT |  |  | delta in metered reactive energy consumption from last specified measured_energy_delta_timestep |
| measured_energy_delta_timestep | double | s | PUBLIC | OUTPUT |  |  | Period of timestep for real and reactive delta energy calculation |
| measured_power | complex | VA | PUBLIC | OUTPUT |  |  | metered complex power |
| measured_power_A | complex | VA | PUBLIC | OUTPUT |  |  | metered complex power on phase A |
| measured_power_B | complex | VA | PUBLIC | OUTPUT |  |  | metered complex power on phase B |
| measured_power_C | complex | VA | PUBLIC | OUTPUT |  |  | metered complex power on phase C |
| measured_demand | double | W | PUBLIC | OUTPUT |  |  | greatest metered real power during simulation |
| measured_real_power | double | W | PUBLIC | OUTPUT |  |  | metered real power |
| measured_reactive_power | double | VAr | PUBLIC | OUTPUT |  |  | metered reactive power |
| meter_power_consumption | complex | VA | PUBLIC | OUTPUT |  |  | metered power used for operating the meter; standby and communication losses |
| measured_voltage_A | complex | V | PUBLIC | OUTPUT |  |  | measured line-to-neutral voltage on phase A |
| measured_voltage_B | complex | V | PUBLIC | OUTPUT |  |  | measured line-to-neutral voltage on phase B |
| measured_voltage_C | complex | V | PUBLIC | OUTPUT |  |  | measured line-to-neutral voltage on phase C |
| measured_voltage_AB | complex | V | PUBLIC | OUTPUT |  |  | measured line-to-line voltage on phase AB |
| measured_voltage_BC | complex | V | PUBLIC | OUTPUT |  |  | measured line-to-line voltage on phase BC |
| measured_voltage_CA | complex | V | PUBLIC | OUTPUT |  |  | measured line-to-line voltage on phase CA |
| measured_real_max_voltage_in_interval_A | double |  | PUBLIC | OUTPUT |  |  | measured real max line-to-neutral voltage on phase A over a specified interval |
| measured_real_max_voltage_in_interval_B | double |  | PUBLIC | OUTPUT |  |  | measured real max line-to-neutral voltage on phase B over a specified interval |
| measured_real_max_voltage_in_interval_C | double |  | PUBLIC | OUTPUT |  |  | measured real max line-to-neutral voltage on phase C over a specified interval |
| measured_reactive_max_voltage_in_interval_A | double |  | PUBLIC | OUTPUT |  |  | measured reactive max line-to-neutral voltage on phase A over a specified interval |
| measured_reactive_max_voltage_in_interval_B | double |  | PUBLIC | OUTPUT |  |  | measured reactive max line-to-neutral voltage on phase B over a specified interval |
| measured_reactive_max_voltage_in_interval_C | double |  | PUBLIC | OUTPUT |  |  | measured reactive max line-to-neutral voltage on phase C over a specified interval |
| measured_real_max_voltage_in_interval_AB | double |  | PUBLIC | OUTPUT |  |  | measured real max line-to-line voltage on phase A over a specified interval |
| measured_real_max_voltage_in_interval_BC | double |  | PUBLIC | OUTPUT |  |  | measured real max line-to-line voltage on phase B over a specified interval |
| measured_real_max_voltage_in_interval_CA | double |  | PUBLIC | OUTPUT |  |  | measured real max line-to-line voltage on phase C over a specified interval |
| measured_reactive_max_voltage_in_interval_AB | double |  | PUBLIC | OUTPUT |  |  | measured reactive max line-to-line voltage on phase A over a specified interval |
| measured_reactive_max_voltage_in_interval_BC | double |  | PUBLIC | OUTPUT |  |  | measured reactive max line-to-line voltage on phase B over a specified interval |
| measured_reactive_max_voltage_in_interval_CA | double |  | PUBLIC | OUTPUT |  |  | measured reactive max line-to-line voltage on phase C over a specified interval |
| measured_real_min_voltage_in_interval_A | double |  | PUBLIC | OUTPUT |  |  | measured real min line-to-neutral voltage on phase A over a specified interval |
| measured_real_min_voltage_in_interval_B | double |  | PUBLIC | OUTPUT |  |  | measured real min line-to-neutral voltage on phase B over a specified interval |
| measured_real_min_voltage_in_interval_C | double |  | PUBLIC | OUTPUT |  |  | measured real min line-to-neutral voltage on phase C over a specified interval |
| measured_reactive_min_voltage_in_interval_A | double |  | PUBLIC | OUTPUT |  |  | measured reactive min line-to-neutral voltage on phase A over a specified interval |
| measured_reactive_min_voltage_in_interval_B | double |  | PUBLIC | OUTPUT |  |  | measured reactive min line-to-neutral voltage on phase B over a specified interval |
| measured_reactive_min_voltage_in_interval_C | double |  | PUBLIC | OUTPUT |  |  | measured reactive min line-to-neutral voltage on phase C over a specified interval |
| measured_real_min_voltage_in_interval_AB | double |  | PUBLIC | OUTPUT |  |  | measured real min line-to-line voltage on phase A over a specified interval |
| measured_real_min_voltage_in_interval_BC | double |  | PUBLIC | OUTPUT |  |  | measured real min line-to-line voltage on phase B over a specified interval |
| measured_real_min_voltage_in_interval_CA | double |  | PUBLIC | OUTPUT |  |  | measured real min line-to-line voltage on phase C over a specified interval |
| measured_reactive_min_voltage_in_interval_AB | double |  | PUBLIC | OUTPUT |  |  | measured reactive min line-to-line voltage on phase A over a specified interval |
| measured_reactive_min_voltage_in_interval_BC | double |  | PUBLIC | OUTPUT |  |  | measured reactive min line-to-line voltage on phase B over a specified interval |
| measured_reactive_min_voltage_in_interval_CA | double |  | PUBLIC | OUTPUT |  |  | measured reactive min line-to-line voltage on phase C over a specified interval |
| measured_avg_voltage_mag_in_interval_A | double |  | PUBLIC | OUTPUT |  |  | measured avg line-to-neutral voltage magnitude on phase A over a specified interval |
| measured_avg_voltage_mag_in_interval_B | double |  | PUBLIC | OUTPUT |  |  | measured avg line-to-neutral voltage magnitude on phase B over a specified interval |
| measured_avg_voltage_mag_in_interval_C | double |  | PUBLIC | OUTPUT |  |  | measured avg line-to-neutral voltage magnitude on phase C over a specified interval |
| measured_avg_voltage_mag_in_interval_AB | double |  | PUBLIC | OUTPUT |  |  | measured avg line-to-line voltage magnitude on phase A over a specified interval |
| measured_avg_voltage_mag_in_interval_BC | double |  | PUBLIC | OUTPUT |  |  | measured avg line-to-line voltage magnitude on phase B over a specified interval |
| measured_avg_voltage_mag_in_interval_CA | double |  | PUBLIC | OUTPUT |  |  | measured avg line-to-line voltage magnitude on phase C over a specified interval |
| measured_current_A | complex | A | PUBLIC | OUTPUT |  |  | measured current on phase A |
| measured_current_B | complex | A | PUBLIC | OUTPUT |  |  | measured current on phase B |
| measured_current_C | complex | A | PUBLIC | OUTPUT |  |  | measured current on phase C |
| customer_interrupted | bool |  | PUBLIC | OUTPUT |  |  | Reliability flag - goes active if the customer is in an 'interrupted' state |
| customer_interrupted_secondary | bool |  | PUBLIC | OUTPUT |  |  | Reliability flag - goes active if the customer is in an 'secondary interrupted' state - i.e., momentary |
| monthly_bill | double | $ | PUBLIC | OUTPUT |  |  | Accumulator for the current month's bill |
| previous_monthly_bill | double | $ | PUBLIC | OUTPUT |  |  | Total monthly bill for the previous month |
| previous_monthly_energy | double | kWh | PUBLIC | OUTPUT |  |  | Total monthly energy for the previous month |
| monthly_fee | double | $ | PUBLIC | OUTPUT |  |  | Once a month flat fee for customer hook-up |
| monthly_energy | double | kWh | PUBLIC | OUTPUT |  |  | Accumulator for the current month's energy consumption |
| bill_mode | enumeration |  | PUBLIC |  | TIERED_RTP HOURLY TIERED UNIFORM NONE | NONE | Billing structure desired |
| power_market | object |  | PUBLIC |  |  |  | Market (auction object) where the price is being received from |
| bill_day | int32 |  | PUBLIC |  |  | 15 | day of month bill is to be processed (currently limited to days 1-28) |
| price | double | $/kWh | PUBLIC |  |  |  | current price of electricity |
| price_base | double | $/kWh | PUBLIC |  |  |  | Used only in TIERED_RTP mode to describe the price before the first tier |
| first_tier_price | double | $/kWh | PUBLIC |  |  |  | price of electricity between first tier and second tier energy usage |
| first_tier_energy | double | kWh | PUBLIC |  |  |  | switching point between base price and first tier price |
| second_tier_price | double | $/kWh | PUBLIC |  |  |  | price of electricity between second tier and third tier energy usage |
| second_tier_energy | double | kWh | PUBLIC |  |  |  | switching point between first tier price and second tier price |
| third_tier_price | double | $/kWh | PUBLIC |  |  |  | price of electricity when energy usage exceeds third tier energy usage |
| third_tier_energy | double | kWh | PUBLIC |  |  |  | switching point between second tier price and third tier price |

### line

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| configuration | object |  | PUBLIC | REQUIRED |  |  | reference to library object that describes this line's configuration |
| length | double | ft | PUBLIC | REQUIRED |  |  | length of this line |

### line_sensor

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| measured_phase | enumeration |  | PUBLIC | REQUIRED | C B A |  | phase from which measurement is taken |
| measured_voltage | complex | V | PUBLIC | OUTPUT |  |  | voltage measurement (with noise) |
| measured_current | complex | A | PUBLIC | OUTPUT |  |  | current measurement (with noise) |
| measured_power | complex | VA | PUBLIC | OUTPUT |  |  | power measurement (with noise) |
| location | double | ft | PUBLIC | REQUIRED |  |  | sensor placement downline |
| covariance | double_array |  | PUBLIC |  |  |  | sensor covariance matrix (components are [Vr Vi Ir Ii]) |

### line_spacing

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| distance_AB | double | ft | PUBLIC |  |  | 0 ft |  |
| distance_BC | double | ft | PUBLIC |  |  | 0 ft |  |
| distance_AC | double | ft | PUBLIC |  |  | 0 ft |  |
| distance_AN | double | ft | PUBLIC |  |  | 0 ft |  |
| distance_BN | double | ft | PUBLIC |  |  | 0 ft |  |
| distance_CN | double | ft | PUBLIC |  |  | 0 ft |  |
| distance_AE | double | ft | PUBLIC |  |  | 0 ft | distance between phase A wire and earth |
| distance_BE | double | ft | PUBLIC |  |  | 0 ft | distance between phase B wire and earth |
| distance_CE | double | ft | PUBLIC |  |  | 0 ft | distance between phase C wire and earth |
| distance_NE | double | ft | PUBLIC |  |  | 0 ft | distance between neutral wire and earth |

### overhead_line

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |

### underground_line

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |

### overhead_line_conductor

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| geometric_mean_radius | double | ft | PUBLIC | REQUIRED |  |  | radius of the conductor |
| resistance | double | Ohm/mile | PUBLIC | REQUIRED |  |  | resistance in Ohms/mile of the conductor |
| diameter | double | in | PUBLIC | REQUIRED |  |  | Diameter of line for capacitance calculations |
| rating.summer.continuous | double | A | PUBLIC |  |  | 1000 A | Continuous summer amp rating |
| rating.summer.emergency | double | A | PUBLIC |  |  | 2000 A | Emergency summer amp rating |
| rating.winter.continuous | double | A | PUBLIC |  |  | 1000 A | Continuous winter amp rating |
| rating.winter.emergency | double | A | PUBLIC |  |  | 2000 A | Emergency winter amp rating |

### underground_line_conductor

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| outer_diameter | double | in | PUBLIC | REQUIRED |  |  | Outer diameter of conductor and sheath |
| conductor_gmr | double | ft | PUBLIC |  |  |  | Geometric mean radius of the conductor |
| conductor_diameter | double | in | PUBLIC | REQUIRED |  |  | Diameter of conductor |
| conductor_resistance | double | Ohm/mile | PUBLIC |  |  |  | Resistance of conductor in ohm/mile |
| neutral_gmr | double | ft | PUBLIC |  |  |  | Geometric mean radius of an individual neutral conductor/strand |
| neutral_diameter | double | in | PUBLIC | REQUIRED |  |  | Diameter of individual neutral conductor/strand of concentric neutral |
| neutral_resistance | double | Ohm/mile | PUBLIC |  |  |  | Resistance of an individual neutral conductor/strand in ohm/mile |
| neutral_strands | int16 |  | PUBLIC |  |  |  | Number of cable strands in neutral conductor |
| shield_thickness | double | in | PUBLIC |  |  |  | The thickness of Tape shield in inches |
| shield_diameter | double | in | PUBLIC |  |  |  | The outside diameter of Tape shield in inches |
| insulation_relative_permitivitty | double | unit | PUBLIC |  |  | 1 unit | Permitivitty of insulation, relative to air |
| shield_gmr | double | ft | PUBLIC |  |  |  | Geometric mean radius of shielding sheath |
| shield_resistance | double | Ohm/mile | PUBLIC |  |  |  | Resistance of shielding sheath in ohms/mile |
| rating.summer.continuous | double | A | PUBLIC |  |  | 1000 A | amp rating in summer, continuous |
| rating.summer.emergency | double | A | PUBLIC |  |  | 2000 A | amp rating in summer, short term |
| rating.winter.continuous | double | A | PUBLIC |  |  | 1000 A | amp rating in winter, continuous |
| rating.winter.emergency | double | A | PUBLIC |  |  | 2000 A | amp rating in winter, short term |

### line_configuration

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| conductor_A | object |  | PUBLIC |  |  |  |  |
| conductor_B | object |  | PUBLIC |  |  |  |  |
| conductor_C | object |  | PUBLIC |  |  |  |  |
| conductor_N | object |  | PUBLIC |  |  |  |  |
| spacing | object |  | PUBLIC |  |  |  |  |
| z11 | complex | Ohm/mile | PUBLIC |  |  | 0+0j |  |
| z12 | complex | Ohm/mile | PUBLIC |  |  | 0+0j |  |
| z13 | complex | Ohm/mile | PUBLIC |  |  | 0+0j |  |
| z21 | complex | Ohm/mile | PUBLIC |  |  | 0+0j |  |
| z22 | complex | Ohm/mile | PUBLIC |  |  | 0+0j |  |
| z23 | complex | Ohm/mile | PUBLIC |  |  | 0+0j |  |
| z31 | complex | Ohm/mile | PUBLIC |  |  | 0+0j |  |
| z32 | complex | Ohm/mile | PUBLIC |  |  | 0+0j |  |
| z33 | complex | Ohm/mile | PUBLIC |  |  | 0+0j |  |
| c11 | double | nF/mile | PUBLIC |  |  | 0 nF/mile |  |
| c12 | double | nF/mile | PUBLIC |  |  | 0 nF/mile |  |
| c13 | double | nF/mile | PUBLIC |  |  | 0 nF/mile |  |
| c21 | double | nF/mile | PUBLIC |  |  | 0 nF/mile |  |
| c22 | double | nF/mile | PUBLIC |  |  | 0 nF/mile |  |
| c23 | double | nF/mile | PUBLIC |  |  | 0 nF/mile |  |
| c31 | double | nF/mile | PUBLIC |  |  | 0 nF/mile |  |
| c32 | double | nF/mile | PUBLIC |  |  | 0 nF/mile |  |
| c33 | double | nF/mile | PUBLIC |  |  | 0 nF/mile |  |
| rating.summer.continuous | double | A | PUBLIC |  |  | 1000 A | amp rating in summer, continuous |
| rating.summer.emergency | double | A | PUBLIC |  |  | 2000 A | amp rating in summer, short term |
| rating.winter.continuous | double | A | PUBLIC |  |  | 1000 A | amp rating in winter, continuous |
| rating.winter.emergency | double | A | PUBLIC |  |  | 2000 A | amp rating in winter, short term |

### transformer_configuration

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| connect_type | enumeration |  | PUBLIC |  | SINGLE_PHASE_CENTER_TAPPED SINGLE_PHASE DELTA_GWYE DELTA_DELTA WYE_WYE UNKNOWN | UNKNOWN | transformation connection type |
| install_type | enumeration |  | PUBLIC |  | VAULT PADMOUNT POLETOP UNKNOWN | UNKNOWN | transformation installation type |
| coolant_type | enumeration |  | PUBLIC |  | DRY MINERAL_OIL UNKNOWN | UNKNOWN | transformation coolant type |
| cooling_type | enumeration |  | PUBLIC |  | DFOW DFOA NDFOW NDFOA FA OA UNKNOWN | UNKNOWN | transformation cooling type |
| primary_voltage | double | V | PUBLIC | REQUIRED |  |  | primary voltage level in L-L value kV |
| secondary_voltage | double | V | PUBLIC | REQUIRED |  |  | secondary voltage level kV |
| power_rating | double | kVA | PUBLIC | REQUIRED |  |  | kVA rating of transformer, total |
| powerA_rating | double | kVA | PUBLIC |  |  |  | kVA rating of transformer, phase A |
| powerB_rating | double | kVA | PUBLIC |  |  |  | kVA rating of transformer, phase B |
| powerC_rating | double | kVA | PUBLIC |  |  |  | kVA rating of transformer, phase C |
| powerN_rating | double | kVA | PUBLIC |  |  |  | kVA rating of transformer, neutral |
| resistance | double | pu*Ohm | PUBLIC |  |  |  | Series impedance, pu, real |
| reactance | double | pu*Ohm | PUBLIC |  |  |  | Series impedance, pu, imag |
| impedance | complex | pu*Ohm | PUBLIC |  |  |  | Series impedance, pu |
| resistance1 | double | pu*Ohm | PUBLIC |  |  |  | Secondary series impedance (only used when you want to define each individual winding seperately, pu, real |
| reactance1 | double | pu*Ohm | PUBLIC |  |  |  | Secondary series impedance (only used when you want to define each individual winding seperately, pu, imag |
| impedance1 | complex | pu*Ohm | PUBLIC |  |  |  | Secondary series impedance (only used when you want to define each individual winding seperately, pu |
| resistance2 | double | pu*Ohm | PUBLIC |  |  |  | Secondary series impedance (only used when you want to define each individual winding seperately, pu, real |
| reactance2 | double | pu*Ohm | PUBLIC |  |  |  | Secondary series impedance (only used when you want to define each individual winding seperately, pu, imag |
| impedance2 | complex | pu*Ohm | PUBLIC |  |  |  | Secondary series impedance (only used when you want to define each individual winding seperately, pu |
| shunt_resistance | double | pu*Ohm | PUBLIC |  |  | +1e+09 pu*Ohm | Shunt impedance on primary side, pu, real |
| shunt_reactance | double | pu*Ohm | PUBLIC |  |  | +1e+09 pu*Ohm | Shunt impedance on primary side, pu, imag |
| shunt_impedance | complex | pu*Ohm | PUBLIC |  |  | +1e+09+1e+09j pu*Ohm | Shunt impedance on primary side, pu |
| core_coil_weight | double | lb | PUBLIC |  |  |  | The weight of the core and coil assembly in pounds |
| tank_fittings_weight | double | lb | PUBLIC |  |  |  | The weight of the tank and fittings in pounds |
| oil_volume | double | gal | PUBLIC |  |  |  | The number of gallons of oil in the transformer |
| rated_winding_time_constant | double | h | PUBLIC |  |  |  | The rated winding time constant in hours |
| rated_winding_hot_spot_rise | double | degC | PUBLIC |  |  |  | winding hottest-spot rise over ambient temperature at rated load, degrees C |
| rated_top_oil_rise | double | degC | PUBLIC |  |  |  | top-oil hottest-spot rise over ambient temperature at rated load, degrees C |
| no_load_loss | double | pu | PUBLIC |  |  |  | Another method of specifying transformer impedances, defined as per unit power values (shunt) |
| full_load_loss | double | pu | PUBLIC |  |  |  | Another method of specifying transformer impedances, defined as per unit power values (shunt and series) |
| reactance_resistance_ratio | double |  | PUBLIC |  |  |  | the reactance to resistance ratio (X/R) |
| installed_insulation_life | double | h | PUBLIC |  |  |  | the normal lifetime of the transformer insulation at rated load, hours |
| magnetization_location | enumeration |  | PUBLIC |  | BOTH SECONDARY PRIMARY NONE |  | winding to place magnetization influence for in-rush calculations |
| inrush_saturation_enabled | bool |  | PUBLIC |  |  |  | flag to include saturation effects during inrush calculations |
| L_A | double | pu | PUBLIC |  |  | +0.2 pu | Air core inductance of transformer |
| phi_K | double | pu | PUBLIC |  |  | +1.17 pu | Knee flux value where the air core inductance interstes the flux axis of the saturation curve |
| phi_M | double | pu | PUBLIC |  |  | +1 pu | Peak magnetization flux at rated voltage of the saturation curve |
| I_M | double | pu | PUBLIC |  |  | +0.01 pu | Peak magnetization current at rated voltage of the saturation curve |
| T_D | double |  | PUBLIC |  |  | +0.5 | Inrush decay time constant for inrush current |

### transformer

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| configuration | object |  | PUBLIC | REQUIRED |  |  | Configuration library used for transformer setup |
| climate | object |  | PUBLIC |  |  |  | climate object used to describe thermal model ambient temperature |
| ambient_temperature | double | degC | PUBLIC |  |  |  | ambient temperature in degrees C |
| top_oil_hot_spot_temperature | double | degC | PUBLIC |  |  |  | top-oil hottest-spot temperature, degrees C |
| winding_hot_spot_temperature | double | degC | PUBLIC |  |  |  | winding hottest-spot temperature, degrees C |
| percent_loss_of_life | double |  | PUBLIC |  |  |  | the percent loss of life |
| aging_constant | double |  | PUBLIC |  |  |  | the aging rate slope for the transformer insulation |
| use_thermal_model | bool |  | PUBLIC |  |  |  | boolean to enable use of thermal model |
| transformer_replacement_count | double |  | PUBLIC |  |  |  | counter of the number times the transformer has been replaced due to lifetime failure |
| aging_granularity | double | s | PUBLIC |  |  |  | maximum timestep before updating thermal and aging model in seconds |
| phase_A_primary_flux_value | double | Wb | PUBLIC |  |  |  | instantaneous magnetic flux in phase A on the primary side of the transformer during saturation calculations |
| phase_B_primary_flux_value | double | Wb | PUBLIC |  |  |  | instantaneous magnetic flux in phase B on the primary side of the transformer during saturation calculations |
| phase_C_primary_flux_value | double | Wb | PUBLIC |  |  |  | instantaneous magnetic flux in phase C on the primary side of the transformer during saturation calculations |
| phase_A_secondary_flux_value | double | Wb | PUBLIC |  |  |  | instantaneous magnetic flux in phase A on the secondary side of the transformer during saturation calculations |
| phase_B_secondary_flux_value | double | Wb | PUBLIC |  |  |  | instantaneous magnetic flux in phase B on the secondary side of the transformer during saturation calculations |
| phase_C_secondary_flux_value | double | Wb | PUBLIC |  |  |  | instantaneous magnetic flux in phase C on the secondary side of the transformer during saturation calculations |

### load

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| load_class | enumeration |  | PUBLIC |  | A I C R U | U | Flag to track load type, not currently used for anything except sorting |
| constant_power_A | complex | VA | PUBLIC |  |  | 0 VA | constant power load on phase A, specified as VA |
| constant_power_B | complex | VA | PUBLIC |  |  | 0 VA | constant power load on phase B, specified as VA |
| constant_power_C | complex | VA | PUBLIC |  |  | 0 VA | constant power load on phase C, specified as VA |
| constant_power_A_real | double | W | PUBLIC |  |  | 0 W | constant power load on phase A, real only, specified as W |
| constant_power_B_real | double | W | PUBLIC |  |  | 0 W | constant power load on phase B, real only, specified as W |
| constant_power_C_real | double | W | PUBLIC |  |  | 0 W | constant power load on phase C, real only, specified as W |
| constant_power_A_reac | double | VAr | PUBLIC |  |  | 0 VAr | constant power load on phase A, imaginary only, specified as VAr |
| constant_power_B_reac | double | VAr | PUBLIC |  |  | 0 VAr | constant power load on phase B, imaginary only, specified as VAr |
| constant_power_C_reac | double | VAr | PUBLIC |  |  | 0 VAr | constant power load on phase C, imaginary only, specified as VAr |
| constant_current_A | complex | A | PUBLIC |  |  | 0 A | constant current load on phase A, specified as Amps |
| constant_current_B | complex | A | PUBLIC |  |  | 0 A | constant current load on phase B, specified as Amps |
| constant_current_C | complex | A | PUBLIC |  |  | 0 A | constant current load on phase C, specified as Amps |
| constant_current_A_real | double | A | PUBLIC |  |  | 0 A | constant current load on phase A, real only, specified as Amps |
| constant_current_B_real | double | A | PUBLIC |  |  | 0 A | constant current load on phase B, real only, specified as Amps |
| constant_current_C_real | double | A | PUBLIC |  |  | 0 A | constant current load on phase C, real only, specified as Amps |
| constant_current_A_reac | double | A | PUBLIC |  |  | 0 A | constant current load on phase A, imaginary only, specified as Amps |
| constant_current_B_reac | double | A | PUBLIC |  |  | 0 A | constant current load on phase B, imaginary only, specified as Amps |
| constant_current_C_reac | double | A | PUBLIC |  |  | 0 A | constant current load on phase C, imaginary only, specified as Amps |
| constant_impedance_A | complex | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance load on phase A, specified as Ohms |
| constant_impedance_B | complex | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance load on phase B, specified as Ohms |
| constant_impedance_C | complex | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance load on phase C, specified as Ohms |
| constant_impedance_A_real | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance load on phase A, real only, specified as Ohms |
| constant_impedance_B_real | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance load on phase B, real only, specified as Ohms |
| constant_impedance_C_real | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance load on phase C, real only, specified as Ohms |
| constant_impedance_A_reac | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance load on phase A, imaginary only, specified as Ohms |
| constant_impedance_B_reac | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance load on phase B, imaginary only, specified as Ohms |
| constant_impedance_C_reac | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance load on phase C, imaginary only, specified as Ohms |
| constant_power_AN | complex | VA | PUBLIC |  |  | 0 VA | constant power wye-connected load on phase A, specified as VA |
| constant_power_BN | complex | VA | PUBLIC |  |  | 0 VA | constant power wye-connected load on phase B, specified as VA |
| constant_power_CN | complex | VA | PUBLIC |  |  | 0 VA | constant power wye-connected load on phase C, specified as VA |
| constant_power_AN_real | double | W | PUBLIC |  |  | 0 W | constant power wye-connected load on phase A, real only, specified as W |
| constant_power_BN_real | double | W | PUBLIC |  |  | 0 W | constant power wye-connected load on phase B, real only, specified as W |
| constant_power_CN_real | double | W | PUBLIC |  |  | 0 W | constant power wye-connected load on phase C, real only, specified as W |
| constant_power_AN_reac | double | VAr | PUBLIC |  |  | 0 VAr | constant power wye-connected load on phase A, imaginary only, specified as VAr |
| constant_power_BN_reac | double | VAr | PUBLIC |  |  | 0 VAr | constant power wye-connected load on phase B, imaginary only, specified as VAr |
| constant_power_CN_reac | double | VAr | PUBLIC |  |  | 0 VAr | constant power wye-connected load on phase C, imaginary only, specified as VAr |
| constant_current_AN | complex | A | PUBLIC |  |  | 0 A | constant current wye-connected load on phase A, specified as Amps |
| constant_current_BN | complex | A | PUBLIC |  |  | 0 A | constant current wye-connected load on phase B, specified as Amps |
| constant_current_CN | complex | A | PUBLIC |  |  | 0 A | constant current wye-connected load on phase C, specified as Amps |
| constant_current_AN_real | double | A | PUBLIC |  |  | 0 A | constant current wye-connected load on phase A, real only, specified as Amps |
| constant_current_BN_real | double | A | PUBLIC |  |  | 0 A | constant current wye-connected load on phase B, real only, specified as Amps |
| constant_current_CN_real | double | A | PUBLIC |  |  | 0 A | constant current wye-connected load on phase C, real only, specified as Amps |
| constant_current_AN_reac | double | A | PUBLIC |  |  | 0 A | constant current wye-connected load on phase A, imaginary only, specified as Amps |
| constant_current_BN_reac | double | A | PUBLIC |  |  | 0 A | constant current wye-connected load on phase B, imaginary only, specified as Amps |
| constant_current_CN_reac | double | A | PUBLIC |  |  | 0 A | constant current wye-connected load on phase C, imaginary only, specified as Amps |
| constant_impedance_AN | complex | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance wye-connected load on phase A, specified as Ohms |
| constant_impedance_BN | complex | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance wye-connected load on phase B, specified as Ohms |
| constant_impedance_CN | complex | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance wye-connected load on phase C, specified as Ohms |
| constant_impedance_AN_real | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance wye-connected load on phase A, real only, specified as Ohms |
| constant_impedance_BN_real | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance wye-connected load on phase B, real only, specified as Ohms |
| constant_impedance_CN_real | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance wye-connected load on phase C, real only, specified as Ohms |
| constant_impedance_AN_reac | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance wye-connected load on phase A, imaginary only, specified as Ohms |
| constant_impedance_BN_reac | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance wye-connected load on phase B, imaginary only, specified as Ohms |
| constant_impedance_CN_reac | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance wye-connected load on phase C, imaginary only, specified as Ohms |
| constant_power_AB | complex | VA | PUBLIC |  |  | 0 VA | constant power delta-connected load on phase A, specified as VA |
| constant_power_BC | complex | VA | PUBLIC |  |  | 0 VA | constant power delta-connected load on phase B, specified as VA |
| constant_power_CA | complex | VA | PUBLIC |  |  | 0 VA | constant power delta-connected load on phase C, specified as VA |
| constant_power_AB_real | double | W | PUBLIC |  |  | 0 W | constant power delta-connected load on phase A, real only, specified as W |
| constant_power_BC_real | double | W | PUBLIC |  |  | 0 W | constant power delta-connected load on phase B, real only, specified as W |
| constant_power_CA_real | double | W | PUBLIC |  |  | 0 W | constant power delta-connected load on phase C, real only, specified as W |
| constant_power_AB_reac | double | VAr | PUBLIC |  |  | 0 VAr | constant power delta-connected load on phase A, imaginary only, specified as VAr |
| constant_power_BC_reac | double | VAr | PUBLIC |  |  | 0 VAr | constant power delta-connected load on phase B, imaginary only, specified as VAr |
| constant_power_CA_reac | double | VAr | PUBLIC |  |  | 0 VAr | constant power delta-connected load on phase C, imaginary only, specified as VAr |
| constant_current_AB | complex | A | PUBLIC |  |  | 0 A | constant current delta-connected load on phase A, specified as Amps |
| constant_current_BC | complex | A | PUBLIC |  |  | 0 A | constant current delta-connected load on phase B, specified as Amps |
| constant_current_CA | complex | A | PUBLIC |  |  | 0 A | constant current delta-connected load on phase C, specified as Amps |
| constant_current_AB_real | double | A | PUBLIC |  |  | 0 A | constant current delta-connected load on phase A, real only, specified as Amps |
| constant_current_BC_real | double | A | PUBLIC |  |  | 0 A | constant current delta-connected load on phase B, real only, specified as Amps |
| constant_current_CA_real | double | A | PUBLIC |  |  | 0 A | constant current delta-connected load on phase C, real only, specified as Amps |
| constant_current_AB_reac | double | A | PUBLIC |  |  | 0 A | constant current delta-connected load on phase A, imaginary only, specified as Amps |
| constant_current_BC_reac | double | A | PUBLIC |  |  | 0 A | constant current delta-connected load on phase B, imaginary only, specified as Amps |
| constant_current_CA_reac | double | A | PUBLIC |  |  | 0 A | constant current delta-connected load on phase C, imaginary only, specified as Amps |
| constant_impedance_AB | complex | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance delta-connected load on phase A, specified as Ohms |
| constant_impedance_BC | complex | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance delta-connected load on phase B, specified as Ohms |
| constant_impedance_CA | complex | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance delta-connected load on phase C, specified as Ohms |
| constant_impedance_AB_real | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance delta-connected load on phase A, real only, specified as Ohms |
| constant_impedance_BC_real | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance delta-connected load on phase B, real only, specified as Ohms |
| constant_impedance_CA_real | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance delta-connected load on phase C, real only, specified as Ohms |
| constant_impedance_AB_reac | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance delta-connected load on phase A, imaginary only, specified as Ohms |
| constant_impedance_BC_reac | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance delta-connected load on phase B, imaginary only, specified as Ohms |
| constant_impedance_CA_reac | double | Ohm | PUBLIC |  |  | 0 Ohm | constant impedance delta-connected load on phase C, imaginary only, specified as Ohms |
| measured_voltage_A | complex | V | PUBLIC | OUTPUT |  |  | current measured voltage on phase A |
| measured_voltage_B | complex | V | PUBLIC | OUTPUT |  |  | current measured voltage on phase B |
| measured_voltage_C | complex | V | PUBLIC | OUTPUT |  |  | current measured voltage on phase C |
| measured_voltage_AB | complex | V | PUBLIC | OUTPUT |  |  | current measured voltage on phases AB |
| measured_voltage_BC | complex | V | PUBLIC | OUTPUT |  |  | current measured voltage on phases BC |
| measured_voltage_CA | complex | V | PUBLIC | OUTPUT |  |  | current measured voltage on phases CA |
| phase_loss_protection | bool |  | PUBLIC |  |  | FALSE | Trip all three phases of the load if a fault occurs |
| base_power_A | double | VA | PUBLIC |  |  | 0 VA | in similar format as ZIPload, this represents the nominal power on phase A before applying ZIP fractions |
| base_power_B | double | VA | PUBLIC |  |  | 0 VA | in similar format as ZIPload, this represents the nominal power on phase B before applying ZIP fractions |
| base_power_C | double | VA | PUBLIC |  |  | 0 VA | in similar format as ZIPload, this represents the nominal power on phase C before applying ZIP fractions |
| power_pf_A | double | pu | PUBLIC |  |  | 1 pu | in similar format as ZIPload, this is the power factor of the phase A constant power portion of load |
| current_pf_A | double | pu | PUBLIC |  |  | 1 pu | in similar format as ZIPload, this is the power factor of the phase A constant current portion of load |
| impedance_pf_A | double | pu | PUBLIC |  |  | 1 pu | in similar format as ZIPload, this is the power factor of the phase A constant impedance portion of load |
| power_pf_B | double | pu | PUBLIC |  |  | 1 pu | in similar format as ZIPload, this is the power factor of the phase B constant power portion of load |
| current_pf_B | double | pu | PUBLIC |  |  | 1 pu | in similar format as ZIPload, this is the power factor of the phase B constant current portion of load |
| impedance_pf_B | double | pu | PUBLIC |  |  | 1 pu | in similar format as ZIPload, this is the power factor of the phase B constant impedance portion of load |
| power_pf_C | double | pu | PUBLIC |  |  | 1 pu | in similar format as ZIPload, this is the power factor of the phase C constant power portion of load |
| current_pf_C | double | pu | PUBLIC |  |  | 1 pu | in similar format as ZIPload, this is the power factor of the phase C constant current portion of load |
| impedance_pf_C | double | pu | PUBLIC |  |  | 1 pu | in similar format as ZIPload, this is the power factor of the phase C constant impedance portion of load |
| power_fraction_A | double | pu | PUBLIC |  |  | 0 pu | this is the constant power fraction of base power on phase A |
| current_fraction_A | double | pu | PUBLIC |  |  | 0 pu | this is the constant current fraction of base power on phase A |
| impedance_fraction_A | double | pu | PUBLIC |  |  | 0 pu | this is the constant impedance fraction of base power on phase A |
| power_fraction_B | double | pu | PUBLIC |  |  | 0 pu | this is the constant power fraction of base power on phase B |
| current_fraction_B | double | pu | PUBLIC |  |  | 0 pu | this is the constant current fraction of base power on phase B |
| impedance_fraction_B | double | pu | PUBLIC |  |  | 0 pu | this is the constant impedance fraction of base power on phase B |
| power_fraction_C | double | pu | PUBLIC |  |  | 0 pu | this is the constant power fraction of base power on phase C |
| current_fraction_C | double | pu | PUBLIC |  |  | 0 pu | this is the constant current fraction of base power on phase C |
| impedance_fraction_C | double | pu | PUBLIC |  |  | 0 pu | this is the constant impedance fraction of base power on phase C |

### regulator_configuration

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| connect_type | enumeration |  | PUBLIC | REQUIRED | CLOSED_DELTA OPEN_DELTA_CABA OPEN_DELTA_BCAC OPEN_DELTA_ABBC WYE_WYE UNKNOWN | UNKNOWN | Designation of connection style |
| band_center | double | V | PUBLIC |  |  | 0 V | band center setting of regulator control |
| band_width | double | V | PUBLIC |  |  | 0 V | band width setting of regulator control |
| time_delay | double | s | PUBLIC |  |  | 0 s | mechanical time delay between tap changes |
| dwell_time | double | s | PUBLIC |  |  | 0 s | time delay before a control action of regulator control |
| raise_taps | int16 |  | PUBLIC |  |  | 0 | number of regulator raise taps, or the maximum raise voltage tap position |
| lower_taps | int16 |  | PUBLIC |  |  | 0 | number of regulator lower taps, or the minimum lower voltage tap position |
| current_transducer_ratio | double | pu | PUBLIC |  |  | 0 pu | primary rating of current transformer |
| power_transducer_ratio | double | pu | PUBLIC |  |  | 0 pu | potential transformer rating |
| compensator_r_setting_A | double | V | PUBLIC |  |  | 0 V | Line Drop Compensation R setting of regulator control (in volts) on Phase A |
| compensator_r_setting_B | double | V | PUBLIC |  |  | 0 V | Line Drop Compensation R setting of regulator control (in volts) on Phase B |
| compensator_r_setting_C | double | V | PUBLIC |  |  | 0 V | Line Drop Compensation R setting of regulator control (in volts) on Phase C |
| compensator_x_setting_A | double | V | PUBLIC |  |  | 0 V | Line Drop Compensation X setting of regulator control (in volts) on Phase A |
| compensator_x_setting_B | double | V | PUBLIC |  |  | 0 V | Line Drop Compensation X setting of regulator control (in volts) on Phase B |
| compensator_x_setting_C | double | V | PUBLIC |  |  | 0 V | Line Drop Compensation X setting of regulator control (in volts) on Phase C |
| CT_phase | set |  | PUBLIC | CHARSET | C B A | ABC | phase(s) monitored by CT |
| PT_phase | set |  | PUBLIC | CHARSET | C B A | ABC | phase(s) monitored by PT |
| regulation | double | % | PUBLIC |  |  | 0 % | regulation of voltage regulator in % |
| control_level | enumeration |  | PUBLIC |  | BANK INDIVIDUAL | INDIVIDUAL | Designates whether control is on per-phase or banked level |
| Control | enumeration |  | PUBLIC |  | REMOTE_NODE LINE_DROP_COMP OUTPUT_VOLTAGE MANUAL | MANUAL | Type of control used for regulating voltage |
| reverse_flow_control | enumeration |  | PUBLIC |  | LOCK_CURRENT_POSITION LOCK_NEUTRAL LOCK_NONE | LOCK_NONE | Type of control used when power is flowing in reverse through the regulator |
| Type | enumeration |  | PUBLIC |  | B A | B | Defines regulator type |
| tap_pos_A | int16 |  | PUBLIC |  |  | 999 | initial tap position of phase A |
| tap_pos_B | int16 |  | PUBLIC |  |  | 999 | initial tap position of phase B |
| tap_pos_C | int16 |  | PUBLIC |  |  | 999 | initial tap position of phase C |

### regulator

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| configuration | object |  | PUBLIC | REQUIRED |  |  | reference to the regulator_configuration object used to determine regulator properties |
| tap_A | int16 |  | PUBLIC |  |  |  | current tap position of tap A |
| tap_B | int16 |  | PUBLIC |  |  |  | current tap position of tap B |
| tap_C | int16 |  | PUBLIC |  |  |  | current tap position of tap C |
| tap_A_change_count | double |  | PUBLIC | OUTPUT |  |  | count of all physical tap changes on phase A since beginning of simulation (plus initial value) |
| tap_B_change_count | double |  | PUBLIC | OUTPUT |  |  | count of all physical tap changes on phase B since beginning of simulation (plus initial value) |
| tap_C_change_count | double |  | PUBLIC | OUTPUT |  |  | count of all physical tap changes on phase C since beginning of simulation (plus initial value) |
| sense_node | object |  | PUBLIC |  |  |  | Node to be monitored for voltage control in remote sense mode |
| regulator_resistance | double | Ohm | PUBLIC |  |  |  | The resistance value of the regulator when it is not blown. |

### triplex_node

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| bustype | enumeration |  | PUBLIC |  | SWING_PQ SWING PV PQ |  | defines whether the node is a PQ, PV, or SWING node |
| busflags | set |  | PUBLIC |  | ISSOURCE HASSOURCE |  | flag indicates node has a source for voltage, i.e. connects to the swing node |
| reference_bus | object |  | PUBLIC |  |  |  | reference bus from which frequency is defined |
| maximum_voltage_error | double | V | PUBLIC |  |  |  | convergence voltage limit or convergence criteria |
| voltage_1 | complex | V | PUBLIC |  |  |  | bus voltage, phase 1 to ground |
| voltage_2 | complex | V | PUBLIC |  |  |  | bus voltage, phase 2 to ground |
| voltage_N | complex | V | PUBLIC |  |  |  | bus voltage, phase N to ground |
| voltage_12 | complex | V | PUBLIC |  |  |  | bus voltage, phase 1 to 2 |
| voltage_1N | complex | V | PUBLIC |  |  |  | bus voltage, phase 1 to N |
| voltage_2N | complex | V | PUBLIC |  |  |  | bus voltage, phase 2 to N |
| current_1 | complex | A | PUBLIC |  |  |  | constant current load on phase 1, also acts as accumulator |
| current_2 | complex | A | PUBLIC |  |  |  | constant current load on phase 2, also acts as accumulator |
| current_N | complex | A | PUBLIC |  |  |  | constant current load on phase N, also acts as accumulator |
| current_1_real | double | A | PUBLIC |  |  |  | constant current load on phase 1, real |
| current_2_real | double | A | PUBLIC |  |  |  | constant current load on phase 2, real |
| current_N_real | double | A | PUBLIC |  |  |  | constant current load on phase N, real |
| current_1_reac | double | A | PUBLIC |  |  |  | constant current load on phase 1, imag |
| current_2_reac | double | A | PUBLIC |  |  |  | constant current load on phase 2, imag |
| current_N_reac | double | A | PUBLIC |  |  |  | constant current load on phase N, imag |
| current_12 | complex | A | PUBLIC |  |  |  | constant current load on phase 1 to 2 |
| current_12_real | double | A | PUBLIC |  |  |  | constant current load on phase 1 to 2, real |
| current_12_reac | double | A | PUBLIC |  |  |  | constant current load on phase 1 to 2, imag |
| prerotated_current_1 | complex | A | HIDDEN |  |  |  | deltamode-functionality - bus current injection (in = positive), but will not be rotated by powerflow for off-nominal frequency, this an accumulator only, not a output or input variable |
| prerotated_current_2 | complex | A | HIDDEN |  |  |  | deltamode-functionality - bus current injection (in = positive), but will not be rotated by powerflow for off-nominal frequency, this an accumulator only, not a output or input variable |
| prerotated_current_12 | complex | A | HIDDEN |  |  |  | deltamode-functionality - bus current injection (in = positive), but will not be rotated by powerflow for off-nominal frequency, this an accumulator only, not a output or input variable |
| residential_nominal_current_1 | complex | A | HIDDEN |  |  |  | posted current on phase 1 from a residential object, if attached |
| residential_nominal_current_2 | complex | A | HIDDEN |  |  |  | posted current on phase 2 from a residential object, if attached |
| residential_nominal_current_12 | complex | A | HIDDEN |  |  |  | posted current on phase 1 to 2 from a residential object, if attached |
| residential_nominal_current_1_real | double | A | HIDDEN |  |  |  | posted current on phase 1, real, from a residential object, if attached |
| residential_nominal_current_1_imag | double | A | HIDDEN |  |  |  | posted current on phase 1, imag, from a residential object, if attached |
| residential_nominal_current_2_real | double | A | HIDDEN |  |  |  | posted current on phase 2, real, from a residential object, if attached |
| residential_nominal_current_2_imag | double | A | HIDDEN |  |  |  | posted current on phase 2, imag, from a residential object, if attached |
| residential_nominal_current_12_real | double | A | HIDDEN |  |  |  | posted current on phase 1 to 2, real, from a residential object, if attached |
| residential_nominal_current_12_imag | double | A | HIDDEN |  |  |  | posted current on phase 1 to 2, imag, from a residential object, if attached |
| power_1 | complex | VA | PUBLIC |  |  |  | constant power on phase 1 (120V) |
| power_2 | complex | VA | PUBLIC |  |  |  | constant power on phase 2 (120V) |
| power_12 | complex | VA | PUBLIC |  |  |  | constant power on phase 1 to 2 (240V) |
| power_1_real | double | W | PUBLIC |  |  |  | constant power on phase 1, real |
| power_2_real | double | W | PUBLIC |  |  |  | constant power on phase 2, real |
| power_12_real | double | W | PUBLIC |  |  |  | constant power on phase 1 to 2, real |
| power_1_reac | double | VAr | PUBLIC |  |  |  | constant power on phase 1, imag |
| power_2_reac | double | VAr | PUBLIC |  |  |  | constant power on phase 2, imag |
| power_12_reac | double | VAr | PUBLIC |  |  |  | constant power on phase 1 to 2, imag |
| shunt_1 | complex | S | PUBLIC |  |  |  | constant shunt impedance on phase 1 |
| shunt_2 | complex | S | PUBLIC |  |  |  | constant shunt impedance on phase 2 |
| shunt_12 | complex | S | PUBLIC |  |  |  | constant shunt impedance on phase 1 to 2 |
| impedance_1 | complex | Ohm | PUBLIC |  |  |  | constant series impedance on phase 1 |
| impedance_2 | complex | Ohm | PUBLIC |  |  |  | constant series impedance on phase 2 |
| impedance_12 | complex | Ohm | PUBLIC |  |  |  | constant series impedance on phase 1 to 2 |
| impedance_1_real | double | Ohm | PUBLIC |  |  |  | constant series impedance on phase 1, real |
| impedance_2_real | double | Ohm | PUBLIC |  |  |  | constant series impedance on phase 2, real |
| impedance_12_real | double | Ohm | PUBLIC |  |  |  | constant series impedance on phase 1 to 2, real |
| impedance_1_reac | double | Ohm | PUBLIC |  |  |  | constant series impedance on phase 1, imag |
| impedance_2_reac | double | Ohm | PUBLIC |  |  |  | constant series impedance on phase 2, imag |
| impedance_12_reac | double | Ohm | PUBLIC |  |  |  | constant series impedance on phase 1 to 2, imag |
| house_present | bool |  | PUBLIC |  |  |  | boolean for detecting whether a house is attached, not an input |
| GFA_enable | bool |  | PUBLIC |  |  |  | Disable/Enable Grid Friendly Applicance(TM)-type functionality |
| GFA_freq_low_trip | double | Hz | PUBLIC |  |  |  | Low frequency trip point for Grid Friendly Appliance(TM)-type functionality |
| GFA_freq_high_trip | double | Hz | PUBLIC |  |  |  | High frequency trip point for Grid Friendly Appliance(TM)-type functionality |
| GFA_volt_low_trip | double | pu | PUBLIC |  |  |  | Low voltage trip point for Grid Friendly Appliance(TM)-type functionality |
| GFA_volt_high_trip | double | pu | PUBLIC |  |  |  | High voltage trip point for Grid Friendly Appliance(TM)-type functionality |
| GFA_reconnect_time | double | s | PUBLIC |  |  |  | Reconnect time for Grid Friendly Appliance(TM)-type functionality |
| GFA_freq_disconnect_time | double | s | PUBLIC |  |  |  | Frequency violation disconnect time for Grid Friendly Appliance(TM)-type functionality |
| GFA_volt_disconnect_time | double | s | PUBLIC |  |  |  | Voltage violation disconnect time for Grid Friendly Appliance(TM)-type functionality |
| GFA_status | bool |  | PUBLIC |  |  |  | Low frequency trip point for Grid Friendly Appliance(TM)-type functionality |
| service_status | enumeration |  | PUBLIC |  | OUT_OF_SERVICE IN_SERVICE |  | In and out of service flag |
| service_status_double | double |  | PUBLIC |  |  |  | In and out of service flag - type double - will indiscriminately override service_status - useful for schedules |
| previous_uptime | double | min | PUBLIC |  |  |  | Previous time between disconnects of node in minutes |
| current_uptime | double | min | PUBLIC |  |  |  | Current time since last disconnect of node in minutes |
| topological_parent | object |  | PUBLIC |  |  |  | topological parent as per GLM configuration |
| frequency_measure_type | enumeration |  | PUBLIC |  | PLL SIMPLE NONE |  | PLL frequency measurement |
| sfm_Tf | double | s | PUBLIC |  |  |  | Transducer time constant for simplified frequency measurement (seconds) |
| pll_Kp | double | pu | PUBLIC |  |  |  | Proportional gain of PLL frequency measurement |
| pll_Ki | double | pu | PUBLIC |  |  |  | Integration gain of PLL frequency measurement |
| measured_angle_1 | double | rad | PUBLIC |  |  |  | bus angle measurement, phase 1N |
| measured_frequency_1 | double | Hz | PUBLIC |  |  |  | frequency measurement, phase 1N |
| measured_angle_2 | double | rad | PUBLIC |  |  |  | bus angle measurement, phase 2N |
| measured_frequency_2 | double | Hz | PUBLIC |  |  |  | frequency measurement, phase 2N |
| measured_angle_12 | double | rad | PUBLIC |  |  |  | bus angle measurement, across the phases |
| measured_frequency_12 | double | Hz | PUBLIC |  |  |  | frequency measurement, across the phases |
| measured_frequency | double | Hz | PUBLIC |  |  |  | frequency measurement - average of present phases |

### triplex_meter

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| measured_real_energy | double | Wh | PUBLIC |  |  |  | metered real energy consumption |
| measured_real_energy_delta | double | Wh | PUBLIC |  |  |  | delta in metered real energy consumption from last specified measured_energy_delta_timestep |
| measured_reactive_energy | double | VAh | PUBLIC |  |  |  | metered reactive energy consumption |
| measured_reactive_energy_delta | double | VAh | PUBLIC |  |  |  | delta in metered reactive energy consumption from last specified measured_energy_delta_timestep |
| measured_energy_delta_timestep | double | s | PUBLIC |  |  |  | Period of timestep for real and reactive delta energy calculation |
| measured_power | complex | VA | PUBLIC |  |  |  | metered power |
| indiv_measured_power_1 | complex | VA | PUBLIC |  |  |  | metered power, phase 1 |
| indiv_measured_power_2 | complex | VA | PUBLIC |  |  |  | metered power, phase 2 |
| indiv_measured_power_N | complex | VA | PUBLIC |  |  |  | metered power, phase N |
| measured_demand | double | W | PUBLIC |  |  |  | metered demand (peak of power) |
| measured_real_power | double | W | PUBLIC |  |  |  | metered real power |
| measured_reactive_power | double | VAr | PUBLIC |  |  |  | metered reactive power |
| meter_power_consumption | complex | VA | PUBLIC |  |  |  | power consumed by meter operation |
| measured_voltage_1 | complex | V | PUBLIC |  |  |  | measured voltage, phase 1 to ground |
| measured_voltage_2 | complex | V | PUBLIC |  |  |  | measured voltage, phase 2 to ground |
| measured_voltage_12 | complex | V | PUBLIC |  |  |  | measured voltage, phase 1 to phase 2 |
| measured_voltage_N | complex | V | PUBLIC |  |  |  | measured voltage, phase N to ground |
| measured_real_max_voltage_1_in_interval | double |  | PUBLIC |  |  |  | measured real max line-to-ground voltage on phase 1 over a specified interval |
| measured_real_max_voltage_2_in_interval | double |  | PUBLIC |  |  |  | measured real max line-to-ground voltage on phase 2 over a specified interval |
| measured_real_max_voltage_12_in_interval | double |  | PUBLIC |  |  |  | measured real max line-to-ground voltage on phase 12 over a specified interval |
| measured_imag_max_voltage_1_in_interval | double |  | PUBLIC |  |  |  | measured imaginary max line-to-ground voltage on phase 1 over a specified interval |
| measured_imag_max_voltage_2_in_interval | double |  | PUBLIC |  |  |  | measured imaginary max line-to-ground voltage on phase 2 over a specified interval |
| measured_imag_max_voltage_12_in_interval | double |  | PUBLIC |  |  |  | measured imaginary max line-to-ground voltage on phase 12 over a specified interval |
| measured_real_min_voltage_1_in_interval | double |  | PUBLIC |  |  |  | measured real min line-to-ground voltage on phase 1 over a specified interval |
| measured_real_min_voltage_2_in_interval | double |  | PUBLIC |  |  |  | measured real min line-to-ground voltage on phase 2 over a specified interval |
| measured_real_min_voltage_12_in_interval | double |  | PUBLIC |  |  |  | measured real min line-to-ground voltage on phase 12 over a specified interval |
| measured_imag_min_voltage_1_in_interval | double |  | PUBLIC |  |  |  | measured imaginary min line-to-ground voltage on phase 1 over a specified interval |
| measured_imag_min_voltage_2_in_interval | double |  | PUBLIC |  |  |  | measured imaginary min line-to-ground voltage on phase 2 over a specified interval |
| measured_imag_min_voltage_12_in_interval | double |  | PUBLIC |  |  |  | measured imaginary min line-to-ground voltage on phase 12 over a specified interval |
| measured_avg_voltage_mag_1_in_interval | double |  | PUBLIC |  |  |  | measured average line-to-ground voltage magnitude on phase 1 over a specified interval |
| measured_avg_voltage_mag_2_in_interval | double |  | PUBLIC |  |  |  | measured average line-to-ground voltage magnitude on phase 2 over a specified interval |
| measured_avg_voltage_mag_12_in_interval | double |  | PUBLIC |  |  |  | measured average line-to-ground voltage magnitude on phase 12 over a specified interval |
| measured_current_1 | complex | A | PUBLIC |  |  |  | measured current, phase 1 |
| measured_current_2 | complex | A | PUBLIC |  |  |  | measured current, phase 2 |
| measured_current_N | complex | A | PUBLIC |  |  |  | measured current, phase N |
| customer_interrupted | bool |  | PUBLIC |  |  |  | Reliability flag - goes active if the customer is in an interrupted state |
| customer_interrupted_secondary | bool |  | PUBLIC |  |  |  | Reliability flag - goes active if the customer is in a secondary interrupted state - i.e., momentary |
| monthly_bill | double | $ | PUBLIC |  |  |  | Accumulator for the current month's bill |
| previous_monthly_bill | double | $ | PUBLIC |  |  |  | Total monthly bill for the previous month |
| previous_monthly_energy | double | kWh | PUBLIC |  |  |  |  |
| monthly_fee | double | $ | PUBLIC |  |  |  | Total monthly energy for the previous month |
| monthly_energy | double | kWh | PUBLIC |  |  |  | Accumulator for the current month's energy |
| bill_mode | enumeration |  | PUBLIC |  | TIERED_RTP HOURLY TIERED UNIFORM NONE |  | Designates the bill mode to be used |
| power_market | object |  | PUBLIC |  |  |  | Designates the auction object where prices are read from for bill mode |
| bill_day | int32 |  | PUBLIC |  |  |  | Day bill is to be processed (assumed to occur at midnight of that day) |
| price | double | $/kWh | PUBLIC |  |  |  | Standard uniform pricing |
| price_base | double | $/kWh | PUBLIC |  |  |  | Used only in TIERED_RTP mode to describe the price before the first tier |
| first_tier_price | double | $/kWh | PUBLIC |  |  |  | first tier price of energy between first and second tier energy |
| first_tier_energy | double | kWh | PUBLIC |  |  |  | price of energy on tier above price or price base |
| second_tier_price | double | $/kWh | PUBLIC |  |  |  | first tier price of energy between second and third tier energy |
| second_tier_energy | double | kWh | PUBLIC |  |  |  | price of energy on tier above first tier |
| third_tier_price | double | $/kWh | PUBLIC |  |  |  | first tier price of energy greater than third tier energy |
| third_tier_energy | double | kWh | PUBLIC |  |  |  | price of energy on tier above second tier |

### triplex_line

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |

### triplex_line_configuration

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| conductor_1 | object |  | PUBLIC |  |  |  | conductor type for phase 1 |
| conductor_2 | object |  | PUBLIC |  |  |  | conductor type for phase 2 |
| conductor_N | object |  | PUBLIC |  |  |  | conductor type for phase N |
| insulation_thickness | double | in | PUBLIC |  |  |  | thickness of insulation around cabling |
| diameter | double | in | PUBLIC |  |  |  | total diameter of cable |
| spacing | object |  | PUBLIC |  |  |  | defines the line spacing configuration |
| z11 | complex | Ohm/mile | PUBLIC |  |  |  | phase 1 self-impedance, used for direct entry of impedance values |
| z12 | complex | Ohm/mile | PUBLIC |  |  |  | phase 1-2 induced impedance, used for direct entry of impedance values |
| z21 | complex | Ohm/mile | PUBLIC |  |  |  | phase 2-1 induced impedance, used for direct entry of impedance values |
| z22 | complex | Ohm/mile | PUBLIC |  |  |  | phase 2 self-impedance, used for direct entry of impedance values |
| rating.summer.continuous | double | A | PUBLIC |  |  |  | amp rating in summer, continuous |
| rating.summer.emergency | double | A | PUBLIC |  |  |  | amp rating in summer, short term |
| rating.winter.continuous | double | A | PUBLIC |  |  |  | amp rating in winter, continuous |
| rating.winter.emergency | double | A | PUBLIC |  |  |  | amp rating in winter, short term |

### triplex_line_conductor

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| resistance | double | Ohm/mile | PUBLIC |  |  |  | resistance of cable in ohm/mile |
| geometric_mean_radius | double | ft | PUBLIC |  |  |  | geometric mean radius of the cable |
| rating.summer.continuous | double | A | PUBLIC |  |  |  | amp ratings for the cable during continuous operation in summer |
| rating.summer.emergency | double | A | PUBLIC |  |  |  | amp ratings for the cable during short term operation in summer |
| rating.winter.continuous | double | A | PUBLIC |  |  |  | amp ratings for the cable during continuous operation in winter |
| rating.winter.emergency | double | A | PUBLIC |  |  |  | amp ratings for the cable during short term operation in winter |

### switch_coordinator

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| status | enumeration |  | PUBLIC |  | DIRECT TOGGLE ARMED IDLE |  | switch coordination status |
| arm | method |  | PUBLIC |  |  |  | method to arm a switch |
| disarm | method |  | PUBLIC |  |  |  | method to disarm a switch |
| connect | method |  | PUBLIC |  |  |  | method to connect switches |
| armed | set |  | PUBLIC | OUTPUT | NONE |  | set of armed switches |

### switch

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| phase_A_state | enumeration |  | PUBLIC |  | CLOSED OPEN | CLOSED | state of phase A switch |
| phase_B_state | enumeration |  | PUBLIC |  | CLOSED OPEN | CLOSED | state of phase B switch |
| phase_C_state | enumeration |  | PUBLIC |  | CLOSED OPEN | CLOSED | state of phase C switch |
| operating_mode | enumeration |  | PUBLIC |  | BANKED INDIVIDUAL | BANKED | switch bank operating mode |
| switch_resistance | double | Ohm | PUBLIC |  |  |  | The resistance value of the switch when it is not blown. |

### substation

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| zero_sequence_voltage | complex | V | PUBLIC |  |  |  | The zero sequence representation of the voltage for the substation object. |
| positive_sequence_voltage | complex | V | PUBLIC |  |  |  | The positive sequence representation of the voltage for the substation object. |
| negative_sequence_voltage | complex | V | PUBLIC |  |  |  | The negative sequence representation of the voltage for the substation object. |
| base_power | double | VA | PUBLIC |  |  |  | The 3 phase VA power rating of the substation. |
| power_convergence_value | double | VA | PUBLIC |  |  |  | Default convergence criterion before power is posted to pw_load objects if connected, otherwise ignored |
| reference_phase | enumeration |  | PUBLIC |  | PHASE_C PHASE_B PHASE_A | PHASE_A | The reference phase for the positive sequence voltage. |
| transmission_level_constant_power_load | complex | VA | PUBLIC | OUTPUT |  |  | the average constant power load to be posted directly to the pw_load object. |
| transmission_level_constant_current_load | complex | A | PUBLIC | OUTPUT |  |  | the average constant current load at nominal voltage to be posted directly to the pw_load object. |
| transmission_level_constant_impedance_load | complex | Ohm | PUBLIC | OUTPUT |  |  | the average constant impedance load at nominal voltage to be posted directly to the pw_load object. |
| distribution_load | complex | VA | PUBLIC | OUTPUT |  |  | The total load of all three phases at the substation object. |
| distribution_power_A | complex | VA | PUBLIC | OUTPUT |  |  |  |
| distribution_power_B | complex | VA | PUBLIC | OUTPUT |  |  |  |
| distribution_power_C | complex | VA | PUBLIC | OUTPUT |  |  |  |
| distribution_voltage_A | complex | V | PUBLIC | OUTPUT |  |  |  |
| distribution_voltage_B | complex | V | PUBLIC | OUTPUT |  |  |  |
| distribution_voltage_C | complex | V | PUBLIC | OUTPUT |  |  |  |
| distribution_voltage_AB | complex | V | PUBLIC | OUTPUT |  |  |  |
| distribution_voltage_BC | complex | V | PUBLIC | OUTPUT |  |  |  |
| distribution_voltage_CA | complex | V | PUBLIC | OUTPUT |  |  |  |
| distribution_current_A | complex | A | PUBLIC | OUTPUT |  |  |  |
| distribution_current_B | complex | A | PUBLIC | OUTPUT |  |  |  |
| distribution_current_C | complex | A | PUBLIC | OUTPUT |  |  |  |
| distribution_real_energy | double | Wh | PUBLIC | OUTPUT |  |  |  |

### pqload

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| weather | object |  | PUBLIC |  |  |  |  |
| T_nominal | double | degF | PUBLIC |  |  |  |  |
| Zp_T | double | ohm/degF | PUBLIC |  |  |  |  |
| Zp_H | double | ohm/% | PUBLIC |  |  |  |  |
| Zp_S | double | ohm*h/Btu | PUBLIC |  |  |  |  |
| Zp_W | double | ohm/mph | PUBLIC |  |  |  |  |
| Zp_R | double | ohm*h/in | PUBLIC |  |  |  |  |
| Zp | double | ohm | PUBLIC |  |  |  |  |
| Zq_T | double | F/degF | PUBLIC |  |  |  |  |
| Zq_H | double | F/% | PUBLIC |  |  |  |  |
| Zq_S | double | F*h/Btu | PUBLIC |  |  |  |  |
| Zq_W | double | F/mph | PUBLIC |  |  |  |  |
| Zq_R | double | F*h/in | PUBLIC |  |  |  |  |
| Zq | double | F | PUBLIC |  |  |  |  |
| Im_T | double | A/degF | PUBLIC |  |  |  |  |
| Im_H | double | A/% | PUBLIC |  |  |  |  |
| Im_S | double | A*h/Btu | PUBLIC |  |  |  |  |
| Im_W | double | A/mph | PUBLIC |  |  |  |  |
| Im_R | double | A*h/in | PUBLIC |  |  |  |  |
| Im | double | A | PUBLIC |  |  |  |  |
| Ia_T | double | deg/degF | PUBLIC |  |  |  |  |
| Ia_H | double | deg/% | PUBLIC |  |  |  |  |
| Ia_S | double | deg*h/Btu | PUBLIC |  |  |  |  |
| Ia_W | double | deg/mph | PUBLIC |  |  |  |  |
| Ia_R | double | deg*h/in | PUBLIC |  |  |  |  |
| Ia | double | deg | PUBLIC |  |  |  |  |
| Pp_T | double | W/degF | PUBLIC |  |  |  |  |
| Pp_H | double | W/% | PUBLIC |  |  |  |  |
| Pp_S | double | W*h/Btu | PUBLIC |  |  |  |  |
| Pp_W | double | W/mph | PUBLIC |  |  |  |  |
| Pp_R | double | W*h/in | PUBLIC |  |  |  |  |
| Pp | double | W | PUBLIC |  |  |  |  |
| Pq_T | double | VAr/degF | PUBLIC |  |  |  |  |
| Pq_H | double | VAr/% | PUBLIC |  |  |  |  |
| Pq_S | double | VAr*h/Btu | PUBLIC |  |  |  |  |
| Pq_W | double | VAr/mph | PUBLIC |  |  |  |  |
| Pq_R | double | VAr*h/in | PUBLIC |  |  |  |  |
| Pq | double | VAr | PUBLIC |  |  |  |  |
| input_temp | double | degF | REFERENCE |  |  |  |  |
| input_humid | double | % | REFERENCE |  |  |  |  |
| input_solar | double | Btu/h | REFERENCE |  |  |  |  |
| input_wind | double | mph | REFERENCE |  |  |  |  |
| input_rain | double | in/h | REFERENCE |  |  |  |  |
| output_imped_p | double | Ohm | REFERENCE |  |  |  |  |
| output_imped_q | double | Ohm | REFERENCE |  |  |  |  |
| output_current_m | double | A | REFERENCE |  |  |  |  |
| output_current_a | double | deg | REFERENCE |  |  |  |  |
| output_power_p | double | W | REFERENCE |  |  |  |  |
| output_power_q | double | VAr | REFERENCE |  |  |  |  |
| output_impedance | complex | ohm | REFERENCE |  |  |  |  |
| output_current | complex | A | REFERENCE |  |  |  |  |
| output_power | complex | VA | REFERENCE |  |  |  |  |

### voltdump

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| group | char32 |  | PUBLIC |  |  |  | the group ID to output data for (all nodes if empty) |
| runtime | timestamp |  | PUBLIC |  |  | NEVER | the time to check voltage data |
| filename | char256 |  | PUBLIC | REQUIRED |  |  | the file to dump the voltage data into |
| file | char256 |  | PUBLIC | DEPRECATED |  |  | the file to dump the voltage data into |
| runcount | int32 |  | REFERENCE |  |  | 0 | the number of times the file has been written to |
| maxcount | int32 |  | PUBLIC |  |  | -1 | the maximum number of times the file is written |
| mode | enumeration |  | PUBLIC |  | polar rect | rect | dumps the voltages in either polar or rectangular notation |
| filemode | char8 |  | PUBLIC |  |  |  | sets the file write mode |
| version | int32 |  | PUBLIC |  |  | 0 | dump format version |
| interval | double | s | PUBLIC |  |  | 0 s | interval at which voltdump runs |

### series_reactor

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| phase_A_impedance | complex | Ohm | PUBLIC |  |  |  | Series impedance of reactor on phase A |
| phase_A_resistance | double | Ohm | PUBLIC |  |  |  | Resistive portion of phase A's impedance |
| phase_A_reactance | double | Ohm | PUBLIC |  |  |  | Reactive portion of phase A's impedance |
| phase_B_impedance | complex | Ohm | PUBLIC |  |  |  | Series impedance of reactor on phase B |
| phase_B_resistance | double | Ohm | PUBLIC |  |  |  | Resistive portion of phase B's impedance |
| phase_B_reactance | double | Ohm | PUBLIC |  |  |  | Reactive portion of phase B's impedance |
| phase_C_impedance | complex | Ohm | PUBLIC |  |  |  | Series impedance of reactor on phase C |
| phase_C_resistance | double | Ohm | PUBLIC |  |  |  | Resistive portion of phase C's impedance |
| phase_C_reactance | double | Ohm | PUBLIC |  |  |  | Reactive portion of phase C's impedance |
| rated_current_limit | double | A | PUBLIC |  |  |  | Rated current limit for the reactor |

### restoration

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| reconfig_attempts | int32 |  | PUBLIC |  |  |  | Number of reconfigurations/timestep to try before giving up |
| reconfig_iteration_limit | int32 |  | PUBLIC |  |  |  | Number of iterations to let PF go before flagging this as a bad reconfiguration |
| source_vertex | object |  | PUBLIC |  |  |  | Source vertex object for reconfiguration |
| faulted_section | object |  | PUBLIC |  |  |  | Faulted section for reconfiguration |
| feeder_power_limit | char1024 |  | PUBLIC |  |  |  | Comma-separated power limit (VA) for feeders during reconfiguration |
| feeder_power_links | char1024 |  | PUBLIC |  |  |  | Comma-separated list of link-based objects for monitoring power through |
| feeder_vertex_list | char1024 |  | PUBLIC |  |  |  | Comma-separated object list that defines the feeder vertices |
| microgrid_power_limit | char1024 |  | PUBLIC |  |  |  | Comma-separated power limit (complex VA) for microgrids during reconfiguration |
| microgrid_power_links | char1024 |  | PUBLIC |  |  |  | Comma-separated list of link-based objects for monitoring power through |
| microgrid_vertex_list | char1024 |  | PUBLIC |  |  |  | Comma-separated object list that defines the microgrid vertices |
| lower_voltage_limit | double | pu | PUBLIC |  |  |  | Lower voltage limit for the reconfiguration validity checks - per unit |
| upper_voltage_limit | double | pu | PUBLIC |  |  |  | Upper voltage limit for the reconfiguration validity checks - per unit |
| output_filename | char1024 |  | PUBLIC |  |  |  | Output text file name to describe final or attempted switching operations |
| generate_all_scenarios | bool |  | PUBLIC |  |  |  | Flag to determine if restoration reconfiguration and continues, or explores the full space |

### frequency_gen

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| Frequency_Mode | enumeration |  | PUBLIC |  | AUTO OFF |  | Frequency object operations mode |
| Frequency | double | Hz | PUBLIC |  |  |  | Instantaneous frequency value |
| FreqChange | double | Hz/s | PUBLIC |  |  |  | Frequency change from last timestep |
| Deadband | double | Hz | PUBLIC |  |  |  | Frequency deadband of the governor |
| Tolerance | double | % | PUBLIC |  |  |  | % threshold a power difference must be before it is cared about |
| M | double | pu*s | PUBLIC |  |  |  | Inertial constant of the system |
| D | double | % | PUBLIC |  |  |  | Load-damping constant |
| Rated_power | double | W | PUBLIC |  |  |  | Rated power of system (base power) |
| Gen_power | double | W | PUBLIC |  |  |  | Mechanical power equivalent |
| Load_power | double | W | PUBLIC |  |  |  | Last sensed load value |
| Gov_delay | double | s | PUBLIC |  |  |  | Governor delay time |
| Ramp_rate | double | W/s | PUBLIC |  |  |  | Ramp ideal ramp rate |
| Low_Freq_OI | double | Hz | PUBLIC |  |  |  | Low frequency setpoint for GFA devices |
| High_Freq_OI | double | Hz | PUBLIC |  |  |  | High frequency setpoint for GFA devices |
| avg24 | double | Hz | PUBLIC |  |  |  | Average of last 24 hourly instantaneous measurements |
| std24 | double | Hz | PUBLIC |  |  |  | Standard deviation of last 24 hourly instantaneous measurements |
| avg168 | double | Hz | PUBLIC |  |  |  | Average of last 168 hourly instantaneous measurements |
| std168 | double | Hz | PUBLIC |  |  |  | Standard deviation of last 168 hourly instantaneous measurements |
| Num_Resp_Eqs | int32 |  | PUBLIC |  |  |  | Total number of equations the response can contain |

### volt_var_control

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| control_method | enumeration |  | PUBLIC |  | STANDBY ACTIVE |  | IVVC activated or in standby |
| capacitor_delay | double | s | PUBLIC |  |  |  | Default capacitor time delay - overridden by local defintions |
| regulator_delay | double | s | PUBLIC |  |  |  | Default regulator time delay - overriden by local definitions |
| desired_pf | double |  | PUBLIC |  |  |  | Desired power-factor magnitude at the substation transformer or regulator |
| d_max | double |  | PUBLIC |  |  |  | Scaling constant for capacitor switching on - typically 0.3 - 0.6 |
| d_min | double |  | PUBLIC |  |  |  | Scaling constant for capacitor switching off - typically 0.1 - 0.4 |
| substation_link | object |  | PUBLIC |  |  |  | Substation link, transformer, or regulator to measure power factor |
| pf_phase | set |  | PUBLIC | CHARSET | C B A |  | Phase to include in power factor monitoring |
| regulator_list | char1024 |  | PUBLIC |  |  |  | List of voltage regulators for the system, separated by commas |
| capacitor_list | char1024 |  | PUBLIC |  |  |  | List of controllable capacitors on the system separated by commas |
| voltage_measurements | char1024 |  | PUBLIC |  |  |  | List of voltage measurement devices, separated by commas |
| minimum_voltages | char1024 |  | PUBLIC |  |  |  | Minimum voltages allowed for feeder, separated by commas |
| maximum_voltages | char1024 |  | PUBLIC |  |  |  | Maximum voltages allowed for feeder, separated by commas |
| desired_voltages | char1024 |  | PUBLIC |  |  |  | Desired operating voltages for the regulators, separated by commas |
| max_vdrop | char1024 |  | PUBLIC |  |  |  | Maximum voltage drop between feeder and end measurements for each regulator, separated by commas |
| high_load_deadband | char1024 |  | PUBLIC |  |  |  | High loading case voltage deadband for each regulator, separated by commas |
| low_load_deadband | char1024 |  | PUBLIC |  |  |  | Low loading case voltage deadband for each regulator, separated by commas |
| pf_signed | bool |  | PUBLIC |  |  |  | Set to true to consider the sign on the power factor.  Otherwise, it just maintains the deadband of +/-desired_pf |

### fault_check

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| check_mode | enumeration |  | PUBLIC |  | ALL ONCHANGE SINGLE |  | Frequency of fault checks |
| output_filename | char1024 |  | PUBLIC |  |  |  | Output filename for list of unsupported nodes |
| reliability_mode | bool |  | PUBLIC |  |  |  | General flag indicating if fault_check is operating under faulting or restoration mode -- reliability set this |
| strictly_radial | bool |  | PUBLIC |  |  |  | Flag to indicate if a system is known to be strictly radial -- uses radial assumptions for reliability alterations |
| full_output_file | bool |  | PUBLIC |  |  |  | Flag to indicate if the output_filename report contains both supported and unsupported nodes -- if false, just does unsupported |
| grid_association | bool |  | PUBLIC |  |  |  | Flag to indicate if multiple, distinct grids are allowed in a GLM, or if anything not attached to the master swing is removed |
| eventgen_object | object |  | PUBLIC |  |  |  | Link to generic eventgen object to handle unexpected faults |

### motor

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| base_power | double | W | PUBLIC |  |  |  | base power |
| n | double |  | PUBLIC |  |  |  | ratio of stator auxiliary windings to stator main windings |
| Rds | double | ohm | PUBLIC |  |  |  | d-axis resistance |
| Rqs | double | ohm | PUBLIC |  |  |  | q-asis resistance |
| Rr | double | ohm | PUBLIC |  |  |  | rotor resistance |
| Xm | double | ohm | PUBLIC |  |  |  | magnetizing reactance |
| Xr | double | ohm | PUBLIC |  |  |  | rotor reactance |
| Xc_run | double | ohm | PUBLIC |  |  |  | running capacitor reactance |
| Xc_start | double | ohm | PUBLIC |  |  |  | starting capacitor reactance |
| Xd_prime | double | ohm | PUBLIC |  |  |  | d-axis reactance |
| Xq_prime | double | ohm | PUBLIC |  |  |  | q-axis reactance |
| A_sat | double |  | PUBLIC |  |  |  | flux saturation parameter, A |
| b_sat | double |  | PUBLIC |  |  |  | flux saturation parameter, b |
| H | double |  | PUBLIC |  |  |  | moment of inertia |
| To_prime | double | s | PUBLIC |  |  |  | rotor time constant |
| capacitor_speed | double | % | PUBLIC |  |  |  | percentage speed of nominal when starting capacitor kicks in |
| trip_time | double | s | PUBLIC |  |  |  | time motor can stay stalled before tripping off  |
| reconnect_time | double | s | PUBLIC |  |  |  | time before tripped motor reconnects |
| mechanical_torque | double |  | PUBLIC |  |  |  | mechanical torque applied to the motor |
| iteration_count | double |  | PUBLIC |  |  |  | maximum number of iterations for steady state model |
| delta_mode_voltage_trigger | double | % | PUBLIC |  |  |  | percentage voltage of nominal when delta mode is triggered |
| delta_mode_rotor_speed_trigger | double | % | PUBLIC |  |  |  | percentage speed of nominal when delta mode is triggered |
| delta_mode_voltage_exit | double | % | PUBLIC |  |  |  | percentage voltage of nominal to exit delta mode |
| delta_mode_rotor_speed_exit | double | % | PUBLIC |  |  |  | percentage speed of nominal to exit delta mode |
| maximum_speed_error | double |  | PUBLIC |  |  |  | maximum speed error for the steady state model |
| wr | double |  | PUBLIC |  |  |  | rotor speed |
| motor_status | enumeration |  | PUBLIC |  | OFF TRIPPED STALLED RUNNING |  | the current status of the motor |
| motor_status_number | int32 |  | PUBLIC |  |  |  | the current status of the motor as an integer |
| motor_override | enumeration |  | PUBLIC |  | OFF ON |  | override function to dictate if motor is turned off or on |
| motor_operation_type | enumeration |  | PUBLIC |  | THREE-PHASE SINGLE-PHASE |  | current operation type of the motor - deltamode related |
| triplex_connection_type | enumeration |  | PUBLIC |  | TRIPLEX_12 TRIPLEX_2N TRIPLEX_1N |  | Describes how the motor will connect to the triplex devices |
| wb | double | rad/s | HIDDEN |  |  |  | base speed |
| ws | double |  | HIDDEN |  |  |  | system speed |
| psi_b | complex |  | HIDDEN |  |  |  | backward rotating flux |
| psi_f | complex |  | HIDDEN |  |  |  | forward rotating flux |
| psi_dr | complex |  | HIDDEN |  |  |  | Rotor d axis flux |
| psi_qr | complex |  | HIDDEN |  |  |  | Rotor q axis flux |
| Ids | complex |  | HIDDEN |  |  |  | time before tripped motor reconnects |
| Iqs | complex |  | HIDDEN |  |  |  | time before tripped motor reconnects |
| If | complex |  | HIDDEN |  |  |  | forward current |
| Ib | complex |  | HIDDEN |  |  |  | backward current |
| Is | complex |  | HIDDEN |  |  |  | motor current |
| Ss | complex |  | HIDDEN |  |  |  | motor power |
| electrical_torque | double |  | HIDDEN |  |  |  | electrical torque |
| Vs | complex |  | HIDDEN |  |  |  | motor voltage |
| motor_trip | bool |  | PUBLIC |  |  |  | boolean variable to check if motor is tripped |
| trip | double |  | HIDDEN |  |  |  | current time in tripped state |
| reconnect | double |  | HIDDEN |  |  |  | current time since motor was tripped |
| rs | double | pu | PUBLIC |  |  |  | stator resistance |
| rr | double | pu | PUBLIC |  |  |  | rotor resistance |
| lm | double | pu | PUBLIC |  |  |  | magnetizing reactance |
| lls | double | pu | PUBLIC |  |  |  | stator leakage reactance |
| llr | double | pu | PUBLIC |  |  |  | rotor leakage reactance |
| TPIM_rated_mechanical_Load_torque | double |  | PUBLIC |  |  |  | rated mechanical load torque applied to three-phase induction motor |
| friction_coefficient | double |  | PUBLIC |  |  |  | coefficient of speed-dependent torque |
| TPIM_initial_status | enumeration |  | PUBLIC |  | STATIONARY RUNNING |  | initial status of three-phase induction motor: RUNNING or STATIONARY |
| wsyn | double |  | HIDDEN |  |  |  | system frequency in pu |
| phips | complex |  | HIDDEN |  |  |  | positive sequence stator flux |
| phins_cj | complex |  | HIDDEN |  |  |  | conjugate of negative sequence stator flux |
| phipr | complex |  | HIDDEN |  |  |  | positive sequence rotor flux |
| phinr_cj | complex |  | HIDDEN |  |  |  | conjugate of negative sequence rotor flux |
| omgr0 | double |  | HIDDEN |  |  |  | dc component of rotor speed |
| TL | double |  | HIDDEN |  |  |  | actually applied mechanical torque |
| Ias | complex |  | HIDDEN |  |  |  | motor phase-a stator current |
| Ibs | complex |  | HIDDEN |  |  |  | motor phase-b stator current |
| Ics | complex |  | HIDDEN |  |  |  | motor phase-c stator current |
| Smt | complex |  | HIDDEN |  |  |  | motor complex power |
| Vas | complex |  | HIDDEN |  |  |  | motor phase-a stator-to-ground voltage |
| Vbs | complex |  | HIDDEN |  |  |  | motor phase-b stator-to-ground voltage |
| Vcs | complex |  | HIDDEN |  |  |  | motor phase-c stator-to-ground voltage |
| Ips | complex |  | HIDDEN |  |  |  | positive sequence stator current |
| Ipr | complex |  | HIDDEN |  |  |  | positive sequence rotor current |
| Ins_cj | complex |  | HIDDEN |  |  |  | conjugate of negative sequence stator current |
| Inr_cj | complex |  | HIDDEN |  |  |  | conjugate of negative sequence rotor current |
| Ls | double |  | HIDDEN |  |  |  | stator synchronous reactance |
| Lr | double |  | HIDDEN |  |  |  | rotor synchronous reactance |
| sigma1 | double |  | HIDDEN |  |  |  | intermediate variable 1 associated with synch. react. |
| sigma2 | double |  | HIDDEN |  |  |  | intermediate variable 2 associated with synch. react. |

### billdump

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| group | char32 |  | PUBLIC |  |  |  | the group ID to output data for (all nodes if empty) |
| runtime | timestamp |  | PUBLIC |  |  |  | the time to check voltage data |
| filename | char256 |  | PUBLIC |  |  |  | the file to dump the voltage data into |
| runcount | int32 |  | REFERENCE |  |  |  | the number of times the file has been written to |
| meter_type | enumeration |  | PUBLIC |  | METER TRIPLEX_METER |  | describes whether to collect from 3-phase or S-phase meters |

### power_metrics

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| SAIFI | double |  | PUBLIC |  |  |  | Displays annual SAIFI values as per IEEE 1366-2003 |
| SAIFI_int | double |  | PUBLIC |  |  |  | Displays SAIFI values over the period specified by base_time_value as per IEEE 1366-2003 |
| SAIDI | double |  | PUBLIC |  |  |  | Displays annual SAIDI values as per IEEE 1366-2003 |
| SAIDI_int | double |  | PUBLIC |  |  |  | Displays SAIDI values over the period specified by base_time_value as per IEEE 1366-2003 |
| CAIDI | double |  | PUBLIC |  |  |  | Displays annual CAIDI values as per IEEE 1366-2003 |
| CAIDI_int | double |  | PUBLIC |  |  |  | Displays SAIDI values over the period specified by base_time_value as per IEEE 1366-2003 |
| ASAI | double |  | PUBLIC |  |  |  | Displays annual AISI values as per IEEE 1366-2003 |
| ASAI_int | double |  | PUBLIC |  |  |  | Displays AISI values over the period specified by base_time_value as per IEEE 1366-2003 |
| MAIFI | double |  | PUBLIC |  |  |  | Displays annual MAIFI values as per IEEE 1366-2003 |
| MAIFI_int | double |  | PUBLIC |  |  |  | Displays MAIFI values over the period specified by base_time_value as per IEEE 1366-2003 |
| base_time_value | double | s | PUBLIC |  |  |  | time period over which _int values are claculated |

### currdump

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| group | char32 |  | PUBLIC |  |  |  | the group ID to output data for (all links if empty) |
| runtime | timestamp |  | PUBLIC |  |  | NEVER | the time to check current data |
| filename | char256 |  | PUBLIC | REQUIRED |  |  | the file to dump the current data into |
| runcount | int32 |  | REFERENCE |  |  | 0 | the number of times the file has been written to |
| maxcount | int32 |  | PUBLIC |  |  | 1 | the maximum number of times the file is written |
| interval | double | s | PUBLIC |  |  | 0 s | interval at which currdump runs |
| filemode | char8 |  | PUBLIC |  |  | w | sets the file write mode |
| mode | enumeration |  | PUBLIC |  | POLAR RECT | RECT |  |
| version | int32 |  | PUBLIC |  |  | 0 | dump format version |

### recloser

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| retry_time | double | s | PUBLIC |  |  |  | the amount of time in seconds to wait before the recloser attempts to close |
| max_number_of_tries | double |  | PUBLIC |  |  |  | the number of times the recloser will try to close before permanently opening |
| number_of_tries | double |  | PUBLIC | OUTPUT |  |  | Current number of tries recloser has attempted |

### sectionalizer

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |

### emissions

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| Nuclear_Order | double |  | PUBLIC |  |  |  |  |
| Hydroelectric_Order | double |  | PUBLIC |  |  |  |  |
| Solarthermal_Order | double |  | PUBLIC |  |  |  |  |
| Biomass_Order | double |  | PUBLIC |  |  |  |  |
| Wind_Order | double |  | PUBLIC |  |  |  |  |
| Coal_Order | double |  | PUBLIC |  |  |  |  |
| Naturalgas_Order | double |  | PUBLIC |  |  |  |  |
| Geothermal_Order | double |  | PUBLIC |  |  |  |  |
| Petroleum_Order | double |  | PUBLIC |  |  |  |  |
| Naturalgas_Max_Out | double | kWh | PUBLIC |  |  |  |  |
| Coal_Max_Out | double | kWh | PUBLIC |  |  |  |  |
| Biomass_Max_Out | double | kWh | PUBLIC |  |  |  |  |
| Geothermal_Max_Out | double | kWh | PUBLIC |  |  |  |  |
| Hydroelectric_Max_Out | double | kWh | PUBLIC |  |  |  |  |
| Nuclear_Max_Out | double | kWh | PUBLIC |  |  |  |  |
| Wind_Max_Out | double | kWh | PUBLIC |  |  |  |  |
| Petroleum_Max_Out | double | kWh | PUBLIC |  |  |  |  |
| Solarthermal_Max_Out | double | kWh | PUBLIC |  |  |  |  |
| Naturalgas_Out | double | kWh | PUBLIC |  |  |  |  |
| Coal_Out | double | kWh | PUBLIC |  |  |  |  |
| Biomass_Out | double | kWh | PUBLIC |  |  |  |  |
| Geothermal_Out | double | kWh | PUBLIC |  |  |  |  |
| Hydroelectric_Out | double | kWh | PUBLIC |  |  |  |  |
| Nuclear_Out | double | kWh | PUBLIC |  |  |  |  |
| Wind_Out | double | kWh | PUBLIC |  |  |  |  |
| Petroleum_Out | double | kWh | PUBLIC |  |  |  |  |
| Solarthermal_Out | double | kWh | PUBLIC |  |  |  |  |
| Naturalgas_Conv_Eff | double | Btu/kWh | PUBLIC |  |  |  |  |
| Coal_Conv_Eff | double | Btu/kWh | PUBLIC |  |  |  |  |
| Biomass_Conv_Eff | double | Btu/kWh | PUBLIC |  |  |  |  |
| Geothermal_Conv_Eff | double | Btu/kWh | PUBLIC |  |  |  |  |
| Hydroelectric_Conv_Eff | double | Btu/kWh | PUBLIC |  |  |  |  |
| Nuclear_Conv_Eff | double | Btu/kWh | PUBLIC |  |  |  |  |
| Wind_Conv_Eff | double | Btu/kWh | PUBLIC |  |  |  |  |
| Petroleum_Conv_Eff | double | Btu/kWh | PUBLIC |  |  |  |  |
| Solarthermal_Conv_Eff | double | Btu/kWh | PUBLIC |  |  |  |  |
| Naturalgas_CO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Coal_CO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Biomass_CO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Geothermal_CO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Hydroelectric_CO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Nuclear_CO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Wind_CO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Petroleum_CO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Solarthermal_CO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Naturalgas_SO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Coal_SO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Biomass_SO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Geothermal_SO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Hydroelectric_SO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Nuclear_SO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Wind_SO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Petroleum_SO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Solarthermal_SO2 | double | lb/Btu | PUBLIC |  |  |  |  |
| Naturalgas_NOx | double | lb/Btu | PUBLIC |  |  |  |  |
| Coal_NOx | double | lb/Btu | PUBLIC |  |  |  |  |
| Biomass_NOx | double | lb/Btu | PUBLIC |  |  |  |  |
| Geothermal_NOx | double | lb/Btu | PUBLIC |  |  |  |  |
| Hydroelectric_NOx | double | lb/Btu | PUBLIC |  |  |  |  |
| Nuclear_NOx | double | lb/Btu | PUBLIC |  |  |  |  |
| Wind_NOx | double | lb/Btu | PUBLIC |  |  |  |  |
| Petroleum_NOx | double | lb/Btu | PUBLIC |  |  |  |  |
| Solarthermal_NOx | double | lb/Btu | PUBLIC |  |  |  |  |
| Naturalgas_emissions_CO2 | double | lb | PUBLIC |  |  |  |  |
| Naturalgas_emissions_SO2 | double | lb | PUBLIC |  |  |  |  |
| Naturalgas_emissions_NOx | double | lb | PUBLIC |  |  |  |  |
| Coal_emissions_CO2 | double | lb | PUBLIC |  |  |  |  |
| Coal_emissions_SO2 | double | lb | PUBLIC |  |  |  |  |
| Coal_emissions_NOx | double | lb | PUBLIC |  |  |  |  |
| Biomass_emissions_CO2 | double | lb | PUBLIC |  |  |  |  |
| Biomass_emissions_SO2 | double | lb | PUBLIC |  |  |  |  |
| Biomass_emissions_NOx | double | lb | PUBLIC |  |  |  |  |
| Geothermal_emissions_CO2 | double | lb | PUBLIC |  |  |  |  |
| Geothermal_emissions_SO2 | double | lb | PUBLIC |  |  |  |  |
| Geothermal_emissions_NOx | double | lb | PUBLIC |  |  |  |  |
| Hydroelectric_emissions_CO2 | double | lb | PUBLIC |  |  |  |  |
| Hydroelectric_emissions_SO2 | double | lb | PUBLIC |  |  |  |  |
| Hydroelectric_emissions_NOx | double | lb | PUBLIC |  |  |  |  |
| Nuclear_emissions_CO2 | double | lb | PUBLIC |  |  |  |  |
| Nuclear_emissions_SO2 | double | lb | PUBLIC |  |  |  |  |
| Nuclear_emissions_NOx | double | lb | PUBLIC |  |  |  |  |
| Wind_emissions_CO2 | double | lb | PUBLIC |  |  |  |  |
| Wind_emissions_SO2 | double | lb | PUBLIC |  |  |  |  |
| Wind_emissions_NOx | double | lb | PUBLIC |  |  |  |  |
| Petroleum_emissions_CO2 | double | lb | PUBLIC |  |  |  |  |
| Petroleum_emissions_SO2 | double | lb | PUBLIC |  |  |  |  |
| Petroleum_emissions_NOx | double | lb | PUBLIC |  |  |  |  |
| Solarthermal_emissions_CO2 | double | lb | PUBLIC |  |  |  |  |
| Solarthermal_emissions_SO2 | double | lb | PUBLIC |  |  |  |  |
| Solarthermal_emissions_NOx | double | lb | PUBLIC |  |  |  |  |
| Total_emissions_CO2 | double | lb | PUBLIC |  |  |  |  |
| Total_emissions_SO2 | double | lb | PUBLIC |  |  |  |  |
| Total_emissions_NOx | double | lb | PUBLIC |  |  |  |  |
| Total_energy_out | double | kWh | PUBLIC |  |  |  |  |
| Region | double |  | PUBLIC |  |  |  |  |
| cycle_interval | double | s | PUBLIC |  |  |  |  |

### load_tracker

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| target | object |  | PUBLIC |  |  |  | target object to track the load of |
| target_property | char256 |  | PUBLIC |  |  |  | property on the target object representing the load |
| operation | enumeration |  | PUBLIC |  | ANGLE MAGNITUDE IMAGINARY REAL |  | operation to perform on complex property types |
| full_scale | double |  | PUBLIC |  |  |  | magnitude of the load at full load, used for feed-forward control |
| setpoint | double |  | PUBLIC |  |  |  | load setpoint to track to |
| deadband | double |  | PUBLIC |  |  |  | percentage deadband |
| damping | double |  | PUBLIC |  |  |  | load setpoint to track to |
| output | double |  | PUBLIC |  |  |  | output scaling value |
| feedback | double |  | PUBLIC |  |  |  | the feedback signal, for reference purposes |

### triplex_load

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| load_class | enumeration |  | PUBLIC |  | A I C R U |  | Flag to track load type, not currently used for anything except sorting |
| constant_power_1 | complex | VA | PUBLIC |  |  |  | constant power load on split phase 1, specified as VA |
| constant_power_2 | complex | VA | PUBLIC |  |  |  | constant power load on split phase 2, specified as VA |
| constant_power_12 | complex | VA | PUBLIC |  |  |  | constant power load on split phase 12, specified as VA |
| constant_power_1_real | double | W | PUBLIC |  |  |  | constant power load on spit phase 1, real only, specified as W |
| constant_power_2_real | double | W | PUBLIC |  |  |  | constant power load on phase 2, real only, specified as W |
| constant_power_12_real | double | W | PUBLIC |  |  |  | constant power load on phase 12, real only, specified as W |
| constant_power_1_reac | double | VAr | PUBLIC |  |  |  | constant power load on phase 1, imaginary only, specified as VAr |
| constant_power_2_reac | double | VAr | PUBLIC |  |  |  | constant power load on phase 2, imaginary only, specified as VAr |
| constant_power_12_reac | double | VAr | PUBLIC |  |  |  | constant power load on phase 12, imaginary only, specified as VAr |
| constant_current_1 | complex | A | PUBLIC |  |  |  | constant current load on phase 1, specified as Amps |
| constant_current_2 | complex | A | PUBLIC |  |  |  | constant current load on phase 2, specified as Amps |
| constant_current_12 | complex | A | PUBLIC |  |  |  | constant current load on phase 12, specified as Amps |
| constant_current_1_real | double | A | PUBLIC |  |  |  | constant current load on phase 1, real only, specified as Amps |
| constant_current_2_real | double | A | PUBLIC |  |  |  | constant current load on phase 2, real only, specified as Amps |
| constant_current_12_real | double | A | PUBLIC |  |  |  | constant current load on phase 12, real only, specified as Amps |
| constant_current_1_reac | double | A | PUBLIC |  |  |  | constant current load on phase 1, imaginary only, specified as Amps |
| constant_current_2_reac | double | A | PUBLIC |  |  |  | constant current load on phase 2, imaginary only, specified as Amps |
| constant_current_12_reac | double | A | PUBLIC |  |  |  | constant current load on phase 12, imaginary only, specified as Amps |
| constant_impedance_1 | complex | Ohm | PUBLIC |  |  |  | constant impedance load on phase 1, specified as Ohms |
| constant_impedance_2 | complex | Ohm | PUBLIC |  |  |  | constant impedance load on phase 2, specified as Ohms |
| constant_impedance_12 | complex | Ohm | PUBLIC |  |  |  | constant impedance load on phase 12, specified as Ohms |
| constant_impedance_1_real | double | Ohm | PUBLIC |  |  |  | constant impedance load on phase 1, real only, specified as Ohms |
| constant_impedance_2_real | double | Ohm | PUBLIC |  |  |  | constant impedance load on phase 2, real only, specified as Ohms |
| constant_impedance_12_real | double | Ohm | PUBLIC |  |  |  | constant impedance load on phase 12, real only, specified as Ohms |
| constant_impedance_1_reac | double | Ohm | PUBLIC |  |  |  | constant impedance load on phase 1, imaginary only, specified as Ohms |
| constant_impedance_2_reac | double | Ohm | PUBLIC |  |  |  | constant impedance load on phase 2, imaginary only, specified as Ohms |
| constant_impedance_12_reac | double | Ohm | PUBLIC |  |  |  | constant impedance load on phase 12, imaginary only, specified as Ohms |
| measured_voltage_1 | complex | V | PUBLIC |  |  |  | measured voltage on phase 1 |
| measured_voltage_2 | complex | V | PUBLIC |  |  |  | measured voltage on phase 2 |
| measured_voltage_12 | complex | V | PUBLIC |  |  |  | measured voltage on phase 12 |
| base_power_1 | double | VA | PUBLIC |  |  |  | in similar format as ZIPload, this represents the nominal power on phase 1 before applying ZIP fractions |
| base_power_2 | double | VA | PUBLIC |  |  |  | in similar format as ZIPload, this represents the nominal power on phase 2 before applying ZIP fractions |
| base_power_12 | double | VA | PUBLIC |  |  |  | in similar format as ZIPload, this represents the nominal power on phase 12 before applying ZIP fractions |
| power_pf_1 | double | pu | PUBLIC |  |  |  | in similar format as ZIPload, this is the power factor of the phase 1 constant power portion of load |
| current_pf_1 | double | pu | PUBLIC |  |  |  | in similar format as ZIPload, this is the power factor of the phase 1 constant current portion of load |
| impedance_pf_1 | double | pu | PUBLIC |  |  |  | in similar format as ZIPload, this is the power factor of the phase 1 constant impedance portion of load |
| power_pf_2 | double | pu | PUBLIC |  |  |  | in similar format as ZIPload, this is the power factor of the phase 2 constant power portion of load |
| current_pf_2 | double | pu | PUBLIC |  |  |  | in similar format as ZIPload, this is the power factor of the phase 2 constant current portion of load |
| impedance_pf_2 | double | pu | PUBLIC |  |  |  | in similar format as ZIPload, this is the power factor of the phase 2 constant impedance portion of load |
| power_pf_12 | double | pu | PUBLIC |  |  |  | in similar format as ZIPload, this is the power factor of the phase 12 constant power portion of load |
| current_pf_12 | double | pu | PUBLIC |  |  |  | in similar format as ZIPload, this is the power factor of the phase 12 constant current portion of load |
| impedance_pf_12 | double | pu | PUBLIC |  |  |  | in similar format as ZIPload, this is the power factor of the phase 12 constant impedance portion of load |
| power_fraction_1 | double | pu | PUBLIC |  |  |  | this is the constant power fraction of base power on phase 1 |
| current_fraction_1 | double | pu | PUBLIC |  |  |  | this is the constant current fraction of base power on phase 1 |
| impedance_fraction_1 | double | pu | PUBLIC |  |  |  | this is the constant impedance fraction of base power on phase 1 |
| power_fraction_2 | double | pu | PUBLIC |  |  |  | this is the constant power fraction of base power on phase 2 |
| current_fraction_2 | double | pu | PUBLIC |  |  |  | this is the constant current fraction of base power on phase 2 |
| impedance_fraction_2 | double | pu | PUBLIC |  |  |  | this is the constant impedance fraction of base power on phase 2 |
| power_fraction_12 | double | pu | PUBLIC |  |  |  | this is the constant power fraction of base power on phase 12 |
| current_fraction_12 | double | pu | PUBLIC |  |  |  | this is the constant current fraction of base power on phase 12 |
| impedance_fraction_12 | double | pu | PUBLIC |  |  |  | this is the constant impedance fraction of base power on phase 12 |

### impedance_dump

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| group | char32 |  | PUBLIC |  |  |  | the group ID to output data for (all links if empty) |
| filename | char256 |  | PUBLIC |  |  |  | the file to dump the current data into |
| runtime | timestamp |  | PUBLIC |  |  |  | the time to check voltage data |
| runcount | int32 |  | REFERENCE |  |  |  | the number of times the file has been written to |

### vfd

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| rated_motor_speed | double | 1/min | PUBLIC |  |  |  | Rated speed of the VFD in RPM. Default = 1800 RPM |
| desired_motor_speed | double | 1/min | PUBLIC |  |  |  | Desired speed of the VFD In ROM. Default = 1800 RPM (max) |
| motor_poles | double |  | PUBLIC |  |  |  | Number of Motor Poles. Default = 4 |
| rated_output_voltage | double | V | PUBLIC |  |  |  | Line to Line Voltage - VFD Rated voltage. Default to TO node nominal_voltage |
| rated_horse_power | double | hp | PUBLIC |  |  |  | Rated Horse Power of the VFD. Default = 75 HP |
| nominal_output_frequency | double | Hz | PUBLIC |  |  |  | Nominal VFD output frequency. Default = 60 Hz |
| desired_output_frequency | double | Hz | PUBLIC |  |  |  | VFD desired output frequency based on the desired RPM |
| current_output_frequency | double | Hz | PUBLIC |  |  |  | VFD currently output frequency |
| efficiency | double | % | PUBLIC |  |  |  | Current VFD efficiency based on the load/VFD output Horsepower |
| stable_time | double | s | PUBLIC |  |  |  | Time taken by the VFD to reach desired frequency (based on RPM). Default = 1.45 seconds |
| vfd_state | enumeration |  | PUBLIC |  | STEADY_STATE CHANGING OFF |  | Current state of the VFD |

### pole

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| pole_status | enumeration |  | PUBLIC |  | FAILED OK |  | pole status |
| tilt_angle | double | rad | PUBLIC |  |  |  | tilt angle of pole |
| tilt_direction | double | deg | PUBLIC |  |  |  | tilt direction of pole |
| weather | object |  | PUBLIC |  |  |  | weather data |
| configuration | object |  | PUBLIC |  |  |  | configuration data |
| equipment_area | double | sf | PUBLIC |  |  |  | equipment cross sectional area |
| equipment_height | double | ft | PUBLIC |  |  |  | equipment height on pole |
| pole_stress | double | pu | PUBLIC |  |  |  | ratio of actual stress to critical stress |
| pole_stress_polynomial_a | double | ft*lb | PUBLIC |  |  |  | constant a of the pole stress polynomial function |
| pole_stress_polynomial_b | double | ft*lb | PUBLIC |  |  |  | constant b of the pole stress polynomial function |
| pole_stress_polynomial_c | double | ft*lb | PUBLIC |  |  |  | constant c of the pole stress polynomial function |
| susceptibility | double | pu*s/m | PUBLIC |  |  |  | susceptibility of pole to wind stress (derivative of pole stress w.r.t wind speed) |
| total_moment | double | ft*lb | PUBLIC |  |  |  | the total moment on the pole. |
| resisting_moment | double | ft*lb | PUBLIC |  |  |  | the resisting moment on the pole. |
| critical_wind_speed | double | m/s | PUBLIC |  |  |  | wind speed at pole failure |
| install_year | int32 |  | PUBLIC |  |  |  | the year of pole was installed |
| repair_time | double | h | PUBLIC |  |  |  | typical repair time after pole failure |

### pole_configuration

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| pole_type | enumeration |  | PUBLIC |  | STEEL CONCRETE WOOD |  | material from which pole is made |
| design_ice_thickness | double | in | PUBLIC |  |  |  | design ice thickness on conductors |
| design_wind_loading | double | psi | PUBLIC |  |  |  | design wind loading on pole |
| design_temperature | double | degF | PUBLIC |  |  |  | design temperature for pole |
| overload_factor_vertical | double |  | PUBLIC |  |  |  | design overload factor (vertical) |
| overload_factor_transverse_general | double |  | PUBLIC |  |  |  | design overload factor (transverse general) |
| overload_factor_transverse_crossing | double |  | PUBLIC |  |  |  | design overload factor (transverse crossing) |
| overload_factor_transverse_wire | double |  | PUBLIC |  |  |  | design overload factor (transverse wire) |
| overload_factor_longitudinal_general | double |  | PUBLIC |  |  |  | design overload factor (longitudinal general) |
| overload_factor_longitudinal_deadend | double |  | PUBLIC |  |  |  | design overload factor (longitudinal deadend) |
| strength_factor_250b_wood | double |  | PUBLIC |  |  |  | design strength factor (Rule 250B wood structure) |
| strength_factor_250b_support | double |  | PUBLIC |  |  |  | design strength factor (Rule 250B support hardware) |
| strength_factor_250c_wood | double |  | PUBLIC |  |  |  | design strength factor (Rule 250C wood structure) |
| strength_factor_250c_support | double |  | PUBLIC |  |  |  | design strength factor (Rule 250C support hardware) |
| pole_length | double | ft | PUBLIC |  |  |  | total length of pole (including underground portion) |
| pole_depth | double | ft | PUBLIC |  |  |  | depth of pole underground |
| ground_diameter | double | in | PUBLIC |  |  |  | diameter of pole at ground level |
| top_diameter | double | in | PUBLIC |  |  |  | diameter of pole at top |
| fiber_strength | double | psi | PUBLIC |  |  |  | pole structural strength |
| repair_time | double | h | PUBLIC |  |  |  | pole repair time |
| degradation_rate | double | in/yr | PUBLIC |  |  |  | rate of pole degradation. |
| treatment_method | enumeration |  | PUBLIC |  | CCA PENTA CREOSOTE NONE |  | pole degradation prevention treatment |

### metrics

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| report_file | char1024 |  | PUBLIC |  |  |  |  |
| customer_group | char1024 |  | PUBLIC |  |  |  |  |
| module_metrics_object | object |  | PUBLIC |  |  |  |  |
| metrics_of_interest | char1024 |  | PUBLIC |  |  |  |  |
| metric_interval | double | s | PUBLIC |  |  |  |  |
| report_interval | double | s | PUBLIC |  |  |  |  |

### eventgen

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| target_group | char1024 |  | PUBLIC |  |  |  |  |
| fault_type | char256 |  | PUBLIC |  |  |  |  |
| failure_dist | enumeration |  | PUBLIC |  | TRIANGLE BETA GAMMA WEIBULL RAYLEIGH EXPONENTIAL PARETO BERNOULLI LOGNORMAL NORMAL UNIFORM |  |  |
| restoration_dist | enumeration |  | PUBLIC |  | TRIANGLE BETA GAMMA WEIBULL RAYLEIGH EXPONENTIAL PARETO BERNOULLI LOGNORMAL NORMAL UNIFORM |  |  |
| failure_dist_param_1 | double |  | PUBLIC |  |  |  |  |
| failure_dist_param_2 | double |  | PUBLIC |  |  |  |  |
| restoration_dist_param_1 | double |  | PUBLIC |  |  |  |  |
| restoration_dist_param_2 | double |  | PUBLIC |  |  |  |  |
| manual_outages | char1024 |  | PUBLIC |  |  |  |  |
| max_outage_length | double | s | PUBLIC |  |  |  |  |
| max_simultaneous_faults | int32 |  | PUBLIC |  |  |  |  |
| controlled_switch | char256 |  | PUBLIC |  |  |  | Name of a switch to manually fault/un-fault |
| switch_state | int32 |  | PUBLIC |  |  |  | Current state (1=closed, 0=open) for the controlled switch |
| external_fault_event | char1024 |  | PUBLIC |  |  |  | This variable is populated from external programs with a fault they would like to add/remove to the system. |
| use_external_faults | bool |  | PUBLIC |  |  |  | Boolean to let the object know to check for faults messages from an external source. |

### cost

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| total | double | $ | PUBLIC | OUTPUT |  |  | Total cost of outages |
| type | set |  | PUBLIC |  | WAGES LIGHTS COOKING HEATING ELDERCARE BUSINESS LODGING GEN_ALL GEN_COMFORT GEN_BASIC SEPTIC WATER FOOD |  | Type of costs incurred during outages |

### residential_enduse

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| shape | loadshape |  | PUBLIC |  |  |  |  |
| load | enduse |  | PUBLIC |  |  |  | the enduse load description |
| energy | complex | kVAh | PUBLIC |  |  |  | the total energy consumed since the last meter reading |
| power | complex | kVA | PUBLIC |  |  |  | the total power consumption of the load |
| peak_demand | complex | kVA | PUBLIC |  |  |  | the peak power consumption since the last meter reading |
| heatgain | double | Btu/h | PUBLIC |  |  |  | the heat transferred from the enduse to the parent |
| cumulative_heatgain | double | Btu | PUBLIC |  |  |  | the cumulative heatgain from the enduse to the parent |
| heatgain_fraction | double | pu | PUBLIC |  |  |  | the fraction of the heat that goes to the parent |
| current_fraction | double | pu | PUBLIC |  |  |  | the fraction of total power that is constant current |
| impedance_fraction | double | pu | PUBLIC |  |  |  | the fraction of total power that is constant impedance |
| power_fraction | double | pu | PUBLIC |  |  |  | the fraction of the total power that is constant power |
| power_factor | double |  | PUBLIC |  |  |  | the power factor of the load |
| constant_power | complex | kVA | PUBLIC |  |  |  | the constant power portion of the total load |
| constant_current | complex | kVA | PUBLIC |  |  |  | the constant current portion of the total load |
| constant_admittance | complex | kVA | PUBLIC |  |  |  | the constant admittance portion of the total load |
| voltage_factor | double | pu | PUBLIC |  |  |  | the voltage change factor |
| breaker_amps | double | A | PUBLIC |  |  |  | the rated breaker amperage |
| configuration | set |  | PUBLIC |  | IS220 IS110 |  | the load configuration options |
| override | enumeration |  | PUBLIC |  | OFF ON NORMAL | NORMAL |  |
| power_state | enumeration |  | PUBLIC |  | UNKNOWN ON OFF | OFF |  |

### appliance

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| powers | complex_array |  | PUBLIC |  |  |  |  |
| impedances | complex_array |  | PUBLIC |  |  |  |  |
| currents | complex_array |  | PUBLIC |  |  |  |  |
| durations | double_array |  | PUBLIC |  |  |  |  |
| transitions | double_array |  | PUBLIC |  |  |  |  |
| heatgains | double_array |  | PUBLIC |  |  |  |  |

### house

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| weather | object |  | PUBLIC |  |  |  | reference to the climate object |
| floor_area | double | sf | PUBLIC |  |  | +0 | home conditioned floor area |
| gross_wall_area | double | sf | PUBLIC |  |  | +0 | gross outdoor wall area |
| ceiling_height | double | ft | PUBLIC |  |  | +0 | average ceiling height |
| aspect_ratio | double |  | PUBLIC |  |  | +0 | aspect ratio of the home's footprint |
| envelope_UA | double | Btu/degF/h | PUBLIC |  |  | +0 | overall UA of the home's envelope |
| window_wall_ratio | double |  | PUBLIC |  |  | +0 | ratio of window area to wall area |
| number_of_doors | double |  | PUBLIC |  |  | +0 | ratio of door area to wall area |
| exterior_wall_fraction | double |  | PUBLIC |  |  | +0 | ratio of exterior wall area to total wall area |
| interior_exterior_wall_ratio | double |  | PUBLIC |  |  | +0 | ratio of interior to exterior walls |
| exterior_ceiling_fraction | double |  | PUBLIC |  |  | +0 | ratio of external ceiling sf to floor area |
| exterior_floor_fraction | double |  | PUBLIC |  |  | +0 | ratio of floor area used in UA calculation |
| window_shading | double |  | PUBLIC |  |  | +0 | transmission coefficient through window due to glazing |
| window_exterior_transmission_coefficient | double |  | PUBLIC |  |  | +0 | coefficient for the amount of energy that passes through window |
| solar_heatgain_factor | double |  | REFERENCE |  |  | +0 | product of the window area, window transmitivity, and the window exterior transmission coefficient |
| airchange_per_hour | double | unit/h | PUBLIC |  |  | -1 | number of air-changes per hour |
| airchange_UA | double | Btu/degF/h | PUBLIC |  |  | +0 | additional UA due to air infiltration |
| UA | double | Btu/degF/h | PUBLIC |  |  | -1 | the total UA |
| internal_gain | double | Btu/h | PUBLIC |  |  | +0 | internal heat gains |
| solar_gain | double | Btu/h | PUBLIC |  |  | +0 | solar heat gains |
| incident_solar_radiation | double | Btu/h/sf | PUBLIC |  |  | +0 | average incident solar radiation hitting the house |
| heat_cool_gain | double | Btu/h | PUBLIC |  |  | +0 | system heat gains(losses) |
| include_solar_quadrant | set |  | PUBLIC |  | ALL ALLWALLS W S E N H NONE | ALLWALLS | bit set for determining which solar incidence quadrants should be included in the solar heatgain |
| horizontal_diffuse_solar_radiation | double | Btu/h/sf | PUBLIC |  |  | +0 | incident solar radiation hitting the top of the house |
| north_incident_solar_radiation | double | Btu/h/sf | PUBLIC |  |  | +0 | incident solar radiation hitting the north side of the house |
| northwest_incident_solar_radiation | double | Btu/h/sf | PUBLIC |  |  | +0 | incident solar radiation hitting the northwest side of the house |
| west_incident_solar_radiation | double | Btu/h/sf | PUBLIC |  |  | +0 | incident solar radiation hitting the west side of the house |
| southwest_incident_solar_radiation | double | Btu/h/sf | PUBLIC |  |  | +0 | incident solar radiation hitting the southwest side of the house |
| south_incident_solar_radiation | double | Btu/h/sf | PUBLIC |  |  | +0 | incident solar radiation hitting the south side of the house |
| southeast_incident_solar_radiation | double | Btu/h/sf | PUBLIC |  |  | +0 | incident solar radiation hitting the southeast side of the house |
| east_incident_solar_radiation | double | Btu/h/sf | PUBLIC |  |  | +0 | incident solar radiation hitting the east side of the house |
| northeast_incident_solar_radiation | double | Btu/h/sf | PUBLIC |  |  | +0 | incident solar radiation hitting the northeast side of the house |
| heating_cop_curve | enumeration |  | PUBLIC |  | CURVED LINEAR FLAT DEFAULT | DEFAULT | Defines the function type to use for the adjusted heating COP as a function of outside air temperature. |
| heating_cap_curve | enumeration |  | PUBLIC |  | CURVED LINEAR FLAT DEFAULT | DEFAULT | Defines the function type to use for the adjusted heating capacity as a function of outside air temperature. |
| cooling_cop_curve | enumeration |  | PUBLIC |  | CURVED LINEAR FLAT DEFAULT | DEFAULT | Defines the function type to use for the adjusted cooling COP as a function of outside air temperature. |
| cooling_cap_curve | enumeration |  | PUBLIC |  | CURVED LINEAR FLAT DEFAULT | DEFAULT | Defines the function type to use for the adjusted cooling capacity as a function of outside air temperature. |
| use_latent_heat | bool |  | PUBLIC |  |  | TRUE | Boolean for using the heat latency of the air to the humidity when cooling. |
| include_fan_heatgain | bool |  | PUBLIC |  |  | TRUE | Boolean to choose whether to include the heat generated by the fan in the ETP model. |
| thermostat_deadband | double | degF | PUBLIC |  |  | +0 | deadband of thermostat control |
| dlc_offset | double | degF | PUBLIC |  |  | +0 | used as a cap to offset the thermostat deadband for direct load control applications |
| thermostat_cycle_time | int16 |  | PUBLIC |  |  | 0 | minimum time in seconds between thermostat updates |
| thermostat_off_cycle_time | int16 |  | PUBLIC |  |  | -1 | the minimum amount of time the thermostat cycle must stay in the off state |
| thermostat_on_cycle_time | int16 |  | PUBLIC |  |  | -1 | the minimum amount of time the thermostat cycle must stay in the on state |
| thermostat_last_cycle_time | timestamp |  | PUBLIC |  |  | 0 | last time the thermostat changed state |
| heating_setpoint | double | degF | PUBLIC |  |  | +0 | thermostat heating setpoint |
| cooling_setpoint | double | degF | PUBLIC |  |  | +0 | thermostat cooling setpoint |
| design_heating_setpoint | double | degF | PUBLIC |  |  | +0 | system design heating setpoint |
| design_cooling_setpoint | double | degF | PUBLIC |  |  | +0 | system design cooling setpoint |
| over_sizing_factor | double |  | PUBLIC |  |  | +0 | over sizes the heating and cooling system from standard specifications (0.2 ='s 120% sizing) |
| simulate_window_openings | bool |  | PUBLIC |  |  | FALSE | activates a representation of an occupant opening a window and de-activating the HVAC system |
| is_window_open | double |  | PUBLIC |  |  | +0 | defines the state of the window opening, 1=open, 2=closed |
| window_low_temperature_cutoff | double | degF | PUBLIC |  |  | +60 | lowest temperature at which the window opening might occur |
| window_high_temperature_cutoff | double | degF | PUBLIC |  |  | +80 | highest temperature at which the window opening might occur |
| window_quadratic_coefficient | double |  | PUBLIC |  |  | +0 | quadratic coefficient for describing function between low and high temperature cutoffs |
| window_linear_coefficient | double |  | PUBLIC |  |  | +0 | linear coefficient for describing function between low and high temperature cutoffs |
| window_constant_coefficient | double |  | PUBLIC |  |  | +1 | constant coefficient for describing function between low and high temperature cutoffs |
| window_temperature_delta | double |  | PUBLIC |  |  | +5 | change in outdoor temperature required to update the window opening model |
| design_heating_capacity | double | Btu/h | PUBLIC |  |  | +0 | system heating capacity |
| design_cooling_capacity | double | Btu/h | PUBLIC |  |  | +0 | system cooling capacity |
| cooling_design_temperature | double | degF | PUBLIC |  |  | +0 | system cooling design temperature |
| heating_design_temperature | double | degF | PUBLIC |  |  | +0 | system heating design temperature |
| design_peak_solar | double | Btu/h/sf | PUBLIC |  |  | +0 | system design solar load |
| design_internal_gains | double | Btu/h | PUBLIC |  |  | +0 | system design internal gains |
| air_heat_fraction | double | pu | PUBLIC |  |  | +0 | fraction of heat gain/loss that goes to air (as opposed to mass) |
| mass_solar_gain_fraction | double | pu | PUBLIC |  |  | +0 | fraction of the heat gain/loss from the solar gains that goes to the mass |
| mass_internal_gain_fraction | double | pu | PUBLIC |  |  | +0 | fraction of heat gain/loss from the internal gains that goes to the mass |
| auxiliary_heat_capacity | double | Btu/h | PUBLIC |  |  | +0 | installed auxiliary heating capacity |
| aux_heat_deadband | double | degF | PUBLIC |  |  | +0 | temperature offset from standard heat activation to auxiliary heat activation |
| aux_heat_temperature_lockout | double | degF | PUBLIC |  |  | +0 | temperature at which auxiliary heat will not engage above |
| aux_heat_time_delay | double | s | PUBLIC |  |  | +0 | time required for heater to run until auxiliary heating engages |
| cooling_supply_air_temp | double | degF | PUBLIC |  |  | +50 | temperature of air blown out of the cooling system |
| heating_supply_air_temp | double | degF | PUBLIC |  |  | +150 | temperature of air blown out of the heating system |
| duct_pressure_drop | double | inH2O | PUBLIC |  |  | +0 | end-to-end pressure drop for the ventilation ducts, in inches of water |
| fan_design_power | double | W | PUBLIC |  |  | -1 | designed maximum power draw of the ventilation fan |
| fan_low_power_fraction | double | pu | PUBLIC |  |  | +0 | fraction of ventilation fan power draw during low-power mode (two-speed only) |
| fan_power | double | kW | REFERENCE |  |  | +0 | current ventilation fan power draw |
| fan_design_airflow | double | cfm | PUBLIC |  |  | +0 | designed airflow for the ventilation system |
| fan_impedance_fraction | double | pu | PUBLIC |  |  | +0.2534 | Impedance component of fan ZIP load |
| fan_power_fraction | double | pu | PUBLIC |  |  | +0.0135 | Power component of fan ZIP load |
| fan_current_fraction | double | pu | PUBLIC |  |  | +0.7332 | Current component of fan ZIP load |
| fan_power_factor | double | pu | PUBLIC |  |  | +0.96 | Power factor of the fan load |
| heating_demand | double | kW | REFERENCE |  |  | +0 | the current power draw to run the heating system |
| cooling_demand | double | kW | REFERENCE |  |  | +0 | the current power draw to run the cooling system |
| heating_COP | double | pu | PUBLIC |  |  | +0 | system heating performance coefficient |
| cooling_COP | double | Btu/kWh | PUBLIC |  |  | +0 | system cooling performance coefficient |
| air_temperature | double | degF | PUBLIC | DYNAMIC |  | +0 | indoor air temperature |
| outdoor_temperature | double | degF | PUBLIC |  |  | +0 | outdoor air temperature |
| outdoor_rh | double | % | PUBLIC |  |  | +0 | outdoor relative humidity |
| mass_heat_capacity | double | Btu/degF | PUBLIC |  |  | +0 | interior mass heat capacity |
| mass_heat_coeff | double | Btu/degF/h | PUBLIC |  |  | +0 | interior mass heat exchange coefficient |
| mass_temperature | double | degF | PUBLIC | DYNAMIC |  | +0 | interior mass temperature |
| air_volume | double | cf | PUBLIC |  |  | +0 | air volume |
| air_mass | double | lb | PUBLIC |  |  | +0 | air mass |
| air_heat_capacity | double | Btu/degF | PUBLIC |  |  | +0 | air thermal mass |
| latent_load_fraction | double | pu | PUBLIC |  |  | +0 | fractional increase in cooling load due to latent heat |
| total_thermal_mass_per_floor_area | double | Btu/degF/sf | PUBLIC |  |  | +0 |  |
| interior_surface_heat_transfer_coeff | double | Btu/h/degF/sf | PUBLIC |  |  | +0 |  |
| number_of_stories | double |  | PUBLIC |  |  | +0 | number of stories within the structure |
| is_AUX_on | double |  | PUBLIC | OUTPUT |  | +0 | logic statement to determine population statistics - is the AUX on? 0 no, 1 yes |
| is_HEAT_on | double |  | PUBLIC | OUTPUT |  | +0 | logic statement to determine population statistics - is the HEAT on? 0 no, 1 yes |
| is_COOL_on | double |  | PUBLIC | OUTPUT |  | +0 | logic statement to determine population statistics - is the COOL on? 0 no, 1 yes |
| thermal_storage_present | double |  | PUBLIC |  |  | +0 | logic statement for determining if energy storage is present |
| thermal_storage_in_use | double |  | PUBLIC |  |  | +0 | logic statement for determining if energy storage is being utilized |
| thermostat_mode | enumeration |  | PUBLIC |  | COOL HEAT AUTO OFF | AUTO | tells the thermostat whether it is even allowed to heat or cool the house. |
| system_type | set |  | PUBLIC |  | RESISTIVE TWOSTAGE FORCEDAIR AIRCONDITIONING GAS NONE | NONE | heating/cooling system type/options |
| auxiliary_strategy | set |  | PUBLIC |  | LOCKOUT TIMER DEADBAND NONE | NONE | auxiliary heating activation strategies |
| system_mode | enumeration |  | PUBLIC |  | AUX COOL OFF HEAT UNKNOWN | UNKNOWN | heating/cooling system operation state |
| last_system_mode | enumeration |  | PUBLIC |  | AUX COOL OFF HEAT UNKNOWN | UNKNOWN | heating/cooling system operation state |
| heating_system_type | enumeration |  | PUBLIC |  | RESISTANCE HEAT_PUMP GAS NONE UNKNOWN | HEAT_PUMP | heating system type |
| cooling_system_type | enumeration |  | PUBLIC |  | HEAT_PUMP ELECTRIC NONE UNKNOWN | UNKNOWN | cooling system type |
| auxiliary_system_type | enumeration |  | PUBLIC |  | ELECTRIC NONE UNKNOWN | UNKNOWN | auxiliary heating type |
| fan_type | enumeration |  | PUBLIC |  | TWO_SPEED ONE_SPEED NONE UNKNOWN | UNKNOWN | fan type |
| thermal_integrity_level | enumeration |  | PUBLIC |  | UNKNOWN VERY_GOOD GOOD ABOVE_NORMAL NORMAL BELOW_NORMAL LITTLE VERY_LITTLE | UNKNOWN | default envelope UA settings |
| glass_type | enumeration |  | PUBLIC |  | LOW_E_GLASS GLASS OTHER | LOW_E_GLASS | glass used in the windows |
| window_frame | enumeration |  | PUBLIC |  | INSULATED WOOD THERMAL_BREAK ALUMINIUM ALUMINUM NONE | THERMAL_BREAK | type of window frame |
| glazing_treatment | enumeration |  | PUBLIC |  | HIGH_S LOW_S REFL ABS CLEAR OTHER | CLEAR | the treatment to increase the reflectivity of the exterior windows |
| glazing_layers | enumeration |  | PUBLIC |  | OTHER THREE TWO ONE | TWO | number of layers of glass in each window |
| motor_model | enumeration |  | PUBLIC |  | FULL BASIC NONE | NONE | indicates the level of detail used in modelling the hvac motor parameters |
| motor_efficiency | enumeration |  | PUBLIC |  | VERY_GOOD GOOD AVERAGE POOR VERY_POOR | AVERAGE | when using motor model, describes the efficiency of the motor |
| last_mode_timer | int64 |  | PUBLIC | OUTPUT |  |  |  |
| hvac_motor_efficiency | double | unit | PUBLIC |  |  | +1 | when using motor model, percent efficiency of hvac motor |
| hvac_motor_loss_power_factor | double | unit | PUBLIC |  |  | +0.125 | when using motor model, power factor of motor losses |
| Rroof | double | degF*sf*h/Btu | PUBLIC |  |  | +0 | roof R-value |
| Rwall | double | degF*sf*h/Btu | PUBLIC |  |  | +0 | wall R-value |
| Rfloor | double | degF*sf*h/Btu | PUBLIC |  |  | +0 | floor R-value |
| Rwindows | double | degF*sf*h/Btu | PUBLIC |  |  | +0 | window R-value |
| Rdoors | double | degF*sf*h/Btu | PUBLIC |  |  | +0 | door R-value |
| hvac_breaker_rating | double | A | PUBLIC |  |  | +0 | determines the amount of current the HVAC circuit breaker can handle |
| hvac_power_factor | double | unit | PUBLIC |  |  | +0 | power factor of hvac |
| hvac_load | double | kW | PUBLIC | OUTPUT |  |  | heating/cooling system load |
| last_heating_load | double | kW | PUBLIC | OUTPUT |  |  | stores the previous heating/cooling system load |
| last_cooling_load | double | kW | PUBLIC | OUTPUT |  |  | stores the previous heating/cooling system load |
| hvac_power | complex | kVA | PUBLIC | OUTPUT |  |  | describes hvac load complex power consumption |
| total_load | double | kW | PUBLIC | OUTPUT |  |  | total panel enduse load |
| panel | enduse |  | PUBLIC |  |  |  | the enduse load description |
| panel.energy | complex | kVAh | PUBLIC |  |  |  | the total energy consumed since the last meter reading |
| panel.power | complex | kVA | PUBLIC |  |  |  | the total power consumption of the load |
| panel.peak_demand | complex | kVA | PUBLIC |  |  |  | the peak power consumption since the last meter reading |
| panel.heatgain | double | Btu/h | PUBLIC |  |  |  | the heat transferred from the enduse to the parent |
| panel.cumulative_heatgain | double | Btu | PUBLIC |  |  |  | the cumulative heatgain from the enduse to the parent |
| panel.heatgain_fraction | double | pu | PUBLIC |  |  |  | the fraction of the heat that goes to the parent |
| panel.current_fraction | double | pu | PUBLIC |  |  |  | the fraction of total power that is constant current |
| panel.impedance_fraction | double | pu | PUBLIC |  |  |  | the fraction of total power that is constant impedance |
| panel.power_fraction | double | pu | PUBLIC |  |  |  | the fraction of the total power that is constant power |
| panel.power_factor | double |  | PUBLIC |  |  |  | the power factor of the load |
| panel.constant_power | complex | kVA | PUBLIC |  |  |  | the constant power portion of the total load |
| panel.constant_current | complex | kVA | PUBLIC |  |  |  | the constant current portion of the total load |
| panel.constant_admittance | complex | kVA | PUBLIC |  |  |  | the constant admittance portion of the total load |
| panel.voltage_factor | double | pu | PUBLIC |  |  |  | the voltage change factor |
| panel.breaker_amps | double | A | PUBLIC |  |  |  | the rated breaker amperage |
| panel.configuration | set |  | PUBLIC |  | IS220 IS110 |  | the load configuration options |
| design_internal_gain_density | double | W/sf | PUBLIC |  |  | +0.6 | average density of heat generating devices in the house |
| compressor_on | bool |  | PUBLIC | OUTPUT |  |  |  |
| compressor_count | int64 |  | PUBLIC | OUTPUT |  |  |  |
| hvac_last_on | timestamp |  | PUBLIC | OUTPUT |  |  |  |
| hvac_last_off | timestamp |  | PUBLIC | OUTPUT |  |  |  |
| hvac_period_length | double | s | PUBLIC | OUTPUT |  |  |  |
| hvac_duty_cycle | double |  | PUBLIC | OUTPUT |  |  |  |
| a | double |  | HIDDEN | OUTPUT |  |  |  |
| b | double |  | HIDDEN | OUTPUT |  |  |  |
| c | double |  | HIDDEN | OUTPUT |  |  |  |
| d | double |  | HIDDEN | OUTPUT |  |  |  |
| c1 | double |  | HIDDEN | OUTPUT |  |  |  |
| c2 | double |  | HIDDEN | OUTPUT |  |  |  |
| A3 | double |  | HIDDEN | OUTPUT |  |  |  |
| A4 | double |  | HIDDEN | OUTPUT |  |  |  |
| k1 | double |  | HIDDEN | OUTPUT |  |  |  |
| k2 | double |  | HIDDEN | OUTPUT |  |  |  |
| r1 | double |  | HIDDEN | OUTPUT |  |  |  |
| r2 | double |  | HIDDEN | OUTPUT |  |  |  |
| Teq | double |  | HIDDEN | OUTPUT |  |  |  |
| Tevent | double |  | HIDDEN | OUTPUT |  |  |  |
| Qi | double |  | HIDDEN | OUTPUT |  |  |  |
| Qa | double |  | HIDDEN | OUTPUT |  |  |  |
| Qm | double |  | HIDDEN | OUTPUT |  |  |  |
| Qh | double |  | HIDDEN | OUTPUT |  |  |  |
| Qlatent | double |  | HIDDEN | OUTPUT |  |  |  |
| dTair | double |  | HIDDEN | OUTPUT |  |  |  |
| adj_cooling_cap | double |  | HIDDEN | OUTPUT |  |  |  |
| adj_heating_cap | double |  | HIDDEN | OUTPUT |  |  |  |
| adj_cooling_cop | double |  | HIDDEN | OUTPUT |  |  |  |
| adj_heating_cop | double |  | HIDDEN | OUTPUT |  |  |  |
| thermostat_control | enumeration |  | PUBLIC |  | NONE BAND FULL | FULL | determine level of internal thermostatic control |
| gas_enduses | char1024 |  | PUBLIC |  |  |  | list of implicit enduses that use gas instead of electricity |
| circuit | method |  | PROTECTED |  |  |  | smart breaker message handlers |
| load_power_fraction | double |  | PUBLIC |  |  | +0.8 | fraction of total load that is constant power |
| load_impedance_fraction | double |  | PUBLIC |  |  | +0.2 | fraction of total load that is constant impedance |
| load_current_fraction | double |  | PUBLIC |  |  | +0 | fraction of total load that is constant current |

### waterheater

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| tank_volume | double | gal | PUBLIC |  |  |  | the volume of water in the tank when it is full |
| tank_UA | double | Btu*h/degF | PUBLIC |  |  |  | the UA of the tank (surface area divided by R-value) |
| tank_diameter | double | ft | PUBLIC |  |  |  | the diameter of the water heater tank |
| tank_height | double | ft | PUBLIC |  |  |  | the height of the water heater tank |
| water_demand | double | gpm | PUBLIC |  |  |  | the hot water draw from the water heater |
| heating_element_capacity | double | kW | PUBLIC |  |  |  | the power of the heating element |
| inlet_water_temperature | double | degF | PUBLIC |  |  |  | the inlet temperature of the water tank |
| waterheater_model | enumeration |  | PUBLIC |  | NONE FORTRAN TWONODE ONENODE |  | the water heater model to use |
| heat_mode | enumeration |  | PUBLIC |  | HEAT_PUMP GASHEAT ELECTRIC |  | the energy source for heating the water heater |
| location | enumeration |  | PUBLIC |  | GARAGE INSIDE |  | whether the water heater is inside or outside |
| tank_setpoint | double | degF | PUBLIC |  |  |  | the temperature around which the water heater will heat its contents |
| thermostat_deadband | double | degF | PUBLIC |  |  |  | the degree to heat the water tank, when needed |
| temperature | double | degF | PUBLIC |  |  |  | the outlet temperature of the water tank |
| height | double | ft | PUBLIC |  |  |  | the height of the hot water column within the water tank |
| demand | complex | kVA | PUBLIC |  |  |  | the water heater power consumption |
| actual_load | double | kW | PUBLIC |  |  |  | the actual load based on the current voltage across the coils |
| previous_load | double | kW | PUBLIC |  |  |  | the previous load based on voltage across the coils at the last sync operation |
| actual_power | complex | kVA | PUBLIC |  |  |  | the actual power based on the current voltage across the coils |
| is_waterheater_on | double |  | PUBLIC |  |  |  | simple logic output to determine state of waterheater (1-on, 0-off) |
| gas_fan_power | double | kW | PUBLIC |  |  |  | load of a running gas waterheater |
| gas_standby_power | double | kW | PUBLIC |  |  |  | load of a gas waterheater in standby |
| heat_pump_coefficient_of_performance | double | Btu/kWh | PUBLIC |  |  |  | current COP of the water heater pump - currently calculated internally and not an input |
| Tcontrol | double | degF | PUBLIC |  |  |  | in heat pump operation, defines the blended temperature used for turning on and off HP - currently calculated internally and not an input |
| current_tank_status | enumeration |  | PUBLIC |  | EMPTY PARTIAL FULL |  |  |
| dr_signal | double |  | PUBLIC |  |  |  | the on/off signal to send to the fortran waterheater model |
| COP | double |  | PUBLIC |  |  |  | the cop of the fortran heat pump water heater model. |
| operating_mode | double |  | PUBLIC |  |  |  | the operating mode the fortran water heater should be using. |
| fortran_sim_time | double | s | PUBLIC |  |  |  | the amount of time the fortran model should simulate. |
| waterheater_power | double | kW | PUBLIC |  |  |  | the current power draw from the fortran water heater. |
| load_state | enumeration |  | PUBLIC |  | STABLE RECOVERING DEPLETING |  |  |
| actual_voltage | double |  | HIDDEN |  |  |  |  |
| nominal_voltage | double |  | HIDDEN |  |  |  |  |
| re_override | enumeration |  | PUBLIC |  | OV_OFF OV_NORMAL OV_ON |  | the override setting for the water heater |

### lights

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| type | enumeration |  | PUBLIC |  | HID SSL CFL FLUORESCENT INCANDESCENT |  | lighting type (affects power_factor) |
| placement | enumeration |  | PUBLIC |  | OUTDOOR INDOOR |  | lighting location (affects where heatgains go) |
| installed_power | double | kW | PUBLIC |  |  |  | installed lighting capacity |
| power_density | double | W/sf | PUBLIC |  |  |  | installed power density |
| curtailment | double | pu | PUBLIC |  |  |  | lighting curtailment factor |
| demand | double | pu | PUBLIC |  |  |  | the current lighting demand |
| actual_power | complex | kVA | PUBLIC |  |  |  | actual power demand of lights object |

### refrigerator

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| size | double | cf | PUBLIC |  |  |  | volume of the refrigerator |
| rated_capacity | double | Btu/h | PUBLIC |  |  |  |  |
| temperature | double | degF | PUBLIC |  |  |  |  |
| setpoint | double | degF | PUBLIC |  |  |  |  |
| deadband | double | degF | PUBLIC |  |  |  |  |
| cycle_time | double | s | PUBLIC |  |  |  |  |
| output | double |  | PUBLIC |  |  |  |  |
| event_temp | double |  | PUBLIC |  |  |  |  |
| UA | double | Btu/degF/h | PUBLIC |  |  |  |  |
| compressor_off_normal_energy | double |  | PUBLIC |  |  |  |  |
| compressor_off_normal_power | double | W | PUBLIC |  |  |  |  |
| compressor_on_normal_energy | double |  | PUBLIC |  |  |  |  |
| compressor_on_normal_power | double | W | PUBLIC |  |  |  |  |
| defrost_energy | double |  | PUBLIC |  |  |  |  |
| defrost_power | double | W | PUBLIC |  |  |  |  |
| icemaking_energy | double |  | PUBLIC |  |  |  |  |
| icemaking_power | double | W | PUBLIC |  |  |  |  |
| ice_making_probability | double |  | PUBLIC |  |  |  |  |
| FF_Door_Openings | int32 |  | PUBLIC |  |  |  |  |
| door_opening_energy | int32 |  | PUBLIC |  |  |  |  |
| door_opening_power | int32 |  | PUBLIC |  |  |  |  |
| DO_Thershold | double |  | PUBLIC |  |  |  |  |
| dr_mode_double | double |  | PUBLIC |  |  |  |  |
| energy_needed | double |  | PUBLIC |  |  |  |  |
| energy_used | double |  | PUBLIC |  |  |  |  |
| refrigerator_power | double |  | PUBLIC |  |  |  |  |
| icemaker_running | bool |  | PUBLIC |  |  |  |  |
| check_DO | int32 |  | PUBLIC |  |  |  |  |
| is_240 | bool |  | PUBLIC |  |  |  |  |
| defrostDelayed | double |  | PUBLIC |  |  |  |  |
| long_compressor_cycle_due | bool |  | PUBLIC |  |  |  |  |
| long_compressor_cycle_time | double |  | PUBLIC |  |  |  |  |
| long_compressor_cycle_power | double |  | PUBLIC |  |  |  |  |
| long_compressor_cycle_energy | double |  | PUBLIC |  |  |  |  |
| long_compressor_cycle_threshold | double |  | PUBLIC |  |  |  |  |
| defrost_criterion | enumeration |  | PUBLIC |  | COMPRESSOR_TIME DOOR_OPENINGS TIMED |  |  |
| run_defrost | bool |  | PUBLIC |  |  |  |  |
| door_opening_criterion | double |  | PUBLIC |  |  |  |  |
| compressor_defrost_time | double |  | PUBLIC |  |  |  |  |
| delay_defrost_time | double |  | PUBLIC |  |  |  |  |
| daily_door_opening | int32 |  | PUBLIC |  |  |  |  |
| state | enumeration |  | PUBLIC |  | COMPRESSSOR_ON_NORMAL COMPRESSSOR_ON_LONG COMPRESSSOR_OFF_NORMAL DEFROST |  |  |

### clotheswasher

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| motor_power | double | kW | PUBLIC |  |  |  |  |
| circuit_split | double |  | PUBLIC |  |  |  |  |
| queue | double | unit | PUBLIC |  |  |  | the total laundry accumulated |
| demand | double | unit/day | PUBLIC |  |  |  | the amount of laundry accumulating daily |
| energy_meter | complex | kWh | PUBLIC |  |  |  |  |
| stall_voltage | double | V | PUBLIC |  |  |  |  |
| start_voltage | double | V | PUBLIC |  |  |  |  |
| clothesWasherPower | double |  | PUBLIC |  |  |  |  |
| stall_impedance | complex | Ohm | PUBLIC |  |  |  |  |
| trip_delay | double | s | PUBLIC |  |  |  |  |
| reset_delay | double | s | PUBLIC |  |  |  |  |
| Is_on | double |  | PUBLIC |  |  |  |  |
| normal_perc | double |  | PUBLIC |  |  |  |  |
| perm_press_perc | double |  | PUBLIC |  |  |  |  |
| NORMAL_PREWASH_POWER | double |  | PUBLIC |  |  |  |  |
| NORMAL_WASH_POWER | double |  | PUBLIC |  |  |  |  |
| NORMAL_SPIN_LOW_POWER | double |  | PUBLIC |  |  |  |  |
| NORMAL_SPIN_MEDIUM_POWER | double |  | PUBLIC |  |  |  |  |
| NORMAL_SPIN_HIGH_POWER | double |  | PUBLIC |  |  |  |  |
| NORMAL_SMALLWASH_POWER | double |  | PUBLIC |  |  |  |  |
| NORMAL_PREWASH_ENERGY | double |  | PUBLIC |  |  |  |  |
| NORMAL_WASH_ENERGY | double |  | PUBLIC |  |  |  |  |
| NORMAL_SPIN_LOW_ENERGY | double |  | PUBLIC |  |  |  |  |
| NORMAL_SPIN_MEDIUM_ENERGY | double |  | PUBLIC |  |  |  |  |
| NORMAL_SPIN_HIGH_ENERGY | double |  | PUBLIC |  |  |  |  |
| NORMAL_SMALLWASH_ENERGY | double |  | PUBLIC |  |  |  |  |
| PERMPRESS_PREWASH_POWER | double |  | PUBLIC |  |  |  |  |
| PERMPRESS_WASH_POWER | double |  | PUBLIC |  |  |  |  |
| PERMPRESS_SPIN_LOW_POWER | double |  | PUBLIC |  |  |  |  |
| PERMPRESS_SPIN_MEDIUM_POWER | double |  | PUBLIC |  |  |  |  |
| PERMPRESS_SPIN_HIGH_POWER | double |  | PUBLIC |  |  |  |  |
| PERMPRESS_SMALLWASH_POWER | double |  | PUBLIC |  |  |  |  |
| PERMPRESS_PREWASH_ENERGY | double |  | PUBLIC |  |  |  |  |
| PERMPRESS_WASH_ENERGY | double |  | PUBLIC |  |  |  |  |
| PERMPRESS_SPIN_LOW_ENERGY | double |  | PUBLIC |  |  |  |  |
| PERMPRESS_SPIN_MEDIUM_ENERGY | double |  | PUBLIC |  |  |  |  |
| PERMPRESS_SPIN_HIGH_ENERGY | double |  | PUBLIC |  |  |  |  |
| PERMPRESS_SMALLWASH_ENERGY | double |  | PUBLIC |  |  |  |  |
| GENTLE_PREWASH_POWER | double |  | PUBLIC |  |  |  |  |
| GENTLE_WASH_POWER | double |  | PUBLIC |  |  |  |  |
| GENTLE_SPIN_LOW_POWER | double |  | PUBLIC |  |  |  |  |
| GENTLE_SPIN_MEDIUM_POWER | double |  | PUBLIC |  |  |  |  |
| GENTLE_SPIN_HIGH_POWER | double |  | PUBLIC |  |  |  |  |
| GENTLE_SMALLWASH_POWER | double |  | PUBLIC |  |  |  |  |
| GENTLE_PREWASH_ENERGY | double |  | PUBLIC |  |  |  |  |
| GENTLE_WASH_ENERGY | double |  | PUBLIC |  |  |  |  |
| GENTLE_SPIN_LOW_ENERGY | double |  | PUBLIC |  |  |  |  |
| GENTLE_SPIN_MEDIUM_ENERGY | double |  | PUBLIC |  |  |  |  |
| GENTLE_SPIN_HIGH_ENERGY | double |  | PUBLIC |  |  |  |  |
| GENTLE_SMALLWASH_ENERGY | double |  | PUBLIC |  |  |  |  |
| queue_min | double | unit | PUBLIC |  |  |  |  |
| queue_max | double | unit | PUBLIC |  |  |  |  |
| clotheswasher_run_prob | double |  | PUBLIC |  |  |  |  |
| state | enumeration |  | PUBLIC |  | SPIN4 SPIN3 SPIN2 SPIN1 WASH PREWASH STOPPED |  |  |
| spin_mode | enumeration |  | PUBLIC |  | SMALLWASH SPIN_WASH SPIN_HIGH SPIN_MEDIUM SPIN_LOW |  |  |
| wash_mode | enumeration |  | PUBLIC |  | GENTLE PERM_PRESS NORMAL |  |  |

### dishwasher

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| control_power | double | W | PUBLIC |  |  |  |  |
| dishwasher_coil_power_1 | double | W | PUBLIC |  |  |  |  |
| dishwasher_coil_power_2 | double | W | PUBLIC |  |  |  |  |
| dishwasher_coil_power_3 | double | W | PUBLIC |  |  |  |  |
| motor_power | double | W | PUBLIC |  |  |  |  |
| circuit_split | double |  | PUBLIC |  |  |  |  |
| queue | double | unit | PUBLIC |  |  |  | number of loads accumulated |
| stall_voltage | double | V | PUBLIC |  |  |  |  |
| start_voltage | double | V | PUBLIC |  |  |  |  |
| stall_impedance | complex | Ohm | PUBLIC |  |  |  |  |
| trip_delay | double | s | PUBLIC |  |  |  |  |
| reset_delay | double | s | PUBLIC |  |  |  |  |
| total_power | double | W | PUBLIC |  |  |  |  |
| state | enumeration |  | PUBLIC |  | HEATEDDRY_ONLY CONTROL_ONLY COIL_ONLY MOTOR_COIL_ONLY MOTOR_ONLY TRIPPED STALLED STOPPED |  |  |
| energy_baseline | double | kWh | PUBLIC |  |  |  |  |
| energy_used | double | kWh | PUBLIC |  |  |  |  |
| control_check1 | bool |  | PUBLIC |  |  |  |  |
| control_check2 | bool |  | PUBLIC |  |  |  |  |
| control_check3 | bool |  | PUBLIC |  |  |  |  |
| control_check4 | bool |  | PUBLIC |  |  |  |  |
| control_check5 | bool |  | PUBLIC |  |  |  |  |
| control_check6 | bool |  | PUBLIC |  |  |  |  |
| control_check7 | bool |  | PUBLIC |  |  |  |  |
| control_check8 | bool |  | PUBLIC |  |  |  |  |
| control_check9 | bool |  | PUBLIC |  |  |  |  |
| control_check10 | bool |  | PUBLIC |  |  |  |  |
| control_check11 | bool |  | PUBLIC |  |  |  |  |
| control_check12 | bool |  | PUBLIC |  |  |  |  |
| control_check_temp | bool |  | PUBLIC |  |  |  |  |
| motor_only_check1 | bool |  | PUBLIC |  |  |  |  |
| motor_only_check2 | bool |  | PUBLIC |  |  |  |  |
| motor_only_check3 | bool |  | PUBLIC |  |  |  |  |
| motor_only_check4 | bool |  | PUBLIC |  |  |  |  |
| motor_only_check5 | bool |  | PUBLIC |  |  |  |  |
| motor_only_check6 | bool |  | PUBLIC |  |  |  |  |
| motor_only_check7 | bool |  | PUBLIC |  |  |  |  |
| motor_only_check8 | bool |  | PUBLIC |  |  |  |  |
| motor_only_check9 | bool |  | PUBLIC |  |  |  |  |
| motor_only_temp1 | bool |  | PUBLIC |  |  |  |  |
| motor_only_temp2 | bool |  | PUBLIC |  |  |  |  |
| motor_coil_only_check1 | bool |  | PUBLIC |  |  |  |  |
| motor_coil_only_check2 | bool |  | PUBLIC |  |  |  |  |
| heateddry_check1 | bool |  | PUBLIC |  |  |  |  |
| heateddry_check2 | bool |  | PUBLIC |  |  |  |  |
| coil_only_check1 | bool |  | PUBLIC |  |  |  |  |
| coil_only_check2 | bool |  | PUBLIC |  |  |  |  |
| coil_only_check3 | bool |  | PUBLIC |  |  |  |  |
| Heateddry_option_check | bool |  | PUBLIC |  |  |  |  |
| queue_min | double | unit | PUBLIC |  |  |  |  |
| queue_max | double | unit | PUBLIC |  |  |  |  |
| pulse_interval_1 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_2 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_3 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_4 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_5 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_6 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_7 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_8 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_9 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_10 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_11 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_12 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_13 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_14 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_15 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_16 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_17 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_18 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_19 | double | s | PUBLIC |  |  |  |  |
| dishwasher_run_prob | double |  | PUBLIC |  |  |  |  |
| energy_needed | double | kWh | PUBLIC |  |  |  |  |
| dishwasher_demand | double | kWh | PUBLIC |  |  |  |  |
| daily_dishwasher_demand | double | kWh | PUBLIC |  |  |  |  |
| actual_dishwasher_demand | double | kWh | PUBLIC |  |  |  |  |
| motor_on_off | double |  | PUBLIC |  |  |  |  |
| motor_coil_on_off | double |  | PUBLIC |  |  |  |  |
| is_240 | bool |  | PUBLIC |  |  |  | load is 220/240 V (across both phases) |

### occupantload

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| number_of_occupants | int32 |  | PUBLIC |  |  |  |  |
| occupancy_fraction | double | unit | PUBLIC |  |  |  |  |
| heatgain_per_person | double | Btu/h | PUBLIC |  |  |  |  |

### plugload

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| circuit_split | double |  | PUBLIC |  |  |  |  |
| demand | double | unit | PUBLIC |  |  |  |  |
| installed_power | double | kW | PUBLIC |  |  |  | installed plugs capacity |
| actual_power | complex | kVA | PUBLIC |  |  |  | actual power demand |

### microwave

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| installed_power | double | kW | PUBLIC |  |  |  | rated microwave power level |
| standby_power | double | kW | PUBLIC |  |  |  | standby microwave power draw (unshaped only) |
| circuit_split | double |  | PUBLIC |  |  |  |  |
| state | enumeration |  | PUBLIC |  | ON OFF |  | on/off state of the microwave |
| cycle_length | double | s | PUBLIC |  |  |  | length of the combined on/off cycle between uses |
| runtime | double | s | PUBLIC |  |  |  |  |
| state_time | double | s | PUBLIC |  |  |  |  |

### range

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| oven_volume | double | gal | PUBLIC |  |  |  | the volume of the oven |
| oven_UA | double | Btu/degF/h | PUBLIC |  |  |  | the UA of the oven (surface area divided by R-value) |
| oven_diameter | double | ft | PUBLIC |  |  |  | the diameter of the oven |
| oven_demand | double | gpm | PUBLIC |  |  |  | the hot food take out from the oven |
| heating_element_capacity | double | kW | PUBLIC |  |  |  | the power of the heating element |
| inlet_food_temperature | double | degF | PUBLIC |  |  |  | the inlet temperature of the food |
| heat_mode | enumeration |  | PUBLIC |  | GASHEAT ELECTRIC |  | the energy source for heating the oven |
| location | enumeration |  | PUBLIC |  | GARAGE INSIDE |  | whether the range is inside or outside |
| oven_setpoint | double | degF | PUBLIC |  |  |  | the temperature around which the oven will heat its contents |
| thermostat_deadband | double | degF | PUBLIC |  |  |  | the degree to heat the food in the oven, when needed |
| temperature | double | degF | PUBLIC |  |  |  | the outlet temperature of the oven |
| height | double | ft | PUBLIC |  |  |  | the height of the oven |
| food_density | double |  | PUBLIC |  |  |  | food density |
| specificheat_food | double |  | PUBLIC |  |  |  |  |
| queue_cooktop | double | unit | PUBLIC |  |  |  | number of loads accumulated |
| queue_oven | double | unit | PUBLIC |  |  |  | number of loads accumulated |
| queue_min | double | unit | PUBLIC |  |  |  |  |
| queue_max | double | unit | PUBLIC |  |  |  |  |
| time_cooktop_operation | double |  | PUBLIC |  |  |  |  |
| time_cooktop_setting | double |  | PUBLIC |  |  |  |  |
| cooktop_run_prob | double |  | PUBLIC |  |  |  |  |
| oven_run_prob | double |  | PUBLIC |  |  |  |  |
| cooktop_coil_setting_1 | double | kW | PUBLIC |  |  |  |  |
| cooktop_coil_setting_2 | double | kW | PUBLIC |  |  |  |  |
| cooktop_coil_setting_3 | double | kW | PUBLIC |  |  |  |  |
| total_power_oven | double | kW | PUBLIC |  |  |  |  |
| total_power_cooktop | double | kW | PUBLIC |  |  |  |  |
| total_power_range | double | kW | PUBLIC |  |  |  |  |
| demand_cooktop | double | unit/day | PUBLIC |  |  |  | number of loads accumulating daily |
| demand_oven | double | unit/day | PUBLIC |  |  |  | number of loads accumulating daily |
| stall_voltage | double | V | PUBLIC |  |  |  |  |
| start_voltage | double | V | PUBLIC |  |  |  |  |
| stall_impedance | complex | Ohm | PUBLIC |  |  |  |  |
| trip_delay | double | s | PUBLIC |  |  |  |  |
| reset_delay | double | s | PUBLIC |  |  |  |  |
| time_oven_operation | double | s | PUBLIC |  |  |  |  |
| time_oven_setting | double | s | PUBLIC |  |  |  |  |
| state_cooktop | enumeration |  | PUBLIC |  | CT_TRIPPED CT_STALLED STAGE_8_ONLY STAGE_7_ONLY STAGE_6_ONLY CT_STOPPED |  |  |
| cooktop_energy_baseline | double | kWh | PUBLIC |  |  |  |  |
| cooktop_energy_used | double |  | PUBLIC |  |  |  |  |
| Toff | double |  | PUBLIC |  |  |  |  |
| Ton | double |  | PUBLIC |  |  |  |  |
| cooktop_interval_setting_1 | double | s | PUBLIC |  |  |  |  |
| cooktop_interval_setting_2 | double | s | PUBLIC |  |  |  |  |
| cooktop_interval_setting_3 | double | s | PUBLIC |  |  |  |  |
| cooktop_energy_needed | double | kWh | PUBLIC |  |  |  |  |
| heat_needed | bool |  | PUBLIC |  |  |  |  |
| oven_check | bool |  | PUBLIC |  |  |  |  |
| remainon | bool |  | PUBLIC |  |  |  |  |
| cooktop_check | bool |  | PUBLIC |  |  |  |  |
| actual_load | double | kW | PUBLIC |  |  |  | the actual load based on the current voltage across the coils |
| previous_load | double | kW | PUBLIC |  |  |  | the actual load based on current voltage stored for use in controllers |
| actual_power | complex | kVA | PUBLIC |  |  |  | the actual power based on the current voltage across the coils |
| is_range_on | double |  | PUBLIC |  |  |  | simple logic output to determine state of range (1-on, 0-off) |

### freezer

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| size | double | cf | PUBLIC |  |  |  |  |
| rated_capacity | double | Btu/h | PUBLIC |  |  |  |  |
| temperature | double | degF | PUBLIC |  |  |  |  |
| setpoint | double | degF | PUBLIC |  |  |  |  |
| deadband | double | degF | PUBLIC |  |  |  |  |
| next_time | timestamp |  | PUBLIC |  |  |  |  |
| output | double |  | PUBLIC |  |  |  |  |
| event_temp | double |  | PUBLIC |  |  |  |  |
| UA | double | Btu/degF/h | PUBLIC |  |  |  |  |
| state | enumeration |  | PUBLIC |  | ON OFF |  |  |

### dryer

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| motor_power | double | W | PUBLIC |  |  |  |  |
| dryer_coil_power | double | W | PUBLIC |  |  |  |  |
| controls_power | double | W | PUBLIC |  |  |  |  |
| circuit_split | double |  | PUBLIC |  |  |  |  |
| queue | double | unit | PUBLIC |  |  |  | number of loads accumulated |
| queue_min | double | unit | PUBLIC |  |  |  |  |
| queue_max | double | unit | PUBLIC |  |  |  |  |
| stall_voltage | double | V | PUBLIC |  |  |  |  |
| start_voltage | double | V | PUBLIC |  |  |  |  |
| stall_impedance | complex | Ohm | PUBLIC |  |  |  |  |
| trip_delay | double | s | PUBLIC |  |  |  |  |
| reset_delay | double | s | PUBLIC |  |  |  |  |
| total_power | double | W | PUBLIC |  |  |  |  |
| state | enumeration |  | PUBLIC |  | CONTROL_ONLY MOTOR_COIL_ONLY MOTOR_ONLY TRIPPED STALLED STOPPED |  |  |
| energy_baseline | double | kWh | PUBLIC |  |  |  |  |
| energy_used | double | kWh | PUBLIC |  |  |  |  |
| next_t | double |  | PUBLIC |  |  |  |  |
| control_check | bool |  | PUBLIC |  |  |  |  |
| motor_only_check1 | bool |  | PUBLIC |  |  |  |  |
| motor_only_check2 | bool |  | PUBLIC |  |  |  |  |
| motor_only_check3 | bool |  | PUBLIC |  |  |  |  |
| motor_only_check4 | bool |  | PUBLIC |  |  |  |  |
| motor_only_check5 | bool |  | PUBLIC |  |  |  |  |
| motor_only_check6 | bool |  | PUBLIC |  |  |  |  |
| dryer_on | bool |  | PUBLIC |  |  |  |  |
| dryer_ready | bool |  | PUBLIC |  |  |  |  |
| dryer_check | bool |  | PUBLIC |  |  |  |  |
| motor_coil_only_check1 | bool |  | PUBLIC |  |  |  |  |
| motor_coil_only_check2 | bool |  | PUBLIC |  |  |  |  |
| motor_coil_only_check3 | bool |  | PUBLIC |  |  |  |  |
| motor_coil_only_check4 | bool |  | PUBLIC |  |  |  |  |
| motor_coil_only_check5 | bool |  | PUBLIC |  |  |  |  |
| motor_coil_only_check6 | bool |  | PUBLIC |  |  |  |  |
| dryer_run_prob | double |  | PUBLIC |  |  |  |  |
| dryer_turn_on | double |  | PUBLIC |  |  |  |  |
| pulse_interval_1 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_2 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_3 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_4 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_5 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_6 | double | s | PUBLIC |  |  |  |  |
| pulse_interval_7 | double | s | PUBLIC |  |  |  |  |
| energy_needed | double | kWh | PUBLIC |  |  |  |  |
| daily_dryer_demand | double | kWh | PUBLIC |  |  |  |  |
| actual_dryer_demand | double | kWh | PUBLIC |  |  |  |  |
| motor_on_off | double |  | PUBLIC |  |  |  |  |
| motor_coil_on_off | double |  | PUBLIC |  |  |  |  |
| is_240 | bool |  | PUBLIC |  |  |  | load is 220/240 V (across both phases) |

### evcharger

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| charger_type | enumeration |  | PUBLIC |  | HIGH MEDIUM LOW |  |  |
| vehicle_type | enumeration |  | PUBLIC |  | HYBRID ELECTRIC |  |  |
| state | enumeration |  | PUBLIC |  | WORK HOME UNKNOWN |  |  |
| p_go_home | double | unit/h | PUBLIC |  |  |  |  |
| p_go_work | double | unit/h | PUBLIC |  |  |  |  |
| work_dist | double | mile | PUBLIC |  |  |  |  |
| capacity | double | kWh | PUBLIC |  |  |  |  |
| charge | double | unit | PUBLIC |  |  |  |  |
| charge_at_work | bool |  | PUBLIC |  |  |  |  |
| charge_throttle | double | unit | PUBLIC |  |  |  |  |
| charger_efficiency | double | unit | PUBLIC |  |  |  | Efficiency of the charger in terms of energy in to battery stored |
| power_train_efficiency | double | mile/kWh | PUBLIC |  |  |  | Miles per kWh of battery charge |
| mileage_classification | double | mile | PUBLIC |  |  |  | Miles expected range on battery only |
| demand_profile | char1024 |  | PUBLIC |  |  |  |  |

### ZIPload

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| heat_fraction | double |  | PUBLIC |  |  |  | fraction of ZIPload that is transferred as heat |
| base_power | double | kW | PUBLIC |  |  |  | base real power of the overall load |
| power_pf | double |  | PUBLIC |  |  |  | power factor for constant power portion |
| current_pf | double |  | PUBLIC |  |  |  | power factor for constant current portion |
| impedance_pf | double |  | PUBLIC |  |  |  | power factor for constant impedance portion |
| is_240 | bool |  | PUBLIC |  |  |  | load is 220/240 V (across both phases) |
| breaker_val | double | A | PUBLIC |  |  |  | Amperage of connected breaker |
| actual_power | complex | kVA | PUBLIC |  |  |  | variable to pull actual load as function of voltage |
| multiplier | double |  | PUBLIC |  |  |  | this variable is used to modify the base power as a function of multiplier x base_power |
| heatgain_only | bool |  | PUBLIC |  |  |  | allows the zipload to generate heat only (no kW), not activated by default |
| demand_response_mode | bool |  | PUBLIC |  |  |  | Activates equilibrium dynamic representation of demand response |
| number_of_devices | int16 |  | PUBLIC |  |  |  | Number of devices to model - base power is the total load of all devices |
| thermostatic_control_range | int16 |  | PUBLIC |  |  |  | Range of the thermostat's control operation |
| number_of_devices_off | double |  | PUBLIC |  |  |  | Total number of devices that are off |
| number_of_devices_on | double |  | PUBLIC |  |  |  | Total number of devices that are on |
| rate_of_cooling | double | K/h | PUBLIC |  |  |  | rate at which devices cool down |
| rate_of_heating | double | K/h | PUBLIC |  |  |  | rate at which devices heat up |
| temperature | int16 |  | PUBLIC |  |  |  | temperature of the device's controlled media (eg air temp or water temp) |
| phi | double |  | PUBLIC |  |  |  | duty cycle of the device |
| demand_rate | double | 1/h | PUBLIC |  |  |  | consumer demand rate that prematurely turns on a device or population |
| nominal_power | double |  | PUBLIC |  |  |  | the rated amount of power demanded by devices that are on |
| duty_cycle | double | pu | PUBLIC |  |  |  | fraction of time in the on state |
| recovery_duty_cycle | double | pu | PUBLIC |  |  |  | fraction of time in the on state, while in recovery interval |
| period | double | h | PUBLIC |  |  |  | time interval to apply duty cycle |
| phase | double | pu | PUBLIC |  |  |  | initial phase of load; duty will be assumed to occur at beginning of period |

### thermal_storage

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| total_capacity | double | Btu | PUBLIC |  |  |  | total storage capacity of unit |
| stored_capacity | double | Btu | PUBLIC |  |  |  | amount of capacity that is stored |
| recharge_power | double | kW | PUBLIC |  |  |  | installed compressor power usage |
| discharge_power | double | kW | PUBLIC |  |  |  | installed pump power usage |
| recharge_pf | double |  | PUBLIC |  |  |  | installed compressor power factor |
| discharge_pf | double |  | PUBLIC |  |  |  | installed pump power factor |
| discharge_schedule_type | enumeration |  | PUBLIC |  | EXTERNAL INTERNAL |  | Scheduling method for discharging |
| recharge_schedule_type | enumeration |  | PUBLIC |  | EXTERNAL INTERNAL |  | Scheduling method for charging |
| recharge_time | double |  | PUBLIC |  |  |  | Flag indicating if recharging is available at the current time (1 or 0) |
| discharge_time | double |  | PUBLIC |  |  |  | Flag indicating if discharging is available at the current time (1 or 0) |
| discharge_rate | double | Btu/h | PUBLIC |  |  |  | rating of discharge or cooling |
| SOC | double | % | PUBLIC |  |  |  | state of charge as percentage of total capacity |
| k | double | W/m/K | PUBLIC |  |  |  | coefficient of thermal conductivity (W/m/K) |

### evcharger_det

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| charge_rate | double | W | PUBLIC |  |  |  | Current demanded charge rate of the vehicle |
| variation_mean | double | s | PUBLIC |  |  |  | Mean of normal variation of schedule variation |
| variation_std_dev | double | s | PUBLIC |  |  |  | Standard deviation of normal variation of schedule times |
| variation_trip_mean | double | mile | PUBLIC |  |  |  | Mean of normal variation of trip distance variation |
| variation_trip_std_dev | double | mile | PUBLIC |  |  |  | Standard deviation of normal variation of trip distance |
| mileage_classification | double | mile | PUBLIC |  |  |  | Mileage classification of electric vehicle |
| work_charging_available | bool |  | PUBLIC |  |  |  | Charging available when at work |
| data_file | char1024 |  | PUBLIC |  |  |  | Path to .CSV file with vehicle travel information |
| vehicle_index | int32 |  | PUBLIC |  |  |  | Index of vehicles in file this particular vehicle's data |
| vehicle_location | enumeration |  | PUBLIC |  | DRIVING_WORK DRIVING_HOME WORK HOME UNKNOWN |  |  |
| travel_distance | double | mile | PUBLIC |  |  |  | Distance vehicle travels from home to home |
| arrival_at_work | double |  | PUBLIC |  |  |  | Time vehicle arrives at work - HHMM |
| duration_at_work | double | s | PUBLIC |  |  |  | Duration the vehicle remains at work |
| arrival_at_home | double |  | PUBLIC |  |  |  | Time vehicle arrives at home - HHMM |
| duration_at_home | double | s | PUBLIC |  |  |  | Duration the vehicle remains at home |
| battery_capacity | double | kWh | PUBLIC |  |  |  | Current capacity of the battery in kWh |
| battery_SOC | double | % | PUBLIC |  |  |  | State of charge of battery |
| battery_size | double | kWh | PUBLIC |  |  |  | Full capacity of battery |
| mileage_efficiency | double | mile/kWh | PUBLIC |  |  |  | Efficiency of drive train in mile/kWh |
| maximum_charge_rate | double | W | PUBLIC |  |  |  | Maximum output rate of charger in kW |
| charging_efficiency | double | unit | PUBLIC |  |  |  | Efficiency of charger (ratio) when charging |

### rbsa

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| floor_area | double | sf | PUBLIC |  |  |  | building floor area |
| filename | method |  | PUBLIC |  |  |  | RBSA data file |
| composition | method |  | PUBLIC |  |  |  | load composition specification |
| total_real_power | double | W | PUBLIC |  |  |  | total real power |
| total_reactive_power | double | W | PUBLIC |  |  |  | total reactive power |
| total_power_A | complex | W | PUBLIC |  |  |  | total complex power on phase A |
| total_power_B | complex | W | PUBLIC |  |  |  | total complex power on phase B |
| total_power_C | complex | W | PUBLIC |  |  |  | total complex power on phase C |
| weather | object |  | PUBLIC |  |  |  | weather object for temperature and solar gain |
| tariff | object |  | PUBLIC |  |  |  | tariff object for energy price sensitivity |

### tariff

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| rate_design | char32 |  | PUBLIC |  |  |  |  |
| minimum_daily_charge | double | $/day | PUBLIC |  |  |  |  |
| energy_charge_base | double | $/kWh | PUBLIC |  |  |  |  |
| energy_charge_100 | double | $/kWh | PUBLIC |  |  |  |  |
| energy_charge_400 | double | $/kWh | PUBLIC |  |  |  |  |
| average_total_rate | double | $/kWh | PUBLIC |  |  |  |  |
| minimum_average_rate_limit | double | $/kWh | PUBLIC |  |  |  |  |
| discount | double |  | PUBLIC |  |  |  |  |

### billing

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| tariff | object |  | PUBLIC | REQUIRED |  |  | reference to the tariff object used for this bill |
| meter | object |  | PUBLIC | REQUIRED |  |  | reference to the meter object use for this bill |
| bill_day | int32 |  | PUBLIC |  |  |  | day of month when the bill is generated |
| bill_date | timestamp |  | PUBLIC | OUTPUT |  |  | date of the last bill generated |
| billing_days | int32 |  | PUBLIC | OUTPUT |  |  | number of day of last bill |
| total_bill | double | $ | PUBLIC | OUTPUT |  |  | last bill total |
| total_charges | double | $ | PUBLIC | OUTPUT |  |  | last bill total charges |
| energy_charges | double | $ | PUBLIC | OUTPUT |  |  | last bill energy charges |
| capacity_charges | double | $ | PUBLIC | OUTPUT |  |  | last bill capacity charges |
| ramping_charges | double | $ | PUBLIC | OUTPUT |  |  | last bill ramping charges |
| fixed_charges | double | $ | PUBLIC | OUTPUT |  |  | last bill fixed charges |
| total_credits | double | $ | PUBLIC | OUTPUT |  |  | last bill total credits |
| energy_credits | double | $ | PUBLIC | OUTPUT |  |  | last bill energy credits |
| capacity_credits | double | $ | PUBLIC | OUTPUT |  |  | last bill capacity credits |
| ramping_credits | double | $ | PUBLIC | OUTPUT |  |  | last bill ramping credits |
| fixed_credits | double | $ | PUBLIC | OUTPUT |  |  | last bill fixed credits |
| metering_interval | double | s | PUBLIC |  |  |  | interval at which meter is observed |
| baseline_demand | double | kWh/day | PUBLIC |  |  |  | daily demand of baseline tier |

### player

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| property | char256 |  | PUBLIC | REQUIRED |  |  | property to play values into |
| file | char1024 |  | PUBLIC | REQUIRED |  |  | file from which to read values |
| filetype | char8 |  | PUBLIC |  |  |  |  |
| mode | char32 |  | PUBLIC |  |  |  |  |
| loop | int32 |  | PUBLIC |  |  | 0 | number of time to loop the player |

### shaper

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| file | char1024 |  | PUBLIC |  |  |  |  |
| filetype | char8 |  | PUBLIC |  |  |  |  |
| mode | char32 |  | PUBLIC |  |  |  |  |
| group | char256 |  | PUBLIC |  |  |  |  |
| property | char256 |  | PUBLIC |  |  |  |  |
| magnitude | double |  | PUBLIC |  |  |  |  |
| events | double |  | PUBLIC |  |  |  |  |

### multi_recorder

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| interval | double | s | PUBLIC |  |  | -1 s | sampling interval |
| property | method |  | PUBLIC | REQUIRED |  |  | list of properties to sample |
| trigger | char32 |  | PUBLIC |  |  |  | condition to trigger recorder |
| file | char1024 |  | PUBLIC | REQUIRED |  |  | file in which to record data |
| filetype | char8 |  | PUBLIC |  |  |  | file type to use when storing data |
| mode | char32 |  | PUBLIC |  |  | file | recorder operating mode |
| multifile | char1024 |  | PUBLIC |  |  |  |  |
| limit | int32 |  | PUBLIC |  |  | 0 | limit on number of rows to record (0=none) |
| plotcommands | char1024 |  | PUBLIC |  |  |  |  |
| xdata | char32 |  | PUBLIC |  |  |  |  |
| columns | char32 |  | PUBLIC |  |  |  |  |
| format | char32 |  | PUBLIC |  |  | 0 |  |
| output | enumeration |  | PUBLIC |  | SVG PNG PDF JPG GIF EPS SCREEN |  |  |
| header_units | enumeration |  | PUBLIC |  | NONE ALL DEFAULT |  |  |
| line_units | enumeration |  | PUBLIC |  | NONE ALL DEFAULT |  |  |

### collector

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| trigger | char32 |  | PUBLIC |  |  |  |  |
| file | char1024 |  | PUBLIC |  |  |  |  |
| limit | int32 |  | PUBLIC |  |  |  |  |
| group | char256 |  | PUBLIC |  |  |  |  |
| flush | int32 |  | PUBLIC |  |  |  |  |
| interval | double | s | PUBLIC |  |  |  |  |
| property | method |  | PUBLIC |  |  |  |  |

### histogram

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| filename | char1024 |  | PUBLIC |  |  |  | the name of the file to write |
| filetype | char8 |  | PUBLIC |  |  |  | the format to output a histogram in |
| mode | char32 |  | PUBLIC |  |  |  | the mode of file output |
| group | char1024 |  | PUBLIC |  |  |  | the GridLAB-D group expression to use for this histogram |
| bins | char1024 |  | PUBLIC |  |  |  | the specific bin values to use |
| property | char256 |  | PUBLIC |  |  |  | the property to sample |
| min | double |  | PUBLIC |  |  |  | the minimum value of the auto-sized bins to use |
| max | double |  | PUBLIC |  |  |  | the maximum value of the auto-sized bins to use |
| samplerate | double | s | PUBLIC |  |  |  | the rate at which samples are read |
| countrate | double | s | PUBLIC |  |  |  | the reate at which bins are counted and written |
| bin_count | int32 |  | PUBLIC |  |  |  | the number of auto-sized bins to use |
| limit | int32 |  | PUBLIC |  |  |  | the number of samples to write |

### group_recorder

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| file | char256 |  | PUBLIC |  |  |  | output file name |
| group | char1024 |  | PUBLIC |  |  |  | group definition string |
| interval | double | s | PUBLIC |  |  |  | recordering interval (0 'every iteration', -1 'on change') |
| flush_interval | double | s | PUBLIC |  |  |  | file flush interval (0 never, negative on samples) |
| strict | bool |  | PUBLIC |  |  |  | causes the group_recorder to stop the simulation should there be a problem opening or writing with the group_recorder |
| print_units | bool |  | PUBLIC |  |  |  | flag to append units to each written value, if applicable |
| property | char256 |  | PUBLIC |  |  |  | property to record |
| limit | int32 |  | PUBLIC |  |  |  | the maximum number of lines to write to the file |
| format | bool |  | PUBLIC |  |  |  | determines whether output timestamp is formatted to be formatted as human-readable (default) or epoch |
| complex_part | enumeration |  | PUBLIC |  | ANG_RAD ANG_DEG MAG IMAG REAL NONE |  | the complex part to record if complex properties are gathered |

### violation_recorder

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| file | char256 |  | PUBLIC |  |  |  | output file name |
| summary | char256 |  | PUBLIC |  |  |  | summary output file name |
| virtual_substation | char256 |  | PUBLIC |  |  |  | name of the substation node to monitor for reverse flow violation |
| interval | double | s | PUBLIC |  |  |  | recording interval (0 'every iteration', -1 'on change') |
| flush_interval | double | s | PUBLIC |  |  |  | file flush interval (0 never, negative on samples) |
| strict | bool |  | PUBLIC |  |  |  | causes the violation_recorder to stop the simulation should there be a problem opening or writing with the violation_recorder |
| echo | bool |  | PUBLIC |  |  |  | causes the violation_recorder to echo messages to the screen |
| limit | int32 |  | PUBLIC |  |  |  | the maximum number of lines to write to the file |
| violation_delay | int32 |  | PUBLIC |  |  |  | the number of seconds to skip before recording violations |
| xfrmr_thermal_limit_upper | double |  | PUBLIC |  |  |  | xfrmr_thermal_limit_upper |
| xfrmr_thermal_limit_lower | double |  | PUBLIC |  |  |  | xfrmr_thermal_limit_lower |
| line_thermal_limit_upper | double |  | PUBLIC |  |  |  | line_thermal_limit_upper |
| line_thermal_limit_lower | double |  | PUBLIC |  |  |  | line_thermal_limit_lower |
| node_instantaneous_voltage_limit_upper | double |  | PUBLIC |  |  |  | node_instantaneous_voltage_limit_upper |
| node_instantaneous_voltage_limit_lower | double |  | PUBLIC |  |  |  | node_instantaneous_voltage_limit_lower |
| node_continuous_voltage_limit_upper | double |  | PUBLIC |  |  |  | node_continuous_voltage_limit_upper |
| node_continuous_voltage_limit_lower | double |  | PUBLIC |  |  |  | node_continuous_voltage_limit_lower |
| node_continuous_voltage_interval | double |  | PUBLIC |  |  |  | node_continuous_voltage_interval |
| secondary_dist_voltage_rise_upper_limit | double |  | PUBLIC |  |  |  | secondary_dist_voltage_rise_upper_limit |
| secondary_dist_voltage_rise_lower_limit | double |  | PUBLIC |  |  |  | secondary_dist_voltage_rise_lower_limit |
| substation_breaker_A_limit | double |  | PUBLIC |  |  |  | breaker_phase_A_limit |
| substation_breaker_B_limit | double |  | PUBLIC |  |  |  | breaker_phase_B_limit |
| substation_breaker_C_limit | double |  | PUBLIC |  |  |  | breaker_phase_C_limit |
| substation_pf_lower_limit | double |  | PUBLIC |  |  |  | substation_pf_lower_limit |
| inverter_v_chng_per_interval_upper_bound | double |  | PUBLIC |  |  |  | inverter_v_chng_per_interval_upper_bound |
| inverter_v_chng_per_interval_lower_bound | double |  | PUBLIC |  |  |  | inverter_v_chng_per_interval_lower_bound |
| inverter_v_chng_interval | double |  | PUBLIC |  |  |  | inverter_v_chng_interval |
| violation_flag | set |  | PUBLIC |  | ALLVIOLATIONS VIOLATION8 VIOLATION7 VIOLATION6 VIOLATION5 VIOLATION4 VIOLATION3 VIOLATION2 VIOLATION1 VIOLATION0 |  | bit set for determining which violations to check |

### metrics_collector

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| interval | double | s | PUBLIC |  |  |  | Interval at which the metrics_collector output is stored in JSON format |

### metrics_collector_writer

| Property | Type | Unit | Access | Flags | Keywords | Default | Description |
| -------- | ---- | ---- | ------ | ----- | -------- | ------- | ----------- |
| filename | char256 |  | PUBLIC |  |  |  | the JSON formatted output file name |
| interval | double | s | PUBLIC |  |  |  | Interval at which the metrics_collector_writer output is stored in JSON format |

## Global data

### Header data

| Name | Type | Access |
| ---- | ---- | ------ |
| id | int32 | PROTECTED |
| class | string | PROTECTED |
| groupid | char32 | PROTECTED |
| name | string | PROTECTED |
| next | object | PROTECTED |
| parent | object | PROTECTED |
| child_count | int32 | PROTECTED |
| rank | int32 | PROTECTED |
| clock | timestamp | PROTECTED |
| valid_to | timestamp | PROTECTED |
| schedule_skew | int64 | PROTECTED |
| latitude | double | PROTECTED |
| longitude | double | PROTECTED |
| in_svc | timestamp | PROTECTED |
| out_svc | timestamp | PROTECTED |
| rng_state | int32 | PROTECTED |
| heartbeat | int64 | PROTECTED |
| guid | int64 | PROTECTED |
| profiler.presync | int32 | PROTECTED |
| profiler.sync | int32 | PROTECTED |
| profiler.postsync | int32 | PROTECTED |
| profiler.init | int32 | PROTECTED |
| profiler.heartbeat | int32 | PROTECTED |
| profiler.precommit | int32 | PROTECTED |
| profiler.commit | int32 | PROTECTED |
| profiler.finalize | int32 | PROTECTED |
| event.init | string | PROTECTED |
| event.precommit | string | PROTECTED |
| event.presync | string | PROTECTED |
| event.sync | string | PROTECTED |
| event.postsync | string | PROTECTED |
| event.commit | string | PROTECTED |
| event.finalize | string | PROTECTED |
| flags | int64 | PROTECTED |

### Data Types

| Type | Format | Default | Size | Converters | Comparators |
| ---- | ------- | ------- | ---- | ---------- | ----------- |
| double | decimal | 0.0 | 24 | tostr fromstr create stream | == <= >= != < > inside outside |
| complex | string | 0+0i | 48 | tostr fromstr create stream | == <= >= != < > inside outside |
| enumeration | string | 0 | 1024 | tostr fromstr create stream | == <= >= != < > inside outside |
| set | string | 0 | 1024 | tostr fromstr create stream | == <= >= != < > inside outside |
| int16 | integer | 0 | 6 | tostr fromstr create stream | == <= >= != < > inside outside |
| int32 | integer | 0 | 12 | tostr fromstr create stream | == <= >= != < > inside outside |
| int64 | integer | 0 | 24 | tostr fromstr create stream | == <= >= != < > inside outside |
| char8 | string |  | 8 | tostr fromstr create stream | == <= >= != < > inside outside |
| char32 | string |  | 32 | tostr fromstr create stream | == <= >= != < > inside outside |
| char256 | string |  | 256 | tostr fromstr create stream | == <= >= != < > inside outside |
| char1024 | string |  | 1024 | tostr fromstr create stream | == <= >= != < > inside outside |
| object | string | (na) | 64 | tostr fromstr create stream | == != |
| delegated | string | (na) | (na) | create stream |  |
| bool | string | FALSE | 6 | tostr fromstr create stream | == != |
| timestamp | string | 0 | 32 | tostr fromstr create stream | == <= >= != < > inside outside |
| double_array | string |  | 1024 | tostr fromstr create stream |  |
| complex_array | string |  | 1024 | tostr fromstr create stream |  |
| real | decimal | 0.0 | 24 | tostr fromstr create stream |  |
| float | decimal | 0.0 | 24 | tostr fromstr create stream |  |
| loadshape | string | (na) | 1024 | tostr fromstr create stream | == <= >= != < > inside outside |
| enduse | string | (na) | 1024 | tostr fromstr create stream | == <= >= != < > inside outside |
| randomvar | string | (na) | 24 | tostr fromstr create stream | == <= >= != < > inside outside |
| method | string | (na) | (na) | create stream |  |
| string | string |  | (na) | create stream | == <= >= != < > inside outside |
| python | string | None | (na) | create stream |  |

### Global variables

| Name | Module | Type | Access | Keywords | Value |
| ---- | ------ | ---- | ------ | -------- | ----- |
| version.major |  | int32 | REFERENCE |  | 4 |
| version.minor |  | int32 | REFERENCE |  | 2 |
| version.patch |  | int32 | REFERENCE |  | 13 |
| version.build |  | int32 | REFERENCE |  | 201012 |
| version.branch |  | char256 | REFERENCE |  | develop_add_markdown_output_options |
| version |  | char1024 | REFERENCE |  | 4.2.13-201012-develop_add_markdown_output_options |
| command_line |  | char1024 | REFERENCE |  | /usr/local/opt/gridlabd/4.2.13-201012-develop_add_markdown_output_options/bin/gridlabd.bin gldcore/converters/autotest/test_markdown.glm -o gldcore/converters/autotest/test_markdown.md |
| environment |  | char1024 | PUBLIC |  | batch |
| quiet |  | bool | PUBLIC |  | FALSE |
| warn |  | bool | PUBLIC |  | TRUE |
| debugger |  | bool | PUBLIC |  | FALSE |
| gdb |  | bool | PUBLIC |  | FALSE |
| debug |  | bool | PUBLIC |  | FALSE |
| test |  | bool | PUBLIC |  | FALSE |
| verbose |  | bool | PUBLIC |  | FALSE |
| iteration_limit |  | int32 | PUBLIC |  | 100 |
| workdir |  | char1024 | REFERENCE |  | /Users/dchassin/github/gridlabd |
| dumpfile |  | char1024 | PUBLIC |  | gridlabd.json |
| savefile |  | char1024 | PUBLIC |  | gldcore/converters/autotest/test_markdown.md |
| dumpall |  | bool | PUBLIC |  | FALSE |
| runchecks |  | bool | PUBLIC |  | FALSE |
| threadcount |  | int32 | PUBLIC |  | 1 |
| profiler |  | bool | PUBLIC |  | FALSE |
| pauseatexit |  | bool | PUBLIC |  | FALSE |
| testoutputfile |  | char1024 | PUBLIC |  | test.txt |
| xml_encoding |  | int32 | PUBLIC |  | 8 |
| clock |  | timestamp | PUBLIC |  | 2000-01-01 00:00:00 UTC |
| starttime |  | timestamp | PUBLIC |  | 2000-01-01 00:00:00 UTC |
| stoptime |  | timestamp | PUBLIC |  | NEVER |
| double_format |  | char32 | PUBLIC |  | %+lg |
| complex_format |  | char256 | PUBLIC |  | %+lg%+lg%c |
| object_format |  | char32 | PUBLIC |  | %s:%d |
| object_scan |  | char32 | PUBLIC |  | %[^:]:%d |
| object_tree_balance |  | bool | PUBLIC |  | FALSE |
| kmlfile |  | char1024 | PUBLIC |  |  |
| kmlhost |  | char1024 | PUBLIC |  | https://raw.githubusercontent.com/hipas/gridlabd/master/gldcore/rt |
| modelname |  | char1024 | REFERENCE |  | gldcore/converters/autotest/test_markdown.glm |
| execdir |  | char1024 | REFERENCE |  | /usr/local/opt/gridlabd/4.2.13-201012-develop_add_markdown_output_options/bin |
| strictnames |  | bool | PUBLIC |  | TRUE |
| website |  | char1024 | PUBLIC |  | http://www.gridlabd.org/ |
| urlbase |  | char1024 | PUBLIC |  | http://www.gridlabd.org/ |
| randomstate |  | int32 | PUBLIC |  | -1342211304 |
| randomseed |  | int32 | PUBLIC |  | -1342211304 |
| include |  | char1024 | REFERENCE |  |  |
| trace |  | char1024 | PUBLIC |  |  |
| gdb_window |  | bool | PUBLIC |  | FALSE |
| tmp |  | char1024 | PUBLIC |  | /Users/dchassin/.gridlabd/tmp |
| force_compile |  | int32 | PUBLIC |  | 0 |
| nolocks |  | bool | PUBLIC |  | FALSE |
| skipsafe |  | bool | PUBLIC |  | FALSE |
| dateformat |  | enumeration | PUBLIC | ISO US EURO ISO8601 | ISO |
| init_sequence |  | enumeration | PUBLIC | CREATION DEFERRED BOTTOMUP TOPDOWN | DEFERRED |
| minimum_timestep |  | int32 | PUBLIC |  | 1 |
| platform |  | char8 | REFERENCE |  | MACOSX |
| suppress_repeat_messages |  | bool | PUBLIC |  | TRUE |
| maximum_synctime |  | int32 | PUBLIC |  | 60 |
| run_realtime |  | bool | PUBLIC |  | FALSE |
| enter_realtime |  | timestamp | PUBLIC |  | NEVER |
| realtime_metric |  | double | REFERENCE |  | +0 |
| no_deprecate |  | bool | PUBLIC |  | FALSE |
| streaming_io |  | bool | PROTECTED |  | FALSE |
| compileonly |  | bool | PROTECTED |  | TRUE |
| relax_naming_rules |  | bool | PUBLIC |  | FALSE |
| browser |  | char1024 | PUBLIC |  | safari |
| server_portnum |  | int32 | PUBLIC |  | 0 |
| server_quit_on_close |  | bool | PUBLIC |  | FALSE |
| client_allowed |  | char1024 | PUBLIC |  |  |
| autoclean |  | bool | PUBLIC |  | TRUE |
| technology_readiness_level |  | enumeration | PUBLIC | PRINCIPLE CONCEPT PROOF STANDALONE INTEGRATED DEMONSTRATED PROTOTYPE QUALIFIED PROVEN UNKNOWN | UNKNOWN |
| show_progress |  | bool | PUBLIC |  | TRUE |
| checkpoint_type |  | enumeration | PUBLIC | NONE WALL SIM | NONE |
| checkpoint_file |  | char1024 | PUBLIC |  |  |
| checkpoint_seqnum |  | int32 | PUBLIC |  | 0 |
| checkpoint_interval |  | int32 | PUBLIC |  | 0 |
| checkpoint_keepall |  | bool | PUBLIC |  | FALSE |
| check_version |  | bool | PUBLIC |  | FALSE |
| random_number_generator |  | enumeration | PUBLIC | RNG2 RNG3 | RNG3 |
| mainloop_state |  | enumeration | PUBLIC | INIT RUNNING PAUSED DONE LOCKED | INIT |
| pauseat |  | timestamp | PUBLIC |  | NEVER |
| infourl |  | char1024 | PUBLIC |  | http://docs.gridlabd.us/index.html?owner=hipas&project=gridlabd&search= |
| hostname |  | char1024 | PUBLIC |  | localhost |
| hostaddr |  | char32 | PUBLIC |  | 127.0.0.1 |
| autostart_gui |  | bool | PUBLIC |  | TRUE |
| master |  | char1024 | PUBLIC |  |  |
| master_port |  | int64 | PUBLIC |  | 0 |
| multirun_mode |  | enumeration | PUBLIC | STANDALONE MASTER SLAVE LIBRARY | STANDALONE |
| multirun_conn |  | enumeration | PUBLIC | NONE MEMORY SOCKET | NONE |
| signal_timeout |  | int32 | PUBLIC |  | 5000 |
| slave_port |  | int16 | PUBLIC |  | 6267 |
| slave_id |  | int64 | PUBLIC |  | 0 |
| return_code |  | int32 | REFERENCE |  | 0 |
| exit_code |  | int16 | REFERENCE |  | 0 |
| module_compiler_flags |  | set | PUBLIC | NONE CLEAN KEEPWORK DEBUG VERBOSE | NONE |
| init_max_defer |  | int32 | REFERENCE |  | 64 |
| mt_analysis |  | bool | PUBLIC |  | FALSE |
| inline_block_size |  | int32 | PUBLIC |  | 1048576 |
| validate |  | set | PUBLIC | NONE TSTD TALL TRUN TERR TEXC TOPT RALL RDIR RGLM | TSTD|RALL |
| sanitize |  | set | PUBLIC | NAMES POSITIONS | NAMES|POSITIONS |
| sanitize_prefix |  | char8 | PUBLIC |  | GLD_ |
| sanitize_index |  | char1024 | PUBLIC |  | .txt |
| sanitize_offset |  | char32 | PUBLIC |  |  |
| simulation_mode |  | enumeration | PUBLIC | INIT EVENT DELTA DELTA_ITER ERROR | INIT |
| deltamode_allowed |  | bool | PUBLIC |  | FALSE |
| deltamode_timestep |  | int32 | PUBLIC |  | 10000000 |
| deltamode_maximumtime |  | int64 | PUBLIC |  | 3600000000000 |
| deltaclock |  | int64 | PUBLIC |  | 0 |
| delta_current_clock |  | double | PUBLIC |  | +0 |
| deltamode_updateorder |  | char1024 | REFERENCE |  |  |
| deltamode_iteration_limit |  | int32 | PUBLIC |  | 10 |
| deltamode_forced_extra_timesteps |  | int32 | PUBLIC |  | 0 |
| deltamode_forced_always |  | bool | PUBLIC |  | FALSE |
| run_powerworld |  | bool | PUBLIC |  | FALSE |
| bigranks |  | bool | PUBLIC |  | TRUE |
| exename |  | char1024 | REFERENCE |  | /usr/local/opt/gridlabd/4.2.13-201012-develop_add_markdown_output_options/bin/gridlabd.bin |
| wget_options |  | char1024 | PUBLIC |  | maxsize:100MB;update:newer |
| curl_options |  | char1024 | PUBLIC |  | maxsize:100MB;update:newer |
| svnroot |  | char1024 | PUBLIC |  | http://gridlab-d.svn.sourceforge.net/svnroot/gridlab-d |
| github |  | char1024 | PUBLIC |  | https://github.com/gridlab-d |
| gitraw |  | char1024 | PUBLIC |  | https://raw.githubusercontent.com/gridlab-d |
| allow_reinclude |  | bool | PUBLIC |  | FALSE |
| output_message_context |  | set | PUBLIC | NONE ALL LOAD CREATE EXEC TIME FIND CLASS OBJECT MODULE INDEX GLOBALS EXCEPTION AGGREGATE COMPARE CONVERT DELTAMODE ENDUSE ENVIRONMENT GUI HTTPCLIENT INSTANCE INTERPOLATE JOB KML LEGAL LINK LIST XML LOADSHAPE LOCALE LOCK MATCH MATLAB PROPERTY RANDOM REALTIME SANITIZE SAVE SCHEDULE SERVER SETUP STREAM TEST THREADPOOL TRANSFORM HTTP UNIT VALIDATE VERSION XCORE MAIN CMDARG | ALL |
| permissive_access |  | int32 | PUBLIC |  | 0 |
| relax_undefined_if |  | bool | PUBLIC |  | FALSE |
| literal_if |  | bool | PUBLIC |  | TRUE |
| validto_context |  | enumeration | PUBLIC | SYNC PRECOMMIT COMMIT | 1129600256 |
| daemon_configfile |  | char1024 | PUBLIC |  | gridlabd.cnf |
| timezone_locale |  | char1024 | REFERENCE |  | UTC0 |
| glm_save_options |  | set | PUBLIC | LEGACY MINIMAL NOGLOBALS NODEFAULTS NOMACROS NOINTERNALS ORIGINAL | MINIMAL |
| filesave_options |  | set | PUBLIC | ALLINITIAL ALLMINIMAL ALL MODULES PROPERTIES CLASSES GLOBALS OBJECTS SCHEDULES FILTERS SCRIPTS CLOCK INITIAL MINIMAL | ALL |
| ignore_errors |  | bool | PUBLIC |  | FALSE |
| keep_progress |  | bool | PUBLIC |  | FALSE |
| allow_variant_aggregates |  | bool | PUBLIC |  | FALSE |
| progress |  | double | REFERENCE |  | +0 |
| server_keepalive |  | bool | PUBLIC |  | FALSE |
| pythonpath |  | char1024 | PUBLIC |  | . |
| datadir |  | char1024 | PUBLIC |  | /usr/local/opt/gridlabd/4.2.13-201012-develop_add_markdown_output_options/share/gridlabd |
| json_complex_format |  | set | PUBLIC | STRING LIST DICT DEGREES RADIANS | STRING |
| rusage_file |  | char1024 | PUBLIC |  | gridlabd-rusage.csv |
| rusage_rate |  | int64 | PUBLIC |  | 0 |
| rusage |  | char1024 | PUBLIC |  | {} |
| message_flags | assert | set | PUBLIC | QUIET WARNING DEBUG VERBOSE | DEBUG|VERBOSE |
| message_flags | climate | set | PUBLIC | QUIET WARNING DEBUG VERBOSE | DEBUG|VERBOSE |
| library_path |  | char1024 | PUBLIC |  | /usr/local/opt/gridlabd/4.2.13-201012-develop_add_markdown_output_options/share/gridlabd/weather/US |
| message_flags | commercial | set | PUBLIC | QUIET WARNING DEBUG VERBOSE | DEBUG|VERBOSE |
| warn_control |  | bool | PUBLIC |  | TRUE |
| warn_low_temp |  | double | PUBLIC |  | +50 |
| warn_high_temp |  | double | PUBLIC |  | +90 |
| default_nominal_voltage_A |  | complex | PUBLIC |  | +240+0d |
| default_nominal_voltage_B |  | complex | PUBLIC |  | +268.328-26.5651d |
| default_nominal_voltage_C |  | complex | PUBLIC |  | +268.328+26.5651d |
| default_nominal_voltage |  | double | PUBLIC |  | +240 |
| default_weekday_code |  | char32 | PUBLIC |  | WEEKDAY |
| default_saturday_code |  | char32 | PUBLIC |  | SATURDAY |
| default_sunday_code |  | char32 | PUBLIC |  | SUNDAY |
| default_holiday_code |  | char32 | PUBLIC |  | HOLIDAY |
| default_month_heading |  | char32 | PUBLIC |  | Month |
| default_daytype_heading |  | char32 | PUBLIC |  | Daytype |
| default_hour_heading |  | char32 | PUBLIC |  | Hour |
| default_temperature_heating_balance |  | double | PUBLIC |  | +55 |
| default_temperature_cooling_balance |  | double | PUBLIC |  | +70 |
| default_temperature_heating_base |  | double | PUBLIC |  | +55 |
| default_temperature_cooling_base |  | double | PUBLIC |  | +70 |
| default_temperature_heating_design |  | double | PUBLIC |  | +0 |
| default_temperature_cooling_design |  | double | PUBLIC |  | +100 |
| default_solargain_base |  | double | PUBLIC |  | +0 |
| default_price_base |  | double | PUBLIC |  | +0 |
| default_occupancy_base |  | double | PUBLIC |  | +1 |
| temperature_variable_name |  | char1024 | PUBLIC |  | temperature |
| solargain_variable_name |  | char1024 | PUBLIC |  | solar_direct |
| price_variable_name |  | char1024 | PUBLIC |  | energy_price |
| occupancy_variable_name |  | char1024 | PUBLIC |  | occupancy_fraction |
| nightlight_threshold |  | double | PUBLIC |  | +0 W/m^2 |
| message_flags | generators | set | PUBLIC | QUIET WARNING DEBUG VERBOSE | DEBUG|VERBOSE |
| enable_subsecond_models |  | bool | PUBLIC |  | FALSE |
| deltamode_timestep |  | double | PUBLIC |  | +1e+07 ns |
| message_flags | industrial | set | PUBLIC | QUIET WARNING DEBUG VERBOSE | DEBUG|VERBOSE |
| naics_data_file |  | char1024 | PUBLIC |  | naics_data_file.csv |
| load_property_name |  | char256 | PUBLIC |  | base_power_ |
| message_flags | influxdb | set | PUBLIC | QUIET WARNING DEBUG VERBOSE | DEBUG|VERBOSE |
| default_username |  | char32 | PUBLIC |  | gridlabd |
| default_password |  | char32 | PUBLIC |  | gridlabd |
| default_hostname |  | char256 | PUBLIC |  | localhost |
| default_port |  | int32 | PUBLIC |  | 8086 |
| default_database |  | char256 | PUBLIC |  | gridlabd |
| connection_protocol |  | char32 | PUBLIC |  | http |
| synchronous_postdata |  | bool | PUBLIC |  | FALSE |
| message_flags | market | set | PUBLIC | QUIET WARNING DEBUG VERBOSE | DEBUG|VERBOSE |
| bid_offset |  | double | PUBLIC |  | +0.0001 $ |
| message_flags | optimize | set | PUBLIC | QUIET WARNING DEBUG VERBOSE | DEBUG|VERBOSE |
| message_flags | powerflow | set | PUBLIC | QUIET WARNING DEBUG VERBOSE | DEBUG|VERBOSE |
| show_matrix_values |  | bool | PUBLIC |  | FALSE |
| primary_voltage_ratio |  | double | PUBLIC |  | +60 |
| nominal_frequency |  | double | PUBLIC |  | +60 |
| require_voltage_control |  | bool | PUBLIC |  | FALSE |
| geographic_degree |  | double | PUBLIC |  | +0 |
| fault_impedance |  | complex | PUBLIC |  | +1e-06+0j |
| ground_impedance |  | complex | PUBLIC |  | +1e-06+0j |
| warning_underfrequency |  | double | PUBLIC |  | +55 |
| warning_overfrequency |  | double | PUBLIC |  | +65 |
| warning_undervoltage |  | double | PUBLIC |  | +0.8 |
| warning_overvoltage |  | double | PUBLIC |  | +1.2 |
| warning_voltageangle |  | double | PUBLIC |  | +2 |
| maximum_voltage_error |  | double | PUBLIC |  | +1e-06 |
| solver_method | powerflow | enumeration | PUBLIC | NR GS FBS | FBS |
| NR_matrix_file |  | char256 | PUBLIC |  |  |
| NR_matrix_output_interval | powerflow | enumeration | PUBLIC | ALL PER_CALL ONCE NEVER | NEVER |
| NR_matrix_output_references |  | bool | PUBLIC |  | FALSE |
| line_capacitance |  | bool | PUBLIC |  | FALSE |
| line_limits |  | bool | PUBLIC |  | TRUE |
| lu_solver |  | char256 | PUBLIC |  |  |
| NR_iteration_limit |  | int64 | PUBLIC |  | 500 |
| NR_deltamode_iteration_limit |  | int64 | PUBLIC |  | 10 |
| NR_superLU_procs |  | int32 | PUBLIC |  | 1 |
| default_maximum_voltage_error |  | double | PUBLIC |  | +1e-06 |
| default_maximum_power_error |  | double | PUBLIC |  | +0.0001 |
| NR_admit_change |  | bool | PUBLIC |  | TRUE |
| enable_subsecond_models |  | bool | PUBLIC |  | FALSE |
| all_powerflow_delta |  | bool | PUBLIC |  | FALSE |
| deltamode_timestep |  | double | PUBLIC |  | +1e+07 ns |
| current_frequency |  | double | PUBLIC |  | +60 Hz |
| master_frequency_update |  | bool | PUBLIC |  | FALSE |
| enable_frequency_dependence |  | bool | PUBLIC |  | FALSE |
| default_resistance |  | double | PUBLIC |  | +0.0001 |
| enable_inrush |  | bool | PUBLIC |  | FALSE |
| low_voltage_impedance_level |  | double | PUBLIC |  | +0.7 |
| enable_mesh_fault_current |  | bool | PUBLIC |  | FALSE |
| convergence_error_handling | powerflow | enumeration | PUBLIC | COLLAPSE IGNORE FATAL | FATAL |
| solver_profile_enable |  | bool | PUBLIC |  | FALSE |
| solver_profile_filename |  | char1024 | PUBLIC |  | solver_nr_profile.csv |
| solver_profile_headers_included |  | bool | PUBLIC |  | TRUE |
| solver_headers |  | char1024 | PUBLIC |  | timestamp,duration[microsec],iteration,bus_count,branch_count,error |
| solver_py_config |  | char1024 | PUBLIC |  | /usr/local/var/gridlabd/solver_py.conf |
| solver_dump_enable |  | bool | PUBLIC |  | FALSE |
| market_price_name |  | char1024 | PUBLIC |  | current_market.clearing_price |
| repair_time |  | double | PUBLIC |  | +24 h |
| climate_impact_zone | powerflow | enumeration | PUBLIC | NONE LOW MODERATE INTERMEDIATE HIGH EXTREME | NONE |
| message_flags | reliability | set | PUBLIC | QUIET WARNING DEBUG VERBOSE | DEBUG|VERBOSE |
| enable_subsecond_models |  | bool | PUBLIC |  | FALSE |
| maximum_event_length |  | double | PUBLIC |  | +432000 s |
| report_event_log |  | bool | PUBLIC |  | TRUE |
| deltamode_timestep |  | int32 | PUBLIC |  | 10000000 |
| default_cost_type |  | set | PUBLIC | WAGES LIGHTS COOKING HEATING ELDERCARE BUSINESS LODGING GEN_ALL GEN_COMFORT GEN_BASIC SEPTIC WATER FOOD | LIGHTS|COOKING|HEATING|FOOD |
| FOOD_delay_time |  | double | PUBLIC |  | +8 h |
| FOOD_fixed_cost |  | double | PUBLIC |  | +72 $ |
| FOOD_variable_cost |  | double | PUBLIC |  | +0 $/h |
| WATER_delay_time |  | double | PUBLIC |  | +1 h |
| WATER_fixed_cost |  | double | PUBLIC |  | +0 $ |
| WATER_variable_cost |  | double | PUBLIC |  | +6.25 $/h |
| SEPTIC_delay_time |  | double | PUBLIC |  | +24 h |
| SEPTIC_fixed_cost |  | double | PUBLIC |  | +1492 $ |
| SEPTIC_variable_cost |  | double | PUBLIC |  | +0 $/h |
| GEN_BASIC_delay_time |  | double | PUBLIC |  | +0 h |
| GEN_BASIC_fixed_cost |  | double | PUBLIC |  | +4000 $ |
| GEN_BASIC_variable_cost |  | double | PUBLIC |  | +39 $/h |
| GEN_COMFORT_delay_time |  | double | PUBLIC |  | +0 h |
| GEN_COMFORT_fixed_cost |  | double | PUBLIC |  | +4000 $ |
| GEN_COMFORT_variable_cost |  | double | PUBLIC |  | +83 $/h |
| GEN_ALL_delay_time |  | double | PUBLIC |  | +0 h |
| GEN_ALL_fixed_cost |  | double | PUBLIC |  | +8000 $ |
| GEN_ALL_variable_cost |  | double | PUBLIC |  | +222 $/h |
| LODGING_delay_time |  | double | PUBLIC |  | +48 h |
| LODGING_fixed_cost |  | double | PUBLIC |  | +0 $ |
| LODGING_variable_cost |  | double | PUBLIC |  | +12.5 $/h |
| BUSINESS_delay_time |  | double | PUBLIC |  | +72 h |
| BUSINESS_fixed_cost |  | double | PUBLIC |  | +0 $ |
| BUSINESS_variable_cost |  | double | PUBLIC |  | +10 $/h |
| ELDERCARE_delay_time |  | double | PUBLIC |  | +24 h |
| ELDERCARE_fixed_cost |  | double | PUBLIC |  | +0 $ |
| ELDERCARE_variable_cost |  | double | PUBLIC |  | +12.5 $/h |
| HEATING_delay_time |  | double | PUBLIC |  | +1 h |
| HEATING_fixed_cost |  | double | PUBLIC |  | +100 $ |
| HEATING_variable_cost |  | double | PUBLIC |  | +2 $/h |
| COOKING_delay_time |  | double | PUBLIC |  | +12 h |
| COOKING_fixed_cost |  | double | PUBLIC |  | +100 $ |
| COOKING_variable_cost |  | double | PUBLIC |  | +1.5 $/h |
| LIGHTS_delay_time |  | double | PUBLIC |  | +0 h |
| LIGHTS_fixed_cost |  | double | PUBLIC |  | +100 $ |
| LIGHTS_variable_cost |  | double | PUBLIC |  | +1.5 $/h |
| WAGES_delay_time |  | double | PUBLIC |  | +48 h |
| WAGES_fixed_cost |  | double | PUBLIC |  | +0 $ |
| WAGES_variable_cost |  | double | PUBLIC |  | +12 $/h |
| message_flags | residential | set | PUBLIC | QUIET WARNING DEBUG VERBOSE | DEBUG|VERBOSE |
| default_line_voltage |  | complex | PUBLIC |  | +240+0d V |
| default_line_current |  | complex | PUBLIC |  | +0+0j A |
| default_outdoor_temperature |  | double | PUBLIC |  | +74 degF |
| default_humidity |  | double | PUBLIC |  | +75 % |
| default_solar |  | double | PUBLIC |  | +0 Btu/sf |
| default_etp_iterations |  | int64 | PUBLIC |  | 100 |
| ANSI_voltage_check |  | bool | PUBLIC |  | TRUE |
| enable_subsecond_models |  | bool | PUBLIC |  | FALSE |
| deltamode_timestep |  | double | PUBLIC |  | +1e+07 ns |
| all_house_delta |  | bool | PUBLIC |  | FALSE |
| implicit_enduses | residential | set | PUBLIC | NONE DRYER CLOTHESWASHER WATERHEATER EVCHARGER RANGE REFRIGERATOR FREEZER MICROWAVE DISHWASHER OCCUPANCY PLUGS LIGHTS | DRYER|CLOTHESWASHER|WATERHEATER|RANGE|REFRIGERATOR|FREEZER|MICROWAVE|DISHWASHER|PLUGS|LIGHTS |
| implicit_enduse_source | residential | enumeration | PUBLIC | EIA2015 RBSA2014_DISCRETE RBSA2014 ELCAP2010 ELCAP1990 | ELCAP1990 |
| house_low_temperature_warning |  | double | PUBLIC |  | +55 degF |
| house_high_temperature_warning |  | double | PUBLIC |  | +95 degF |
| thermostat_control_warning |  | double | PUBLIC |  | +4.94066e-324 |
| system_dwell_time |  | double | PUBLIC |  | +1 s |
| aux_cutin_temperature |  | double | PUBLIC |  | +10 degF |
| paneldump_filename |  | char1024 | PUBLIC |  | paneldump.csv |
| paneldump_interval |  | int64 | PUBLIC |  | 0 |
| paneldump_resolution |  | double | PUBLIC |  | +0 |
| default_nominal_voltage_A |  | complex | PUBLIC |  | +240+0d |
| default_nominal_voltage_B |  | complex | PUBLIC |  | +268.328-26.5651d |
| default_nominal_voltage_C |  | complex | PUBLIC |  | +268.328+26.5651d |
| default_nominal_voltage |  | double | PUBLIC |  | +240 |
| default_weekday_code |  | char32 | PUBLIC |  | WEEKDAY |
| default_saturday_code |  | char32 | PUBLIC |  | SATURDAY |
| default_sunday_code |  | char32 | PUBLIC |  | SUNDAY |
| default_holiday_code |  | char32 | PUBLIC |  | HOLIDAY |
| default_month_heading |  | char32 | PUBLIC |  | Month |
| default_daytype_heading |  | char32 | PUBLIC |  | Daytype |
| default_hour_heading |  | char32 | PUBLIC |  | Hour |
| default_temperature_heating_balance |  | double | PUBLIC |  | +55 |
| default_temperature_cooling_balance |  | double | PUBLIC |  | +70 |
| default_temperature_heating_base |  | double | PUBLIC |  | +55 |
| default_temperature_cooling_base |  | double | PUBLIC |  | +70 |
| default_temperature_heating_design |  | double | PUBLIC |  | +0 |
| default_temperature_cooling_design |  | double | PUBLIC |  | +100 |
| default_solargain_base |  | double | PUBLIC |  | +0 |
| default_price_base |  | double | PUBLIC |  | +0 |
| default_occupancy_base |  | double | PUBLIC |  | +1 |
| temperature_variable_name |  | char1024 | PUBLIC |  | temperature |
| solargain_variable_name |  | char1024 | PUBLIC |  | solar_direct |
| price_variable_name |  | char1024 | PUBLIC |  | energy_price |
| occupancy_variable_name |  | char1024 | PUBLIC |  | occupancy_fraction |
| message_flags | revenue | set | PUBLIC | QUIET WARNING DEBUG VERBOSE | DEBUG|VERBOSE |
| lowincome_discount |  | double | PUBLIC |  | +0 $ |
| program_credit |  | double | PUBLIC |  | +0 $ |
| program_credit_months |  | char1024 | PUBLIC |  |  |
| summer_season_months |  | char1024 | PUBLIC |  |  |
| winter_season_months |  | char1024 | PUBLIC |  |  |
| billing_module |  | char1024 | PUBLIC |  | default_billing.py |
| billing_library |  | char1024 | PUBLIC |  | /usr/local/opt/gridlabd/current/share |
| billing_function |  | char1024 | PUBLIC |  | compute_bill |
| gnuplot_path |  | char1024 | PUBLIC |  | /usr/bin/gnuplot |
| flush_interval |  | int32 | PUBLIC |  | 0 |
| csv_data_only |  | int32 | PUBLIC |  | 0 |
| csv_header_type | tape | enumeration | PUBLIC | NAME NONE FULL | FULL |
| csv_keep_clean |  | int32 | PUBLIC |  | 0 |
| delta_mode_needed |  | timestamp | PUBLIC |  | NEVER |

