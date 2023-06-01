[[/Module/Powerflow/Triplex_meter]] -- Class triplex_meter

# Synopsis

GLM:

~~~
  object triplex_meter {
    measured_real_energy "<decimal> Wh";
    measured_real_energy_delta "<decimal> Wh";
    measured_reactive_energy "<decimal> VAh";
    measured_reactive_energy_delta "<decimal> VAh";
    measured_energy_delta_timestep "<decimal> s";
    measured_power "<string> VA";
    indiv_measured_power_1 "<string> VA";
    indiv_measured_power_2 "<string> VA";
    indiv_measured_power_N "<string> VA";
    measured_demand "<decimal> W";
    measured_real_power "<decimal> W";
    measured_reactive_power "<decimal> VAr";
    meter_power_consumption "<string> VA";
    measured_voltage_1 "<string> V";
    measured_voltage_2 "<string> V";
    measured_voltage_12 "<string> V";
    measured_voltage_N "<string> V";
    measured_real_max_voltage_1_in_interval "<decimal>";
    measured_real_max_voltage_2_in_interval "<decimal>";
    measured_real_max_voltage_12_in_interval "<decimal>";
    measured_imag_max_voltage_1_in_interval "<decimal>";
    measured_imag_max_voltage_2_in_interval "<decimal>";
    measured_imag_max_voltage_12_in_interval "<decimal>";
    measured_real_min_voltage_1_in_interval "<decimal>";
    measured_real_min_voltage_2_in_interval "<decimal>";
    measured_real_min_voltage_12_in_interval "<decimal>";
    measured_imag_min_voltage_1_in_interval "<decimal>";
    measured_imag_min_voltage_2_in_interval "<decimal>";
    measured_imag_min_voltage_12_in_interval "<decimal>";
    measured_avg_voltage_mag_1_in_interval "<decimal>";
    measured_avg_voltage_mag_2_in_interval "<decimal>";
    measured_avg_voltage_mag_12_in_interval "<decimal>";
    measured_current_1 "<string> A";
    measured_current_2 "<string> A";
    measured_current_N "<string> A";
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
    bustype "{SWING_PQ,SWING,PV,PQ}";
    busflags "[ISSOURCE|HASSOURCE]";
    reference_bus "<string>";
    maximum_voltage_error "<decimal> V";
    voltage_1 "<string> V";
    voltage_2 "<string> V";
    voltage_N "<string> V";
    voltage_12 "<string> V";
    voltage_1N "<string> V";
    voltage_2N "<string> V";
    current_1 "<string> A";
    current_2 "<string> A";
    current_N "<string> A";
    current_1_real "<decimal> A";
    current_2_real "<decimal> A";
    current_N_real "<decimal> A";
    current_1_reac "<decimal> A";
    current_2_reac "<decimal> A";
    current_N_reac "<decimal> A";
    current_12 "<string> A";
    current_12_real "<decimal> A";
    current_12_reac "<decimal> A";
    prerotated_current_1 "<string> A";
    prerotated_current_2 "<string> A";
    prerotated_current_12 "<string> A";
    residential_nominal_current_1 "<string> A";
    residential_nominal_current_2 "<string> A";
    residential_nominal_current_12 "<string> A";
    residential_nominal_current_1_real "<decimal> A";
    residential_nominal_current_1_imag "<decimal> A";
    residential_nominal_current_2_real "<decimal> A";
    residential_nominal_current_2_imag "<decimal> A";
    residential_nominal_current_12_real "<decimal> A";
    residential_nominal_current_12_imag "<decimal> A";
    power_1 "<string> VA";
    power_2 "<string> VA";
    power_12 "<string> VA";
    power_1_real "<decimal> W";
    power_2_real "<decimal> W";
    power_12_real "<decimal> W";
    power_1_reac "<decimal> VAr";
    power_2_reac "<decimal> VAr";
    power_12_reac "<decimal> VAr";
    shunt_1 "<string> S";
    shunt_2 "<string> S";
    shunt_12 "<string> S";
    impedance_1 "<string> Ohm";
    impedance_2 "<string> Ohm";
    impedance_12 "<string> Ohm";
    impedance_1_real "<decimal> Ohm";
    impedance_2_real "<decimal> Ohm";
    impedance_12_real "<decimal> Ohm";
    impedance_1_reac "<decimal> Ohm";
    impedance_2_reac "<decimal> Ohm";
    impedance_12_reac "<decimal> Ohm";
    house_present "<string>";
    GFA_enable "<string>";
    GFA_freq_low_trip "<decimal> Hz";
    GFA_freq_high_trip "<decimal> Hz";
    GFA_volt_low_trip "<decimal> pu";
    GFA_volt_high_trip "<decimal> pu";
    GFA_reconnect_time "<decimal> s";
    GFA_freq_disconnect_time "<decimal> s";
    GFA_volt_disconnect_time "<decimal> s";
    GFA_status "<string>";
    service_status "{OUT_OF_SERVICE,IN_SERVICE}";
    service_status_double "<decimal>";
    previous_uptime "<decimal> min";
    current_uptime "<decimal> min";
    topological_parent "<string>";
    frequency_measure_type "{PLL,SIMPLE,NONE}";
    sfm_Tf "<decimal> s";
    pll_Kp "<decimal> pu";
    pll_Ki "<decimal> pu";
    measured_angle_1 "<decimal> rad";
    measured_frequency_1 "<decimal> Hz";
    measured_angle_2 "<decimal> rad";
    measured_frequency_2 "<decimal> Hz";
    measured_angle_12 "<decimal> rad";
    measured_frequency_12 "<decimal> Hz";
    measured_frequency "<decimal> Hz";
    phases "[ABCDNSG]";
    nominal_voltage "<decimal> V";
    service_level {LOW,NORMAL,HIGH,CRITICAL};
  }
