[[/Module/Powerflow/Meter]] -- Class meter

# Synopsis

GLM:

~~~
  object meter {
    measured_real_energy "<decimal> Wh";
    measured_real_energy_delta "<decimal> Wh";
    measured_reactive_energy "<decimal> VAh";
    measured_reactive_energy_delta "<decimal> VAh";
    measured_energy_delta_timestep "<decimal> s";
    measured_power "<string> VA";
    measured_power_A "<string> VA";
    measured_power_B "<string> VA";
    measured_power_C "<string> VA";
    measured_demand "<decimal> W";
    measured_real_power "<decimal> W";
    measured_reactive_power "<decimal> VAr";
    meter_power_consumption "<string> VA";
    measured_voltage_A "<string> V";
    measured_voltage_B "<string> V";
    measured_voltage_C "<string> V";
    measured_voltage_AB "<string> V";
    measured_voltage_BC "<string> V";
    measured_voltage_CA "<string> V";
    measured_real_max_voltage_in_interval_A "<decimal>";
    measured_real_max_voltage_in_interval_B "<decimal>";
    measured_real_max_voltage_in_interval_C "<decimal>";
    measured_reactive_max_voltage_in_interval_A "<decimal>";
    measured_reactive_max_voltage_in_interval_B "<decimal>";
    measured_reactive_max_voltage_in_interval_C "<decimal>";
    measured_real_max_voltage_in_interval_AB "<decimal>";
    measured_real_max_voltage_in_interval_BC "<decimal>";
    measured_real_max_voltage_in_interval_CA "<decimal>";
    measured_reactive_max_voltage_in_interval_AB "<decimal>";
    measured_reactive_max_voltage_in_interval_BC "<decimal>";
    measured_reactive_max_voltage_in_interval_CA "<decimal>";
    measured_real_min_voltage_in_interval_A "<decimal>";
    measured_real_min_voltage_in_interval_B "<decimal>";
    measured_real_min_voltage_in_interval_C "<decimal>";
    measured_reactive_min_voltage_in_interval_A "<decimal>";
    measured_reactive_min_voltage_in_interval_B "<decimal>";
    measured_reactive_min_voltage_in_interval_C "<decimal>";
    measured_real_min_voltage_in_interval_AB "<decimal>";
    measured_real_min_voltage_in_interval_BC "<decimal>";
    measured_real_min_voltage_in_interval_CA "<decimal>";
    measured_reactive_min_voltage_in_interval_AB "<decimal>";
    measured_reactive_min_voltage_in_interval_BC "<decimal>";
    measured_reactive_min_voltage_in_interval_CA "<decimal>";
    measured_avg_voltage_mag_in_interval_A "<decimal>";
    measured_avg_voltage_mag_in_interval_B "<decimal>";
    measured_avg_voltage_mag_in_interval_C "<decimal>";
    measured_avg_voltage_mag_in_interval_AB "<decimal>";
    measured_avg_voltage_mag_in_interval_BC "<decimal>";
    measured_avg_voltage_mag_in_interval_CA "<decimal>";
    measured_current_A "<string> A";
    measured_current_B "<string> A";
    measured_current_C "<string> A";
    customer_interrupted "<string>";
    customer_interrupted_secondary "<string>";
    monthly_bill "<decimal> $";
    previous_monthly_bill "<decimal> $";
    previous_monthly_energy "<decimal> kWh";
    monthly_fee "<decimal> $";
    monthly_energy "<decimal> kWh";
    bill_mode "{TIERED_RTP,HOURLY,TIERED,UNIFORM,NONE}";
    power_market "<string>";
    bill_day "<integer>";
    price "<decimal> $/kWh";
    price_base "<decimal> $/kWh";
    first_tier_price "<decimal> $/kWh";
    first_tier_energy "<decimal> kWh";
    second_tier_price "<decimal> $/kWh";
    second_tier_energy "<decimal> kWh";
    third_tier_price "<decimal> $/kWh";
    third_tier_energy "<decimal> kWh";
    service_level "{LOW, NORMAL, HIGH, CRITICAL}";
    bustype "{SWING_PQ,SWING,PV,PQ}";
    busflags "[ISSOURCE|HASSOURCE]";
    reference_bus "<string>";
    maximum_voltage_error "<decimal> V";
    voltage_A "<string> V";
    voltage_B "<string> V";
    voltage_C "<string> V";
    voltage_AB "<string> V";
    voltage_BC "<string> V";
    voltage_CA "<string> V";
    current_A "<string> A";
    current_B "<string> A";
    current_C "<string> A";
    power_A "<string> VA";
    power_B "<string> VA";
    power_C "<string> VA";
    shunt_A "<string> S";
    shunt_B "<string> S";
    shunt_C "<string> S";
    prerotated_current_A "<string> A";
    prerotated_current_B "<string> A";
    prerotated_current_C "<string> A";
    current_AB "<string> A";
    current_BC "<string> A";
    current_CA "<string> A";
    current_AN "<string> A";
    current_BN "<string> A";
    current_CN "<string> A";
    power_AB "<string> VA";
    power_BC "<string> VA";
    power_CA "<string> VA";
    power_AN "<string> VA";
    power_BN "<string> VA";
    power_CN "<string> VA";
    shunt_AB "<string> S";
    shunt_BC "<string> S";
    shunt_CA "<string> S";
    shunt_AN "<string> S";
    shunt_BN "<string> S";
    shunt_CN "<string> S";
    mean_repair_time "<decimal> s";
    frequency_measure_type "{PLL,SIMPLE,NONE}";
    sfm_Tf "<decimal> s";
    pll_Kp "<decimal> pu";
    pll_Ki "<decimal> pu";
    measured_angle_A "<decimal> rad";
    measured_frequency_A "<decimal> Hz";
    measured_angle_B "<decimal> rad";
    measured_frequency_B "<decimal> Hz";
    measured_angle_C "<decimal> rad";
    measured_frequency_C "<decimal> Hz";
    measured_frequency "<decimal> Hz";
    service_status "{OUT_OF_SERVICE,IN_SERVICE}";
    service_status_double "<decimal>";
    previous_uptime "<decimal> min";
    current_uptime "<decimal> min";
    Norton_dynamic "<string>";
    generator_dynamic "<string>";
    GFA_enable "<string>";
    GFA_freq_low_trip "<decimal> Hz";
    GFA_freq_high_trip "<decimal> Hz";
    GFA_volt_low_trip "<decimal> pu";
    GFA_volt_high_trip "<decimal> pu";
    GFA_reconnect_time "<decimal> s";
    GFA_freq_disconnect_time "<decimal> s";
    GFA_volt_disconnect_time "<decimal> s";
    GFA_status "<string>";
    GFA_trip_method "{OVER_VOLTAGE,UNDER_VOLTAGE,OVER_FREQUENCY,UNDER_FREQUENCY,NONE}";
    topological_parent "<string>";
    phases "[ABCDNSG]";
    nominal_voltage "<decimal> V";
    service_level "{LOW,NORMAL,HIGH,CRITICAL}";
  }
