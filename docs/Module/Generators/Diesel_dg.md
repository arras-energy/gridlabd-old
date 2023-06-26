[[/Module/Generators/Diesel_dg]] -- Class diesel_dg

# Synopsis

GLM:

~~~
  object diesel_dg {
    Gen_mode "{CONSTANTP,CONSTANTPQ,CONSTANTE,UNKNOWN}";
    Gen_status "{ONLINE,OFFLINE,UNKNOWN}";
    Gen_type "{DYN_SYNCHRONOUS,SYNCHRONOUS,INDUCTION}";
    pf "<decimal>";
    GenElecEff "<decimal>";
    TotalOutputPow "<string> VA";
    TotalRealPow "<decimal> W";
    TotalReacPow "<decimal> VAr";
    speed "<decimal> 1/min";
    cylinders "<decimal>";
    stroke "<decimal>";
    torque "<decimal> N.m";
    pressure "<decimal> N/m^2";
    time_operation "<decimal> min";
    fuel "<decimal> kg";
    w_coolingwater "<decimal> kg";
    inlet_temperature "<decimal> degC";
    outlet_temperature "<decimal> degC";
    air_fuel "<decimal> kg";
    room_temperature "<decimal> degC";
    exhaust_temperature "<decimal> degC";
    cylinder_length "<decimal> m";
    cylinder_radius "<decimal> m";
    brake_diameter "<decimal> m";
    calotific_fuel "<decimal> kJ/kg";
    steam_exhaust "<decimal> kg";
    specific_heat_steam "<decimal> kJ/kg/K";
    specific_heat_dry "<decimal> kJ/kg/K";
    indicated_hp "<decimal> W";
    brake_hp "<decimal> W";
    thermal_efficiency "<decimal>";
    energy_supplied "<decimal> kJ";
    heat_equivalent_ip "<decimal> kJ";
    energy_coolingwater "<decimal> kJ";
    mass_exhaustgas "<decimal> kg";
    energy_exhaustgas "<decimal> kJ";
    energy_steam "<decimal> kJ";
    total_energy_exhaustgas "<decimal> kJ";
    unaccounted_energyloss "<decimal> kJ";
    Pconv "<decimal> kW";
    Rated_V "<decimal> V";
    Rated_VA "<decimal> VA";
    power_out_A "<string> VA";
    power_out_B "<string> VA";
    power_out_C "<string> VA";
    Rs "<decimal>";
    Xs "<decimal>";
    Rg "<decimal>";
    Xg "<decimal>";
    voltage_A "<string> V";
    voltage_B "<string> V";
    voltage_C "<string> V";
    current_A "<string> A";
    current_B "<string> A";
    current_C "<string> A";
    EfA "<string> V";
    EfB "<string> V";
    EfC "<string> V";
    omega_ref "<decimal> rad/s";
    inertia "<decimal>";
    damping "<decimal>";
    number_poles "<decimal>";
    Ra "<decimal> pu";
    Xd "<decimal> pu";
    Xq "<decimal> pu";
    Xdp "<decimal> pu";
    Xqp "<decimal> pu";
    Xdpp "<decimal> pu";
    Xqpp "<decimal> pu";
    Xl "<decimal> pu";
    Tdp "<decimal> s";
    Tdop "<decimal> s";
    Tqop "<decimal> s";
    Tdopp "<decimal> s";
    Tqopp "<decimal> s";
    Ta "<decimal> s";
    X0 "<string> pu";
    X2 "<string> pu";
    rotor_speed_convergence "<decimal> rad";
    voltage_convergence "<decimal> V";
    rotor_speed_convergence_enabled "<string>";
    voltage_magnitude_convergence_enabled "<string>";
    rotor_angle "<decimal> rad";
    rotor_speed "<decimal> rad/s";
    field_voltage "<decimal> pu";
    flux1d "<decimal> pu";
    flux2q "<decimal> pu";
    EpRotated "<string> pu";
    VintRotated "<string> pu";
    Eint_A "<string> V";
    Eint_B "<string> V";
    Eint_C "<string> V";
    Irotated "<string> pu";
    pwr_electric "<string> VA";
    pwr_mech "<decimal> W";
    torque_mech "<decimal> N*m";
    torque_elec "<decimal> N*m";
    wref "<decimal> pu";
    vset "<decimal> pu";
    Pref "<decimal> pu";
    Qref "<decimal> pu";
    Exciter_type "{SEXS,NO_EXC}";
    KA "<decimal> pu";
    TA "<decimal> s";
    TB "<decimal> s";
    TC "<decimal> s";
    EMAX "<decimal> pu";
    EMIN "<decimal> pu";
    Vterm_max "<decimal> pu";
    Vterm_min "<decimal> pu";
    bias "<decimal>";
    xe "<decimal>";
    xb "<decimal>";
    x_cvr1 "<decimal>";
    x_cvr2 "<decimal>";
    Vref "<decimal>";
    CVR_mode "{Feedback,HighOrder}";
    P_CONSTANT_ki "<decimal>";
    P_CONSTANT_kp "<decimal>";
    Exciter_Q_constant_mode "<string>";
    Exciter_Q_constant_ki "<decimal>";
    Exciter_Q_constant_kp "<decimal>";
    P_CONSTANT_Pref "<decimal> pu";
    Exciter_Q_constant_Qref "<decimal> pu";
    CVR_enabled "<string>";
    CVR_ki_cvr "<decimal>";
    CVR_kp_cvr "<decimal>";
    CVR_kd_cvr "<decimal>";
    CVR_kt_cvr "<decimal>";
    CVR_kw_cvr "<decimal>";
    CVR_PI "<string>";
    CVR_PID "<string>";
    vset_EMAX "<decimal>";
    vset_EMIN "<decimal>";
    CVR_Kd1 "<decimal>";
    CVR_Kd2 "<decimal>";
    CVR_Kd3 "<decimal>";
    CVR_Kn1 "<decimal>";
    CVR_Kn2 "<decimal>";
    vset_delta_MAX "<decimal>";
    vset_delta_MIN "<decimal>";
    vadd "<decimal>";
    vadd_a "<decimal>";
    Governor_type "{P_CONSTANT,GGOV1,GGOV1_OLD,GAST,DEGOV1,NO_GOV}";
    DEGOV1_R "<decimal> pu";
    DEGOV1_T1 "<decimal> s";
    DEGOV1_T2 "<decimal> s";
    DEGOV1_T3 "<decimal> s";
    DEGOV1_T4 "<decimal> s";
    DEGOV1_T5 "<decimal> s";
    DEGOV1_T6 "<decimal> s";
    DEGOV1_K "<decimal> pu";
    DEGOV1_TMAX "<decimal> pu";
    DEGOV1_TMIN "<decimal> pu";
    DEGOV1_TD "<decimal> s";
    DEGOV1_x1 "<decimal>";
    DEGOV1_x2 "<decimal>";
    DEGOV1_x4 "<decimal>";
    DEGOV1_x5 "<decimal>";
    DEGOV1_x6 "<decimal>";
    DEGOV1_throttle "<decimal>";
    GAST_R "<decimal> pu";
    GAST_T1 "<decimal> s";
    GAST_T2 "<decimal> s";
    GAST_T3 "<decimal> s";
    GAST_AT "<decimal> s";
    GAST_KT "<decimal> pu";
    GAST_VMAX "<decimal> pu";
    GAST_VMIN "<decimal> pu";
    GAST_x1 "<decimal>";
    GAST_x2 "<decimal>";
    GAST_x3 "<decimal>";
    GAST_throttle "<decimal>";
    GGOV1_R "<decimal> pu";
    GGOV1_Rselect "<integer>";
    GGOV1_Tpelec "<decimal> s";
    GGOV1_maxerr "<decimal>";
    GGOV1_minerr "<decimal>";
    GGOV1_Kpgov "<decimal>";
    GGOV1_Kigov "<decimal>";
    GGOV1_Kdgov "<decimal>";
    GGOV1_Tdgov "<decimal> s";
    GGOV1_vmax "<decimal>";
    GGOV1_vmin "<decimal>";
    GGOV1_Tact "<decimal>";
    GGOV1_Kturb "<decimal>";
    GGOV1_wfnl "<decimal> pu";
    GGOV1_Tb "<decimal> s";
    GGOV1_Tc "<decimal> s";
    GGOV1_Fuel_lag "<integer>";
    GGOV1_Teng "<decimal>";
    GGOV1_Tfload "<decimal>";
    GGOV1_Kpload "<decimal>";
    GGOV1_Kiload "<decimal>";
    GGOV1_Ldref "<decimal> pu";
    GGOV1_Dm "<decimal> pu";
    GGOV1_ropen "<decimal> pu/s";
    GGOV1_rclose "<decimal> pu/s";
    GGOV1_Kimw "<decimal>";
    GGOV1_Pmwset "<decimal> MW";
    GGOV1_aset "<decimal> pu/s";
    GGOV1_Ka "<decimal>";
    GGOV1_Ta "<decimal> s";
    GGOV1_db "<decimal>";
    GGOV1_Tsa "<decimal> s";
    GGOV1_Tsb "<decimal> s";
    GGOV1_Load_Limit_enable "<string>";
    GGOV1_Acceleration_Limit_enable "<string>";
    GGOV1_PID_enable "<string>";
    GGOV1_fsrt "<decimal>";
    GGOV1_fsra "<decimal>";
    GGOV1_fsrn "<decimal>";
    GGOV1_speed_error "<decimal> pu";
    GGOV1_x1 "<decimal>";
    GGOV1_x2 "<decimal>";
    GGOV1_x2a "<decimal>";
    GGOV1_x3 "<decimal>";
    GGOV1_x3a "<decimal>";
    GGOV1_x4 "<decimal>";
    GGOV1_x4a "<decimal>";
    GGOV1_x4b "<decimal>";
    GGOV1_x5 "<decimal>";
    GGOV1_x5a "<decimal>";
    GGOV1_x5b "<decimal>";
    GGOV1_x6 "<decimal>";
    GGOV1_x7 "<decimal>";
    GGOV1_x7a "<decimal>";
    GGOV1_x8 "<decimal>";
    GGOV1_x8a "<decimal>";
    GGOV1_x9 "<decimal>";
    GGOV1_x9a "<decimal>";
    GGOV1_x10 "<decimal>";
    GGOV1_x10a "<decimal>";
    GGOV1_x10b "<decimal>";
    GGOV1_ValveStroke "<decimal>";
    GGOV1_FuelFlow "<decimal>";
    GGOV1_GovOutPut "<decimal>";
    GGOV1_RselectValue "<decimal>";
    GGOV1_fsrtNoLim "<decimal>";
    GGOV1_err2 "<decimal>";
    GGOV1_err2a "<decimal>";
    GGOV1_err3 "<decimal>";
    GGOV1_err4 "<decimal>";
    GGOV1_err7 "<decimal>";
    GGOV1_LowValSelect1 "<decimal>";
    GGOV1_LowValSelect "<decimal>";
    P_CONSTANT_Tpelec "<decimal> s";
    P_CONSTANT_Tact "<decimal>";
    P_CONSTANT_Kturb "<decimal>";
    P_CONSTANT_wfnl "<decimal> pu";
    P_CONSTANT_Tb "<decimal> s";
    P_CONSTANT_Tc "<decimal> s";
    P_CONSTANT_Teng "<decimal>";
    P_CONSTANT_ropen "<decimal> pu/s";
    P_CONSTANT_rclose "<decimal> pu/s";
    P_CONSTANT_Kimw "<decimal>";
    P_CONSTANT_x1 "<decimal>";
    P_CONSTANT_x4 "<decimal>";
    P_CONSTANT_x4a "<decimal>";
    P_CONSTANT_x4b "<decimal>";
    P_CONSTANT_x5 "<decimal>";
    P_CONSTANT_x5a "<decimal>";
    P_CONSTANT_x5b "<decimal>";
    P_CONSTANT_x_Pconstant "<decimal>";
    P_CONSTANT_err4 "<decimal>";
    P_CONSTANT_ValveStroke "<decimal>";
    P_CONSTANT_FuelFlow "<decimal>";
    P_CONSTANT_GovOutPut "<decimal>";
    fuelEmissionCal "<string>";
    outputEnergy "<decimal>";
    FuelUse "<decimal>";
    efficiency "<decimal>";
    CO2_emission "<decimal>";
    SOx_emission "<decimal>";
    NOx_emission "<decimal>";
    PM10_emission "<decimal>";
    frequency_deviation "<decimal>";
    frequency_deviation_energy "<decimal>";
    frequency_deviation_max "<decimal>";
    realPowerChange "<decimal>";
    ratio_f_p "<decimal>";
    phases "[SNCBA]";
  }
