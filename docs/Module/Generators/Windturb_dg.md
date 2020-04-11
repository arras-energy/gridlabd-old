[[/Module/Generators/Windturb_dg]] -- Class windturb_dg

# Synopsis

GLM:

~~~
  object windturb_dg {
    Gen_status "{ONLINE,OFFLINE}";
    Gen_type "{SYNCHRONOUS,INDUCTION}";
    Gen_mode "{CONSTANTPQ,CONSTANTP,CONSTANTE}";
    Turbine_Model "{BERGEY_10kW,GE_25MW,VESTAS_V82,USER_DEFINED,GENERIC_IND_LARGE,GENERIC_IND_MID,GENERIC_IND_SMALL,GENERIC_SYNCH_LARGE,GENERIC_SYNCH_MID,GENERIC_SYNCH_SMALL}";
    turbine_height "<decimal> m";
    roughness_length_factor "<decimal>";
    blade_diam "<decimal> m";
    blade_diameter "<decimal> m";
    cut_in_ws "<decimal> m/s";
    cut_out_ws "<decimal> m/s";
    ws_rated "<decimal> m/s";
    ws_maxcp "<decimal> m/s";
    Cp_max "<decimal> pu";
    Cp_rated "<decimal> pu";
    Cp "<decimal> pu";
    Rated_VA "<decimal> VA";
    Rated_V "<decimal> V";
    Pconv "<decimal> W";
    P_converted "<decimal> W";
    GenElecEff "<decimal> %";
    generator_efficiency "<decimal> %";
    TotalRealPow "<decimal> W";
    total_real_power "<decimal> W";
    TotalReacPow "<decimal> VA";
    total_reactive_power "<decimal> VA";
    power_A "<string> VA";
    power_B "<string> VA";
    power_C "<string> VA";
    WSadj "<decimal> m/s";
    wind_speed_adjusted "<decimal> m/s";
    Wind_Speed "<decimal> m/s";
    wind_speed "<decimal> m/s";
    air_density "<decimal> kg/m^3";
    R_stator "<decimal> pu*Ohm";
    X_stator "<decimal> pu*Ohm";
    R_rotor "<decimal> pu*Ohm";
    X_rotor "<decimal> pu*Ohm";
    R_core "<decimal> pu*Ohm";
    X_magnetic "<decimal> pu*Ohm";
    Max_Vrotor "<decimal> pu*V";
    Min_Vrotor "<decimal> pu*V";
    Rs "<decimal> pu*Ohm";
    Xs "<decimal> pu*Ohm";
    Rg "<decimal> pu*Ohm";
    Xg "<decimal> pu*Ohm";
    Max_Ef "<decimal> pu*V";
    Min_Ef "<decimal> pu*V";
    pf "<decimal> pu";
    power_factor "<decimal> pu";
    voltage_A "<string> V";
    voltage_B "<string> V";
    voltage_C "<string> V";
    current_A "<string> A";
    current_B "<string> A";
    current_C "<string> A";
    EfA "<string> V";
    EfB "<string> V";
    EfC "<string> V";
    Vrotor_A "<string> V";
    Vrotor_B "<string> V";
    Vrotor_C "<string> V";
    Irotor_A "<string> V";
    Irotor_B "<string> V";
    Irotor_C "<string> V";
    phases "[SNCBA]";
  }
~~~

# Description

TODO

## Properties

### `Gen_status`

~~~
  enumeration {ONLINE, OFFLINE} Gen_status;
~~~

Generator is currently available to supply power

### `Gen_type`

~~~
  enumeration {SYNCHRONOUS, INDUCTION} Gen_type;
~~~

Standard synchronous generator; is also used to 'fake' a doubly-fed induction generator for now

### `Gen_mode`

~~~
  enumeration {CONSTANTPQ, CONSTANTP, CONSTANTE} Gen_mode;
~~~

Maintains the real and reactive output at the terminals - currently unsupported

### `Turbine_Model`

~~~
  enumeration {BERGEY_10kW, GE_25MW, VESTAS_V82, USER_DEFINED, GENERIC_IND_LARGE, GENERIC_IND_MID, GENERIC_IND_SMALL, GENERIC_SYNCH_LARGE, GENERIC_SYNCH_MID, GENERIC_SYNCH_SMALL} Turbine_Model;
~~~

Sets all defaults to represent the power output of a Bergey 10kW turbine

### `turbine_height`

~~~
  double turbine_height[m];
~~~

Describes the height of the wind turbine hub above the ground

### `roughness_length_factor`

~~~
  double roughness_length_factor;