~~~

# Description

TODO

## Properties

### `measured_real_energy`

~~~
  double measured_real_energy[Wh];
~~~

Metered real energy consumption, cummalitive

### `measured_real_energy_delta`

~~~
  double measured_real_energy_delta[Wh];
~~~

Delta in metered real energy consumption from last specified measured_energy_delta_timestep

### `measured_reactive_energy`

~~~
  double measured_reactive_energy[VAh];
~~~

Metered reactive energy consumption, cummalitive

### `measured_reactive_energy_delta`

~~~
  double measured_reactive_energy_delta[VAh];
~~~

Delta in metered reactive energy consumption from last specified measured_energy_delta_timestep

### `measured_energy_delta_timestep`

~~~
  double measured_energy_delta_timestep[s];
~~~

Period of timestep for real and reactive delta energy calculation

### `measured_power`

~~~
  complex measured_power[VA];
~~~

Metered complex power

### `measured_power_A`

~~~
  complex measured_power_A[VA];
~~~

Metered complex power on phase A

### `measured_power_B`

~~~
  complex measured_power_B[VA];
~~~

Metered complex power on phase B

### `measured_power_C`

~~~
  complex measured_power_C[VA];
~~~

Metered complex power on phase C

### `measured_demand`

~~~
  double measured_demand[W];
~~~

Greatest metered real power during simulation

### `measured_real_power`

~~~
  double measured_real_power[W];
~~~

Metered real power

### `measured_reactive_power`

~~~
  double measured_reactive_power[VAr];
~~~

Metered reactive power

### `meter_power_consumption`

~~~
  complex meter_power_consumption[VA];
~~~

Metered power used for operating the meter; standby and communication losses

### `measured_voltage_A`

