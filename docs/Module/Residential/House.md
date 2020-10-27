[[/Module/Residential/House]] -- Class house

# Synopsis

GLM:

~~~
  object house {
    weather "<string>";
    floor_area "<decimal> sf";
    gross_wall_area "<decimal> sf";
    ceiling_height "<decimal> ft";
    aspect_ratio "<decimal>";
    envelope_UA "<decimal> Btu/degF/h";
    window_wall_ratio "<decimal>";
    number_of_doors "<decimal>";
    exterior_wall_fraction "<decimal>";
    interior_exterior_wall_ratio "<decimal>";
    exterior_ceiling_fraction "<decimal>";
    exterior_floor_fraction "<decimal>";
    window_shading "<decimal>";
    window_exterior_transmission_coefficient "<decimal>";
    solar_heatgain_factor "<decimal>";
    airchange_per_hour "<decimal> unit/h";
    airchange_UA "<decimal> Btu/degF/h";
    UA "<decimal> Btu/degF/h";
    internal_gain "<decimal> Btu/h";
    solar_gain "<decimal> Btu/h";
    incident_solar_radiation "<decimal> Btu/h/sf";
    heat_cool_gain "<decimal> Btu/h";
    include_solar_quadrant "[W|S|E|N|H|NONE]";
    horizontal_diffuse_solar_radiation "<decimal> Btu/h/sf";
    north_incident_solar_radiation "<decimal> Btu/h/sf";
    northwest_incident_solar_radiation "<decimal> Btu/h/sf";
    west_incident_solar_radiation "<decimal> Btu/h/sf";
    southwest_incident_solar_radiation "<decimal> Btu/h/sf";
    south_incident_solar_radiation "<decimal> Btu/h/sf";
    southeast_incident_solar_radiation "<decimal> Btu/h/sf";
    east_incident_solar_radiation "<decimal> Btu/h/sf";
    northeast_incident_solar_radiation "<decimal> Btu/h/sf";
    heating_cop_curve "{CURVED,LINEAR,FLAT,DEFAULT}";
    heating_cap_curve "{CURVED,LINEAR,FLAT,DEFAULT}";
    cooling_cop_curve "{CURVED,LINEAR,FLAT,DEFAULT}";
    cooling_cap_curve "{CURVED,LINEAR,FLAT,DEFAULT}";
    use_latent_heat "<string>";
    include_fan_heatgain "<string>";
    thermostat_deadband "<decimal> degF";
    dlc_offset "<decimal> degF";
    thermostat_cycle_time "<integer>";
    thermostat_off_cycle_time "<integer>";
    thermostat_on_cycle_time "<integer>";
    thermostat_last_cycle_time "<string>";
    heating_setpoint "<decimal> degF";
    cooling_setpoint "<decimal> degF";
    design_heating_setpoint "<decimal> degF";
    design_cooling_setpoint "<decimal> degF";
    over_sizing_factor "<decimal>";
    simulate_window_openings "<string>";
    is_window_open "<decimal>";
    window_low_temperature_cutoff "<decimal> degF";
    window_high_temperature_cutoff "<decimal> degF";
    window_quadratic_coefficient "<decimal>";
    window_linear_coefficient "<decimal>";
    window_constant_coefficient "<decimal>";
    window_temperature_delta "<decimal>";
    design_heating_capacity "<decimal> Btu/h";
    design_cooling_capacity "<decimal> Btu/h";
    cooling_design_temperature "<decimal> degF";
    heating_design_temperature "<decimal> degF";
    design_peak_solar "<decimal> Btu/h/sf";
    design_internal_gains "<decimal> Btu/h";
    air_heat_fraction "<decimal> pu";
    mass_solar_gain_fraction "<decimal> pu";
    mass_internal_gain_fraction "<decimal> pu";
    auxiliary_heat_capacity "<decimal> Btu/h";
    aux_heat_deadband "<decimal> degF";
    aux_heat_temperature_lockout "<decimal> degF";
    aux_heat_time_delay "<decimal> s";
    cooling_supply_air_temp "<decimal> degF";
    heating_supply_air_temp "<decimal> degF";
    duct_pressure_drop "<decimal> inH2O";
    fan_design_power "<decimal> W";
    fan_low_power_fraction "<decimal> pu";
    fan_power "<decimal> kW";
    fan_design_airflow "<decimal> cfm";
    fan_impedance_fraction "<decimal> pu";
    fan_power_fraction "<decimal> pu";
    fan_current_fraction "<decimal> pu";
    fan_power_factor "<decimal> pu";
    heating_demand "<decimal> kW";
    cooling_demand "<decimal> kW";
    heating_COP "<decimal> pu";
    cooling_COP "<decimal> Btu/kWh";
    air_temperature "<decimal> degF";
    outdoor_temperature "<decimal> degF";
    outdoor_rh "<decimal> %";
    mass_heat_capacity "<decimal> Btu/degF";
    mass_heat_coeff "<decimal> Btu/degF/h";
    mass_temperature "<decimal> degF";
    air_volume "<decimal> cf";
    air_mass "<decimal> lb";
    air_heat_capacity "<decimal> Btu/degF";
    latent_load_fraction "<decimal> pu";
    total_thermal_mass_per_floor_area "<decimal> Btu/degF/sf";
    interior_surface_heat_transfer_coeff "<decimal> Btu/h/degF/sf";
    number_of_stories "<decimal>";
    is_AUX_on "<decimal>";
    is_HEAT_on "<decimal>";
    is_COOL_on "<decimal>";
    thermal_storage_present "<decimal>";
    thermal_storage_in_use "<decimal>";
    thermostat_mode "{COOL,HEAT,AUTO,OFF}";
    system_type "[RESISTIVE|TWOSTAGE|FORCEDAIR|AIRCONDITIONING|GAS|NONE]";
    auxiliary_strategy "[LOCKOUT|TIMER|DEADBAND|NONE]";
    system_mode "{AUX,COOL,OFF,HEAT,UNKNOWN}";
    last_system_mode "{AUX,COOL,OFF,HEAT,UNKNOWN}";
    heating_system_type "{RESISTANCE,HEAT_PUMP,GAS,NONE}";
    cooling_system_type "{HEAT_PUMP,ELECTRIC,NONE}";
    auxiliary_system_type "{ELECTRIC,NONE}";
    fan_type "{TWO_SPEED,ONE_SPEED,NONE}";
    thermal_integrity_level "{UNKNOWN,VERY_GOOD,GOOD,ABOVE_NORMAL,NORMAL,BELOW_NORMAL,LITTLE,VERY_LITTLE}";
    glass_type "{LOW_E_GLASS,GLASS,OTHER}";
    window_frame "{INSULATED,WOOD,THERMAL_BREAK,ALUMINIUM,ALUMINUM,NONE}";
    glazing_treatment "{HIGH_S,LOW_S,REFL,ABS,CLEAR,OTHER}";
    glazing_layers "{OTHER,THREE,TWO,ONE}";
    motor_model "{FULL,BASIC,NONE}";
    motor_efficiency "{VERY_GOOD,GOOD,AVERAGE,POOR,VERY_POOR}";
    last_mode_timer "<integer>";
    hvac_motor_efficiency "<decimal> unit";
    hvac_motor_loss_power_factor "<decimal> unit";
    Rroof "<decimal> degF*sf*h/Btu";
    Rwall "<decimal> degF*sf*h/Btu";
    Rfloor "<decimal> degF*sf*h/Btu";
    Rwindows "<decimal> degF*sf*h/Btu";
    Rdoors "<decimal> degF*sf*h/Btu";
    hvac_breaker_rating "<decimal> A";
    hvac_power_factor "<decimal> unit";
    hvac_load "<decimal> kW";
    last_heating_load "<decimal> kW";
    last_cooling_load "<decimal> kW";
    hvac_power "<string> kVA";
    total_load "<decimal> kW";
    panel "<string>";
    panel.energy "<string> kVAh";
    panel.power "<string> kVA";
    panel.peak_demand "<string> kVA";
    panel.heatgain "<decimal> Btu/h";
    panel.cumulative_heatgain "<decimal> Btu";
    panel.heatgain_fraction "<decimal> pu";
    panel.current_fraction "<decimal> pu";
    panel.impedance_fraction "<decimal> pu";
    panel.power_fraction "<decimal> pu";
    panel.power_factor "<decimal>";
    panel.constant_power "<string> kVA";
    panel.constant_current "<string> kVA";
    panel.constant_admittance "<string> kVA";
    panel.voltage_factor "<decimal> pu";
    panel.breaker_amps "<decimal> A";
    panel.configuration "[IS220|IS110]";
    design_internal_gain_density "<decimal> W/sf";
    compressor_on "<string>";
    compressor_count "<integer>";
    hvac_last_on "<string>";
    hvac_last_off "<string>";
    hvac_period_length "<decimal> s";
    hvac_duty_cycle "<decimal>";
    a "<decimal>";
    b "<decimal>";
    c "<decimal>";
    d "<decimal>";
    c1 "<decimal>";
    c2 "<decimal>";
    A3 "<decimal>";
    A4 "<decimal>";
    k1 "<decimal>";
    k2 "<decimal>";
    r1 "<decimal>";
    r2 "<decimal>";
    Teq "<decimal>";
    Tevent "<decimal>";
    Qi "<decimal>";
    Qa "<decimal>";
    Qm "<decimal>";
    Qh "<decimal>";
    Qlatent "<decimal>";
    dTair "<decimal>";
    adj_cooling_cap "<decimal>";
    adj_heating_cap "<decimal>";
    adj_cooling_cop "<decimal>";
    adj_heating_cop "<decimal>";
    thermostat_control "{NONE,BAND,FULL}";
    gas_enduses "<string>";
    circuit "<string>";
    shape "<string>";
    load "<string>";
    energy "<string> kVAh";
    power "<string> kVA";
    peak_demand "<string> kVA";
    heatgain "<decimal> Btu/h";
    cumulative_heatgain "<decimal> Btu";
    heatgain_fraction "<decimal> pu";
    current_fraction "<decimal> pu";
    impedance_fraction "<decimal> pu";
    power_fraction "<decimal> pu";
    power_factor "<decimal>";
    constant_power "<string> kVA";
    constant_current "<string> kVA";
    constant_admittance "<string> kVA";
    voltage_factor "<decimal> pu";
    breaker_amps "<decimal> A";
    configuration "[IS220|IS110]";
    override "{OFF,ON,NORMAL}";
    power_state "{UNKNOWN,ON,OFF}";
  }