~~~

# Description

TODO

## Properties

### `Gen_mode`

~~~
  enumeration {CONSTANTP, CONSTANTPQ, CONSTANTE, UNKNOWN} Gen_mode;
~~~

TODO

### `Gen_status`

~~~
  enumeration {ONLINE, OFFLINE, UNKNOWN} Gen_status;
~~~

TODO

### `Gen_type`

~~~
  enumeration {DYN_SYNCHRONOUS, SYNCHRONOUS, INDUCTION} Gen_type;
~~~

Dynamics-capable implementation of synchronous diesel generator

### `pf`

~~~
  double pf;
~~~

Desired power factor

### `GenElecEff`

~~~
  double GenElecEff;
~~~

Calculated electrical efficiency of generator

### `TotalOutputPow`

~~~
  complex TotalOutputPow[VA];
~~~

Total complex power generated

### `TotalRealPow`

~~~
  double TotalRealPow[W];
~~~

Total real power generated

### `TotalReacPow`

~~~
  double TotalReacPow[VAr];
~~~

Total reactive power generated

### `speed`

~~~
  double speed[1/min];
~~~

Speed of an engine

### `cylinders`

~~~
  double cylinders;
~~~

Total number of cylinders in a diesel engine

### `stroke`

~~~
  double stroke;
