[[/Module/Powerflow/Load]] -- Class load

# Synopsis

GLM:

~~~
  object load {
    load_class "{A,I,C,R,U}";
    constant_power_A "<string> VA";
    constant_power_B "<string> VA";
    constant_power_C "<string> VA";
    constant_power_A_real "<decimal> W";
    constant_power_B_real "<decimal> W";
    constant_power_C_real "<decimal> W";
    constant_power_A_reac "<decimal> VAr";
    constant_power_B_reac "<decimal> VAr";
    constant_power_C_reac "<decimal> VAr";
    constant_current_A "<string> A";
    constant_current_B "<string> A";
    constant_current_C "<string> A";
    constant_current_A_real "<decimal> A";
    constant_current_B_real "<decimal> A";
    constant_current_C_real "<decimal> A";
    constant_current_A_reac "<decimal> A";
    constant_current_B_reac "<decimal> A";
    constant_current_C_reac "<decimal> A";
    constant_impedance_A "<string> Ohm";
    constant_impedance_B "<string> Ohm";
    constant_impedance_C "<string> Ohm";
    constant_impedance_A_real "<decimal> Ohm";
    constant_impedance_B_real "<decimal> Ohm";
    constant_impedance_C_real "<decimal> Ohm";
    constant_impedance_A_reac "<decimal> Ohm";
    constant_impedance_B_reac "<decimal> Ohm";
    constant_impedance_C_reac "<decimal> Ohm";
    constant_power_AN "<string> VA";
    constant_power_BN "<string> VA";
    constant_power_CN "<string> VA";
    constant_power_AN_real "<decimal> W";
    constant_power_BN_real "<decimal> W";
    constant_power_CN_real "<decimal> W";
    constant_power_AN_reac "<decimal> VAr";
    constant_power_BN_reac "<decimal> VAr";
    constant_power_CN_reac "<decimal> VAr";
    constant_current_AN "<string> A";
    constant_current_BN "<string> A";
    constant_current_CN "<string> A";
    constant_current_AN_real "<decimal> A";
    constant_current_BN_real "<decimal> A";
    constant_current_CN_real "<decimal> A";
    constant_current_AN_reac "<decimal> A";
    constant_current_BN_reac "<decimal> A";
    constant_current_CN_reac "<decimal> A";
    constant_impedance_AN "<string> Ohm";
    constant_impedance_BN "<string> Ohm";
    constant_impedance_CN "<string> Ohm";
    constant_impedance_AN_real "<decimal> Ohm";
    constant_impedance_BN_real "<decimal> Ohm";
    constant_impedance_CN_real "<decimal> Ohm";
    constant_impedance_AN_reac "<decimal> Ohm";
    constant_impedance_BN_reac "<decimal> Ohm";
    constant_impedance_CN_reac "<decimal> Ohm";
    constant_power_AB "<string> VA";
    constant_power_BC "<string> VA";
    constant_power_CA "<string> VA";
    constant_power_AB_real "<decimal> W";
    constant_power_BC_real "<decimal> W";
    constant_power_CA_real "<decimal> W";
    constant_power_AB_reac "<decimal> VAr";
    constant_power_BC_reac "<decimal> VAr";
    constant_power_CA_reac "<decimal> VAr";
    constant_current_AB "<string> A";
    constant_current_BC "<string> A";
    constant_current_CA "<string> A";
    constant_current_AB_real "<decimal> A";
    constant_current_BC_real "<decimal> A";
    constant_current_CA_real "<decimal> A";
    constant_current_AB_reac "<decimal> A";
    constant_current_BC_reac "<decimal> A";
    constant_current_CA_reac "<decimal> A";
    constant_impedance_AB "<string> Ohm";
    constant_impedance_BC "<string> Ohm";
    constant_impedance_CA "<string> Ohm";
    constant_impedance_AB_real "<decimal> Ohm";
    constant_impedance_BC_real "<decimal> Ohm";
    constant_impedance_CA_real "<decimal> Ohm";
    constant_impedance_AB_reac "<decimal> Ohm";
    constant_impedance_BC_reac "<decimal> Ohm";
    constant_impedance_CA_reac "<decimal> Ohm";
    measured_voltage_A "<string>";
    measured_voltage_B "<string>";
    measured_voltage_C "<string>";
    measured_voltage_AB "<string>";
    measured_voltage_BC "<string>";
    measured_voltage_CA "<string>";
    phase_loss_protection "<string>";
    base_power_A "<decimal> VA";
    base_power_B "<decimal> VA";
    base_power_C "<decimal> VA";
    power_pf_A "<decimal> pu";
    current_pf_A "<decimal> pu";
    impedance_pf_A "<decimal> pu";
    power_pf_B "<decimal> pu";
    current_pf_B "<decimal> pu";
    impedance_pf_B "<decimal> pu";
    power_pf_C "<decimal> pu";
    current_pf_C "<decimal> pu";
    impedance_pf_C "<decimal> pu";
    power_fraction_A "<decimal> pu";
    current_fraction_A "<decimal> pu";
    impedance_fraction_A "<decimal> pu";
    power_fraction_B "<decimal> pu";
    current_fraction_B "<decimal> pu";
    impedance_fraction_B "<decimal> pu";
    power_fraction_C "<decimal> pu";
    current_fraction_C "<decimal> pu";
    impedance_fraction_C "<decimal> pu";
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

### `load_class`
~~~
  enumeration {A, I, C, R, U} load_class;
~~~

Flag to track load type, not currently used for anything except sorting

### `constant_power_A`
~~~
  complex constant_power_A[VA];
~~~

Constant power load on phase A, specified as VA

### `constant_power_B`
~~~
  complex constant_power_B[VA];
~~~

Constant power load on phase B, specified as VA

### `constant_power_C`
~~~
  complex constant_power_C[VA];
~~~

Constant power load on phase C, specified as VA

### `constant_power_A_real`
~~~
  double constant_power_A_real[W];
~~~

Constant power load on phase A, real only, specified as W

### `constant_power_B_real`
~~~
  double constant_power_B_real[W];
~~~

Constant power load on phase B, real only, specified as W

### `constant_power_C_real`
~~~
  double constant_power_C_real[W];
~~~

Constant power load on phase C, real only, specified as W

### `constant_power_A_reac`
~~~
  double constant_power_A_reac[VAr];
~~~

Constant power load on phase A, imaginary only, specified as VAr

### `constant_power_B_reac`
~~~
  double constant_power_B_reac[VAr];
~~~

Constant power load on phase B, imaginary only, specified as VAr

### `constant_power_C_reac`
~~~
  double constant_power_C_reac[VAr];
~~~

Constant power load on phase C, imaginary only, specified as VAr

### `constant_current_A`
~~~
  complex constant_current_A[A];
~~~

Constant current load on phase A, specified as Amps

### `constant_current_B`
~~~
  complex constant_current_B[A];
~~~

Constant current load on phase B, specified as Amps

### `constant_current_C`
~~~
  complex constant_current_C[A];
~~~

Constant current load on phase C, specified as Amps

### `constant_current_A_real`
~~~
  double constant_current_A_real[A];
~~~

Constant current load on phase A, real only, specified as Amps

### `constant_current_B_real`
~~~
  double constant_current_B_real[A];
~~~

Constant current load on phase B, real only, specified as Amps

### `constant_current_C_real`
~~~
  double constant_current_C_real[A];
~~~

Constant current load on phase C, real only, specified as Amps

### `constant_current_A_reac`
~~~
  double constant_current_A_reac[A];
~~~

Constant current load on phase A, imaginary only, specified as Amps

### `constant_current_B_reac`
~~~
  double constant_current_B_reac[A];
~~~

Constant current load on phase B, imaginary only, specified as Amps

### `constant_current_C_reac`
~~~
  double constant_current_C_reac[A];
~~~

Constant current load on phase C, imaginary only, specified as Amps

### `constant_impedance_A`
~~~
  complex constant_impedance_A[Ohm];
~~~

Constant impedance load on phase A, specified as Ohms

### `constant_impedance_B`
~~~
  complex constant_impedance_B[Ohm];
~~~

Constant impedance load on phase B, specified as Ohms

### `constant_impedance_C`
~~~
  complex constant_impedance_C[Ohm];
~~~

Constant impedance load on phase C, specified as Ohms

### `constant_impedance_A_real`
~~~
  double constant_impedance_A_real[Ohm];
~~~

Constant impedance load on phase A, real only, specified as Ohms

### `constant_impedance_B_real`
~~~
  double constant_impedance_B_real[Ohm];
~~~

Constant impedance load on phase B, real only, specified as Ohms

### `constant_impedance_C_real`
~~~
  double constant_impedance_C_real[Ohm];
~~~

Constant impedance load on phase C, real only, specified as Ohms

### `constant_impedance_A_reac`
~~~
  double constant_impedance_A_reac[Ohm];
~~~

Constant impedance load on phase A, imaginary only, specified as Ohms

### `constant_impedance_B_reac`
~~~
  double constant_impedance_B_reac[Ohm];
~~~

Constant impedance load on phase B, imaginary only, specified as Ohms

### `constant_impedance_C_reac`
~~~
  double constant_impedance_C_reac[Ohm];
~~~

Constant impedance load on phase C, imaginary only, specified as Ohms

### `constant_power_AN`
~~~
  complex constant_power_AN[VA];
~~~

Constant power wye-connected load on phase A, specified as VA

### `constant_power_BN`
~~~
  complex constant_power_BN[VA];
~~~

Constant power wye-connected load on phase B, specified as VA

### `constant_power_CN`
~~~
  complex constant_power_CN[VA];
~~~

Constant power wye-connected load on phase C, specified as VA

### `constant_power_AN_real`
~~~
  double constant_power_AN_real[W];
~~~

Constant power wye-connected load on phase A, real only, specified as W

### `constant_power_BN_real`
~~~
  double constant_power_BN_real[W];
~~~

Constant power wye-connected load on phase B, real only, specified as W

### `constant_power_CN_real`
~~~
  double constant_power_CN_real[W];
~~~

Constant power wye-connected load on phase C, real only, specified as W

### `constant_power_AN_reac`
~~~
  double constant_power_AN_reac[VAr];
~~~

Constant power wye-connected load on phase A, imaginary only, specified as VAr

### `constant_power_BN_reac`
~~~
  double constant_power_BN_reac[VAr];
~~~

Constant power wye-connected load on phase B, imaginary only, specified as VAr

### `constant_power_CN_reac`
~~~
  double constant_power_CN_reac[VAr];
~~~

Constant power wye-connected load on phase C, imaginary only, specified as VAr

### `constant_current_AN`
~~~
  complex constant_current_AN[A];
~~~

Constant current wye-connected load on phase A, specified as Amps

### `constant_current_BN`
~~~
  complex constant_current_BN[A];
~~~

Constant current wye-connected load on phase B, specified as Amps

### `constant_current_CN`
~~~
  complex constant_current_CN[A];
~~~

Constant current wye-connected load on phase C, specified as Amps

### `constant_current_AN_real`
~~~
  double constant_current_AN_real[A];
~~~

Constant current wye-connected load on phase A, real only, specified as Amps

### `constant_current_BN_real`
~~~
  double constant_current_BN_real[A];
~~~

Constant current wye-connected load on phase B, real only, specified as Amps

### `constant_current_CN_real`
~~~
  double constant_current_CN_real[A];
~~~

Constant current wye-connected load on phase C, real only, specified as Amps

### `constant_current_AN_reac`
~~~
  double constant_current_AN_reac[A];
~~~

Constant current wye-connected load on phase A, imaginary only, specified as Amps

### `constant_current_BN_reac`
~~~
  double constant_current_BN_reac[A];
~~~

Constant current wye-connected load on phase B, imaginary only, specified as Amps

### `constant_current_CN_reac`
~~~
  double constant_current_CN_reac[A];
~~~

Constant current wye-connected load on phase C, imaginary only, specified as Amps

### `constant_impedance_AN`
~~~
  complex constant_impedance_AN[Ohm];
~~~

Constant impedance wye-connected load on phase A, specified as Ohms

### `constant_impedance_BN`
~~~
  complex constant_impedance_BN[Ohm];
~~~

Constant impedance wye-connected load on phase B, specified as Ohms

### `constant_impedance_CN`
~~~
  complex constant_impedance_CN[Ohm];
~~~

Constant impedance wye-connected load on phase C, specified as Ohms

### `constant_impedance_AN_real`
~~~
  double constant_impedance_AN_real[Ohm];
~~~

Constant impedance wye-connected load on phase A, real only, specified as Ohms

### `constant_impedance_BN_real`
~~~
  double constant_impedance_BN_real[Ohm];
~~~

Constant impedance wye-connected load on phase B, real only, specified as Ohms

### `constant_impedance_CN_real`
~~~
  double constant_impedance_CN_real[Ohm];
~~~

Constant impedance wye-connected load on phase C, real only, specified as Ohms

### `constant_impedance_AN_reac`
~~~
  double constant_impedance_AN_reac[Ohm];
~~~

Constant impedance wye-connected load on phase A, imaginary only, specified as Ohms

### `constant_impedance_BN_reac`
~~~
  double constant_impedance_BN_reac[Ohm];
~~~

Constant impedance wye-connected load on phase B, imaginary only, specified as Ohms

### `constant_impedance_CN_reac`
~~~
  double constant_impedance_CN_reac[Ohm];
~~~

Constant impedance wye-connected load on phase C, imaginary only, specified as Ohms

### `constant_power_AB`
~~~
  complex constant_power_AB[VA];
~~~

Constant power delta-connected load on phase A, specified as VA

### `constant_power_BC`
~~~
  complex constant_power_BC[VA];
~~~

Constant power delta-connected load on phase B, specified as VA

### `constant_power_CA`
~~~
  complex constant_power_CA[VA];
~~~

Constant power delta-connected load on phase C, specified as VA

### `constant_power_AB_real`
~~~
  double constant_power_AB_real[W];
~~~

Constant power delta-connected load on phase A, real only, specified as W

### `constant_power_BC_real`
~~~
  double constant_power_BC_real[W];
~~~

Constant power delta-connected load on phase B, real only, specified as W

### `constant_power_CA_real`
~~~
  double constant_power_CA_real[W];
~~~

Constant power delta-connected load on phase C, real only, specified as W

### `constant_power_AB_reac`
~~~
  double constant_power_AB_reac[VAr];
~~~

Constant power delta-connected load on phase A, imaginary only, specified as VAr

### `constant_power_BC_reac`
~~~
  double constant_power_BC_reac[VAr];
~~~

Constant power delta-connected load on phase B, imaginary only, specified as VAr

### `constant_power_CA_reac`
~~~
  double constant_power_CA_reac[VAr];
~~~

Constant power delta-connected load on phase C, imaginary only, specified as VAr

### `constant_current_AB`
~~~
  complex constant_current_AB[A];
~~~

Constant current delta-connected load on phase A, specified as Amps

### `constant_current_BC`
~~~
  complex constant_current_BC[A];
~~~

Constant current delta-connected load on phase B, specified as Amps

### `constant_current_CA`
~~~
  complex constant_current_CA[A];
~~~

Constant current delta-connected load on phase C, specified as Amps

### `constant_current_AB_real`
~~~
  double constant_current_AB_real[A];
~~~

Constant current delta-connected load on phase A, real only, specified as Amps

### `constant_current_BC_real`
~~~
  double constant_current_BC_real[A];
~~~

Constant current delta-connected load on phase B, real only, specified as Amps

### `constant_current_CA_real`
~~~
  double constant_current_CA_real[A];
~~~

Constant current delta-connected load on phase C, real only, specified as Amps

### `constant_current_AB_reac`
~~~
  double constant_current_AB_reac[A];
~~~

Constant current delta-connected load on phase A, imaginary only, specified as Amps

### `constant_current_BC_reac`
~~~
  double constant_current_BC_reac[A];
~~~

Constant current delta-connected load on phase B, imaginary only, specified as Amps

### `constant_current_CA_reac`
~~~
  double constant_current_CA_reac[A];
~~~

Constant current delta-connected load on phase C, imaginary only, specified as Amps

### `constant_impedance_AB`
~~~
  complex constant_impedance_AB[Ohm];
~~~

Constant impedance delta-connected load on phase A, specified as Ohms

### `constant_impedance_BC`
~~~
  complex constant_impedance_BC[Ohm];
~~~

Constant impedance delta-connected load on phase B, specified as Ohms

### `constant_impedance_CA`
~~~
  complex constant_impedance_CA[Ohm];
~~~

Constant impedance delta-connected load on phase C, specified as Ohms

### `constant_impedance_AB_real`
~~~
  double constant_impedance_AB_real[Ohm];
~~~

Constant impedance delta-connected load on phase A, real only, specified as Ohms

### `constant_impedance_BC_real`
~~~
  double constant_impedance_BC_real[Ohm];
~~~

Constant impedance delta-connected load on phase B, real only, specified as Ohms

### `constant_impedance_CA_real`
~~~
  double constant_impedance_CA_real[Ohm];
~~~

Constant impedance delta-connected load on phase C, real only, specified as Ohms

### `constant_impedance_AB_reac`
~~~
  double constant_impedance_AB_reac[Ohm];
~~~

Constant impedance delta-connected load on phase A, imaginary only, specified as Ohms

### `constant_impedance_BC_reac`
~~~
  double constant_impedance_BC_reac[Ohm];
~~~

Constant impedance delta-connected load on phase B, imaginary only, specified as Ohms

### `constant_impedance_CA_reac`
~~~
  double constant_impedance_CA_reac[Ohm];
~~~

Constant impedance delta-connected load on phase C, imaginary only, specified as Ohms

### `measured_voltage_A`
~~~
  complex measured_voltage_A;
~~~

Current measured voltage on phase A

### `measured_voltage_B`
~~~
  complex measured_voltage_B;
~~~

Current measured voltage on phase B

### `measured_voltage_C`
~~~
  complex measured_voltage_C;
~~~

Current measured voltage on phase C

### `measured_voltage_AB`
~~~
  complex measured_voltage_AB;
~~~

Current measured voltage on phases AB

### `measured_voltage_BC`
~~~
  complex measured_voltage_BC;
~~~

Current measured voltage on phases BC

### `measured_voltage_CA`
~~~
  complex measured_voltage_CA;
~~~

Current measured voltage on phases CA

### `phase_loss_protection`
~~~
  bool phase_loss_protection;
~~~

Trip all three phases of the load if a fault occurs

### `base_power_A`
~~~
  double base_power_A[VA];
~~~

In similar format as ZIPload, this represents the nominal power on phase A before applying ZIP fractions

### `base_power_B`
~~~
  double base_power_B[VA];
~~~

In similar format as ZIPload, this represents the nominal power on phase B before applying ZIP fractions

### `base_power_C`
~~~
  double base_power_C[VA];
~~~

In similar format as ZIPload, this represents the nominal power on phase C before applying ZIP fractions

### `power_pf_A`
~~~
  double power_pf_A[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase A constant power portion of load

### `current_pf_A`
~~~
  double current_pf_A[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase A constant current portion of load

### `impedance_pf_A`
~~~
  double impedance_pf_A[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase A constant impedance portion of load

### `power_pf_B`
~~~
  double power_pf_B[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase B constant power portion of load

### `current_pf_B`
~~~
  double current_pf_B[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase B constant current portion of load

### `impedance_pf_B`
~~~
  double impedance_pf_B[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase B constant impedance portion of load

### `power_pf_C`
~~~
  double power_pf_C[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase C constant power portion of load

### `current_pf_C`
~~~
  double current_pf_C[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase C constant current portion of load

### `impedance_pf_C`
~~~
  double impedance_pf_C[pu];
~~~

In similar format as ZIPload, this is the power factor of the phase C constant impedance portion of load

### `power_fraction_A`
~~~
  double power_fraction_A[pu];
~~~

This is the constant power fraction of base power on phase A

### `current_fraction_A`
~~~
  double current_fraction_A[pu];
~~~

This is the constant current fraction of base power on phase A

### `impedance_fraction_A`
~~~
  double impedance_fraction_A[pu];
~~~

This is the constant impedance fraction of base power on phase A

### `power_fraction_B`
~~~
  double power_fraction_B[pu];
~~~

This is the constant power fraction of base power on phase B

### `current_fraction_B`
~~~
  double current_fraction_B[pu];
~~~

This is the constant current fraction of base power on phase B

### `impedance_fraction_B`
~~~
  double impedance_fraction_B[pu];
~~~

This is the constant impedance fraction of base power on phase B

### `power_fraction_C`
~~~
  double power_fraction_C[pu];
~~~

This is the constant power fraction of base power on phase C

### `current_fraction_C`
~~~
  double current_fraction_C[pu];
~~~

This is the constant current fraction of base power on phase C

### `impedance_fraction_C`
~~~
  double impedance_fraction_C[pu];
~~~

This is the constant impedance fraction of base power on phase C

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
  object load {
    load_class "0";
    constant_power_A "0+0i";
    constant_power_B "0+0i";
    constant_power_C "0+0i";
    constant_power_A_real "0.0";
    constant_power_B_real "0.0";
    constant_power_C_real "0.0";
    constant_power_A_reac "0.0";
    constant_power_B_reac "0.0";
    constant_power_C_reac "0.0";
    constant_current_A "0+0i";
    constant_current_B "0+0i";
    constant_current_C "0+0i";
    constant_current_A_real "0.0";
    constant_current_B_real "0.0";
    constant_current_C_real "0.0";
    constant_current_A_reac "0.0";
    constant_current_B_reac "0.0";
    constant_current_C_reac "0.0";
    constant_impedance_A "0+0i";
    constant_impedance_B "0+0i";
    constant_impedance_C "0+0i";
    constant_impedance_A_real "0.0";
    constant_impedance_B_real "0.0";
    constant_impedance_C_real "0.0";
    constant_impedance_A_reac "0.0";
    constant_impedance_B_reac "0.0";
    constant_impedance_C_reac "0.0";
    constant_power_AN "0+0i";
    constant_power_BN "0+0i";
    constant_power_CN "0+0i";
    constant_power_AN_real "0.0";
    constant_power_BN_real "0.0";
    constant_power_CN_real "0.0";
    constant_power_AN_reac "0.0";
    constant_power_BN_reac "0.0";
    constant_power_CN_reac "0.0";
    constant_current_AN "0+0i";
    constant_current_BN "0+0i";
    constant_current_CN "0+0i";
    constant_current_AN_real "0.0";
    constant_current_BN_real "0.0";
    constant_current_CN_real "0.0";
    constant_current_AN_reac "0.0";
    constant_current_BN_reac "0.0";
    constant_current_CN_reac "0.0";
    constant_impedance_AN "0+0i";
    constant_impedance_BN "0+0i";
    constant_impedance_CN "0+0i";
    constant_impedance_AN_real "0.0";
    constant_impedance_BN_real "0.0";
    constant_impedance_CN_real "0.0";
    constant_impedance_AN_reac "0.0";
    constant_impedance_BN_reac "0.0";
    constant_impedance_CN_reac "0.0";
    constant_power_AB "0+0i";
    constant_power_BC "0+0i";
    constant_power_CA "0+0i";
    constant_power_AB_real "0.0";
    constant_power_BC_real "0.0";
    constant_power_CA_real "0.0";
    constant_power_AB_reac "0.0";
    constant_power_BC_reac "0.0";
    constant_power_CA_reac "0.0";
    constant_current_AB "0+0i";
    constant_current_BC "0+0i";
    constant_current_CA "0+0i";
    constant_current_AB_real "0.0";
    constant_current_BC_real "0.0";
    constant_current_CA_real "0.0";
    constant_current_AB_reac "0.0";
    constant_current_BC_reac "0.0";
    constant_current_CA_reac "0.0";
    constant_impedance_AB "0+0i";
    constant_impedance_BC "0+0i";
    constant_impedance_CA "0+0i";
    constant_impedance_AB_real "0.0";
    constant_impedance_BC_real "0.0";
    constant_impedance_CA_real "0.0";
    constant_impedance_AB_reac "0.0";
    constant_impedance_BC_reac "0.0";
    constant_impedance_CA_reac "0.0";
    measured_voltage_A "0+0i";
    measured_voltage_B "0+0i";
    measured_voltage_C "0+0i";
    measured_voltage_AB "0+0i";
    measured_voltage_BC "0+0i";
    measured_voltage_CA "0+0i";
    phase_loss_protection "FALSE";
    base_power_A "0.0";
    base_power_B "0.0";
    base_power_C "0.0";
    power_pf_A "0.0";
    current_pf_A "0.0";
    impedance_pf_A "0.0";
    power_pf_B "0.0";
    current_pf_B "0.0";
    impedance_pf_B "0.0";
    power_pf_C "0.0";
    current_pf_C "0.0";
    impedance_pf_C "0.0";
    power_fraction_A "0.0";
    current_fraction_A "0.0";
    impedance_fraction_A "0.0";
    power_fraction_B "0.0";
    current_fraction_B "0.0";
    impedance_fraction_B "0.0";
    power_fraction_C "0.0";
    current_fraction_C "0.0";
    impedance_fraction_C "0.0";
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