~~~
  complex measured_voltage_A[V];
~~~

Measured line-to-neutral voltage on phase A

### `measured_voltage_B`

~~~
  complex measured_voltage_B[V];
~~~

Measured line-to-neutral voltage on phase B

### `measured_voltage_C`

~~~
  complex measured_voltage_C[V];
~~~

Measured line-to-neutral voltage on phase C

### `measured_voltage_AB`

~~~
  complex measured_voltage_AB[V];
~~~

Measured line-to-line voltage on phase AB

### `measured_voltage_BC`

~~~
  complex measured_voltage_BC[V];
~~~

Measured line-to-line voltage on phase BC

### `measured_voltage_CA`

~~~
  complex measured_voltage_CA[V];
~~~

Measured line-to-line voltage on phase CA

### `measured_real_max_voltage_in_interval_A`

~~~
  double measured_real_max_voltage_in_interval_A;
~~~

Measured real max line-to-neutral voltage on phase A over a specified interval

### `measured_real_max_voltage_in_interval_B`

~~~
  double measured_real_max_voltage_in_interval_B;
~~~

Measured real max line-to-neutral voltage on phase B over a specified interval

### `measured_real_max_voltage_in_interval_C`

~~~
  double measured_real_max_voltage_in_interval_C;
~~~

Measured real max line-to-neutral voltage on phase C over a specified interval

### `measured_reactive_max_voltage_in_interval_A`

~~~
  double measured_reactive_max_voltage_in_interval_A;
~~~

Measured reactive max line-to-neutral voltage on phase A over a specified interval

### `measured_reactive_max_voltage_in_interval_B`

~~~
  double measured_reactive_max_voltage_in_interval_B;
~~~

Measured reactive max line-to-neutral voltage on phase B over a specified interval

### `measured_reactive_max_voltage_in_interval_C`

~~~
  double measured_reactive_max_voltage_in_interval_C;
~~~

Measured reactive max line-to-neutral voltage on phase C over a specified interval

### `measured_real_max_voltage_in_interval_AB`

~~~
  double measured_real_max_voltage_in_interval_AB;
~~~

Measured real max line-to-line voltage on phase A over a specified interval

### `measured_real_max_voltage_in_interval_BC`

~~~
  double measured_real_max_voltage_in_interval_BC;
~~~

Measured real max line-to-line voltage on phase B over a specified interval

### `measured_real_max_voltage_in_interval_CA`

~~~
  double measured_real_max_voltage_in_interval_CA;
~~~

Measured real max line-to-line voltage on phase C over a specified interval

### `measured_reactive_max_voltage_in_interval_AB`

~~~
  double measured_reactive_max_voltage_in_interval_AB;
~~~

Measured reactive max line-to-line voltage on phase A over a specified interval

### `measured_reactive_max_voltage_in_interval_BC`

~~~
  double measured_reactive_max_voltage_in_interval_BC;
~~~

Measured reactive max line-to-line voltage on phase B over a specified interval

### `measured_reactive_max_voltage_in_interval_CA`

~~~
  double measured_reactive_max_voltage_in_interval_CA;
~~~

Measured reactive max line-to-line voltage on phase C over a specified interval

### `measured_real_min_voltage_in_interval_A`

~~~
  double measured_real_min_voltage_in_interval_A;
~~~

Measured real min line-to-neutral voltage on phase A over a specified interval

### `measured_real_min_voltage_in_interval_B`

~~~
  double measured_real_min_voltage_in_interval_B;
~~~

Measured real min line-to-neutral voltage on phase B over a specified interval

### `measured_real_min_voltage_in_interval_C`

~~~
  double measured_real_min_voltage_in_interval_C;
~~~

Measured real min line-to-neutral voltage on phase C over a specified interval

### `measured_reactive_min_voltage_in_interval_A`

~~~
  double measured_reactive_min_voltage_in_interval_A;
~~~

Measured reactive min line-to-neutral voltage on phase A over a specified interval

### `measured_reactive_min_voltage_in_interval_B`

~~~
  double measured_reactive_min_voltage_in_interval_B;
~~~

Measured reactive min line-to-neutral voltage on phase B over a specified interval

### `measured_reactive_min_voltage_in_interval_C`

~~~
  double measured_reactive_min_voltage_in_interval_C;
~~~

Measured reactive min line-to-neutral voltage on phase C over a specified interval

### `measured_real_min_voltage_in_interval_AB`

