[[/Module/Generators]] -- Module generators

# Synopsis
GLM:
~~~
  object power_electronics {
    generator_mode "{SUPPLY_DRIVEN,CONSTANT_PF,CONSTANT_PQ,CONSTANT_V,UNKNOWN}";
    generator_status "{ONLINE,OFFLINE}";
    converter_type "{CURRENT_SOURCED,VOLTAGE_SOURCED}";
    switch_type "{DARLINGTON,IBJT,JFET,SCR,MOSFET,BJT,IDEAL_SWITCH}";
    filter_type "{BAND_PASS,BAND_STOP,HIGH_PASS,LOW_PASS}";
    filter_implementation "{PARALLEL_RESONANT,SERIES_RESONANT,INDUCTIVE,CAPACITVE,IDEAL_FILTER}";
    filter_frequency "{F240HZ,F180HZ,F120HZ}";
    power_type "{DC,AC}";
    Rated_kW "<decimal> kW";
    Max_P "<decimal> kW";
    Min_P "<decimal> kW";
    Rated_kVA "<decimal> kVA";
    Rated_kV "<decimal> kV";
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

### `converter_type`
~~~
  enumeration {CURRENT_SOURCED, VOLTAGE_SOURCED} converter_type;
~~~

TODO

### `switch_type`
~~~
  enumeration {DARLINGTON, IBJT, JFET, SCR, MOSFET, BJT, IDEAL_SWITCH} switch_type;
~~~

TODO

### `filter_type`
~~~
  enumeration {BAND_PASS, BAND_STOP, HIGH_PASS, LOW_PASS} filter_type;
~~~

TODO

### `filter_implementation`
~~~
  enumeration {PARALLEL_RESONANT, SERIES_RESONANT, INDUCTIVE, CAPACITVE, IDEAL_FILTER} filter_implementation;
~~~

TODO

### `filter_frequency`
~~~
  enumeration {F240HZ, F180HZ, F120HZ} filter_frequency;
~~~

TODO

### `power_type`
~~~
  enumeration {DC, AC} power_type;
~~~

TODO

### `Rated_kW`
~~~
  double Rated_kW[kW];
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

### `Rated_kVA`
~~~
  double Rated_kVA[kVA];
~~~

TODO

### `Rated_kV`
~~~
  double Rated_kV[kV];
~~~

TODO

### `phases`
~~~
  set {S, N, C, B, A} phases;
~~~

TODO

# Example

~~~
  object power_electronics {
    generator_mode "0";
    generator_status "0";
    converter_type "0";
    switch_type "0";
    filter_type "0";
    filter_implementation "0";
    filter_frequency "0";
    power_type "0";
    Rated_kW "0.0";
    Max_P "0.0";
    Min_P "0.0";
    Rated_kVA "0.0";
    Rated_kV "0.0";
    phases "0";
  }
~~~

# See also
* [[/Module/Generators]]