~~~

# Description

TODO

## Properties

### `measured_real_energy`

~~~
  double measured_real_energy[Wh];
~~~

Metered real energy consumption

### `measured_real_energy_delta`

~~~
  double measured_real_energy_delta[Wh];
~~~

Delta in metered real energy consumption from last specified measured_energy_delta_timestep

### `measured_reactive_energy`

~~~
  double measured_reactive_energy[VAh];
~~~

Metered reactive energy consumption

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

Metered power

### `indiv_measured_power_1`

~~~
  complex indiv_measured_power_1[VA];
~~~

Metered power, phase 1

### `indiv_measured_power_2`

~~~
  complex indiv_measured_power_2[VA];
~~~

Metered power, phase 2

### `indiv_measured_power_N`

~~~
  complex indiv_measured_power_N[VA];
~~~

Metered power, phase N

### `measured_demand`

~~~
  double measured_demand[W];
~~~

Metered demand (peak of power)

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

Power consumed by meter operation

### `measured_voltage_1`

~~~
  complex measured_voltage_1[V];
~~~

Measured voltage, phase 1 to ground

### `measured_voltage_2`

~~~
  complex measured_voltage_2[V];
~~~

Measured voltage, phase 2 to ground

### `measured_voltage_12`

~~~
  complex measured_voltage_12[V];
~~~

Measured voltage, phase 1 to phase 2

### `measured_voltage_N`

~~~
  complex measured_voltage_N[V];
~~~

Measured voltage, phase N to ground

### `measured_real_max_voltage_1_in_interval`

~~~
  double measured_real_max_voltage_1_in_interval;
~~~

Measured real max line-to-ground voltage on phase 1 over a specified interval

### `measured_real_max_voltage_2_in_interval`

~~~
  double measured_real_max_voltage_2_in_interval;
~~~

Measured real max line-to-ground voltage on phase 2 over a specified interval

### `measured_real_max_voltage_12_in_interval`

~~~
  double measured_real_max_voltage_12_in_interval;
~~~

Measured real max line-to-ground voltage on phase 12 over a specified interval

### `measured_imag_max_voltage_1_in_interval`

~~~
  double measured_imag_max_voltage_1_in_interval;
~~~

Measured imaginary max line-to-ground voltage on phase 1 over a specified interval

### `measured_imag_max_voltage_2_in_interval`

~~~
  double measured_imag_max_voltage_2_in_interval;
~~~

Measured imaginary max line-to-ground voltage on phase 2 over a specified interval

### `measured_imag_max_voltage_12_in_interval`