~~~
  double measured_real_min_voltage_in_interval_AB;
~~~

Measured real min line-to-line voltage on phase A over a specified interval

### `measured_real_min_voltage_in_interval_BC`

~~~
  double measured_real_min_voltage_in_interval_BC;
~~~

Measured real min line-to-line voltage on phase B over a specified interval

### `measured_real_min_voltage_in_interval_CA`

~~~
  double measured_real_min_voltage_in_interval_CA;
~~~

Measured real min line-to-line voltage on phase C over a specified interval

### `measured_reactive_min_voltage_in_interval_AB`

~~~
  double measured_reactive_min_voltage_in_interval_AB;
~~~

Measured reactive min line-to-line voltage on phase A over a specified interval

### `measured_reactive_min_voltage_in_interval_BC`

~~~
  double measured_reactive_min_voltage_in_interval_BC;
~~~

Measured reactive min line-to-line voltage on phase B over a specified interval

### `measured_reactive_min_voltage_in_interval_CA`

~~~
  double measured_reactive_min_voltage_in_interval_CA;
~~~

Measured reactive min line-to-line voltage on phase C over a specified interval

### `measured_avg_voltage_mag_in_interval_A`

~~~
  double measured_avg_voltage_mag_in_interval_A;
~~~

Measured avg line-to-neutral voltage magnitude on phase A over a specified interval

### `measured_avg_voltage_mag_in_interval_B`

~~~
  double measured_avg_voltage_mag_in_interval_B;
~~~

Measured avg line-to-neutral voltage magnitude on phase B over a specified interval

### `measured_avg_voltage_mag_in_interval_C`

~~~
  double measured_avg_voltage_mag_in_interval_C;
~~~

Measured avg line-to-neutral voltage magnitude on phase C over a specified interval

### `measured_avg_voltage_mag_in_interval_AB`

~~~
  double measured_avg_voltage_mag_in_interval_AB;
~~~

Measured avg line-to-line voltage magnitude on phase A over a specified interval

### `measured_avg_voltage_mag_in_interval_BC`

~~~
  double measured_avg_voltage_mag_in_interval_BC;
~~~

Measured avg line-to-line voltage magnitude on phase B over a specified interval

### `measured_avg_voltage_mag_in_interval_CA`

~~~
  double measured_avg_voltage_mag_in_interval_CA;
~~~

Measured avg line-to-line voltage magnitude on phase C over a specified interval

### `measured_current_A`

~~~
  complex measured_current_A[A];
~~~

Measured current on phase A

### `measured_current_B`

~~~
  complex measured_current_B[A];
~~~

Measured current on phase B

### `measured_current_C`

~~~
  complex measured_current_C[A];
~~~

Measured current on phase C

### `customer_interrupted`

~~~
  bool customer_interrupted;
~~~

Reliability flag - goes active if the customer is in an 'interrupted' state

### `customer_interrupted_secondary`

~~~
  bool customer_interrupted_secondary;
~~~

Reliability flag - goes active if the customer is in an 'secondary interrupted' state - i.e., momentary

### `monthly_bill`

~~~
  double monthly_bill[$];
~~~

Accumulator for the current month's bill

### `previous_monthly_bill`

~~~
  double previous_monthly_bill[$];
~~~

Total monthly bill for the previous month

### `previous_monthly_energy`

~~~
  double previous_monthly_energy[kWh];
~~~

Total monthly energy for the previous month

### `monthly_fee`

~~~
  double monthly_fee[$];
~~~

Once a month flat fee for customer hook-up

### `monthly_energy`

~~~
  double monthly_energy[kWh];
~~~

Accumulator for the current month's energy consumption

### `bill_mode`

~~~
  enumeration {TIERED_RTP, HOURLY, TIERED, UNIFORM, NONE} bill_mode;
~~~

Billing structure desired

### `power_market`

~~~
  object power_market;
~~~

Market (auction object) where the price is being received from

### `bill_day`

~~~
  int32 bill_day;
~~~

Day of month bill is to be processed (currently limited to days 1-28)

### `price`

~~~
  double price[$/kWh];
~~~

Current price of electricity

### `price_base`

~~~
  double price_base[$/kWh];
~~~

Used only in TIERED_RTP mode to describe the price before the first tier

### `first_tier_price`

~~~
  double first_tier_price[$/kWh];
~~~

Price of electricity between first tier and second tier energy usage

### `first_tier_energy`