~~~

Category of internal combustion engines

### `torque`

~~~
  double torque[N.m];
~~~

Net brake load

### `pressure`

~~~
  double pressure[N/m^2];
~~~


### `time_operation`

~~~
  double time_operation[min];
~~~


### `fuel`

~~~
  double fuel[kg];
~~~

Fuel consumption

### `w_coolingwater`

~~~
  double w_coolingwater[kg];
~~~

Weight of cooling water supplied per minute

### `inlet_temperature`

~~~
  double inlet_temperature[degC];
~~~

Inlet temperature of cooling water in degC

### `outlet_temperature`

~~~
  double outlet_temperature[degC];
~~~

Outlet temperature of cooling water in degC

### `air_fuel`

~~~
  double air_fuel[kg];
~~~

Air used per kg fuel

### `room_temperature`

~~~
  double room_temperature[degC];
~~~

Room temperature in degC

### `exhaust_temperature`

~~~
  double exhaust_temperature[degC];
~~~

Exhaust gas temperature in degC

### `cylinder_length`

~~~
  double cylinder_length[m];
~~~

Length of the cylinder, used in efficiency calculations

### `cylinder_radius`

~~~
  double cylinder_radius[m];
~~~

Inner radius of cylinder, used in efficiency calculations

### `brake_diameter`

~~~
  double brake_diameter[m];
~~~

Diameter of brake, used in efficiency calculations

### `calotific_fuel`

~~~
  double calotific_fuel[kJ/kg];
~~~

Calorific value of fuel

### `steam_exhaust`

~~~
  double steam_exhaust[kg];
~~~

Steam formed per kg of fuel in the exhaust

### `specific_heat_steam`

~~~
  double specific_heat_steam[kJ/kg/K];
~~~

Specific heat of steam in exhaust

### `specific_heat_dry`

~~~
  double specific_heat_dry[kJ/kg/K];
~~~

Specific heat of dry exhaust gases

### `indicated_hp`

~~~
  double indicated_hp[W];
~~~

Indicated horse power is the power developed inside the cylinder

### `brake_hp`

~~~
  double brake_hp[W];
~~~

Brake horse power is the output of the engine at the shaft measured by a dynamometer

### `thermal_efficiency`

~~~
  double thermal_efficiency;
~~~

Thermal efficiency or mechanical efiiciency of the engine is efined as bp/ip

### `energy_supplied`

~~~
  double energy_supplied[kJ];
~~~

Energy supplied during the trail

### `heat_equivalent_ip`

~~~
  double heat_equivalent_ip[kJ];
~~~

Heat equivalent of IP in a given time of operation

### `energy_coolingwater`

~~~
  double energy_coolingwater[kJ];
~~~

Energy carried away by cooling water

### `mass_exhaustgas`

~~~
  double mass_exhaustgas[kg];
~~~

Mass of dry exhaust gas

### `energy_exhaustgas`

~~~
  double energy_exhaustgas[kJ];
~~~

Energy carried away by dry exhaust gases

### `energy_steam`

~~~
  double energy_steam[kJ];
~~~

Energy carried away by steam

### `total_energy_exhaustgas`

~~~
  double total_energy_exhaustgas[kJ];
~~~

Total energy carried away by dry exhaust gases is the sum of energy carried away bt steam and energy carried away by dry exhaust gases

### `unaccounted_energyloss`

~~~
  double unaccounted_energyloss[kJ];
~~~

Unaccounted for energy loss

### `Pconv`

~~~
  double Pconv[kW];
~~~

Converted power = Mechanical input - (F & W loasses + Stray losses + Core losses)

### `Rated_V`

~~~
  double Rated_V[V];
~~~

Nominal line-line voltage in Volts

### `Rated_VA`

~~~
  double Rated_VA[VA];
~~~

Nominal capacity in VA

### `power_out_A`

~~~
  complex power_out_A[VA];
~~~

Output power of phase A

### `power_out_B`

~~~
  complex power_out_B[VA];
~~~

Output power of phase B

### `power_out_C`

~~~
  complex power_out_C[VA];
~~~

Output power of phase C

### `Rs`

~~~
  double Rs;
~~~

Internal transient resistance in p.u.

### `Xs`

~~~
  double Xs;
~~~

Internal transient impedance in p.u.

### `Rg`

~~~
  double Rg;
~~~

Grounding resistance in p.u.

### `Xg`

~~~
  double Xg;
~~~

Grounding impedance in p.u.

### `voltage_A`

~~~
  complex voltage_A[V];
~~~

Voltage at generator terminal, phase A

### `voltage_B`

~~~
  complex voltage_B[V];
~~~

Voltage at generator terminal, phase B

### `voltage_C`

~~~
  complex voltage_C[V];
~~~

Voltage at generator terminal, phase C

### `current_A`

~~~
  complex current_A[A];
~~~

Current generated at generator terminal, phase A

### `current_B`

~~~
  complex current_B[A];
~~~

Current generated at generator terminal, phase B

### `current_C`

~~~
  complex current_C[A];
~~~

Current generated at generator terminal, phase C

### `EfA`

~~~
  complex EfA[V];
~~~

Induced voltage on phase A

### `EfB`

~~~
  complex EfB[V];
~~~

Induced voltage on phase B

### `EfC`

~~~
  complex EfC[V];
~~~

Induced voltage on phase C

### `omega_ref`

~~~
  double omega_ref[rad/s];
~~~

Reference frequency of generator (rad/s)

### `inertia`

~~~
  double inertia;
~~~

Inertial constant (H) of generator

### `damping`

~~~
  double damping;
~~~

Damping constant (D) of generator

### `number_poles`

~~~
  double number_poles;
~~~

Number of poles in the generator

### `Ra`

~~~
  double Ra[pu];
~~~

Stator resistance (p.u.)

### `Xd`

~~~
  double Xd[pu];
~~~

D-axis reactance (p.u.)

### `Xq`

~~~
  double Xq[pu];
~~~

Q-axis reactance (p.u.)

### `Xdp`

~~~
  double Xdp[pu];
~~~

D-axis transient reactance (p.u.)

### `Xqp`

~~~
  double Xqp[pu];
~~~

Q-axis transient reactance (p.u.)

### `Xdpp`

~~~
  double Xdpp[pu];
~~~

D-axis subtransient reactance (p.u.)

### `Xqpp`

~~~
  double Xqpp[pu];
~~~

Q-axis subtransient reactance (p.u.)

### `Xl`

~~~
  double Xl[pu];
~~~

Leakage reactance (p.u.)

### `Tdp`

~~~
  double Tdp[s];
~~~

D-axis short circuit time constant (s)

### `Tdop`

~~~
  double Tdop[s];
~~~

D-axis open circuit time constant (s)

### `Tqop`

~~~
  double Tqop[s];
~~~

Q-axis open circuit time constant (s)

### `Tdopp`

~~~
  double Tdopp[s];
~~~

D-axis open circuit subtransient time constant (s)

### `Tqopp`

~~~
  double Tqopp[s];
~~~