~~~

# Description

TODO

## Properties

### `weather`

~~~
  object weather;
~~~

Reference to the climate object

### `floor_area`

~~~
  double floor_area[sf];
~~~

Home conditioned floor area

### `gross_wall_area`

~~~
  double gross_wall_area[sf];
~~~

Gross outdoor wall area

### `ceiling_height`

~~~
  double ceiling_height[ft];
~~~

Average ceiling height

### `aspect_ratio`

~~~
  double aspect_ratio;
~~~

Aspect ratio of the home's footprint

### `envelope_UA`

~~~
  double envelope_UA[Btu/degF/h];
~~~

Overall UA of the home's envelope

### `window_wall_ratio`

~~~
  double window_wall_ratio;
~~~

Ratio of window area to wall area

### `number_of_doors`

~~~
  double number_of_doors;
~~~

Ratio of door area to wall area

### `exterior_wall_fraction`

~~~
  double exterior_wall_fraction;
~~~

Ratio of exterior wall area to total wall area

### `interior_exterior_wall_ratio`

~~~
  double interior_exterior_wall_ratio;
~~~

Ratio of interior to exterior walls

### `exterior_ceiling_fraction`

~~~
  double exterior_ceiling_fraction;
~~~

Ratio of external ceiling sf to floor area

### `exterior_floor_fraction`