~~~
  double first_tier_energy[kWh];
~~~

Switching point between base price and first tier price

### `second_tier_price`

~~~
  double second_tier_price[$/kWh];
~~~

Price of electricity between second tier and third tier energy usage

### `second_tier_energy`

~~~
  double second_tier_energy[kWh];
~~~

Switching point between first tier price and second tier price

### `third_tier_price`

~~~
  double third_tier_price[$/kWh];
~~~

Price of electricity when energy usage exceeds third tier energy usage

### `third_tier_energy`

~~~
  double third_tier_energy[kWh];
~~~

Switching point between second tier price and third tier price

### `service_level` 

~~~
  enumeration {LOW, NORMAL, HIGH, CRITICAL} service_level;
~~~

Defines the criticality of the associated load

### `bustype`

~~~
  enumeration {SWING_PQ, SWING, PV, PQ} bustype;
~~~

Defines whether the node is a PQ, PV, or SWING node

### `busflags`

~~~
  set {ISSOURCE, HASSOURCE} busflags;
~~~

Flag indicates node has a source for voltage, i.e. connects to the swing node

### `reference_bus`

~~~
  object reference_bus;
~~~

Reference bus from which frequency is defined

### `maximum_voltage_error`

~~~
  double maximum_voltage_error[V];
~~~

Convergence voltage limit or convergence criteria

### `voltage_A`

~~~
  complex voltage_A[V];
~~~

Bus voltage, Phase A to ground

### `voltage_B`

~~~
  complex voltage_B[V];
~~~

Bus voltage, Phase B to ground

### `voltage_C`

~~~
  complex voltage_C[V];
~~~

Bus voltage, Phase C to ground

### `voltage_AB`

~~~
  complex voltage_AB[V];
~~~

Line voltages, Phase AB

### `voltage_BC`

~~~
  complex voltage_BC[V];
~~~

Line voltages, Phase BC

### `voltage_CA`

~~~
  complex voltage_CA[V];
~~~

Line voltages, Phase CA

### `current_A`

~~~
  complex current_A[A];
~~~

Bus current injection (in = positive), this an accumulator only, not a output or input variable

### `current_B`

~~~
  complex current_B[A];
~~~

Bus current injection (in = positive), this an accumulator only, not a output or input variable

### `current_C`

~~~
  complex current_C[A];
~~~

Bus current injection (in = positive), this an accumulator only, not a output or input variable

### `power_A`

~~~
  complex power_A[VA];
~~~

Bus power injection (in = positive), this an accumulator only, not a output or input variable

### `power_B`

~~~
  complex power_B[VA];
~~~

Bus power injection (in = positive), this an accumulator only, not a output or input variable

### `power_C`

~~~
  complex power_C[VA];
~~~

Bus power injection (in = positive), this an accumulator only, not a output or input variable

### `shunt_A`

~~~
  complex shunt_A[S];
~~~

Bus shunt admittance, this an accumulator only, not a output or input variable

### `shunt_B`

~~~
  complex shunt_B[S];
~~~

Bus shunt admittance, this an accumulator only, not a output or input variable

### `shunt_C`

~~~
  complex shunt_C[S];
~~~

Bus shunt admittance, this an accumulator only, not a output or input variable

### `prerotated_current_A`

~~~
  complex prerotated_current_A[A];
~~~

Deltamode-functionality - bus current injection (in = positive), but will not be rotated by powerflow for off-nominal frequency, this an accumulator only, not a output or input variable

### `prerotated_current_B`

~~~
  complex prerotated_current_B[A];
~~~

Deltamode-functionality - bus current injection (in = positive), but will not be rotated by powerflow for off-nominal frequency, this an accumulator only, not a output or input variable

### `prerotated_current_C`

~~~
  complex prerotated_current_C[A];
~~~

Deltamode-functionality - bus current injection (in = positive), but will not be rotated by powerflow for off-nominal frequency, this an accumulator only, not a output or input variable

### `current_AB`

~~~
  complex current_AB[A];
~~~

Bus current delta-connected injection (in = positive), this an accumulator only, not a output or input variable

### `current_BC`

~~~
  complex current_BC[A];
~~~

Bus current delta-connected injection (in = positive), this an accumulator only, not a output or input variable

### `current_CA`

~~~
  complex current_CA[A];
~~~

Bus current delta-connected injection (in = positive), this an accumulator only, not a output or input variable

### `current_AN`