~~~
  double measured_imag_max_voltage_12_in_interval;
~~~

Measured imaginary max line-to-ground voltage on phase 12 over a specified interval

### `measured_real_min_voltage_1_in_interval`

~~~
  double measured_real_min_voltage_1_in_interval;
~~~

Measured real min line-to-ground voltage on phase 1 over a specified interval

### `measured_real_min_voltage_2_in_interval`

~~~
  double measured_real_min_voltage_2_in_interval;
~~~

Measured real min line-to-ground voltage on phase 2 over a specified interval

### `measured_real_min_voltage_12_in_interval`

~~~
  double measured_real_min_voltage_12_in_interval;
~~~

Measured real min line-to-ground voltage on phase 12 over a specified interval

### `measured_imag_min_voltage_1_in_interval`

~~~
  double measured_imag_min_voltage_1_in_interval;
~~~

Measured imaginary min line-to-ground voltage on phase 1 over a specified interval

### `measured_imag_min_voltage_2_in_interval`

~~~
  double measured_imag_min_voltage_2_in_interval;
~~~

Measured imaginary min line-to-ground voltage on phase 2 over a specified interval

### `measured_imag_min_voltage_12_in_interval`

~~~
  double measured_imag_min_voltage_12_in_interval;
~~~

Measured imaginary min line-to-ground voltage on phase 12 over a specified interval

### `measured_avg_voltage_mag_1_in_interval`

~~~
  double measured_avg_voltage_mag_1_in_interval;
~~~

Measured average line-to-ground voltage magnitude on phase 1 over a specified interval

### `measured_avg_voltage_mag_2_in_interval`

~~~
  double measured_avg_voltage_mag_2_in_interval;
~~~

Measured average line-to-ground voltage magnitude on phase 2 over a specified interval

### `measured_avg_voltage_mag_12_in_interval`

~~~
  double measured_avg_voltage_mag_12_in_interval;
~~~

Measured average line-to-ground voltage magnitude on phase 12 over a specified interval

### `measured_current_1`

~~~
  complex measured_current_1[A];
~~~

Measured current, phase 1

### `measured_current_2`

~~~
  complex measured_current_2[A];
~~~

Measured current, phase 2

### `measured_current_N`

~~~
  complex measured_current_N[A];
~~~

Measured current, phase N

### `customer_interrupted`

~~~
  bool customer_interrupted;
~~~

Reliability flag - goes active if the customer is in an interrupted state

### `customer_interrupted_secondary`

~~~
  bool customer_interrupted_secondary;
~~~

Reliability flag - goes active if the customer is in a secondary interrupted state - i.e., momentary

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


### `monthly_fee`

~~~
  double monthly_fee[$];
~~~

Total monthly energy for the previous month

### `monthly_energy`

~~~
  double monthly_energy[kWh];
~~~

Accumulator for the current month's energy

### `bill_mode`

~~~
  enumeration {TIERED_RTP, HOURLY, TIERED, UNIFORM, NONE} bill_mode;
~~~

Designates the bill mode to be used

### `power_market`

~~~
  object power_market;
~~~

Designates the auction object where prices are read from for bill mode

### `bill_day`

~~~
  int32 bill_day;
~~~

Day bill is to be processed (assumed to occur at midnight of that day)

### `price`

~~~
  double price[$/kWh];
~~~

Standard uniform pricing

### `price_base`

~~~
  double price_base[$/kWh];
~~~

Used only in TIERED_RTP mode to describe the price before the first tier

### `first_tier_price`

~~~
  double first_tier_price[$/kWh];
~~~

First tier price of energy between first and second tier energy

### `first_tier_energy`

~~~
  double first_tier_energy[kWh];
~~~

Price of energy on tier above price or price base

### `second_tier_price`

~~~
  double second_tier_price[$/kWh];
~~~

First tier price of energy between second and third tier energy

### `second_tier_energy`

~~~
  double second_tier_energy[kWh];
~~~

Price of energy on tier above first tier

### `third_tier_price`

~~~
  double third_tier_price[$/kWh];
~~~

First tier price of energy greater than third tier energy

### `third_tier_energy`

~~~
  double third_tier_energy[kWh];
~~~