~~~
  double exterior_floor_fraction;
~~~

Ratio of floor area used in UA calculation

### `window_shading`

~~~
  double window_shading;
~~~

Transmission coefficient through window due to glazing

### `window_exterior_transmission_coefficient`

~~~
  double window_exterior_transmission_coefficient;
~~~

Coefficient for the amount of energy that passes through window

### `solar_heatgain_factor`

~~~
  double solar_heatgain_factor;
~~~

Product of the window area, window transmitivity, and the window exterior transmission coefficient

### `airchange_per_hour`

~~~
  double airchange_per_hour[unit/h];
~~~

Number of air-changes per hour

### `airchange_UA`

~~~
  double airchange_UA[Btu/degF/h];
~~~

Additional UA due to air infiltration

### `UA`

~~~
  double UA[Btu/degF/h];
~~~

The total UA

### `internal_gain`

~~~
  double internal_gain[Btu/h];
~~~

Internal heat gains

### `solar_gain`

~~~
  double solar_gain[Btu/h];
~~~

Solar heat gains

### `incident_solar_radiation`

~~~
  double incident_solar_radiation[Btu/h/sf];
~~~

Average incident solar radiation hitting the house

### `heat_cool_gain`

~~~
  double heat_cool_gain[Btu/h];
~~~

System heat gains(losses)

### `include_solar_quadrant`

~~~
  set {W, S, E, N, H, NONE} include_solar_quadrant;
~~~

Bit set for determining which solar incidence quadrants should be included in the solar heatgain

### `horizontal_diffuse_solar_radiation`

~~~
  double horizontal_diffuse_solar_radiation[Btu/h/sf];
~~~

Incident solar radiation hitting the top of the house

### `north_incident_solar_radiation`

~~~
  double north_incident_solar_radiation[Btu/h/sf];
~~~

Incident solar radiation hitting the north side of the house

### `northwest_incident_solar_radiation`

~~~
  double northwest_incident_solar_radiation[Btu/h/sf];
~~~

Incident solar radiation hitting the northwest side of the house

### `west_incident_solar_radiation`

~~~
  double west_incident_solar_radiation[Btu/h/sf];
~~~

Incident solar radiation hitting the west side of the house

### `southwest_incident_solar_radiation`

~~~
  double southwest_incident_solar_radiation[Btu/h/sf];
~~~

Incident solar radiation hitting the southwest side of the house

### `south_incident_solar_radiation`

~~~
  double south_incident_solar_radiation[Btu/h/sf];
~~~

Incident solar radiation hitting the south side of the house

### `southeast_incident_solar_radiation`

~~~
  double southeast_incident_solar_radiation[Btu/h/sf];
~~~

Incident solar radiation hitting the southeast side of the house

### `east_incident_solar_radiation`

~~~
  double east_incident_solar_radiation[Btu/h/sf];
~~~

Incident solar radiation hitting the east side of the house

### `northeast_incident_solar_radiation`

~~~
  double northeast_incident_solar_radiation[Btu/h/sf];
~~~

Incident solar radiation hitting the northeast side of the house

### `heating_cop_curve`

~~~
  enumeration {CURVED, LINEAR, FLAT, DEFAULT} heating_cop_curve;
~~~

Defines the function type to use for the adjusted heating COP as a function of outside air temperature.

### `heating_cap_curve`

~~~
  enumeration {CURVED, LINEAR, FLAT, DEFAULT} heating_cap_curve;
~~~

Defines the function type to use for the adjusted heating capacity as a function of outside air temperature.

### `cooling_cop_curve`

~~~
  enumeration {CURVED, LINEAR, FLAT, DEFAULT} cooling_cop_curve;
~~~

Defines the function type to use for the adjusted cooling COP as a function of outside air temperature.

### `cooling_cap_curve`

~~~
  enumeration {CURVED, LINEAR, FLAT, DEFAULT} cooling_cap_curve;
~~~

