[[/Module/Powerflow/Transformer_configuration]] -- Class transformer_configuration

# Synopsis
GLM:
~~~
  object transformer_configuration {
    connect_type "{SINGLE_PHASE_CENTER_TAPPED,SINGLE_PHASE,DELTA_GWYE,DELTA_DELTA,WYE_WYE,UNKNOWN}";
    install_type "{VAULT,PADMOUNT,POLETOP,UNKNOWN}";
    coolant_type "{DRY,MINERAL_OIL,UNKNOWN}";
    cooling_type "{DFOW,DFOA,NDFOW,NDFOA,FA,OA,UNKNOWN}";
    primary_voltage "<decimal> V";
    secondary_voltage "<decimal> V";
    power_rating "<decimal> kVA";
    powerA_rating "<decimal> kVA";
    powerB_rating "<decimal> kVA";
    powerC_rating "<decimal> kVA";
    resistance "<decimal> pu*Ohm";
    reactance "<decimal> pu*Ohm";
    impedance "<string> pu*Ohm";
    resistance1 "<decimal> pu*Ohm";
    reactance1 "<decimal> pu*Ohm";
    impedance1 "<string> pu*Ohm";
    resistance2 "<decimal> pu*Ohm";
    reactance2 "<decimal> pu*Ohm";
    impedance2 "<string> pu*Ohm";
    shunt_resistance "<decimal> pu*Ohm";
    shunt_reactance "<decimal> pu*Ohm";
    shunt_impedance "<string> pu*Ohm";
    core_coil_weight "<decimal> lb";
    tank_fittings_weight "<decimal> lb";
    oil_volume "<decimal> gal";
    rated_winding_time_constant "<decimal> h";
    rated_winding_hot_spot_rise "<decimal> degC";
    rated_top_oil_rise "<decimal> degC";
    no_load_loss "<decimal> pu";
    full_load_loss "<decimal> pu";
    reactance_resistance_ratio "<decimal>";
    installed_insulation_life "<decimal> h";
    magnetization_location "{BOTH,SECONDARY,PRIMARY,NONE}";
    inrush_saturation_enabled "<string>";
    L_A "<decimal> pu";
    phi_K "<decimal> pu";
    phi_M "<decimal> pu";
    I_M "<decimal> pu";
    T_D "<decimal>";
  }
~~~

# Description

TODO

## Properties

### `connect_type`
~~~
  enumeration {SINGLE_PHASE_CENTER_TAPPED, SINGLE_PHASE, DELTA_GWYE, DELTA_DELTA, WYE_WYE, UNKNOWN} connect_type;
~~~

Connect type enum: Wye-Wye, single-phase, etc.

### `install_type`
~~~
  enumeration {VAULT, PADMOUNT, POLETOP, UNKNOWN} install_type;
~~~

Defines location of the transformer installation

### `coolant_type`
~~~
  enumeration {DRY, MINERAL_OIL, UNKNOWN} coolant_type;
~~~

Coolant type, used in life time model

### `cooling_type`
~~~
  enumeration {DFOW, DFOA, NDFOW, NDFOA, FA, OA, UNKNOWN} cooling_type;
~~~

Type of coolant fluid used in life time model

### `primary_voltage`
~~~
  double primary_voltage[V];
~~~

Primary voltage level in L-L value kV

### `secondary_voltage`
~~~
  double secondary_voltage[V];
~~~

Secondary voltage level kV

### `power_rating`
~~~
  double power_rating[kVA];
~~~

KVA rating of transformer, total

### `powerA_rating`
~~~
  double powerA_rating[kVA];
~~~

KVA rating of transformer, phase A

### `powerB_rating`
~~~
  double powerB_rating[kVA];
~~~

KVA rating of transformer, phase B

### `powerC_rating`
~~~
  double powerC_rating[kVA];
~~~

KVA rating of transformer, phase C

### `resistance`
~~~
  double resistance[pu*Ohm];
~~~

Series impedance, pu, real

### `reactance`
~~~
  double reactance[pu*Ohm];
~~~

Series impedance, pu, imag

### `impedance`
~~~
  complex impedance[pu*Ohm];
~~~

Series impedance, pu

### `resistance1`
~~~
  double resistance1[pu*Ohm];
~~~