Price of energy on tier above second tier

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

### `voltage_1`

~~~
  complex voltage_1[V];
~~~

Bus voltage, phase 1 to ground

### `voltage_2`

~~~
  complex voltage_2[V];
~~~

Bus voltage, phase 2 to ground

### `voltage_N`

~~~
  complex voltage_N[V];
~~~

Bus voltage, phase N to ground

### `voltage_12`

~~~
  complex voltage_12[V];
~~~

Bus voltage, phase 1 to 2

### `voltage_1N`

~~~
  complex voltage_1N[V];
~~~

Bus voltage, phase 1 to N

### `voltage_2N`

~~~
  complex voltage_2N[V];
~~~

Bus voltage, phase 2 to N

### `current_1`

~~~
  complex current_1[A];
~~~

Constant current load on phase 1, also acts as accumulator

### `current_2`

~~~
  complex current_2[A];
~~~

Constant current load on phase 2, also acts as accumulator

### `current_N`

~~~
  complex current_N[A];
~~~

Constant current load on phase N, also acts as accumulator

### `current_1_real`

~~~
  double current_1_real[A];
~~~

Constant current load on phase 1, real

### `current_2_real`

~~~
  double current_2_real[A];
~~~

Constant current load on phase 2, real

### `current_N_real`

~~~
  double current_N_real[A];
~~~

Constant current load on phase N, real

### `current_1_reac`

~~~
  double current_1_reac[A];
~~~

Constant current load on phase 1, imag

### `current_2_reac`

~~~
  double current_2_reac[A];
~~~

Constant current load on phase 2, imag

### `current_N_reac`

~~~
  double current_N_reac[A];
~~~

Constant current load on phase N, imag

### `current_12`

~~~
  complex current_12[A];
~~~

Constant current load on phase 1 to 2

### `current_12_real`

~~~
  double current_12_real[A];
~~~

Constant current load on phase 1 to 2, real

### `current_12_reac`

~~~
  double current_12_reac[A];
~~~

Constant current load on phase 1 to 2, imag

### `prerotated_current_1`

~~~
  complex prerotated_current_1[A];
~~~

Deltamode-functionality - bus current injection (in = positive), but will not be rotated by powerflow for off-nominal frequency, this an accumulator only, not a output or input variable

### `prerotated_current_2`

~~~
  complex prerotated_current_2[A];
~~~

Deltamode-functionality - bus current injection (in = positive), but will not be rotated by powerflow for off-nominal frequency, this an accumulator only, not a output or input variable

### `prerotated_current_12`

~~~
  complex prerotated_current_12[A];
~~~

Deltamode-functionality - bus current injection (in = positive), but will not be rotated by powerflow for off-nominal frequency, this an accumulator only, not a output or input variable

### `residential_nominal_current_1`

~~~
  complex residential_nominal_current_1[A];
~~~

Posted current on phase 1 from a residential object, if attached

### `residential_nominal_current_2`

~~~
  complex residential_nominal_current_2[A];
~~~

Posted current on phase 2 from a residential object, if attached

### `residential_nominal_current_12`

~~~
  complex residential_nominal_current_12[A];
~~~

Posted current on phase 1 to 2 from a residential object, if attached

### `residential_nominal_current_1_real`

~~~
  double residential_nominal_current_1_real[A];
~~~

Posted current on phase 1, real, from a residential object, if attached

### `residential_nominal_current_1_imag`

~~~
  double residential_nominal_current_1_imag[A];
~~~

Posted current on phase 1, imag, from a residential object, if attached

### `residential_nominal_current_2_real`

~~~
  double residential_nominal_current_2_real[A];
~~~

Posted current on phase 2, real, from a residential object, if attached

### `residential_nominal_current_2_imag`

~~~
  double residential_nominal_current_2_imag[A];
~~~

Posted current on phase 2, imag, from a residential object, if attached

### `residential_nominal_current_12_real`

~~~
  double residential_nominal_current_12_real[A];
~~~

Posted current on phase 1 to 2, real, from a residential object, if attached

### `residential_nominal_current_12_imag`

~~~
  double residential_nominal_current_12_imag[A];
~~~

Posted current on phase 1 to 2, imag, from a residential object, if attached

### `power_1`