Defines the function type to use for the adjusted cooling capacity as a function of outside air temperature.

### `use_latent_heat`

~~~
  bool use_latent_heat;
~~~

Boolean for using the heat latency of the air to the humidity when cooling.

### `include_fan_heatgain`

~~~
  bool include_fan_heatgain;
~~~

Boolean to choose whether to include the heat generated by the fan in the ETP model.

### `thermostat_deadband`

~~~
  double thermostat_deadband[degF];
~~~

Deadband of thermostat control

### `dlc_offset`

~~~
  double dlc_offset[degF];
~~~

Used as a cap to offset the thermostat deadband for direct load control applications

### `thermostat_cycle_time`

~~~
  int16 thermostat_cycle_time;
~~~

Minimum time in seconds between thermostat updates

### `thermostat_off_cycle_time`

~~~
  int16 thermostat_off_cycle_time;
~~~

The minimum amount of time the thermostat cycle must stay in the off state

### `thermostat_on_cycle_time`

~~~
  int16 thermostat_on_cycle_time;
~~~

The minimum amount of time the thermostat cycle must stay in the on state

### `thermostat_last_cycle_time`

~~~
  timestamp thermostat_last_cycle_time;
~~~

Last time the thermostat changed state

### `heating_setpoint`

~~~
  double heating_setpoint[degF];
~~~

Thermostat heating setpoint

### `cooling_setpoint`

~~~
  double cooling_setpoint[degF];
~~~

Thermostat cooling setpoint

### `design_heating_setpoint`

~~~
  double design_heating_setpoint[degF];
~~~

System design heating setpoint

### `design_cooling_setpoint`

~~~
  double design_cooling_setpoint[degF];
~~~

System design cooling setpoint

### `over_sizing_factor`

~~~
  double over_sizing_factor;
~~~