Q-axis open circuit subtransient time constant (s)

### `Ta`

~~~
  double Ta[s];
~~~

Armature short-circuit time constant (s)

### `X0`

~~~
  complex X0[pu];
~~~

Zero sequence impedance (p.u.)

### `X2`

~~~
  complex X2[pu];
~~~

Negative sequence impedance (p.u.)

### `rotor_speed_convergence`

~~~
  double rotor_speed_convergence[rad];
~~~

Convergence criterion on rotor speed used to determine when to exit deltamode

### `voltage_convergence`

~~~
  double voltage_convergence[V];
~~~

Convergence criterion for voltage changes (if exciter present) to determine when to exit deltamode

### `rotor_speed_convergence_enabled`

~~~
  bool rotor_speed_convergence_enabled;
~~~

Uses rotor_speed_convergence to determine if an exit of deltamode is needed

### `voltage_magnitude_convergence_enabled`

~~~
  bool voltage_magnitude_convergence_enabled;
~~~

Uses voltage_convergence to determine if an exit of deltamode is needed - only works if an exciter is present

### `rotor_angle`

~~~
  double rotor_angle[rad];
~~~

Rotor angle state variable

### `rotor_speed`

~~~
  double rotor_speed[rad/s];
~~~

Machine speed state variable

### `field_voltage`

~~~
  double field_voltage[pu];
~~~

Machine field voltage state variable

### `flux1d`

~~~
  double flux1d[pu];
~~~

Machine transient flux on d-axis state variable

### `flux2q`

~~~
  double flux2q[pu];
~~~

Machine subtransient flux on q-axis state variable

### `EpRotated`

~~~
  complex EpRotated[pu];
~~~

D-q rotated E-prime internal voltage state variable

### `VintRotated`

~~~
  complex VintRotated[pu];
~~~

D-q rotated Vint voltage state variable

### `Eint_A`

~~~
  complex Eint_A[V];
~~~

Unrotated, unsequenced phase A internal voltage

### `Eint_B`

~~~
  complex Eint_B[V];
~~~

Unrotated, unsequenced phase B internal voltage

### `Eint_C`

~~~
  complex Eint_C[V];
~~~

Unrotated, unsequenced phase C internal voltage

### `Irotated`

~~~
  complex Irotated[pu];
~~~

D-q rotated sequence current state variable

### `pwr_electric`

~~~
  complex pwr_electric[VA];
~~~

Current electrical output of machine

### `pwr_mech`

~~~
  double pwr_mech[W];
~~~

Current mechanical output of machine

### `torque_mech`

~~~
  double torque_mech[N*m];
~~~

Current mechanical torque of machine

### `torque_elec`

~~~
  double torque_elec[N*m];
~~~

Current electrical torque output of machine

### `wref`

~~~
  double wref[pu];
~~~

Wref input to governor controls (per-unit)

### `vset`

~~~
  double vset[pu];
~~~

Vset input to AVR controls (per-unit)

### `Pref`

~~~
  double Pref[pu];
~~~

Pref input to governor controls (per-unit), if supported

### `Qref`

~~~
  double Qref[pu];
~~~

Qref input to govornor or AVR controls (per-unit), if supported

### `Exciter_type`

~~~
  enumeration {SEXS, NO_EXC} Exciter_type;
~~~

Simplified Excitation System

### `KA`

~~~
  double KA[pu];
~~~

Exciter gain (p.u.)

### `TA`

~~~
  double TA[s];
~~~

Exciter time constant (seconds)

### `TB`

~~~
  double TB[s];
~~~

Exciter transient gain reduction time constant (seconds)

### `TC`

~~~
  double TC[s];
~~~

Exciter transient gain reduction time constant (seconds)

### `EMAX`

~~~
  double EMAX[pu];
~~~

Exciter upper limit (p.u.)

### `EMIN`

~~~
  double EMIN[pu];
~~~

Exciter lower limit (p.u.)

### `Vterm_max`

~~~
  double Vterm_max[pu];
~~~

Upper voltage limit for super-second (p.u.)

### `Vterm_min`

~~~
  double Vterm_min[pu];
~~~

Lower voltage limit for super-second (p.u.)

### `bias`

~~~
  double bias;
~~~

Exciter bias state variable

### `xe`

~~~
  double xe;
~~~

Exciter state variable

### `xb`

~~~
  double xb;
~~~

Exciter state variable

### `x_cvr1`

~~~
  double x_cvr1;
~~~

Exciter state variable

### `x_cvr2`

~~~
  double x_cvr2;
~~~

Exciter state variable

### `Vref`

~~~
  double Vref;
~~~

Exciter CVR control voltage reference value

### `CVR_mode`

~~~
  enumeration {Feedback, HighOrder} CVR_mode;
~~~

First order control mode with feedback loop

### `P_CONSTANT_ki`

~~~
  double P_CONSTANT_ki;
~~~

Parameter of the integration control for constant P mode

### `P_CONSTANT_kp`

~~~
  double P_CONSTANT_kp;
~~~

Parameter of the proportional control for constant P mode

### `Exciter_Q_constant_mode`

~~~
  bool Exciter_Q_constant_mode;
~~~

True if the generator is operating under constant Q mode

### `Exciter_Q_constant_ki`

~~~
  double Exciter_Q_constant_ki;
~~~

Parameter of the integration control for constant Q mode

### `Exciter_Q_constant_kp`

~~~
  double Exciter_Q_constant_kp;
~~~

Parameter of the propotional control for constant Q mode

### `P_CONSTANT_Pref`

~~~
  double P_CONSTANT_Pref[pu];
~~~

Pref input to governor controls (per-unit), if supported

### `Exciter_Q_constant_Qref`

~~~
  double Exciter_Q_constant_Qref[pu];
~~~

Qref input to govornor or AVR controls (per-unit), if supported

### `CVR_enabled`

~~~
  bool CVR_enabled;
~~~

True if the CVR control is enabled in the exciter

### `CVR_ki_cvr`

~~~
  double CVR_ki_cvr;
~~~

Parameter of the integration control for CVR control

### `CVR_kp_cvr`

~~~
  double CVR_kp_cvr;
~~~

Parameter of the proportional control for CVR control

### `CVR_kd_cvr`

~~~
  double CVR_kd_cvr;
~~~

Parameter of the deviation control for CVR control

### `CVR_kt_cvr`

~~~
  double CVR_kt_cvr;
~~~

Parameter of the gain in feedback loop for CVR control

### `CVR_kw_cvr`

~~~
  double CVR_kw_cvr;
~~~

Parameter of the gain in feedback loop for CVR control

### `CVR_PI`

~~~
  bool CVR_PI;
~~~

True if the PI controller is implemented in CVR control

### `CVR_PID`

~~~
  bool CVR_PID;
~~~

True if the PID controller is implemented in CVR control

### `vset_EMAX`

~~~
  double vset_EMAX;
~~~

Maximum Vset limit

### `vset_EMIN`

~~~
  double vset_EMIN;
~~~

Minimum Vset limit

### `CVR_Kd1`