~~~
  complex power_1[VA];
~~~

Constant power on phase 1 (120V)

### `power_2`

~~~
  complex power_2[VA];
~~~

Constant power on phase 2 (120V)

### `power_12`

~~~
  complex power_12[VA];
~~~

Constant power on phase 1 to 2 (240V)

### `power_1_real`

~~~
  double power_1_real[W];
~~~

Constant power on phase 1, real

### `power_2_real`

~~~
  double power_2_real[W];
~~~

Constant power on phase 2, real

### `power_12_real`

~~~
  double power_12_real[W];
~~~

Constant power on phase 1 to 2, real

### `power_1_reac`

~~~
  double power_1_reac[VAr];
~~~

Constant power on phase 1, imag

### `power_2_reac`

~~~
  double power_2_reac[VAr];
~~~

Constant power on phase 2, imag

### `power_12_reac`

~~~
  double power_12_reac[VAr];
~~~

Constant power on phase 1 to 2, imag

### `shunt_1`

~~~
  complex shunt_1[S];
~~~

Constant shunt impedance on phase 1

### `shunt_2`

~~~
  complex shunt_2[S];
~~~

Constant shunt impedance on phase 2

### `shunt_12`

~~~
  complex shunt_12[S];
~~~

Constant shunt impedance on phase 1 to 2

### `impedance_1`

~~~
  complex impedance_1[Ohm];
~~~

Constant series impedance on phase 1

### `impedance_2`

~~~
  complex impedance_2[Ohm];
~~~

Constant series impedance on phase 2

### `impedance_12`

~~~
  complex impedance_12[Ohm];
~~~

Constant series impedance on phase 1 to 2

### `impedance_1_real`

~~~
  double impedance_1_real[Ohm];
~~~

Constant series impedance on phase 1, real

### `impedance_2_real`

~~~
  double impedance_2_real[Ohm];
~~~

Constant series impedance on phase 2, real

### `impedance_12_real`

~~~
  double impedance_12_real[Ohm];
~~~

Constant series impedance on phase 1 to 2, real

### `impedance_1_reac`

~~~
  double impedance_1_reac[Ohm];
~~~

Constant series impedance on phase 1, imag

### `impedance_2_reac`

~~~
  double impedance_2_reac[Ohm];
~~~

Constant series impedance on phase 2, imag

### `impedance_12_reac`

~~~
  double impedance_12_reac[Ohm];
~~~

Constant series impedance on phase 1 to 2, imag

### `house_present`

~~~
  bool house_present;
~~~

Boolean for detecting whether a house is attached, not an input

### `GFA_enable`

~~~
  bool GFA_enable;
~~~

Disable/Enable Grid Friendly Applicance(TM)-type functionality

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

### `topological_parent`

~~~
  object topological_parent;
~~~

Topological parent as per GLM configuration

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

### `measured_angle_1`

~~~
  double measured_angle_1[rad];
~~~

Bus angle measurement, phase 1N

### `measured_frequency_1`

~~~
  double measured_frequency_1[Hz];
~~~

Frequency measurement, phase 1N

### `measured_angle_2`

~~~
  double measured_angle_2[rad];
~~~

Bus angle measurement, phase 2N

### `measured_frequency_2`

~~~
  double measured_frequency_2[Hz];
~~~

Frequency measurement, phase 2N

### `measured_angle_12`

~~~
  double measured_angle_12[rad];
~~~

Bus angle measurement, across the phases

### `measured_frequency_12`

~~~
  double measured_frequency_12[Hz];
~~~

Frequency measurement, across the phases

### `measured_frequency`

~~~
  double measured_frequency[Hz];
~~~