Over sizes the heating and cooling system from standard specifications (0.2 ='s 120% sizing)

### `simulate_window_openings`

~~~
  bool simulate_window_openings;
~~~

Activates a representation of an occupant opening a window and de-activating the HVAC system

### `is_window_open`

~~~
  double is_window_open;
~~~

Defines the state of the window opening, 1=open, 2=closed

### `window_low_temperature_cutoff`

~~~
  double window_low_temperature_cutoff[degF];
~~~

Lowest temperature at which the window opening might occur

### `window_high_temperature_cutoff`

~~~
  double window_high_temperature_cutoff[degF];
~~~

Highest temperature at which the window opening might occur

### `window_quadratic_coefficient`

~~~
  double window_quadratic_coefficient;
~~~

Quadratic coefficient for describing function between low and high temperature cutoffs

### `window_linear_coefficient`

~~~
  double window_linear_coefficient;
~~~

Linear coefficient for describing function between low and high temperature cutoffs

### `window_constant_coefficient`

~~~
  double window_constant_coefficient;
~~~

Constant coefficient for describing function between low and high temperature cutoffs

### `window_temperature_delta`

~~~
  double window_temperature_delta;
~~~

Change in outdoor temperature required to update the window opening model

### `design_heating_capacity`

~~~
  double design_heating_capacity[Btu/h];
~~~

System heating capacity

### `design_cooling_capacity`

~~~
  double design_cooling_capacity[Btu/h];
~~~

System cooling capacity

### `cooling_design_temperature`

~~~
  double cooling_design_temperature[degF];
~~~

System cooling design temperature

### `heating_design_temperature`

~~~
  double heating_design_temperature[degF];
~~~

System heating design temperature

### `design_peak_solar`

~~~
  double design_peak_solar[Btu/h/sf];
~~~

System design solar load

### `design_internal_gains`

~~~
  double design_internal_gains[Btu/h];
~~~

System design internal gains

### `air_heat_fraction`

~~~
  double air_heat_fraction[pu];
~~~

Fraction of heat gain/loss that goes to air (as opposed to mass)

### `mass_solar_gain_fraction`

~~~
  double mass_solar_gain_fraction[pu];
~~~

Fraction of the heat gain/loss from the solar gains that goes to the mass

### `mass_internal_gain_fraction`

~~~
  double mass_internal_gain_fraction[pu];
~~~

Fraction of heat gain/loss from the internal gains that goes to the mass

### `auxiliary_heat_capacity`

~~~
  double auxiliary_heat_capacity[Btu/h];
~~~

Installed auxiliary heating capacity

### `aux_heat_deadband`

~~~
  double aux_heat_deadband[degF];
~~~

Temperature offset from standard heat activation to auxiliary heat activation

### `aux_heat_temperature_lockout`

~~~
  double aux_heat_temperature_lockout[degF];
~~~

Temperature at which auxiliary heat will not engage above

### `aux_heat_time_delay`

~~~
  double aux_heat_time_delay[s];
~~~

Time required for heater to run until auxiliary heating engages

### `cooling_supply_air_temp`

~~~
  double cooling_supply_air_temp[degF];
~~~

Temperature of air blown out of the cooling system

### `heating_supply_air_temp`

~~~
  double heating_supply_air_temp[degF];
~~~

Temperature of air blown out of the heating system

### `duct_pressure_drop`

~~~
  double duct_pressure_drop[inH2O];
~~~

End-to-end pressure drop for the ventilation ducts, in inches of water

### `fan_design_power`

~~~
  double fan_design_power[W];
~~~

Designed maximum power draw of the ventilation fan

### `fan_low_power_fraction`

~~~
  double fan_low_power_fraction[pu];
~~~

Fraction of ventilation fan power draw during low-power mode (two-speed only)

### `fan_power`

~~~
  double fan_power[kW];
~~~

Current ventilation fan power draw

### `fan_design_airflow`

~~~
  double fan_design_airflow[cfm];
~~~

Designed airflow for the ventilation system

### `fan_impedance_fraction`

~~~
  double fan_impedance_fraction[pu];
~~~

Impedance component of fan ZIP load

### `fan_power_fraction`

~~~
  double fan_power_fraction[pu];
~~~

Power component of fan ZIP load

### `fan_current_fraction`

~~~
  double fan_current_fraction[pu];
~~~

Current component of fan ZIP load

### `fan_power_factor`

~~~
  double fan_power_factor[pu];
~~~

Power factor of the fan load

### `heating_demand`

~~~
  double heating_demand[kW];
~~~

The current power draw to run the heating system

### `cooling_demand`

~~~
  double cooling_demand[kW];
~~~

The current power draw to run the cooling system

### `heating_COP`

~~~
  double heating_COP[pu];
~~~

System heating performance coefficient

### `cooling_COP`

~~~
  double cooling_COP[Btu/kWh];
~~~

System cooling performance coefficient

### `air_temperature`

~~~
  double air_temperature[degF];
~~~

Indoor air temperature

### `outdoor_temperature`

~~~
  double outdoor_temperature[degF];
~~~

Outdoor air temperature

### `outdoor_rh`

~~~
  double outdoor_rh[%];
~~~

Outdoor relative humidity

### `mass_heat_capacity`

~~~
  double mass_heat_capacity[Btu/degF];
~~~

Interior mass heat capacity

### `mass_heat_coeff`

~~~
  double mass_heat_coeff[Btu/degF/h];
~~~

Interior mass heat exchange coefficient

### `mass_temperature`

~~~
  double mass_temperature[degF];
~~~

Interior mass temperature

### `air_volume`

~~~
  double air_volume[cf];
~~~

Air volume

### `air_mass`

~~~
  double air_mass[lb];
~~~

Air mass

### `air_heat_capacity`

~~~
  double air_heat_capacity[Btu/degF];
~~~

Air thermal mass

### `latent_load_fraction`

~~~
  double latent_load_fraction[pu];
~~~

Fractional increase in cooling load due to latent heat

### `total_thermal_mass_per_floor_area`

~~~
  double total_thermal_mass_per_floor_area[Btu/degF/sf];
~~~

TODO

### `interior_surface_heat_transfer_coeff`

~~~
  double interior_surface_heat_transfer_coeff[Btu/h/degF/sf];
~~~

TODO

### `number_of_stories`

~~~
  double number_of_stories;
~~~

Number of stories within the structure

### `is_AUX_on`

~~~
  double is_AUX_on;
~~~

Logic statement to determine population statistics - is the AUX on? 0 no, 1 yes

### `is_HEAT_on`

~~~
  double is_HEAT_on;
~~~

Logic statement to determine population statistics - is the HEAT on? 0 no, 1 yes

### `is_COOL_on`

~~~
  double is_COOL_on;
~~~

Logic statement to determine population statistics - is the COOL on? 0 no, 1 yes

### `thermal_storage_present`

~~~
  double thermal_storage_present;
~~~

Logic statement for determining if energy storage is present

### `thermal_storage_in_use`

~~~
  double thermal_storage_in_use;
~~~

Logic statement for determining if energy storage is being utilized

### `thermostat_mode`

~~~
  enumeration {COOL, HEAT, AUTO, OFF} thermostat_mode;
~~~

Tells the thermostat whether it is even allowed to heat or cool the house.

### `system_type`

~~~
  set {RESISTIVE, TWOSTAGE, FORCEDAIR, AIRCONDITIONING, GAS, NONE} system_type;
~~~

Heating/cooling system type/options

### `auxiliary_strategy`

~~~
  set {LOCKOUT, TIMER, DEADBAND, NONE} auxiliary_strategy;
~~~

Auxiliary heating activation strategies

### `system_mode`

~~~
  enumeration {AUX, COOL, OFF, HEAT, UNKNOWN} system_mode;
~~~

Heating/cooling system operation state

### `last_system_mode`

~~~
  enumeration {AUX, COOL, OFF, HEAT, UNKNOWN} last_system_mode;
~~~

Heating/cooling system operation state

### `heating_system_type`

~~~
  enumeration {RESISTANCE, HEAT_PUMP, GAS, NONE} heating_system_type;
~~~

TODO

### `cooling_system_type`

~~~
  enumeration {HEAT_PUMP, ELECTRIC, NONE} cooling_system_type;
~~~

TODO

### `auxiliary_system_type`

~~~
  enumeration {ELECTRIC, NONE} auxiliary_system_type;
~~~

TODO

### `fan_type`

~~~
  enumeration {TWO_SPEED, ONE_SPEED, NONE} fan_type;
~~~

TODO

### `thermal_integrity_level`

~~~
  enumeration {UNKNOWN, VERY_GOOD, GOOD, ABOVE_NORMAL, NORMAL, BELOW_NORMAL, LITTLE, VERY_LITTLE} thermal_integrity_level;
~~~

Default envelope UA settings

### `glass_type`

~~~
  enumeration {LOW_E_GLASS, GLASS, OTHER} glass_type;
~~~

Glass used in the windows

### `window_frame`

~~~
  enumeration {INSULATED, WOOD, THERMAL_BREAK, ALUMINIUM, ALUMINUM, NONE} window_frame;
~~~

Type of window frame

### `glazing_treatment`

~~~
  enumeration {HIGH_S, LOW_S, REFL, ABS, CLEAR, OTHER} glazing_treatment;
~~~

The treatment to increase the reflectivity of the exterior windows

### `glazing_layers`

~~~
  enumeration {OTHER, THREE, TWO, ONE} glazing_layers;
~~~

Number of layers of glass in each window

### `motor_model`

~~~
  enumeration {FULL, BASIC, NONE} motor_model;
~~~

Indicates the level of detail used in modelling the hvac motor parameters

### `motor_efficiency`

~~~
  enumeration {VERY_GOOD, GOOD, AVERAGE, POOR, VERY_POOR} motor_efficiency;
~~~

When using motor model, describes the efficiency of the motor

### `last_mode_timer`

~~~
  int64 last_mode_timer;
~~~

TODO

### `hvac_motor_efficiency`

~~~
  double hvac_motor_efficiency[unit];
~~~

When using motor model, percent efficiency of hvac motor

### `hvac_motor_loss_power_factor`

~~~
  double hvac_motor_loss_power_factor[unit];
~~~

When using motor model, power factor of motor losses

### `Rroof`

~~~
  double Rroof[degF*sf*h/Btu];
~~~

Roof R-value

### `Rwall`

~~~
  double Rwall[degF*sf*h/Btu];
~~~

Wall R-value

### `Rfloor`

~~~
  double Rfloor[degF*sf*h/Btu];
~~~

Floor R-value

### `Rwindows`

~~~
  double Rwindows[degF*sf*h/Btu];
~~~

Window R-value

### `Rdoors`

~~~
  double Rdoors[degF*sf*h/Btu];
~~~

Door R-value

### `hvac_breaker_rating`

~~~
  double hvac_breaker_rating[A];
~~~

Determines the amount of current the HVAC circuit breaker can handle

### `hvac_power_factor`

~~~
  double hvac_power_factor[unit];
~~~

Power factor of hvac

### `hvac_load`

~~~
  double hvac_load[kW];
~~~

Heating/cooling system load

### `last_heating_load`

~~~
  double last_heating_load[kW];
~~~

Stores the previous heating/cooling system load

### `last_cooling_load`

~~~
  double last_cooling_load[kW];
~~~

Stores the previous heating/cooling system load

### `hvac_power`

~~~
  complex hvac_power[kVA];
~~~

Describes hvac load complex power consumption

### `total_load`

~~~
  double total_load[kW];
~~~

Total panel enduse load

### `panel`

~~~
  enduse panel;
~~~

The enduse load description

### `panel.energy`

~~~
  complex panel.energy[kVAh];
~~~

The total energy consumed since the last meter reading

### `panel.power`

~~~
  complex panel.power[kVA];
~~~

The total power consumption of the load

### `panel.peak_demand`

~~~
  complex panel.peak_demand[kVA];
~~~

The peak power consumption since the last meter reading

### `panel.heatgain`

~~~
  double panel.heatgain[Btu/h];
~~~

The heat transferred from the enduse to the parent

### `panel.cumulative_heatgain`

~~~
  double panel.cumulative_heatgain[Btu];
~~~

The cumulative heatgain from the enduse to the parent

### `panel.heatgain_fraction`

~~~
  double panel.heatgain_fraction[pu];
~~~

The fraction of the heat that goes to the parent

### `panel.current_fraction`

~~~
  double panel.current_fraction[pu];
~~~

The fraction of total power that is constant current

### `panel.impedance_fraction`

~~~
  double panel.impedance_fraction[pu];
~~~

The fraction of total power that is constant impedance

### `panel.power_fraction`

~~~
  double panel.power_fraction[pu];
~~~

The fraction of the total power that is constant power

### `panel.power_factor`

~~~
  double panel.power_factor;
~~~

The power factor of the load

### `panel.constant_power`

~~~
  complex panel.constant_power[kVA];
~~~

The constant power portion of the total load

### `panel.constant_current`

~~~
  complex panel.constant_current[kVA];
~~~

The constant current portion of the total load

### `panel.constant_admittance`

~~~
  complex panel.constant_admittance[kVA];
~~~

The constant admittance portion of the total load

### `panel.voltage_factor`

~~~
  double panel.voltage_factor[pu];
~~~

The voltage change factor

### `panel.breaker_amps`

~~~
  double panel.breaker_amps[A];
~~~

The rated breaker amperage

### `panel.configuration`

~~~
  set {IS220, IS110} panel.configuration;
~~~

The load configuration options

### `design_internal_gain_density`

~~~
  double design_internal_gain_density[W/sf];
~~~

Average density of heat generating devices in the house

### `compressor_on`

~~~
  bool compressor_on;
~~~

TODO

### `compressor_count`

~~~
  int64 compressor_count;
~~~

TODO

### `hvac_last_on`

~~~
  timestamp hvac_last_on;
~~~

TODO

### `hvac_last_off`

~~~
  timestamp hvac_last_off;
~~~

TODO

### `hvac_period_length`

~~~
  double hvac_period_length[s];
~~~

TODO

### `hvac_duty_cycle`

~~~
  double hvac_duty_cycle;
~~~

TODO

### `a`

~~~
  double a;
~~~

TODO

### `b`

~~~
  double b;
~~~

TODO

### `c`

~~~
  double c;
~~~

TODO

### `d`

~~~
  double d;
~~~

TODO

### `c1`

~~~
  double c1;
~~~

TODO

### `c2`

~~~
  double c2;
~~~

TODO

### `A3`

~~~
  double A3;
~~~

TODO

### `A4`

~~~
  double A4;
~~~

TODO

### `k1`

~~~
  double k1;
~~~

TODO

### `k2`

~~~
  double k2;
~~~

TODO

### `r1`

~~~
  double r1;
~~~

TODO

### `r2`

~~~
  double r2;
~~~

TODO

### `Teq`

~~~
  double Teq;
~~~

TODO

### `Tevent`

~~~
  double Tevent;
~~~

TODO

### `Qi`

~~~
  double Qi;
~~~

TODO

### `Qa`

~~~
  double Qa;
~~~

TODO

### `Qm`

~~~
  double Qm;
~~~

TODO

### `Qh`

~~~
  double Qh;
~~~

TODO

### `Qlatent`

~~~
  double Qlatent;
~~~

TODO

### `dTair`

~~~
  double dTair;
~~~

TODO

### `adj_cooling_cap`

~~~
  double adj_cooling_cap;
~~~

TODO

### `adj_heating_cap`

~~~
  double adj_heating_cap;
~~~

TODO

### `adj_cooling_cop`

~~~
  double adj_cooling_cop;
~~~

TODO

### `adj_heating_cop`

~~~
  double adj_heating_cop;
~~~

TODO

### `thermostat_control`

~~~
  enumeration {NONE, BAND, FULL} thermostat_control;
~~~

Determine level of internal thermostatic control

### `gas_enduses`

~~~
  char1024 gas_enduses;
~~~

List of implicit enduses that use gas instead of electricity

### `circuit`

~~~
  method circuit;
~~~

Smart breaker message handlers

### `shape`

~~~
  loadshape shape;
~~~

TODO

### `load`

~~~
  enduse load;
~~~

The enduse load description

### `energy`

~~~
  complex energy[kVAh];
~~~

The total energy consumed since the last meter reading

### `power`

~~~
  complex power[kVA];
~~~

The total power consumption of the load

### `peak_demand`

~~~
  complex peak_demand[kVA];
~~~

The peak power consumption since the last meter reading

### `heatgain`

~~~
  double heatgain[Btu/h];
~~~

The heat transferred from the enduse to the parent

### `cumulative_heatgain`

~~~
  double cumulative_heatgain[Btu];
~~~

The cumulative heatgain from the enduse to the parent

### `heatgain_fraction`

~~~
  double heatgain_fraction[pu];
~~~

The fraction of the heat that goes to the parent

### `current_fraction`

~~~
  double current_fraction[pu];
~~~

The fraction of total power that is constant current

### `impedance_fraction`

~~~
  double impedance_fraction[pu];
~~~

The fraction of total power that is constant impedance

### `power_fraction`

~~~
  double power_fraction[pu];
~~~

The fraction of the total power that is constant power

### `power_factor`

~~~
  double power_factor;
~~~

The power factor of the load

### `constant_power`

~~~
  complex constant_power[kVA];
~~~

The constant power portion of the total load

### `constant_current`

~~~
  complex constant_current[kVA];
~~~

The constant current portion of the total load

### `constant_admittance`

~~~
  complex constant_admittance[kVA];
~~~

The constant admittance portion of the total load

### `voltage_factor`

~~~
  double voltage_factor[pu];
~~~

The voltage change factor

### `breaker_amps`

~~~
  double breaker_amps[A];
~~~

The rated breaker amperage

### `configuration`

~~~
  set {IS220, IS110} configuration;
~~~

The load configuration options

### `override`

~~~
  enumeration {OFF, ON, NORMAL} override;
~~~

TODO

### `power_state`

~~~
  enumeration {UNKNOWN, ON, OFF} power_state;
~~~

TODO

# Example

~~~
  object house {
    floor_area "0.0";
    gross_wall_area "0.0";
    ceiling_height "0.0";
    aspect_ratio "0.0";
    envelope_UA "0.0";
    window_wall_ratio "0.0";
    number_of_doors "0.0";
    exterior_wall_fraction "0.0";
    interior_exterior_wall_ratio "0.0";
    exterior_ceiling_fraction "0.0";
    exterior_floor_fraction "0.0";
    window_shading "0.0";
    window_exterior_transmission_coefficient "0.0";
    solar_heatgain_factor "0.0";
    airchange_per_hour "0.0";
    airchange_UA "0.0";
    UA "0.0";
    internal_gain "0.0";
    solar_gain "0.0";
    incident_solar_radiation "0.0";
    heat_cool_gain "0.0";
    include_solar_quadrant "0";
    horizontal_diffuse_solar_radiation "0.0";
    north_incident_solar_radiation "0.0";
    northwest_incident_solar_radiation "0.0";
    west_incident_solar_radiation "0.0";
    southwest_incident_solar_radiation "0.0";
    south_incident_solar_radiation "0.0";
    southeast_incident_solar_radiation "0.0";
    east_incident_solar_radiation "0.0";
    northeast_incident_solar_radiation "0.0";
    heating_cop_curve "0";
    heating_cap_curve "0";
    cooling_cop_curve "0";
    cooling_cap_curve "0";
    use_latent_heat "FALSE";
    include_fan_heatgain "FALSE";
    thermostat_deadband "0.0";
    dlc_offset "0.0";
    thermostat_cycle_time "0";
    thermostat_off_cycle_time "0";
    thermostat_on_cycle_time "0";
    thermostat_last_cycle_time "TS_ZERO";
    heating_setpoint "0.0";
    cooling_setpoint "0.0";
    design_heating_setpoint "0.0";
    design_cooling_setpoint "0.0";
    over_sizing_factor "0.0";
    simulate_window_openings "FALSE";
    is_window_open "0.0";
    window_low_temperature_cutoff "0.0";
    window_high_temperature_cutoff "0.0";
    window_quadratic_coefficient "0.0";
    window_linear_coefficient "0.0";
    window_constant_coefficient "0.0";
    window_temperature_delta "0.0";
    design_heating_capacity "0.0";
    design_cooling_capacity "0.0";
    cooling_design_temperature "0.0";
    heating_design_temperature "0.0";
    design_peak_solar "0.0";
    design_internal_gains "0.0";
    air_heat_fraction "0.0";
    mass_solar_gain_fraction "0.0";
    mass_internal_gain_fraction "0.0";
    auxiliary_heat_capacity "0.0";
    aux_heat_deadband "0.0";
    aux_heat_temperature_lockout "0.0";
    aux_heat_time_delay "0.0";
    cooling_supply_air_temp "0.0";
    heating_supply_air_temp "0.0";
    duct_pressure_drop "0.0";
    fan_design_power "0.0";
    fan_low_power_fraction "0.0";
    fan_power "0.0";
    fan_design_airflow "0.0";
    fan_impedance_fraction "0.0";
    fan_power_fraction "0.0";
    fan_current_fraction "0.0";
    fan_power_factor "0.0";
    heating_demand "0.0";
    cooling_demand "0.0";
    heating_COP "0.0";
    cooling_COP "0.0";
    air_temperature "0.0";
    outdoor_temperature "0.0";
    outdoor_rh "0.0";
    mass_heat_capacity "0.0";
    mass_heat_coeff "0.0";
    mass_temperature "0.0";
    air_volume "0.0";
    air_mass "0.0";
    air_heat_capacity "0.0";
    latent_load_fraction "0.0";
    total_thermal_mass_per_floor_area "0.0";
    interior_surface_heat_transfer_coeff "0.0";
    number_of_stories "0.0";
    is_AUX_on "0.0";
    is_HEAT_on "0.0";
    is_COOL_on "0.0";
    thermal_storage_present "0.0";
    thermal_storage_in_use "0.0";
    thermostat_mode "0";
    system_type "0";
    auxiliary_strategy "0";
    system_mode "0";
    last_system_mode "0";
    heating_system_type "0";
    cooling_system_type "0";
    auxiliary_system_type "0";
    fan_type "0";
    thermal_integrity_level "0";
    glass_type "0";
    window_frame "0";
    glazing_treatment "0";
    glazing_layers "0";
    motor_model "0";
    motor_efficiency "0";
    last_mode_timer "0";
    hvac_motor_efficiency "0.0";
    hvac_motor_loss_power_factor "0.0";
    Rroof "0.0";
    Rwall "0.0";
    Rfloor "0.0";
    Rwindows "0.0";
    Rdoors "0.0";
    hvac_breaker_rating "0.0";
    hvac_power_factor "0.0";
    hvac_load "0.0";
    last_heating_load "0.0";
    last_cooling_load "0.0";
    hvac_power "0+0i";
    total_load "0.0";
    design_internal_gain_density "0.0";
    compressor_on "FALSE";
    compressor_count "0";
    hvac_last_on "TS_ZERO";
    hvac_last_off "TS_ZERO";
    hvac_period_length "0.0";
    hvac_duty_cycle "0.0";
    a "0.0";
    b "0.0";
    c "0.0";
    d "0.0";
    c1 "0.0";
    c2 "0.0";
    A3 "0.0";
    A4 "0.0";
    k1 "0.0";
    k2 "0.0";
    r1 "0.0";
    r2 "0.0";
    Teq "0.0";
    Tevent "0.0";
    Qi "0.0";
    Qa "0.0";
    Qm "0.0";
    Qh "0.0";
    Qlatent "0.0";
    dTair "0.0";
    adj_cooling_cap "0.0";
    adj_heating_cap "0.0";
    adj_cooling_cop "0.0";
    adj_heating_cop "0.0";
    thermostat_control "0";
    gas_enduses "";
    override "0";
    power_state "0";
  }
~~~

# See also

* [[/Module/Residential]]
* [ISU Notes on House Model](http://www2.econ.iastate.edu/tesfatsi/GLDETPHouseholdModel.Notes.LTesfatsionSBattula.pdf)

