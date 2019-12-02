[[/Module/Generators/Rectifier]] -- Class rectifier

# Synopsis
GLM:
~~~
  object rectifier {
    rectifier_type "{TWELVE_PULSE,SIX_PULSE,THREE_PULSE,TWO_PULSE,ONE_PULSE}";
    generator_mode "{SUPPLY_DRIVEN,CONSTANT_PF,CONSTANT_PQ,CONSTANT_V,UNKNOWN}";
    V_Out "<string> V";
    V_Rated "<decimal> V";
    I_Out "<string> A";
    VA_Out "<string> VA";
    P_Out "<decimal>";
    Q_Out "<decimal>";
    Vdc "<string> V";
    voltage_A "<string> V";
    voltage_B "<string> V";
    voltage_C "<string> V";
    current_A "<string> V";
    current_B "<string> V";
    current_C "<string> V";
    power_A_In "<string> VA";
    power_B_In "<string> VA";
    power_C_In "<string> VA";
    phases "[SNCBA]";
  }
~~~

# Description

TODO

## Properties

### `rectifier_type`
~~~
  enumeration {TWELVE_PULSE, SIX_PULSE, THREE_PULSE, TWO_PULSE, ONE_PULSE} rectifier_type;
~~~

TODO

### `generator_mode`
~~~
  enumeration {SUPPLY_DRIVEN, CONSTANT_PF, CONSTANT_PQ, CONSTANT_V, UNKNOWN} generator_mode;
~~~

TODO

### `V_Out`
~~~
  complex V_Out[V];
~~~

TODO

### `V_Rated`
~~~
  double V_Rated[V];
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

### `P_Out`
~~~
  double P_Out;
~~~

TODO

### `Q_Out`
~~~
  double Q_Out;
~~~

TODO

### `Vdc`
~~~
  complex Vdc[V];
~~~

TODO

### `voltage_A`
~~~
  complex voltage_A[V];
~~~

TODO

### `voltage_B`
~~~
  complex voltage_B[V];
~~~

TODO

### `voltage_C`
~~~
  complex voltage_C[V];
~~~

TODO

### `current_A`
~~~
  complex current_A[V];
~~~

TODO

### `current_B`
~~~
  complex current_B[V];
~~~

TODO

### `current_C`
~~~
  complex current_C[V];
~~~

TODO

### `power_A_In`
~~~
  complex power_A_In[VA];
~~~

TODO

### `power_B_In`
~~~
  complex power_B_In[VA];
~~~

TODO

### `power_C_In`
~~~
  complex power_C_In[VA];
~~~

TODO

### `phases`
~~~
  set {S, N, C, B, A} phases;
~~~

TODO

# Example

~~~
  object rectifier {
    rectifier_type "0";
    generator_mode "0";
    V_Out "0+0i";
    V_Rated "0.0";
    I_Out "0+0i";
    VA_Out "0+0i";
    P_Out "0.0";
    Q_Out "0.0";
    Vdc "0+0i";
    voltage_A "0+0i";
    voltage_B "0+0i";
    voltage_C "0+0i";
    current_A "0+0i";
    current_B "0+0i";
    current_C "0+0i";
    power_A_In "0+0i";
    power_B_In "0+0i";
    power_C_In "0+0i";
    phases "0";
  }
~~~

# See also
* [[/Module/Generators]]

