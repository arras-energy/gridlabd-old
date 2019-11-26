[[/Module/Powerflow]] -- Module powerflow

# Synopsis
GLM:
~~~
  object motor {
    base_power "<decimal> W";
    n "<decimal>";
    Rds "<decimal> ohm";
    Rqs "<decimal> ohm";
    Rr "<decimal> ohm";
    Xm "<decimal> ohm";
    Xr "<decimal> ohm";
    Xc_run "<decimal> ohm";
    Xc_start "<decimal> ohm";
    Xd_prime "<decimal> ohm";
    Xq_prime "<decimal> ohm";
    A_sat "<decimal>";
    b_sat "<decimal>";
    H "<decimal>";
    To_prime "<decimal> s";
    capacitor_speed "<decimal> %";
    trip_time "<decimal> s";
    reconnect_time "<decimal> s";
    mechanical_torque "<decimal>";
    iteration_count "<decimal>";
    delta_mode_voltage_trigger "<decimal> %";
    delta_mode_rotor_speed_trigger "<decimal> %";
    delta_mode_voltage_exit "<decimal> %";
    delta_mode_rotor_speed_exit "<decimal> %";
    maximum_speed_error "<decimal>";
    wr "<decimal>";
    motor_status "{OFF,TRIPPED,STALLED,RUNNING}";
    motor_status_number "<integer>";
    motor_override "{OFF,ON}";
    motor_operation_type "{THREE-PHASE,SINGLE-PHASE}";
    triplex_connection_type "{TRIPLEX_12,TRIPLEX_2N,TRIPLEX_1N}";
    wb "<decimal> rad/s";
    ws "<decimal>";
    psi_b "<string>";
    psi_f "<string>";
    psi_dr "<string>";
    psi_qr "<string>";
    Ids "<string>";
    Iqs "<string>";
    If "<string>";
    Ib "<string>";
    Is "<string>";
    Ss "<string>";
    electrical_torque "<decimal>";
    Vs "<string>";
    motor_trip "<string>";
    trip "<decimal>";
    reconnect "<decimal>";
    rs "<decimal> pu";
    rr "<decimal> pu";
    lm "<decimal> pu";
    lls "<decimal> pu";
    llr "<decimal> pu";
    TPIM_rated_mechanical_Load_torque "<decimal>";
    friction_coefficient "<decimal>";
    TPIM_initial_status "{STATIONARY,RUNNING}";
    wsyn "<decimal>";
    phips "<string>";
    phins_cj "<string>";
    phipr "<string>";
    phinr_cj "<string>";
    omgr0 "<decimal>";
    TL "<decimal>";
    Ias "<string>";
    Ibs "<string>";
    Ics "<string>";
    Smt "<string>";
    Vas "<string>";
    Vbs "<string>";
    Vcs "<string>";
    Ips "<string>";
    Ipr "<string>";
    Ins_cj "<string>";
    Inr_cj "<string>";
    Ls "<decimal>";
    Lr "<decimal>";
    sigma1 "<decimal>";
    sigma2 "<decimal>";
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

### `base_power`
~~~
  double base_power[W];
~~~

Base power

### `n`
~~~
  double n;
~~~

Ratio of stator auxiliary windings to stator main windings

### `Rds`
~~~
  double Rds[ohm];
~~~

D-axis resistance

### `Rqs`
~~~
  double Rqs[ohm];
~~~

Q-asis resistance

### `Rr`
~~~
  double Rr[ohm];
~~~

Rotor resistance

### `Xm`
~~~
  double Xm[ohm];
~~~

Magnetizing reactance

### `Xr`
~~~
  double Xr[ohm];
~~~

Rotor reactance

### `Xc_run`
~~~
  double Xc_run[ohm];
~~~

Running capacitor reactance

### `Xc_start`
~~~
  double Xc_start[ohm];
~~~

Starting capacitor reactance

### `Xd_prime`
~~~
  double Xd_prime[ohm];
~~~

D-axis reactance

### `Xq_prime`
~~~
  double Xq_prime[ohm];
~~~

Q-axis reactance

### `A_sat`
~~~
  double A_sat;
~~~

Flux saturation parameter, A

### `b_sat`
~~~
  double b_sat;
~~~

Flux saturation parameter, b

### `H`
~~~
  double H;
~~~

Moment of inertia

### `To_prime`
~~~
  double To_prime[s];
~~~

Rotor time constant

### `capacitor_speed`
~~~
  double capacitor_speed[%];
~~~

Percentage speed of nominal when starting capacitor kicks in

### `trip_time`
~~~
  double trip_time[s];
~~~

Time motor can stay stalled before tripping off 

### `reconnect_time`
~~~
  double reconnect_time[s];
~~~

Time before tripped motor reconnects

### `mechanical_torque`
~~~
  double mechanical_torque;
~~~

Mechanical torque applied to the motor

### `iteration_count`
~~~
  double iteration_count;
~~~

Maximum number of iterations for steady state model

### `delta_mode_voltage_trigger`
~~~
  double delta_mode_voltage_trigger[%];
~~~

Percentage voltage of nominal when delta mode is triggered

### `delta_mode_rotor_speed_trigger`
~~~
  double delta_mode_rotor_speed_trigger[%];
~~~

Percentage speed of nominal when delta mode is triggered

### `delta_mode_voltage_exit`
~~~
  double delta_mode_voltage_exit[%];
~~~

Percentage voltage of nominal to exit delta mode

### `delta_mode_rotor_speed_exit`
~~~
  double delta_mode_rotor_speed_exit[%];
~~~

Percentage speed of nominal to exit delta mode

### `maximum_speed_error`
~~~
  double maximum_speed_error;
~~~

Maximum speed error for the steady state model

### `wr`
~~~
  double wr;
~~~

Rotor speed

### `motor_status`
~~~
  enumeration {OFF, TRIPPED, STALLED, RUNNING} motor_status;
~~~

The current status of the motor

### `motor_status_number`
~~~
  int32 motor_status_number;
~~~

The current status of the motor as an integer

### `motor_override`
~~~
  enumeration {OFF, ON} motor_override;
~~~

Override function to dictate if motor is turned off or on

### `motor_operation_type`
~~~
  enumeration {THREE-PHASE, SINGLE-PHASE} motor_operation_type;
~~~

Current operation type of the motor - deltamode related

### `triplex_connection_type`
~~~
  enumeration {TRIPLEX_12, TRIPLEX_2N, TRIPLEX_1N} triplex_connection_type;
~~~

Describes how the motor will connect to the triplex devices

### `wb`
~~~
  double wb[rad/s];
~~~

Base speed

### `ws`
~~~
  double ws;
~~~

System speed

### `psi_b`
~~~
  complex psi_b;
~~~

Backward rotating flux

### `psi_f`
~~~
  complex psi_f;
~~~

Forward rotating flux

### `psi_dr`
~~~
  complex psi_dr;
~~~

Rotor d axis flux

### `psi_qr`
~~~
  complex psi_qr;
~~~

Rotor q axis flux

### `Ids`
~~~
  complex Ids;
~~~

Time before tripped motor reconnects

### `Iqs`
~~~
  complex Iqs;
~~~

Time before tripped motor reconnects

### `If`
~~~
  complex If;
~~~

Forward current

### `Ib`
~~~
  complex Ib;
~~~

Backward current

### `Is`
~~~
  complex Is;
~~~

Motor current

### `Ss`
~~~
  complex Ss;
~~~

Motor power

### `electrical_torque`
~~~
  double electrical_torque;
~~~

Electrical torque

### `Vs`
~~~
  complex Vs;
~~~

Motor voltage

### `motor_trip`
~~~
  bool motor_trip;
~~~

Boolean variable to check if motor is tripped

### `trip`
~~~
  double trip;
~~~

Current time in tripped state

### `reconnect`
~~~
  double reconnect;
~~~

Current time since motor was tripped

### `rs`
~~~
  double rs[pu];
~~~

Stator resistance

### `rr`
~~~
  double rr[pu];
~~~

Rotor resistance

### `lm`
~~~
  double lm[pu];
~~~

Magnetizing reactance

### `lls`
~~~
  double lls[pu];
~~~

Stator leakage reactance

### `llr`
~~~
  double llr[pu];
~~~

Rotor leakage reactance

### `TPIM_rated_mechanical_Load_torque`
~~~
  double TPIM_rated_mechanical_Load_torque;
~~~

Rated mechanical load torque applied to three-phase induction motor

### `friction_coefficient`
~~~
  double friction_coefficient;
~~~

Coefficient of speed-dependent torque

### `TPIM_initial_status`
~~~
  enumeration {STATIONARY, RUNNING} TPIM_initial_status;
~~~

Initial status of three-phase induction motor: RUNNING or STATIONARY

### `wsyn`
~~~
  double wsyn;
~~~

System frequency in pu

### `phips`
~~~
  complex phips;
~~~

Positive sequence stator flux

### `phins_cj`
~~~
  complex phins_cj;
~~~

Conjugate of negative sequence stator flux

### `phipr`
~~~
  complex phipr;
~~~

Positive sequence rotor flux

### `phinr_cj`
~~~
  complex phinr_cj;
~~~

Conjugate of negative sequence rotor flux

### `omgr0`
~~~
  double omgr0;
~~~

Dc component of rotor speed

### `TL`
~~~
  double TL;
~~~

Actually applied mechanical torque

### `Ias`
~~~
  complex Ias;
~~~

Motor phase-a stator current

### `Ibs`
~~~
  complex Ibs;
~~~

Motor phase-b stator current

### `Ics`
~~~
  complex Ics;
~~~

Motor phase-c stator current

### `Smt`
~~~
  complex Smt;
~~~

Motor complex power

### `Vas`
~~~
  complex Vas;
~~~

Motor phase-a stator-to-ground voltage

### `Vbs`
~~~
  complex Vbs;
~~~

Motor phase-b stator-to-ground voltage

### `Vcs`
~~~
  complex Vcs;
~~~

Motor phase-c stator-to-ground voltage

### `Ips`
~~~
  complex Ips;
~~~

Positive sequence stator current

### `Ipr`
~~~
  complex Ipr;
~~~

Positive sequence rotor current

### `Ins_cj`
~~~
  complex Ins_cj;
~~~

Conjugate of negative sequence stator current

### `Inr_cj`
~~~
  complex Inr_cj;
~~~

Conjugate of negative sequence rotor current

### `Ls`
~~~
  double Ls;
~~~

Stator synchronous reactance

### `Lr`
~~~
  double Lr;
~~~

Rotor synchronous reactance

### `sigma1`
~~~
  double sigma1;
~~~

Intermediate variable 1 associated with synch. react.

### `sigma2`
~~~
  double sigma2;
~~~

Intermediate variable 2 associated with synch. react.

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
  object motor {
    base_power "0.0";
    n "0.0";
    Rds "0.0";
    Rqs "0.0";
    Rr "0.0";
    Xm "0.0";
    Xr "0.0";
    Xc_run "0.0";
    Xc_start "0.0";
    Xd_prime "0.0";
    Xq_prime "0.0";
    A_sat "0.0";
    b_sat "0.0";
    H "0.0";
    To_prime "0.0";
    capacitor_speed "0.0";
    trip_time "0.0";
    reconnect_time "0.0";
    mechanical_torque "0.0";
    iteration_count "0.0";
    delta_mode_voltage_trigger "0.0";
    delta_mode_rotor_speed_trigger "0.0";
    delta_mode_voltage_exit "0.0";
    delta_mode_rotor_speed_exit "0.0";
    maximum_speed_error "0.0";
    wr "0.0";
    motor_status "0";
    motor_status_number "0";
    motor_override "0";
    motor_operation_type "0";
    triplex_connection_type "0";
    wb "0.0";
    ws "0.0";
    psi_b "0+0i";
    psi_f "0+0i";
    psi_dr "0+0i";
    psi_qr "0+0i";
    Ids "0+0i";
    Iqs "0+0i";
    If "0+0i";
    Ib "0+0i";
    Is "0+0i";
    Ss "0+0i";
    electrical_torque "0.0";
    Vs "0+0i";
    motor_trip "FALSE";
    trip "0.0";
    reconnect "0.0";
    rs "0.0";
    rr "0.0";
    lm "0.0";
    lls "0.0";
    llr "0.0";
    TPIM_rated_mechanical_Load_torque "0.0";
    friction_coefficient "0.0";
    TPIM_initial_status "0";
    wsyn "0.0";
    phips "0+0i";
    phins_cj "0+0i";
    phipr "0+0i";
    phinr_cj "0+0i";
    omgr0 "0.0";
    TL "0.0";
    Ias "0+0i";
    Ibs "0+0i";
    Ics "0+0i";
    Smt "0+0i";
    Vas "0+0i";
    Vbs "0+0i";
    Vcs "0+0i";
    Ips "0+0i";
    Ipr "0+0i";
    Ins_cj "0+0i";
    Inr_cj "0+0i";
    Ls "0.0";
    Lr "0.0";
    sigma1 "0.0";
    sigma2 "0.0";
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