~~~
  double CVR_Kd1;
~~~

Parameter of the second order transfer function for CVR control

### `CVR_Kd2`

~~~
  double CVR_Kd2;
~~~

Parameter of the second order transfer function for CVR control

### `CVR_Kd3`

~~~
  double CVR_Kd3;
~~~

Parameter of the second order transfer function for CVR control

### `CVR_Kn1`

~~~
  double CVR_Kn1;
~~~

Parameter of the second order transfer function for CVR control

### `CVR_Kn2`

~~~
  double CVR_Kn2;
~~~

Parameter of the second order transfer function for CVR control

### `vset_delta_MAX`

~~~
  double vset_delta_MAX;
~~~

Maximum delta Vset limit

### `vset_delta_MIN`

~~~
  double vset_delta_MIN;
~~~

Minimum delta Vset limit

### `vadd`

~~~
  double vadd;
~~~

Delta Vset

### `vadd_a`

~~~
  double vadd_a;
~~~

Delta Vset before going into bound check

### `Governor_type`

~~~
  enumeration {P_CONSTANT, GGOV1, GGOV1_OLD, GAST, DEGOV1, NO_GOV} Governor_type;
~~~

P_CONSTANT mode Governor Model

### `DEGOV1_R`

~~~
  double DEGOV1_R[pu];
~~~

Governor droop constant (p.u.)

### `DEGOV1_T1`

~~~
  double DEGOV1_T1[s];
~~~

Governor electric control box time constant (s)

### `DEGOV1_T2`

~~~
  double DEGOV1_T2[s];
~~~

Governor electric control box time constant (s)

### `DEGOV1_T3`

~~~
  double DEGOV1_T3[s];
~~~

Governor electric control box time constant (s)

### `DEGOV1_T4`

~~~
  double DEGOV1_T4[s];
~~~

Governor actuator time constant (s)

### `DEGOV1_T5`

~~~
  double DEGOV1_T5[s];
~~~

Governor actuator time constant (s)

### `DEGOV1_T6`

~~~
  double DEGOV1_T6[s];
~~~

Governor actuator time constant (s)

### `DEGOV1_K`

~~~
  double DEGOV1_K[pu];
~~~

Governor actuator gain

### `DEGOV1_TMAX`

~~~
  double DEGOV1_TMAX[pu];
~~~

Governor actuator upper limit (p.u.)

### `DEGOV1_TMIN`

~~~
  double DEGOV1_TMIN[pu];
~~~

Governor actuator lower limit (p.u.)

### `DEGOV1_TD`

~~~
  double DEGOV1_TD[s];
~~~

Governor combustion delay (s)

### `DEGOV1_x1`

~~~
  double DEGOV1_x1;
~~~

Governor electric box state variable

### `DEGOV1_x2`

~~~
  double DEGOV1_x2;
~~~

Governor electric box state variable

### `DEGOV1_x4`

~~~
  double DEGOV1_x4;
~~~

Governor electric box state variable

### `DEGOV1_x5`

~~~
  double DEGOV1_x5;
~~~

Governor electric box state variable

### `DEGOV1_x6`

~~~
  double DEGOV1_x6;
~~~

Governor electric box state variable

### `DEGOV1_throttle`

~~~
  double DEGOV1_throttle;
~~~

Governor electric box state variable

### `GAST_R`

~~~
  double GAST_R[pu];
~~~

Governor droop constant (p.u.)

### `GAST_T1`

~~~
  double GAST_T1[s];
~~~

Governor electric control box time constant (s)

### `GAST_T2`

~~~
  double GAST_T2[s];
~~~

Governor electric control box time constant (s)

### `GAST_T3`

~~~
  double GAST_T3[s];
~~~

Governor temperature limiter time constant (s)

### `GAST_AT`

~~~
  double GAST_AT[s];
~~~

Governor Ambient Temperature load limit (units)

### `GAST_KT`

~~~
  double GAST_KT[pu];
~~~

Governor temperature control loop gain

### `GAST_VMAX`

~~~
  double GAST_VMAX[pu];
~~~

Governor actuator upper limit (p.u.)

### `GAST_VMIN`

~~~
  double GAST_VMIN[pu];
~~~

Governor actuator lower limit (p.u.)

### `GAST_x1`

~~~
  double GAST_x1;
~~~

Governor electric box state variable

### `GAST_x2`

~~~
  double GAST_x2;
~~~

Governor electric box state variable

### `GAST_x3`

~~~
  double GAST_x3;
~~~

Governor electric box state variable

### `GAST_throttle`

~~~
  double GAST_throttle;
~~~

Governor electric box state variable

### `GGOV1_R`

~~~
  double GGOV1_R[pu];
~~~

Permanent droop, p.u.

### `GGOV1_Rselect`

~~~
  int32 GGOV1_Rselect;
~~~

Feedback signal for droop, = 1 selected electrical power, = 0 none (isochronous governor), = -1 fuel valve stroke ( true stroke),= -2 governor output ( requested stroke)

### `GGOV1_Tpelec`

~~~
  double GGOV1_Tpelec[s];
~~~

Electrical power transducer time constant, sec. (>0.)

### `GGOV1_maxerr`

~~~
  double GGOV1_maxerr;
~~~

Maximum value for speed error signal

### `GGOV1_minerr`

~~~
  double GGOV1_minerr;
~~~

Minimum value for speed error signal

### `GGOV1_Kpgov`

~~~
  double GGOV1_Kpgov;
~~~

Governor proportional gain

### `GGOV1_Kigov`

~~~
  double GGOV1_Kigov;
~~~

Governor integral gain

### `GGOV1_Kdgov`

~~~
  double GGOV1_Kdgov;
~~~

Governor derivative gain

### `GGOV1_Tdgov`

~~~
  double GGOV1_Tdgov[s];
~~~

Governor derivative controller time constant, sec.

### `GGOV1_vmax`

~~~
  double GGOV1_vmax;
~~~

Maximum valve position limit

### `GGOV1_vmin`

~~~
  double GGOV1_vmin;
~~~

Minimum valve position limit

### `GGOV1_Tact`

~~~
  double GGOV1_Tact;
~~~

Actuator time constant

### `GGOV1_Kturb`

~~~
  double GGOV1_Kturb;
~~~

Turbine gain (>0.)

### `GGOV1_wfnl`

~~~
  double GGOV1_wfnl[pu];
~~~

No load fuel flow, p.u

### `GGOV1_Tb`

~~~
  double GGOV1_Tb[s];
~~~

Turbine lag time constant, sec. (>0.)

### `GGOV1_Tc`

~~~
  double GGOV1_Tc[s];
~~~

Turbine lead time constant, sec.

### `GGOV1_Fuel_lag`

~~~
  int32 GGOV1_Fuel_lag;
~~~

Switch for fuel source characteristic, = 0 for fuel flow independent of speed, = 1 fuel flow proportional to speed

### `GGOV1_Teng`

~~~
  double GGOV1_Teng;
~~~

Transport lag time constant for diesel engine

### `GGOV1_Tfload`

