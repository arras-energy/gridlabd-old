[[/Module/Generators/Energy_storage]] -- Class energy_storage

# Synopsis

GLM:

~~~
  object energy_storage {
    generator_mode "{SUPPLY_DRIVEN,CONSTANT_PF,CONSTANT_PQ,CONSTANT_V,UNKNOWN}";
    generator_status "{ONLINE,OFFLINE}";
    power_type "{DC,AC}";
    Rinternal "<decimal>";
    V_Max "<decimal> V";
    I_Max "<string> A";
    E_Max "<decimal>";
    Energy "<decimal>";
    efficiency "<decimal>";
    Rated_kVA "<decimal> kVA";
    V_Out "<string> V";
    I_Out "<string> A";
    VA_Out "<string> VA";
    V_In "<string> V";
    I_In "<string> A";
    V_Internal "<string> V";
    I_Internal "<string> A";
    I_Prev "<string> A";
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

### `E_Max`
~~~
  double E_Max;
~~~

TODO

### `Energy`
~~~
  double Energy;
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

### `V_Out`
~~~
  complex V_Out[V];
~~~

TODO

### `I_Out`
~~~
  complex I_Out[A];
~~~

TODO

### `VA_Out`
~~~
  complex VA_Out[VA];
~~~

TODO

### `V_In`
~~~
  complex V_In[V];
~~~

TODO

### `I_In`
~~~
  complex I_In[A];
~~~

TODO

### `V_Internal`
~~~
  complex V_Internal[V];
~~~

TODO

### `I_Internal`
~~~
  complex I_Internal[A];
~~~

TODO

### `I_Prev`
~~~
  complex I_Prev[A];
~~~

TODO

### `phases`
~~~
  set {S, N, C, B, A} phases;
~~~

TODO

# Example

~~~
  object energy_storage {
    generator_mode "0";
    generator_status "0";
    power_type "0";
    Rinternal "0.0";
    V_Max "0.0";
    I_Max "0+0i";
    E_Max "0.0";
    Energy "0.0";
    efficiency "0.0";
    Rated_kVA "0.0";
    V_Out "0+0i";
    I_Out "0+0i";
    VA_Out "0+0i";
    V_In "0+0i";
    I_In "0+0i";
    V_Internal "0+0i";
    I_Internal "0+0i";
    I_Prev "0+0i";
    phases "0";
  }
~~~

# See also
* [[/Module/Generators]]