~~~

European Wind Atlas unitless correction factor for adjusting wind speed at various heights above ground and terrain types, default=0.055

### `blade_diam`

~~~
  double blade_diam[m];
~~~

Diameter of blades

### `blade_diameter`

~~~
  double blade_diameter[m];
~~~

Diameter of blades

### `cut_in_ws`

~~~
  double cut_in_ws[m/s];
~~~

Minimum wind speed for generator operation

### `cut_out_ws`

~~~
  double cut_out_ws[m/s];
~~~

Maximum wind speed for generator operation

### `ws_rated`

~~~
  double ws_rated[m/s];
~~~

Rated wind speed for generator operation

### `ws_maxcp`

~~~
  double ws_maxcp[m/s];
~~~

Wind speed at which generator reaches maximum Cp

### `Cp_max`

~~~
  double Cp_max[pu];
~~~

Maximum coefficient of performance

### `Cp_rated`

~~~
  double Cp_rated[pu];
~~~

Rated coefficient of performance

### `Cp`

~~~
  double Cp[pu];
~~~

Calculated coefficient of performance

### `Rated_VA`

~~~
  double Rated_VA[VA];
~~~

Rated generator power output

### `Rated_V`

~~~
  double Rated_V[V];
~~~

Rated generator terminal voltage

### `Pconv`

~~~
  double Pconv[W];
~~~

Amount of electrical power converted from mechanical power delivered

### `P_converted`

~~~
  double P_converted[W];
~~~

Amount of electrical power converted from mechanical power delivered

### `GenElecEff`

~~~
  double GenElecEff[%];
~~~

Calculated generator electrical efficiency

### `generator_efficiency`

~~~
  double generator_efficiency[%];
~~~

Calculated generator electrical efficiency

### `TotalRealPow`

~~~
  double TotalRealPow[W];
~~~

Total real power output

### `total_real_power`

~~~
  double total_real_power[W];
~~~

Total real power output

### `TotalReacPow`

~~~
  double TotalReacPow[VA];
~~~

Total reactive power output

### `total_reactive_power`

~~~
  double total_reactive_power[VA];
~~~

Total reactive power output

### `power_A`

~~~
  complex power_A[VA];
~~~

Total complex power injected on phase A

### `power_B`

~~~
  complex power_B[VA];
~~~

Total complex power injected on phase B

### `power_C`

~~~
  complex power_C[VA];
~~~

Total complex power injected on phase C

### `WSadj`

~~~
  double WSadj[m/s];
~~~

Speed of wind at hub height

### `wind_speed_adjusted`

~~~
  double wind_speed_adjusted[m/s];
~~~

Speed of wind at hub height

### `Wind_Speed`

~~~
  double Wind_Speed[m/s];
~~~

Wind speed at 5-15m level (typical measurement height)

### `wind_speed`

~~~
  double wind_speed[m/s];
~~~

Wind speed at 5-15m level (typical measurement height)

### `air_density`

~~~
  double air_density[kg/m^3];
~~~

Estimated air density

### `R_stator`

~~~
  double R_stator[pu*Ohm];
~~~

Induction generator primary stator resistance in p.u.

### `X_stator`

~~~
  double X_stator[pu*Ohm];
~~~

Induction generator primary stator reactance in p.u.

### `R_rotor`

~~~
  double R_rotor[pu*Ohm];
~~~

Induction generator primary rotor resistance in p.u.

### `X_rotor`

~~~
  double X_rotor[pu*Ohm];
~~~

Induction generator primary rotor reactance in p.u.

### `R_core`

~~~
  double R_core[pu*Ohm];
~~~

Induction generator primary core resistance in p.u.

### `X_magnetic`

~~~
  double X_magnetic[pu*Ohm];
~~~

Induction generator primary core reactance in p.u.

### `Max_Vrotor`

~~~
  double Max_Vrotor[pu*V];
~~~

Induction generator maximum induced rotor voltage in p.u., e.g. 1.2

### `Min_Vrotor`

~~~
  double Min_Vrotor[pu*V];
~~~

Induction generator minimum induced rotor voltage in p.u., e.g. 0.8

### `Rs`

~~~
  double Rs[pu*Ohm];
~~~

Synchronous generator primary stator resistance in p.u.

### `Xs`

~~~
  double Xs[pu*Ohm];
~~~

Synchronous generator primary stator reactance in p.u.

### `Rg`

~~~
  double Rg[pu*Ohm];
~~~

Synchronous generator grounding resistance in p.u.

### `Xg`

~~~
  double Xg[pu*Ohm];
~~~