~~~
  double GGOV1_Tfload;
~~~

Load Limiter time constant, sec. (>0.)

### `GGOV1_Kpload`

~~~
  double GGOV1_Kpload;
~~~

Load limiter proportional gain for PI controller

### `GGOV1_Kiload`

~~~
  double GGOV1_Kiload;
~~~

Load limiter integral gain for PI controller

### `GGOV1_Ldref`

~~~
  double GGOV1_Ldref[pu];
~~~

Load limiter reference value p.u.

### `GGOV1_Dm`

~~~
  double GGOV1_Dm[pu];
~~~

Speed sensitivity coefficient, p.u.

### `GGOV1_ropen`

~~~
  double GGOV1_ropen[pu/s];
~~~

Maximum valve opening rate, p.u./sec.

### `GGOV1_rclose`

~~~
  double GGOV1_rclose[pu/s];
~~~

Minimum valve closing rate, p.u./sec.

### `GGOV1_Kimw`

~~~
  double GGOV1_Kimw;
~~~

Power controller (reset) gain

### `GGOV1_Pmwset`

~~~
  double GGOV1_Pmwset[MW];
~~~

Power controller setpoint, MW

### `GGOV1_aset`

~~~
  double GGOV1_aset[pu/s];
~~~

Acceleration limiter setpoint, p.u. / sec.

### `GGOV1_Ka`

~~~
  double GGOV1_Ka;
~~~

Acceleration limiter Gain

### `GGOV1_Ta`

~~~
  double GGOV1_Ta[s];
~~~

Acceleration limiter time constant, sec. (>0.)

### `GGOV1_db`

~~~
  double GGOV1_db;
~~~

Speed governor dead band

### `GGOV1_Tsa`

~~~
  double GGOV1_Tsa[s];
~~~

Temperature detection lead time constant, sec.

### `GGOV1_Tsb`

~~~
  double GGOV1_Tsb[s];
~~~

Temperature detection lag time constant, sec.

### `GGOV1_Load_Limit_enable`

~~~
  bool GGOV1_Load_Limit_enable;
~~~

Enables/disables load limiter (fsrt) of low-value-select

### `GGOV1_Acceleration_Limit_enable`

~~~
  bool GGOV1_Acceleration_Limit_enable;
~~~

Enables/disables acceleration limiter (fsra) of low-value-select

### `GGOV1_PID_enable`

~~~
  bool GGOV1_PID_enable;
~~~

Enables/disables PID controller (fsrn) of low-value-select

### `GGOV1_fsrt`

~~~
  double GGOV1_fsrt;
~~~

Load limiter block input to low-value-select

### `GGOV1_fsra`

~~~
  double GGOV1_fsra;
~~~

Acceleration limiter block input to low-value-select

### `GGOV1_fsrn`

~~~
  double GGOV1_fsrn;
~~~

PID block input to low-value-select

### `GGOV1_speed_error`

~~~
  double GGOV1_speed_error[pu];
~~~

Speed difference in per-unit for input to PID controller

### `GGOV1_x1`

~~~
  double GGOV1_x1;
~~~

TODO

### `GGOV1_x2`

~~~
  double GGOV1_x2;
~~~

TODO

### `GGOV1_x2a`

~~~
  double GGOV1_x2a;
~~~

TODO

### `GGOV1_x3`

~~~
  double GGOV1_x3;
~~~

TODO

### `GGOV1_x3a`

~~~
  double GGOV1_x3a;
~~~

TODO

### `GGOV1_x4`

~~~
  double GGOV1_x4;
~~~

TODO

### `GGOV1_x4a`

~~~
  double GGOV1_x4a;
~~~

TODO

### `GGOV1_x4b`

~~~
  double GGOV1_x4b;
~~~

TODO

### `GGOV1_x5`

~~~
  double GGOV1_x5;
~~~

TODO

### `GGOV1_x5a`

~~~
  double GGOV1_x5a;
~~~

TODO

### `GGOV1_x5b`

~~~
  double GGOV1_x5b;
~~~

TODO

### `GGOV1_x6`

~~~
  double GGOV1_x6;
~~~

TODO

### `GGOV1_x7`

~~~
  double GGOV1_x7;
~~~

TODO

### `GGOV1_x7a`

~~~
  double GGOV1_x7a;
~~~

TODO

### `GGOV1_x8`

~~~
  double GGOV1_x8;
~~~

TODO

### `GGOV1_x8a`

~~~
  double GGOV1_x8a;
~~~

TODO

### `GGOV1_x9`

~~~
  double GGOV1_x9;
~~~

TODO

### `GGOV1_x9a`

~~~
  double GGOV1_x9a;
~~~

TODO

### `GGOV1_x10`

~~~
  double GGOV1_x10;
~~~

TODO

### `GGOV1_x10a`

~~~
  double GGOV1_x10a;
~~~

TODO

### `GGOV1_x10b`

~~~
  double GGOV1_x10b;
~~~

TODO

### `GGOV1_ValveStroke`

~~~
  double GGOV1_ValveStroke;
~~~

TODO

### `GGOV1_FuelFlow`

~~~
  double GGOV1_FuelFlow;
~~~

TODO

### `GGOV1_GovOutPut`

~~~
  double GGOV1_GovOutPut;
~~~

TODO

### `GGOV1_RselectValue`

~~~
  double GGOV1_RselectValue;
~~~

TODO

### `GGOV1_fsrtNoLim`

~~~
  double GGOV1_fsrtNoLim;
~~~

TODO

### `GGOV1_err2`

~~~
  double GGOV1_err2;
~~~

TODO

### `GGOV1_err2a`

~~~
  double GGOV1_err2a;
~~~

TODO

### `GGOV1_err3`

~~~
  double GGOV1_err3;
~~~

TODO

### `GGOV1_err4`

~~~
  double GGOV1_err4;
~~~

TODO

### `GGOV1_err7`

~~~
  double GGOV1_err7;
~~~

TODO

### `GGOV1_LowValSelect1`

~~~
  double GGOV1_LowValSelect1;
~~~

TODO

### `GGOV1_LowValSelect`

~~~
  double GGOV1_LowValSelect;
~~~

TODO

### `P_CONSTANT_Tpelec`

~~~
  double P_CONSTANT_Tpelec[s];
~~~

Electrical power transducer time constant, sec. (>0.)

### `P_CONSTANT_Tact`

~~~
  double P_CONSTANT_Tact;
~~~

Actuator time constant

### `P_CONSTANT_Kturb`

~~~
  double P_CONSTANT_Kturb;
~~~

Turbine gain (>0.)

### `P_CONSTANT_wfnl`

~~~
  double P_CONSTANT_wfnl[pu];
~~~

No load fuel flow, p.u

### `P_CONSTANT_Tb`

~~~
  double P_CONSTANT_Tb[s];
~~~

Turbine lag time constant, sec. (>0.)

### `P_CONSTANT_Tc`

~~~
  double P_CONSTANT_Tc[s];
~~~

Turbine lead time constant, sec.

### `P_CONSTANT_Teng`

