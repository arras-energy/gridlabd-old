[[/Module/Powerflow/Triplex_load]] -- Class triplex_load

# Synopsis

GLM:

~~~
  object triplex_load {
    load_class "{A,I,C,R,U}";
    constant_power_1 "<string> VA";
    constant_power_2 "<string> VA";
    constant_power_12 "<string> VA";
    constant_power_1_real "<decimal> W";
    constant_power_2_real "<decimal> W";
    constant_power_12_real "<decimal> W";
    constant_power_1_reac "<decimal> VAr";
    constant_power_2_reac "<decimal> VAr";
    constant_power_12_reac "<decimal> VAr";
    constant_current_1 "<string> A";
    constant_current_2 "<string> A";
    constant_current_12 "<string> A";
    constant_current_1_real "<decimal> A";
    constant_current_2_real "<decimal> A";
    constant_current_12_real "<decimal> A";
    constant_current_1_reac "<decimal> A";
    constant_current_2_reac "<decimal> A";
    constant_current_12_reac "<decimal> A";
    constant_impedance_1 "<string> Ohm";
    constant_impedance_2 "<string> Ohm";
    constant_impedance_12 "<string> Ohm";
    constant_impedance_1_real "<decimal> Ohm";
    constant_impedance_2_real "<decimal> Ohm";
    constant_impedance_12_real "<decimal> Ohm";
    constant_impedance_1_reac "<decimal> Ohm";
    constant_impedance_2_reac "<decimal> Ohm";
    constant_impedance_12_reac "<decimal> Ohm";
    measured_voltage_1 "<string> V";
    measured_voltage_2 "<string> V";
    measured_voltage_12 "<string> V";
    base_power_1 "<decimal> VA";
    base_power_2 "<decimal> VA";
    base_power_12 "<decimal> VA";
    power_pf_1 "<decimal> pu";
    current_pf_1 "<decimal> pu";
    impedance_pf_1 "<decimal> pu";
    power_pf_2 "<decimal> pu";
    current_pf_2 "<decimal> pu";
    impedance_pf_2 "<decimal> pu";
    power_pf_12 "<decimal> pu";
    current_pf_12 "<decimal> pu";
    impedance_pf_12 "<decimal> pu";
    power_fraction_1 "<decimal> pu";
    current_fraction_1 "<decimal> pu";
    impedance_fraction_1 "<decimal> pu";
    power_fraction_2 "<decimal> pu";
    current_fraction_2 "<decimal> pu";
    impedance_fraction_2 "<decimal> pu";
    power_fraction_12 "<decimal> pu";
    current_fraction_12 "<decimal> pu";
    impedance_fraction_12 "<decimal> pu";
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
    supernode_name "<string>";
  }
~~~

# Description

TODO

## Properties

### `load_class`
~~~
  enumeration {A, I, C, R, U} load_class;
~~~

Flag to track load type, not currently used for anything except sorting

### `constant_power_1`
~~~
  complex constant_power_1[VA];
~~~

Constant power load on split phase 1, specified as VA

### `constant_power_2`
~~~
  complex constant_power_2[VA];
~~~

Constant power load on split phase 2, specified as VA

### `constant_power_12`
~~~
  complex constant_power_12[VA];
~~~

Constant power load on split phase 12, specified as VA

### `constant_power_1_real`
~~~
  double constant_power_1_real[W];
~~~

Constant power load on spit phase 1, real only, specified as W

### `constant_power_2_real`
~~~
  double constant_power_2_real[W];
~~~

Constant power load on phase 2, real only, specified as W

### `constant_power_12_real`
~~~
  double constant_power_12_real[W];
~~~

Constant power load on phase 12, real only, specified as W

### `constant_power_1_reac`
~~~
  double constant_power_1_reac[VAr];
~~~

Constant power load on phase 1, imaginary only, specified as VAr

### `constant_power_2_reac`
~~~
  double constant_power_2_reac[VAr];
~~~

Constant power load on phase 2, imaginary only, specified as VAr