~~~
  complex current_AN[A];
~~~

Bus current wye-connected injection (in = positive), this an accumulator only, not a output or input variable

### `current_BN`

~~~
  complex current_BN[A];
~~~

Bus current wye-connected injection (in = positive), this an accumulator only, not a output or input variable

### `current_CN`

~~~
  complex current_CN[A];
~~~

Bus current wye-connected injection (in = positive), this an accumulator only, not a output or input variable

### `power_AB`

~~~
  complex power_AB[VA];
~~~

Bus power delta-connected injection (in = positive), this an accumulator only, not a output or input variable

### `power_BC`

~~~
  complex power_BC[VA];
~~~

Bus power delta-connected injection (in = positive), this an accumulator only, not a output or input variable

### `power_CA`

~~~
  complex power_CA[VA];
~~~

Bus power delta-connected injection (in = positive), this an accumulator only, not a output or input variable

### `power_AN`

~~~
  complex power_AN[VA];
~~~

Bus power wye-connected injection (in = positive), this an accumulator only, not a output or input variable

### `power_BN`

~~~
  complex power_BN[VA];
~~~

Bus power wye-connected injection (in = positive), this an accumulator only, not a output or input variable

### `power_CN`

~~~
  complex power_CN[VA];
~~~

Bus power wye-connected injection (in = positive), this an accumulator only, not a output or input variable

### `shunt_AB`

~~~
  complex shunt_AB[S];
~~~

Bus shunt delta-connected admittance, this an accumulator only, not a output or input variable

### `shunt_BC`

~~~
  complex shunt_BC[S];
~~~

Bus shunt delta-connected admittance, this an accumulator only, not a output or input variable

### `shunt_CA`

~~~
  complex shunt_CA[S];
~~~

Bus shunt delta-connected admittance, this an accumulator only, not a output or input variable

### `shunt_AN`

~~~
  complex shunt_AN[S];
~~~

Bus shunt wye-connected admittance, this an accumulator only, not a output or input variable

### `shunt_BN`

~~~
  complex shunt_BN[S];
~~~

Bus shunt wye-connected admittance, this an accumulator only, not a output or input variable

### `shunt_CN`

~~~
  complex shunt_CN[S];
~~~

Bus shunt wye-connected admittance, this an accumulator only, not a output or input variable

### `mean_repair_time`

~~~
  double mean_repair_time[s];
~~~

Time after a fault clears for the object to be back in service

### `frequency_measure_type`

~~~
  enumeration {PLL, SIMPLE, NONE} frequency_measure_type;
~~~

PLL frequency measurement

### `sfm_Tf`

~~~
  double sfm_Tf[s];
~~~

Transducer time constant for simplified frequency measurement (seconds)

### `pll_Kp`

~~~
  double pll_Kp[pu];
~~~

Proportional gain of PLL frequency measurement

### `pll_Ki`

~~~
  double pll_Ki[pu];
~~~

Integration gain of PLL frequency measurement

### `measured_angle_A`

~~~
  double measured_angle_A[rad];
~~~

Bus angle measurement, phase A

### `measured_frequency_A`

~~~
  double measured_frequency_A[Hz];
~~~

Frequency measurement, phase A

### `measured_angle_B`

~~~
  double measured_angle_B[rad];
~~~

Bus angle measurement, phase B

### `measured_frequency_B`

~~~
  double measured_frequency_B[Hz];
~~~

Frequency measurement, phase B

### `measured_angle_C`

~~~
  double measured_angle_C[rad];
~~~

Bus angle measurement, phase C

### `measured_frequency_C`

~~~
  double measured_frequency_C[Hz];
~~~

Frequency measurement, phase C

### `measured_frequency`

~~~
  double measured_frequency[Hz];
~~~

Frequency measurement - average of present phases

### `service_status`

~~~
  enumeration {OUT_OF_SERVICE, IN_SERVICE} service_status;
~~~

In and out of service flag

### `service_status_double`

~~~
  double service_status_double;
~~~

In and out of service flag - type double - will indiscriminately override service_status - useful for schedules

### `previous_uptime`

~~~
  double previous_uptime[min];
~~~

Previous time between disconnects of node in minutes

### `current_uptime`

~~~
  double current_uptime[min];
~~~

Current time since last disconnect of node in minutes

### `Norton_dynamic`

~~~
  bool Norton_dynamic;
~~~

Flag to indicate a Norton-equivalent connection -- used for generators and deltamode