Frequency measurement - average of present phases

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
  object triplex_meter {
    measured_real_energy "0.0";
    measured_real_energy_delta "0.0";
    measured_reactive_energy "0.0";
    measured_reactive_energy_delta "0.0";
    measured_energy_delta_timestep "0.0";
    measured_power "0+0i";
    indiv_measured_power_1 "0+0i";
    indiv_measured_power_2 "0+0i";
    indiv_measured_power_N "0+0i";
    measured_demand "0.0";
    measured_real_power "0.0";
    measured_reactive_power "0.0";
    meter_power_consumption "0+0i";
    measured_voltage_1 "0+0i";
    measured_voltage_2 "0+0i";
    measured_voltage_12 "0+0i";
    measured_voltage_N "0+0i";
    measured_real_max_voltage_1_in_interval "0.0";
    measured_real_max_voltage_2_in_interval "0.0";
    measured_real_max_voltage_12_in_interval "0.0";
    measured_imag_max_voltage_1_in_interval "0.0";
    measured_imag_max_voltage_2_in_interval "0.0";
    measured_imag_max_voltage_12_in_interval "0.0";
    measured_real_min_voltage_1_in_interval "0.0";
    measured_real_min_voltage_2_in_interval "0.0";
    measured_real_min_voltage_12_in_interval "0.0";
    measured_imag_min_voltage_1_in_interval "0.0";
    measured_imag_min_voltage_2_in_interval "0.0";
    measured_imag_min_voltage_12_in_interval "0.0";
    measured_avg_voltage_mag_1_in_interval "0.0";
    measured_avg_voltage_mag_2_in_interval "0.0";
    measured_avg_voltage_mag_12_in_interval "0.0";
    measured_current_1 "0+0i";
    measured_current_2 "0+0i";
    measured_current_N "0+0i";
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
    voltage_1 "0+0i";
    voltage_2 "0+0i";
    voltage_N "0+0i";
    voltage_12 "0+0i";
    voltage_1N "0+0i";
    voltage_2N "0+0i";
    current_1 "0+0i";
    current_2 "0+0i";
    current_N "0+0i";
    current_1_real "0.0";
    current_2_real "0.0";
    current_N_real "0.0";
    current_1_reac "0.0";
    current_2_reac "0.0";
    current_N_reac "0.0";
    current_12 "0+0i";
    current_12_real "0.0";
    current_12_reac "0.0";
    prerotated_current_1 "0+0i";
    prerotated_current_2 "0+0i";
    prerotated_current_12 "0+0i";
    residential_nominal_current_1 "0+0i";
    residential_nominal_current_2 "0+0i";
    residential_nominal_current_12 "0+0i";
    residential_nominal_current_1_real "0.0";
    residential_nominal_current_1_imag "0.0";
    residential_nominal_current_2_real "0.0";
    residential_nominal_current_2_imag "0.0";
    residential_nominal_current_12_real "0.0";
    residential_nominal_current_12_imag "0.0";
    power_1 "0+0i";
    power_2 "0+0i";
    power_12 "0+0i";
    power_1_real "0.0";
    power_2_real "0.0";
    power_12_real "0.0";
    power_1_reac "0.0";
    power_2_reac "0.0";
    power_12_reac "0.0";
    shunt_1 "0+0i";
    shunt_2 "0+0i";
    shunt_12 "0+0i";
    impedance_1 "0+0i";
    impedance_2 "0+0i";
    impedance_12 "0+0i";
    impedance_1_real "0.0";
    impedance_2_real "0.0";
    impedance_12_real "0.0";
    impedance_1_reac "0.0";
    impedance_2_reac "0.0";
    impedance_12_reac "0.0";
    house_present "FALSE";
    GFA_enable "FALSE";
    GFA_freq_low_trip "0.0";
    GFA_freq_high_trip "0.0";
    GFA_volt_low_trip "0.0";
    GFA_volt_high_trip "0.0";
    GFA_reconnect_time "0.0";
    GFA_freq_disconnect_time "0.0";
    GFA_volt_disconnect_time "0.0";
    GFA_status "FALSE";
    service_status "0";
    service_status_double "0.0";
    previous_uptime "0.0";
    current_uptime "0.0";
    frequency_measure_type "0";
    sfm_Tf "0.0";
    pll_Kp "0.0";
    pll_Ki "0.0";
    measured_angle_1 "0.0";
    measured_frequency_1 "0.0";
    measured_angle_2 "0.0";
    measured_frequency_2 "0.0";
    measured_angle_12 "0.0";
    measured_frequency_12 "0.0";
    measured_frequency "0.0";
    phases "0";
    nominal_voltage "0.0";
    service_level "NORMAL";
  }
~~~

# See also

* [[/Module/Powerflow]]