### `constant_power_12_reac`
~~~
  double constant_power_12_reac[VAr];
~~~

Constant power load on phase 12, imaginary only, specified as VAr

### `constant_current_1`
~~~
  complex constant_current_1[A];
~~~

Constant current load on phase 1, specified as Amps

### `constant_current_2`
~~~
  complex constant_current_2[A];
~~~

Constant current load on phase 2, specified as Amps

### `constant_current_12`
~~~
  complex constant_current_12[A];
~~~

Constant current load on phase 12, specified as Amps

### `constant_current_1_real`
~~~
  double constant_current_1_real[A];
~~~

Constant current load on phase 1, real only, specified as Amps

### `constant_current_2_real`
~~~
  double constant_current_2_real[A];
~~~

Constant current load on phase 2, real only, specified as Amps

### `constant_current_12_real`
~~~
  double constant_current_12_real[A];
~~~

Constant current load on phase 12, real only, specified as Amps

### `constant_current_1_reac`
~~~
  double constant_current_1_reac[A];
~~~

Constant current load on phase 1, imaginary only, specified as Amps

### `constant_current_2_reac`
~~~
  double constant_current_2_reac[A];
~~~

Constant current load on phase 2, imaginary only, specified as Amps

### `constant_current_12_reac`
~~~
  double constant_current_12_reac[A];
~~~

Constant current load on phase 12, imaginary only, specified as Amps

### `constant_impedance_1`
~~~
  complex constant_impedance_1[Ohm];
~~~

Constant impedance load on phase 1, specified as Ohms

### `constant_impedance_2`
~~~
  complex constant_impedance_2[Ohm];
~~~

Constant impedance load on phase 2, specified as Ohms

### `constant_impedance_12`
~~~
  complex constant_impedance_12[Ohm];
~~~

Constant impedance load on phase 12, specified as Ohms

### `constant_impedance_1_real`
~~~
  double constant_impedance_1_real[Ohm];
~~~

Constant impedance load on phase 1, real only, specified as Ohms

### `constant_impedance_2_real`
~~~
  double constant_impedance_2_real[Ohm];
~~~

Constant impedance load on phase 2, real only, specified as Ohms

### `constant_impedance_12_real`
~~~
  double constant_impedance_12_real[Ohm];
~~~

Constant impedance load on phase 12, real only, specified as Ohms

### `constant_impedance_1_reac`
~~~
  double constant_impedance_1_reac[Ohm];
~~~

Constant impedance load on phase 1, imaginary only, specified as Ohms

### `constant_impedance_2_reac`
~~~
  double constant_impedance_2_reac[Ohm];
~~~

Constant impedance load on phase 2, imaginary only, specified as Ohms

### `constant_impedance_12_reac`
~~~
  double constant_impedance_12_reac[Ohm];
~~~

Constant impedance load on phase 12, imaginary only, specified as Ohms

### `measured_voltage_1`
~~~
  complex measured_voltage_1[V];
~~~

Measured voltage on phase 1

### `measured_voltage_2`
~~~
  complex measured_voltage_2[V];
~~~

Measured voltage on phase 2

### `measured_voltage_12`
~~~
  complex measured_voltage_12[V];
~~~

Measured voltage on phase 12

### `base_power_1`
~~~
  double base_power_1[VA];
~~~

In similar format as ZIPload, this represents the nominal power on phase 1 before applying ZIP fractions

### `base_power_2`
~~~
  double base_power_2[VA];
~~~

In similar format as ZIPload, this represents the nominal power on phase 2 before applying ZIP fractions

### `base_power_12`
~~~
  double base_power_12[VA];
~~~

In similar format as ZIPload, this represents the nominal power on phase 12 before applying ZIP fractions