### `generator_dynamic`

~~~
  bool generator_dynamic;
~~~

Flag to indicate a voltage-sourcing or swing-type generator is present -- used for generators and deltamode

### `GFA_enable`

~~~
  bool GFA_enable;
~~~

Disable/Enable Grid Friendly Appliance(TM)-type functionality

### `GFA_freq_low_trip`

~~~
  double GFA_freq_low_trip[Hz];
~~~

Low frequency trip point for Grid Friendly Appliance(TM)-type functionality

### `GFA_freq_high_trip`

~~~
  double GFA_freq_high_trip[Hz];
~~~

High frequency trip point for Grid Friendly Appliance(TM)-type functionality

### `GFA_volt_low_trip`

~~~
  double GFA_volt_low_trip[pu];
~~~

Low voltage trip point for Grid Friendly Appliance(TM)-type functionality

### `GFA_volt_high_trip`

~~~
  double GFA_volt_high_trip[pu];
~~~

High voltage trip point for Grid Friendly Appliance(TM)-type functionality

### `GFA_reconnect_time`

~~~
  double GFA_reconnect_time[s];
~~~

Reconnect time for Grid Friendly Appliance(TM)-type functionality

### `GFA_freq_disconnect_time`

~~~
  double GFA_freq_disconnect_time[s];
~~~

Frequency violation disconnect time for Grid Friendly Appliance(TM)-type functionality

### `GFA_volt_disconnect_time`

~~~
  double GFA_volt_disconnect_time[s];
~~~

Voltage violation disconnect time for Grid Friendly Appliance(TM)-type functionality

### `GFA_status`

~~~
  bool GFA_status;
~~~

Low frequency trip point for Grid Friendly Appliance(TM)-type functionality

### `GFA_trip_method`

~~~
  enumeration {OVER_VOLTAGE, UNDER_VOLTAGE, OVER_FREQUENCY, UNDER_FREQUENCY, NONE} GFA_trip_method;
~~~

GFA trip for over-voltage

### `topological_parent`

~~~
  object topological_parent;
~~~

Topological parent as per GLM configuration

### `phases`

~~~
  set {A, B, C, D, N, S, G} phases;
~~~

TODO

### `nominal_voltage`

~~~
  double nominal_voltage[V];
~~~

TODO

### `service_level`

~~~
  enumeration {LOW,NORMAL,HIGH,CRITICAL} service_level;
~~~

This enumeration indicates whether the meter serves a facility requires a particular level of service reliability.  `LOW` should be used for customers that have relatively unlimited self-generation backup. `NORMAL` should be used for customers that have typical service reliability (i.e., no backup).  `HIGH` should be used for customer that need higher than normal reliability but do not provide critical life-support services or have sufficient backup to operate up to 48 hours on backup supply.  `CRITICAL` should be used for customers that need high than normal reliability, provide critical life-support services, and do not have sufficient backup resources to operate 48 hours on backup supply.  This is used by the resilience module to determine whether PSPS must consider the services as must-run/must-serve.

# Example