Secondary series impedance (only used when you want to define each individual winding seperately, pu, real

### `reactance1`
~~~
  double reactance1[pu*Ohm];
~~~

Secondary series impedance (only used when you want to define each individual winding seperately, pu, imag

### `impedance1`
~~~
  complex impedance1[pu*Ohm];
~~~

Secondary series impedance (only used when you want to define each individual winding seperately, pu

### `resistance2`
~~~
  double resistance2[pu*Ohm];
~~~

Secondary series impedance (only used when you want to define each individual winding seperately, pu, real

### `reactance2`
~~~
  double reactance2[pu*Ohm];
~~~

Secondary series impedance (only used when you want to define each individual winding seperately, pu, imag

### `impedance2`
~~~
  complex impedance2[pu*Ohm];
~~~

Secondary series impedance (only used when you want to define each individual winding seperately, pu

### `shunt_resistance`
~~~
  double shunt_resistance[pu*Ohm];
~~~

Shunt impedance on primary side, pu, real

### `shunt_reactance`
~~~
  double shunt_reactance[pu*Ohm];
~~~

Shunt impedance on primary side, pu, imag

### `shunt_impedance`
~~~
  complex shunt_impedance[pu*Ohm];
~~~

Shunt impedance on primary side, pu

### `core_coil_weight`
~~~
  double core_coil_weight[lb];
~~~

The weight of the core and coil assembly in pounds

### `tank_fittings_weight`
~~~
  double tank_fittings_weight[lb];
~~~

The weight of the tank and fittings in pounds

### `oil_volume`
~~~
  double oil_volume[gal];
~~~

The number of gallons of oil in the transformer

### `rated_winding_time_constant`
~~~
  double rated_winding_time_constant[h];
~~~

The rated winding time constant in hours

### `rated_winding_hot_spot_rise`
~~~
  double rated_winding_hot_spot_rise[degC];
~~~

Winding hottest-spot rise over ambient temperature at rated load, degrees C

### `rated_top_oil_rise`
~~~
  double rated_top_oil_rise[degC];
~~~

Top-oil hottest-spot rise over ambient temperature at rated load, degrees C

### `no_load_loss`
~~~
  double no_load_loss[pu];
~~~

Another method of specifying transformer impedances, defined as per unit power values (shunt)

### `full_load_loss`
~~~
  double full_load_loss[pu];
~~~

Another method of specifying transformer impedances, defined as per unit power values (shunt and series)

### `reactance_resistance_ratio`
~~~
  double reactance_resistance_ratio;
~~~

The reactance to resistance ratio (X/R)

### `installed_insulation_life`
~~~
  double installed_insulation_life[h];
~~~

The normal lifetime of the transformer insulation at rated load, hours

### `magnetization_location`
~~~
  enumeration {BOTH, SECONDARY, PRIMARY, NONE} magnetization_location;
~~~

Winding to place magnetization influence for in-rush calculations

### `inrush_saturation_enabled`
~~~
  bool inrush_saturation_enabled;
~~~

Flag to include saturation effects during inrush calculations

### `L_A`
~~~
  double L_A[pu];
~~~

Air core inductance of transformer

### `phi_K`
~~~
  double phi_K[pu];
~~~

Knee flux value where the air core inductance interstes the flux axis of the saturation curve

### `phi_M`
~~~
  double phi_M[pu];
~~~

Peak magnetization flux at rated voltage of the saturation curve

### `I_M`
~~~
  double I_M[pu];
~~~

Peak magnetization current at rated voltage of the saturation curve

### `T_D`
~~~
  double T_D;
~~~

Inrush decay time constant for inrush current

# Example

~~~
  object transformer_configuration {
    connect_type "0";
    install_type "0";
    coolant_type "0";
    cooling_type "0";
    primary_voltage "0.0";
    secondary_voltage "0.0";
    power_rating "0.0";
    powerA_rating "0.0";
    powerB_rating "0.0";
    powerC_rating "0.0";
    resistance "0.0";
    reactance "0.0";
    impedance "0+0i";
    resistance1 "0.0";
    reactance1 "0.0";
    impedance1 "0+0i";
    resistance2 "0.0";
    reactance2 "0.0";
    impedance2 "0+0i";
    shunt_resistance "+1e+09 pu*Ohm";
    shunt_reactance "+1e+09 pu*Ohm";
    shunt_impedance "+1e+09+1e+09j pu*Ohm";
    core_coil_weight "0.0";
    tank_fittings_weight "0.0";
    oil_volume "0.0";
    rated_winding_time_constant "0.0";
    rated_winding_hot_spot_rise "0.0";
    rated_top_oil_rise "0.0";
    no_load_loss "0.0";
    full_load_loss "0.0";
    reactance_resistance_ratio "0.0";
    installed_insulation_life "0.0";
    magnetization_location "0";
    inrush_saturation_enabled "FALSE";
    L_A "+0.2 pu";
    phi_K "+1.17 pu";
    phi_M "+1 pu";
    I_M "+0.01 pu";
    T_D "+0.5";
  }
~~~

# See also
* [[/Module/Powerflow]]