### `power_pf_1`
~~~
  double power_pf_1[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase 1 constant power portion of load

### `current_pf_1`
~~~
  double current_pf_1[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase 1 constant current portion of load

### `impedance_pf_1`
~~~
  double impedance_pf_1[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase 1 constant impedance portion of load

### `power_pf_2`
~~~
  double power_pf_2[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase 2 constant power portion of load

### `current_pf_2`
~~~
  double current_pf_2[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase 2 constant current portion of load

### `impedance_pf_2`
~~~
  double impedance_pf_2[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase 2 constant impedance portion of load

### `power_pf_12`
~~~
  double power_pf_12[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase 12 constant power portion of load

### `current_pf_12`
~~~
  double current_pf_12[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase 12 constant current portion of load

### `impedance_pf_12`
~~~
  double impedance_pf_12[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase 12 constant impedance portion of load

### `power_fraction_1`
~~~
  double power_fraction_1[pu];
~~~

This is the constant power fraction of base power on phase 1

### `current_fraction_1`
~~~
  double current_fraction_1[pu];
~~~

This is the constant current fraction of base power on phase 1

### `impedance_fraction_1`
~~~
  double impedance_fraction_1[pu];
~~~

This is the constant impedance fraction of base power on phase 1

### `power_fraction_2`
~~~
  double power_fraction_2[pu];
~~~

This is the constant power fraction of base power on phase 2

### `current_fraction_2`
~~~
  double current_fraction_2[pu];
~~~

This is the constant current fraction of base power on phase 2

### `impedance_fraction_2`
~~~
  double impedance_fraction_2[pu];
~~~

This is the constant impedance fraction of base power on phase 2

### `power_fraction_12`
~~~
  double power_fraction_12[pu];
~~~

This is the constant power fraction of base power on phase 12

### `current_fraction_12`
~~~
  double current_fraction_12[pu];
~~~

This is the constant current fraction of base power on phase 12

### `impedance_fraction_12`
~~~
  double impedance_fraction_12[pu];
~~~

This is the constant impedance fraction of base power on phase 12

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

### `supernode_name`
~~~
  char1024 supernode_name;
~~~

TODO

# Example

~~~
  object triplex_load {
    load_class "0";
    constant_power_1 "0+0i";
    constant_power_2 "0+0i";
    constant_power_12 "0+0i";
    constant_power_1_real "0.0";
    constant_power_2_real "0.0";
    constant_power_12_real "0.0";
    constant_power_1_reac "0.0";
    constant_power_2_reac "0.0";
    constant_power_12_reac "0.0";
    constant_current_1 "0+0i";
    constant_current_2 "0+0i";
    constant_current_12 "0+0i";
    constant_current_1_real "0.0";
    constant_current_2_real "0.0";
    constant_current_12_real "0.0";
    constant_current_1_reac "0.0";
    constant_current_2_reac "0.0";
    constant_current_12_reac "0.0";
    constant_impedance_1 "0+0i";
    constant_impedance_2 "0+0i";
    constant_impedance_12 "0+0i";
    constant_impedance_1_real "0.0";
    constant_impedance_2_real "0.0";
    constant_impedance_12_real "0.0";
    constant_impedance_1_reac "0.0";
    constant_impedance_2_reac "0.0";
    constant_impedance_12_reac "0.0";
    measured_voltage_1 "0+0i";
    measured_voltage_2 "0+0i";
    measured_voltage_12 "0+0i";
    base_power_1 "0.0";
    base_power_2 "0.0";
    base_power_12 "0.0";
    power_pf_1 "0.0";
    current_pf_1 "0.0";
    impedance_pf_1 "0.0";
    power_pf_2 "0.0";
    current_pf_2 "0.0";
    impedance_pf_2 "0.0";
    power_pf_12 "0.0";
    current_pf_12 "0.0";
    impedance_pf_12 "0.0";
    power_fraction_1 "0.0";
    current_fraction_1 "0.0";
    impedance_fraction_1 "0.0";
    power_fraction_2 "0.0";
    current_fraction_2 "0.0";
    impedance_fraction_2 "0.0";
    power_fraction_12 "0.0";
    current_fraction_12 "0.0";
    impedance_fraction_12 "0.0";
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
    supernode_name "";
  }
~~~

# See also
* [[/Module/Powerflow]]

