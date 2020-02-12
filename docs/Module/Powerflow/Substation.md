[[/Module/Powerflow/Substation]] -- Class substation

# Synopsis

GLM:

~~~
  object substation {
    zero_sequence_voltage "<string> V";
    positive_sequence_voltage "<string> V";
    negative_sequence_voltage "<string> V";
    base_power "<decimal> VA";
    power_convergence_value "<decimal> VA";
    reference_phase "{PHASE_C,PHASE_B,PHASE_A}";
    transmission_level_constant_power_load "<string> VA";
    transmission_level_constant_current_load "<string> A";
    transmission_level_constant_impedance_load "<string> Ohm";
    distribution_load "<string> VA";
    distribution_power_A "<string> VA";
    distribution_power_B "<string> VA";
    distribution_power_C "<string> VA";
    distribution_voltage_A "<string> V";
    distribution_voltage_B "<string> V";
    distribution_voltage_C "<string> V";
    distribution_voltage_AB "<string> V";
    distribution_voltage_BC "<string> V";
    distribution_voltage_CA "<string> V";
    distribution_current_A "<string> A";
    distribution_current_B "<string> A";
    distribution_current_C "<string> A";
    distribution_real_energy "<decimal> Wh";
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
    supernode_name "<string>";
  }
~~~

# Description

TODO

## Properties

### `zero_sequence_voltage`

~~~
  complex zero_sequence_voltage[V];
~~~

The zero sequence representation of the voltage for the substation object.

### `positive_sequence_voltage`

~~~
  complex positive_sequence_voltage[V];
~~~

The positive sequence representation of the voltage for the substation object.

### `negative_sequence_voltage`

~~~
  complex negative_sequence_voltage[V];
~~~

The negative sequence representation of the voltage for the substation object.

### `base_power`

~~~
  double base_power[VA];
~~~

The 3 phase VA power rating of the substation.

### `power_convergence_value`

~~~
  double power_convergence_value[VA];
~~~

Default convergence criterion before power is posted to pw_load objects if connected, otherwise ignored

### `reference_phase`

~~~
  enumeration {PHASE_C, PHASE_B, PHASE_A} reference_phase;
~~~

The reference phase for the positive sequence voltage.

### `transmission_level_constant_power_load`

~~~
  complex transmission_level_constant_power_load[VA];
~~~

The average constant power load to be posted directly to the pw_load object.

### `transmission_level_constant_current_load`

~~~
  complex transmission_level_constant_current_load[A];
~~~

The average constant current load at nominal voltage to be posted directly to the pw_load object.

### `transmission_level_constant_impedance_load`

~~~
  complex transmission_level_constant_impedance_load[Ohm];
~~~

The average constant impedance load at nominal voltage to be posted directly to the pw_load object.

### `distribution_load`

~~~
  complex distribution_load[VA];
~~~

The total load of all three phases at the substation object.

### `distribution_power_A`

~~~
  complex distribution_power_A[VA];
~~~

TODO

### `distribution_power_B`

~~~
  complex distribution_power_B[VA];
~~~

TODO

### `distribution_power_C`

~~~
  complex distribution_power_C[VA];
~~~

TODO

### `distribution_voltage_A`

~~~
  complex distribution_voltage_A[V];
~~~

TODO

### `distribution_voltage_B`

~~~
  complex distribution_voltage_B[V];
~~~

TODO

### `distribution_voltage_C`

~~~
  complex distribution_voltage_C[V];
~~~

TODO

### `distribution_voltage_AB`

~~~
  complex distribution_voltage_AB[V];
~~~

TODO

### `distribution_voltage_BC`

~~~
  complex distribution_voltage_BC[V];
~~~

TODO

### `distribution_voltage_CA`

~~~
  complex distribution_voltage_CA[V];
~~~

TODO

### `distribution_current_A`

~~~
  complex distribution_current_A[A];
~~~

TODO

### `distribution_current_B`

~~~
  complex distribution_current_B[A];
~~~

TODO

### `distribution_current_C`

~~~
  complex distribution_current_C[A];
~~~

TODO

### `distribution_real_energy`

~~~
  double distribution_real_energy[Wh];
~~~

TODO

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

### `supernode_name`

~~~
  char1024 supernode_name;
~~~

TODO

# Example

~~~
  object substation {
    zero_sequence_voltage "0+0i";
    positive_sequence_voltage "0+0i";
    negative_sequence_voltage "0+0i";
    base_power "0.0";
    power_convergence_value "0.0";
    reference_phase "0";
    transmission_level_constant_power_load "0+0i";
    transmission_level_constant_current_load "0+0i";
    transmission_level_constant_impedance_load "0+0i";
    distribution_load "0+0i";
    distribution_power_A "0+0i";
    distribution_power_B "0+0i";
    distribution_power_C "0+0i";
    distribution_voltage_A "0+0i";
    distribution_voltage_B "0+0i";
    distribution_voltage_C "0+0i";
    distribution_voltage_AB "0+0i";
    distribution_voltage_BC "0+0i";
    distribution_voltage_CA "0+0i";
    distribution_current_A "0+0i";
    distribution_current_B "0+0i";
    distribution_current_C "0+0i";
    distribution_real_energy "0.0";
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
    supernode_name "";
  }
~~~

# See also

* [[/Module/Powerflow]]