~~~
  double P_CONSTANT_Teng;
~~~

Transport lag time constant for diesel engine

### `P_CONSTANT_ropen`

~~~
  double P_CONSTANT_ropen[pu/s];
~~~

Maximum valve opening rate, p.u./sec.

### `P_CONSTANT_rclose`

~~~
  double P_CONSTANT_rclose[pu/s];
~~~

Minimum valve closing rate, p.u./sec.

### `P_CONSTANT_Kimw`

~~~
  double P_CONSTANT_Kimw;
~~~

Power controller (reset) gain

### `P_CONSTANT_x1`

~~~
  double P_CONSTANT_x1;
~~~

TODO

### `P_CONSTANT_x4`

~~~
  double P_CONSTANT_x4;
~~~

TODO

### `P_CONSTANT_x4a`

~~~
  double P_CONSTANT_x4a;
~~~

TODO

### `P_CONSTANT_x4b`

~~~
  double P_CONSTANT_x4b;
~~~

TODO

### `P_CONSTANT_x5`

~~~
  double P_CONSTANT_x5;
~~~

TODO

### `P_CONSTANT_x5a`

~~~
  double P_CONSTANT_x5a;
~~~

TODO

### `P_CONSTANT_x5b`

~~~
  double P_CONSTANT_x5b;
~~~

TODO

### `P_CONSTANT_x_Pconstant`

~~~
  double P_CONSTANT_x_Pconstant;
~~~

TODO

### `P_CONSTANT_err4`

~~~
  double P_CONSTANT_err4;
~~~

TODO

### `P_CONSTANT_ValveStroke`

~~~
  double P_CONSTANT_ValveStroke;
~~~

TODO

### `P_CONSTANT_FuelFlow`

~~~
  double P_CONSTANT_FuelFlow;
~~~

TODO

### `P_CONSTANT_GovOutPut`

~~~
  double P_CONSTANT_GovOutPut;
~~~

TODO

### `fuelEmissionCal`

~~~
  bool fuelEmissionCal;
~~~

Boolean value indicating whether fuel and emission calculations are used or not

### `outputEnergy`

~~~
  double outputEnergy;
~~~

Total energy(kWh) output from the generator

### `FuelUse`

~~~
  double FuelUse;
~~~

Total fuel usage (gal) based on kW power output

### `efficiency`

~~~
  double efficiency;
~~~

Total energy output per fuel usage (kWh/gal)

### `CO2_emission`

~~~
  double CO2_emission;
~~~

Total CO2 emissions (lbs) based on fule usage

### `SOx_emission`

~~~
  double SOx_emission;
~~~

Total SOx emissions (lbs) based on fule usage

### `NOx_emission`

~~~
  double NOx_emission;
~~~

Total NOx emissions (lbs) based on fule usage

### `PM10_emission`

~~~
  double PM10_emission;
~~~

Total PM-10 emissions (lbs) based on fule usage

### `frequency_deviation`

~~~
  double frequency_deviation;
~~~

Frequency deviation of diesel_dg

### `frequency_deviation_energy`

~~~
  double frequency_deviation_energy;
~~~

Frequency deviation accumulation of diesel_dg

### `frequency_deviation_max`

~~~
  double frequency_deviation_max;
~~~

Frequency deviation of diesel_dg

### `realPowerChange`

~~~
  double realPowerChange;
~~~

Real power output change of diesel_dg

### `ratio_f_p`

~~~
  double ratio_f_p;
~~~

Ratio of frequency deviation to real power output change of diesel_dg

### `phases`

~~~
  set {S, N, C, B, A} phases;
~~~

Specifies which phases to connect to - currently not supported and assumes three-phase connection

# Example