Synchronous generator grounding reactance in p.u.

### `Max_Ef`

~~~
  double Max_Ef[pu*V];
~~~

Synchronous generator maximum induced rotor voltage in p.u., e.g. 0.8

### `Min_Ef`

~~~
  double Min_Ef[pu*V];
~~~

Synchronous generator minimum induced rotor voltage in p.u., e.g. 0.8

### `pf`

~~~
  double pf[pu];
~~~

Desired power factor in CONSTANTP mode (can be modified over time)

### `power_factor`

~~~
  double power_factor[pu];
~~~

Desired power factor in CONSTANTP mode (can be modified over time)

### `voltage_A`

~~~
  complex voltage_A[V];
~~~

Terminal voltage on phase A

### `voltage_B`

~~~
  complex voltage_B[V];
~~~

Terminal voltage on phase B

### `voltage_C`

~~~
  complex voltage_C[V];
~~~

Terminal voltage on phase C

### `current_A`

~~~
  complex current_A[A];
~~~

Calculated terminal current on phase A

### `current_B`

~~~
  complex current_B[A];
~~~

Calculated terminal current on phase B

### `current_C`

~~~
  complex current_C[A];
~~~

Calculated terminal current on phase C

### `EfA`

~~~
  complex EfA[V];
~~~

Synchronous generator induced voltage on phase A

### `EfB`

~~~
  complex EfB[V];
~~~

Synchronous generator induced voltage on phase B

### `EfC`

~~~
  complex EfC[V];
~~~

Synchronous generator induced voltage on phase C

### `Vrotor_A`

~~~
  complex Vrotor_A[V];
~~~

Induction generator induced voltage on phase A in p.u.

### `Vrotor_B`

~~~
  complex Vrotor_B[V];
~~~

Induction generator induced voltage on phase B in p.u.

### `Vrotor_C`

~~~
  complex Vrotor_C[V];
~~~

Induction generator induced voltage on phase C in p.u.

### `Irotor_A`

~~~
  complex Irotor_A[V];
~~~

Induction generator induced current on phase A in p.u.

### `Irotor_B`

~~~
  complex Irotor_B[V];
~~~

Induction generator induced current on phase B in p.u.

### `Irotor_C`

~~~
  complex Irotor_C[V];
~~~

Induction generator induced current on phase C in p.u.

### `phases`

~~~
  set {S, N, C, B, A} phases;
~~~

Specifies which phases to connect to - currently not supported and assumes three-phase connection

# Example

~~~
  object windturb_dg {
    Gen_status "0";
    Gen_type "0";
    Gen_mode "0";
    Turbine_Model "0";
    turbine_height "0.0";
    roughness_length_factor "0.0";
    blade_diam "0.0";
    blade_diameter "0.0";
    cut_in_ws "0.0";
    cut_out_ws "0.0";
    ws_rated "0.0";
    ws_maxcp "0.0";
    Cp_max "0.0";
    Cp_rated "0.0";
    Cp "0.0";
    Rated_VA "0.0";
    Rated_V "0.0";
    Pconv "0.0";
    P_converted "0.0";
    GenElecEff "0.0";
    generator_efficiency "0.0";
    TotalRealPow "0.0";
    total_real_power "0.0";
    TotalReacPow "0.0";
    total_reactive_power "0.0";
    power_A "0+0i";
    power_B "0+0i";
    power_C "0+0i";
    WSadj "0.0";
    wind_speed_adjusted "0.0";
    Wind_Speed "0.0";
    wind_speed "0.0";
    air_density "0.0";
    R_stator "0.0";
    X_stator "0.0";
    R_rotor "0.0";
    X_rotor "0.0";
    R_core "0.0";
    X_magnetic "0.0";
    Max_Vrotor "0.0";
    Min_Vrotor "0.0";
    Rs "0.0";
    Xs "0.0";
    Rg "0.0";
    Xg "0.0";
    Max_Ef "0.0";
    Min_Ef "0.0";
    pf "0.0";
    power_factor "0.0";
    voltage_A "0+0i";
    voltage_B "0+0i";
    voltage_C "0+0i";
    current_A "0+0i";
    current_B "0+0i";
    current_C "0+0i";
    EfA "0+0i";
    EfB "0+0i";
    EfC "0+0i";
    Vrotor_A "0+0i";
    Vrotor_B "0+0i";
    Vrotor_C "0+0i";
    Irotor_A "0+0i";
    Irotor_B "0+0i";
    Irotor_C "0+0i";
    phases "0";
  }
~~~

# See also

* [[/Module/Generators]]

