[[/Module/Generators]] -- Module generators

# Synopsis
GLM:
~~~
  object microturbine {
    generator_mode "{SUPPLY_DRIVEN,CONSTANT_PF,CONSTANT_PQ,CONSTANT_V,UNKNOWN}";
    generator_status "{ONLINE,OFFLINE}";
    power_type "{DC,AC}";
    Rinternal "<decimal>";
    Rload "<decimal>";
    V_Max "<decimal> V";
    I_Max "<string> A";
    frequency "<decimal> Hz";
    Max_Frequency "<decimal> Hz";
    Min_Frequency "<decimal> Hz";
    Fuel_Used "<decimal> kVA";
    Heat_Out "<decimal> kVA";
    KV "<decimal>";
    Power_Angle "<decimal>";
    Max_P "<decimal> kW";
    Min_P "<decimal> kW";
    phaseA_V_Out "<string> kV";
    phaseB_V_Out "<string> kV";
    phaseC_V_Out "<string> kV";
    phaseA_I_Out "<string> A";
    phaseB_I_Out "<string> A";
    phaseC_I_Out "<string> A";
    power_A_Out "<string>";
    power_B_Out "<string>";
    power_C_Out "<string>";
    VA_Out "<string>";
    pf_Out "<decimal>";
    E_A_Internal "<string>";
    E_B_Internal "<string>";
    E_C_Internal "<string>";
    efficiency "<decimal>";
    Rated_kVA "<decimal> kVA";
    phases "[SNCBA]";
  }
~~~

# Description

TODO

## Properties

### `generator_mode`
~~~
  enumeration {SUPPLY_DRIVEN, CONSTANT_PF, CONSTANT_PQ, CONSTANT_V, UNKNOWN} generator_mode;
~~~

TODO

### `generator_status`
~~~
  enumeration {ONLINE, OFFLINE} generator_status;
~~~

TODO

### `power_type`
~~~
  enumeration {DC, AC} power_type;
~~~

TODO

### `Rinternal`
~~~
  double Rinternal;
~~~

TODO

### `Rload`
~~~
  double Rload;
~~~

TODO

### `V_Max`
~~~
  double V_Max[V];
~~~

TODO

### `I_Max`
~~~
  complex I_Max[A];
~~~

TODO

### `frequency`
~~~
  double frequency[Hz];
~~~

TODO

### `Max_Frequency`
~~~
  double Max_Frequency[Hz];
~~~

TODO

### `Min_Frequency`
~~~
  double Min_Frequency[Hz];
~~~

TODO

### `Fuel_Used`
~~~
  double Fuel_Used[kVA];
~~~

TODO

### `Heat_Out`
~~~
  double Heat_Out[kVA];
~~~

TODO

### `KV`
~~~
  double KV;
~~~

TODO

### `Power_Angle`
~~~
  double Power_Angle;
~~~

TODO

### `Max_P`
~~~
  double Max_P[kW];
~~~

TODO

### `Min_P`
~~~
  double Min_P[kW];
~~~

TODO

### `phaseA_V_Out`
~~~
  complex phaseA_V_Out[kV];
~~~

TODO

### `phaseB_V_Out`
~~~
  complex phaseB_V_Out[kV];
~~~

TODO

### `phaseC_V_Out`
~~~
  complex phaseC_V_Out[kV];
~~~

TODO

### `phaseA_I_Out`
~~~
  complex phaseA_I_Out[A];
~~~

TODO

### `phaseB_I_Out`
~~~
  complex phaseB_I_Out[A];
~~~

TODO

### `phaseC_I_Out`
~~~
  complex phaseC_I_Out[A];
~~~

TODO

### `power_A_Out`
~~~
  complex power_A_Out;
~~~

TODO

### `power_B_Out`
~~~
  complex power_B_Out;
~~~

TODO

### `power_C_Out`
~~~
  complex power_C_Out;
~~~

TODO

### `VA_Out`
~~~
  complex VA_Out;
~~~

TODO

### `pf_Out`
~~~
  double pf_Out;
~~~

TODO

### `E_A_Internal`
~~~
  complex E_A_Internal;
~~~

TODO

### `E_B_Internal`
~~~
  complex E_B_Internal;
~~~

TODO

### `E_C_Internal`
~~~
  complex E_C_Internal;
~~~

TODO

### `efficiency`
~~~
  double efficiency;
~~~

TODO

### `Rated_kVA`
~~~
  double Rated_kVA[kVA];
~~~

TODO

### `phases`
~~~
  set {S, N, C, B, A} phases;
~~~

TODO

# Example

~~~
  object microturbine {
    generator_mode "0";
    generator_status "0";
    power_type "0";
    Rinternal "0.0";
    Rload "0.0";
    V_Max "0.0";
    I_Max "0+0i";
    frequency "0.0";
    Max_Frequency "0.0";
    Min_Frequency "0.0";
    Fuel_Used "0.0";
    Heat_Out "0.0";
    KV "0.0";
    Power_Angle "0.0";
    Max_P "0.0";
    Min_P "0.0";
    phaseA_V_Out "0+0i";
    phaseB_V_Out "0+0i";
    phaseC_V_Out "0+0i";
    phaseA_I_Out "0+0i";
    phaseB_I_Out "0+0i";
    phaseC_I_Out "0+0i";
    power_A_Out "0+0i";
    power_B_Out "0+0i";
    power_C_Out "0+0i";
    VA_Out "0+0i";
    pf_Out "0.0";
    E_A_Internal "0+0i";
    E_B_Internal "0+0i";
    E_C_Internal "0+0i";
    efficiency "0.0";
    Rated_kVA "0.0";
    phases "0";
  }
~~~

# See also
* [[/Module/Generators]]