~~~
  object diesel_dg {
    Gen_mode "0";
    Gen_status "0";
    Gen_type "0";
    pf "0.0";
    GenElecEff "0.0";
    TotalOutputPow "0+0i";
    TotalRealPow "0.0";
    TotalReacPow "0.0";
    speed "0.0";
    cylinders "0.0";
    stroke "0.0";
    torque "0.0";
    pressure "0.0";
    time_operation "0.0";
    fuel "0.0";
    w_coolingwater "0.0";
    inlet_temperature "0.0";
    outlet_temperature "0.0";
    air_fuel "0.0";
    room_temperature "0.0";
    exhaust_temperature "0.0";
    cylinder_length "0.0";
    cylinder_radius "0.0";
    brake_diameter "0.0";
    calotific_fuel "0.0";
    steam_exhaust "0.0";
    specific_heat_steam "0.0";
    specific_heat_dry "0.0";
    indicated_hp "0.0";
    brake_hp "0.0";
    thermal_efficiency "0.0";
    energy_supplied "0.0";
    heat_equivalent_ip "0.0";
    energy_coolingwater "0.0";
    mass_exhaustgas "0.0";
    energy_exhaustgas "0.0";
    energy_steam "0.0";
    total_energy_exhaustgas "0.0";
    unaccounted_energyloss "0.0";
    Pconv "0.0";
    Rated_V "0.0";
    Rated_VA "0.0";
    power_out_A "0+0i";
    power_out_B "0+0i";
    power_out_C "0+0i";
    Rs "0.0";
    Xs "0.0";
    Rg "0.0";
    Xg "0.0";
    voltage_A "0+0i";
    voltage_B "0+0i";
    voltage_C "0+0i";
    current_A "0+0i";
    current_B "0+0i";
    current_C "0+0i";
    EfA "0+0i";
    EfB "0+0i";
    EfC "0+0i";
    omega_ref "0.0";
    inertia "0.0";
    damping "0.0";
    number_poles "0.0";
    Ra "0.0";
    Xd "0.0";
    Xq "0.0";
    Xdp "0.0";
    Xqp "0.0";
    Xdpp "0.0";
    Xqpp "0.0";
    Xl "0.0";
    Tdp "0.0";
    Tdop "0.0";
    Tqop "0.0";
    Tdopp "0.0";
    Tqopp "0.0";
    Ta "0.0";
    X0 "0+0i";
    X2 "0+0i";
    rotor_speed_convergence "0.0";
    voltage_convergence "0.0";
    rotor_speed_convergence_enabled "FALSE";
    voltage_magnitude_convergence_enabled "FALSE";
    rotor_angle "0.0";
    rotor_speed "0.0";
    field_voltage "0.0";
    flux1d "0.0";
    flux2q "0.0";
    EpRotated "0+0i";
    VintRotated "0+0i";
    Eint_A "0+0i";
    Eint_B "0+0i";
    Eint_C "0+0i";
    Irotated "0+0i";
    pwr_electric "0+0i";
    pwr_mech "0.0";
    torque_mech "0.0";
    torque_elec "0.0";
    wref "0.0";
    vset "0.0";
    Pref "0.0";
    Qref "0.0";
    Exciter_type "0";
    KA "0.0";
    TA "0.0";
    TB "0.0";
    TC "0.0";
    EMAX "0.0";
    EMIN "0.0";
    Vterm_max "0.0";
    Vterm_min "0.0";
    bias "0.0";
    xe "0.0";
    xb "0.0";
    x_cvr1 "0.0";
    x_cvr2 "0.0";
    Vref "0.0";
    CVR_mode "0";
    P_CONSTANT_ki "0.0";
    P_CONSTANT_kp "0.0";
    Exciter_Q_constant_mode "FALSE";
    Exciter_Q_constant_ki "0.0";
    Exciter_Q_constant_kp "0.0";
    P_CONSTANT_Pref "0.0";
    Exciter_Q_constant_Qref "0.0";
    CVR_enabled "FALSE";
    CVR_ki_cvr "0.0";
    CVR_kp_cvr "0.0";
    CVR_kd_cvr "0.0";
    CVR_kt_cvr "0.0";
    CVR_kw_cvr "0.0";
    CVR_PI "FALSE";
    CVR_PID "FALSE";
    vset_EMAX "0.0";
    vset_EMIN "0.0";
    CVR_Kd1 "0.0";
    CVR_Kd2 "0.0";
    CVR_Kd3 "0.0";
    CVR_Kn1 "0.0";
    CVR_Kn2 "0.0";
    vset_delta_MAX "0.0";
    vset_delta_MIN "0.0";
    vadd "0.0";
    vadd_a "0.0";
    Governor_type "0";
    DEGOV1_R "0.0";
    DEGOV1_T1 "0.0";
    DEGOV1_T2 "0.0";
    DEGOV1_T3 "0.0";
    DEGOV1_T4 "0.0";
    DEGOV1_T5 "0.0";
    DEGOV1_T6 "0.0";
    DEGOV1_K "0.0";
    DEGOV1_TMAX "0.0";
    DEGOV1_TMIN "0.0";
    DEGOV1_TD "0.0";
    DEGOV1_x1 "0.0";
    DEGOV1_x2 "0.0";
    DEGOV1_x4 "0.0";
    DEGOV1_x5 "0.0";
    DEGOV1_x6 "0.0";
    DEGOV1_throttle "0.0";
    GAST_R "0.0";
    GAST_T1 "0.0";
    GAST_T2 "0.0";
    GAST_T3 "0.0";
    GAST_AT "0.0";
    GAST_KT "0.0";
    GAST_VMAX "0.0";
    GAST_VMIN "0.0";
    GAST_x1 "0.0";
    GAST_x2 "0.0";
    GAST_x3 "0.0";
    GAST_throttle "0.0";
    GGOV1_R "0.0";
    GGOV1_Rselect "0";
    GGOV1_Tpelec "0.0";
    GGOV1_maxerr "0.0";
    GGOV1_minerr "0.0";
    GGOV1_Kpgov "0.0";
    GGOV1_Kigov "0.0";
    GGOV1_Kdgov "0.0";
    GGOV1_Tdgov "0.0";
    GGOV1_vmax "0.0";
    GGOV1_vmin "0.0";
    GGOV1_Tact "0.0";
    GGOV1_Kturb "0.0";
    GGOV1_wfnl "0.0";
    GGOV1_Tb "0.0";
    GGOV1_Tc "0.0";
    GGOV1_Fuel_lag "0";
    GGOV1_Teng "0.0";
    GGOV1_Tfload "0.0";
    GGOV1_Kpload "0.0";
    GGOV1_Kiload "0.0";
    GGOV1_Ldref "0.0";
    GGOV1_Dm "0.0";
    GGOV1_ropen "0.0";
    GGOV1_rclose "0.0";
    GGOV1_Kimw "0.0";
    GGOV1_Pmwset "0.0";
    GGOV1_aset "0.0";
    GGOV1_Ka "0.0";
    GGOV1_Ta "0.0";
    GGOV1_db "0.0";
    GGOV1_Tsa "0.0";
    GGOV1_Tsb "0.0";
    GGOV1_Load_Limit_enable "FALSE";
    GGOV1_Acceleration_Limit_enable "FALSE";
    GGOV1_PID_enable "FALSE";
    GGOV1_fsrt "0.0";
    GGOV1_fsra "0.0";
    GGOV1_fsrn "0.0";
    GGOV1_speed_error "0.0";
    GGOV1_x1 "0.0";
    GGOV1_x2 "0.0";
    GGOV1_x2a "0.0";
    GGOV1_x3 "0.0";
    GGOV1_x3a "0.0";
    GGOV1_x4 "0.0";
    GGOV1_x4a "0.0";
    GGOV1_x4b "0.0";
    GGOV1_x5 "0.0";
    GGOV1_x5a "0.0";
    GGOV1_x5b "0.0";
    GGOV1_x6 "0.0";
    GGOV1_x7 "0.0";
    GGOV1_x7a "0.0";
    GGOV1_x8 "0.0";
    GGOV1_x8a "0.0";
    GGOV1_x9 "0.0";
    GGOV1_x9a "0.0";
    GGOV1_x10 "0.0";
    GGOV1_x10a "0.0";
    GGOV1_x10b "0.0";
    GGOV1_ValveStroke "0.0";
    GGOV1_FuelFlow "0.0";
    GGOV1_GovOutPut "0.0";
    GGOV1_RselectValue "0.0";
    GGOV1_fsrtNoLim "0.0";
    GGOV1_err2 "0.0";
    GGOV1_err2a "0.0";
    GGOV1_err3 "0.0";
    GGOV1_err4 "0.0";
    GGOV1_err7 "0.0";
    GGOV1_LowValSelect1 "0.0";
    GGOV1_LowValSelect "0.0";
    P_CONSTANT_Tpelec "0.0";
    P_CONSTANT_Tact "0.0";
    P_CONSTANT_Kturb "0.0";
    P_CONSTANT_wfnl "0.0";
    P_CONSTANT_Tb "0.0";
    P_CONSTANT_Tc "0.0";
    P_CONSTANT_Teng "0.0";
    P_CONSTANT_ropen "0.0";
    P_CONSTANT_rclose "0.0";
    P_CONSTANT_Kimw "0.0";
    P_CONSTANT_x1 "0.0";
    P_CONSTANT_x4 "0.0";
    P_CONSTANT_x4a "0.0";
    P_CONSTANT_x4b "0.0";
    P_CONSTANT_x5 "0.0";
    P_CONSTANT_x5a "0.0";
    P_CONSTANT_x5b "0.0";
    P_CONSTANT_x_Pconstant "0.0";
    P_CONSTANT_err4 "0.0";
    P_CONSTANT_ValveStroke "0.0";
    P_CONSTANT_FuelFlow "0.0";
    P_CONSTANT_GovOutPut "0.0";
    fuelEmissionCal "FALSE";
    outputEnergy "0.0";
    FuelUse "0.0";
    efficiency "0.0";
    CO2_emission "0.0";
    SOx_emission "0.0";
    NOx_emission "0.0";
    PM10_emission "0.0";
    frequency_deviation "0.0";
    frequency_deviation_energy "0.0";
    frequency_deviation_max "0.0";
    realPowerChange "0.0";
    ratio_f_p "0.0";
    phases "0";
  }
~~~

# See also

* [[/Module/Generators]]