~~~
  object meter {
    measured_real_energy "0.0";
    measured_real_energy_delta "0.0";
    measured_reactive_energy "0.0";
    measured_reactive_energy_delta "0.0";
    measured_energy_delta_timestep "0.0";
    measured_power "0+0i";
    measured_power_A "0+0i";
    measured_power_B "0+0i";
    measured_power_C "0+0i";
    measured_demand "0.0";
    measured_real_power "0.0";
    measured_reactive_power "0.0";
    meter_power_consumption "0+0i";
    measured_voltage_A "0+0i";
    measured_voltage_B "0+0i";
    measured_voltage_C "0+0i";
    measured_voltage_AB "0+0i";
    measured_voltage_BC "0+0i";
    measured_voltage_CA "0+0i";
    measured_real_max_voltage_in_interval_A "0.0";
    measured_real_max_voltage_in_interval_B "0.0";
    measured_real_max_voltage_in_interval_C "0.0";
    measured_reactive_max_voltage_in_interval_A "0.0";
    measured_reactive_max_voltage_in_interval_B "0.0";
    measured_reactive_max_voltage_in_interval_C "0.0";
    measured_real_max_voltage_in_interval_AB "0.0";
    measured_real_max_voltage_in_interval_BC "0.0";
    measured_real_max_voltage_in_interval_CA "0.0";
    measured_reactive_max_voltage_in_interval_AB "0.0";
    measured_reactive_max_voltage_in_interval_BC "0.0";
    measured_reactive_max_voltage_in_interval_CA "0.0";
    measured_real_min_voltage_in_interval_A "0.0";
    measured_real_min_voltage_in_interval_B "0.0";
    measured_real_min_voltage_in_interval_C "0.0";
    measured_reactive_min_voltage_in_interval_A "0.0";
    measured_reactive_min_voltage_in_interval_B "0.0";
    measured_reactive_min_voltage_in_interval_C "0.0";
    measured_real_min_voltage_in_interval_AB "0.0";
    measured_real_min_voltage_in_interval_BC "0.0";
    measured_real_min_voltage_in_interval_CA "0.0";
    measured_reactive_min_voltage_in_interval_AB "0.0";
    measured_reactive_min_voltage_in_interval_BC "0.0";
    measured_reactive_min_voltage_in_interval_CA "0.0";
    measured_avg_voltage_mag_in_interval_A "0.0";
    measured_avg_voltage_mag_in_interval_B "0.0";
    measured_avg_voltage_mag_in_interval_C "0.0";
    measured_avg_voltage_mag_in_interval_AB "0.0";
    measured_avg_voltage_mag_in_interval_BC "0.0";
    measured_avg_voltage_mag_in_interval_CA "0.0";
    measured_current_A "0+0i";
    measured_current_B "0+0i";
    measured_current_C "0+0i";
    customer_interrupted "FALSE";
    customer_interrupted_secondary "FALSE";
    monthly_bill "0.0";
    previous_monthly_bill "0.0";
    previous_monthly_energy "0.0";
    monthly_fee "0.0";
    monthly_energy "0.0";
    bill_mode "0";
    bill_day "0";
    price "0.0";
    price_base "0.0";
    first_tier_price "0.0";
    first_tier_energy "0.0";
    second_tier_price "0.0";
    second_tier_energy "0.0";
    third_tier_price "0.0";
    third_tier_energy "0.0";
    bustype "0";
    busflags "0";
    maximum_voltage_error "0.0";
    voltage_A "0+0i";
    voltage_B "0+0i";
    voltage_C "0+0i";
    voltage_AB "0+0i";
    voltage_BC "0+0i";
    voltage_CA "0+0i";
    current_A "0+0i";
    current_B "0+0i";
    current_C "0+0i";
    power_A "0+0i";
    power_B "0+0i";
    power_C "0+0i";
    shunt_A "0+0i";
    shunt_B "0+0i";
    shunt_C "0+0i";
    prerotated_current_A "0+0i";
    prerotated_current_B "0+0i";
    prerotated_current_C "0+0i";
    current_AB "0+0i";
    current_BC "0+0i";
    current_CA "0+0i";
    current_AN "0+0i";
    current_BN "0+0i";
    current_CN "0+0i";
    power_AB "0+0i";
    power_BC "0+0i";
    power_CA "0+0i";
    power_AN "0+0i";
    power_BN "0+0i";
    power_CN "0+0i";
    shunt_AB "0+0i";
    shunt_BC "0+0i";
    shunt_CA "0+0i";
    shunt_AN "0+0i";
    shunt_BN "0+0i";
    shunt_CN "0+0i";
    mean_repair_time "0.0";
    frequency_measure_type "0";
    sfm_Tf "0.0";
    pll_Kp "0.0";
    pll_Ki "0.0";
    measured_angle_A "0.0";
    measured_frequency_A "0.0";
    measured_angle_B "0.0";
    measured_frequency_B "0.0";
    measured_angle_C "0.0";
    measured_frequency_C "0.0";
    measured_frequency "0.0";
    service_status "0";
    service_status_double "0.0";
    previous_uptime "0.0";
    current_uptime "0.0";
    Norton_dynamic "FALSE";
    generator_dynamic "FALSE";
    GFA_enable "FALSE";
    GFA_freq_low_trip "0.0";
    GFA_freq_high_trip "0.0";
    GFA_volt_low_trip "0.0";
    GFA_volt_high_trip "0.0";
    GFA_reconnect_time "0.0";
    GFA_freq_disconnect_time "0.0";
    GFA_volt_disconnect_time "0.0";
    GFA_status "FALSE";
    GFA_trip_method "0";
    phases "0";
    nominal_voltage "0.0";
    service_level "NORMAL";
  }
~~~

# See also

* [[/Module/Powerflow]]

