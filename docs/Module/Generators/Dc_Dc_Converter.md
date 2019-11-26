[[/Module/Generators]] -- Module generators

# Synopsis
GLM:
~~~
  object dc_dc_converter {
    dc_dc_converter_type "{BUCK_BOOST,BOOST,BUCK}";
    generator_mode "{SUPPLY_DRIVEN,CONSTANT_PF,CONSTANT_PQ,CONSTANT_V,UNKNOWN}";
    V_Out "<string> V";
    I_Out "<string> A";
    Vdc "<string> V";
    VA_Out "<string> VA";
    P_Out "<decimal>";
    Q_Out "<decimal>";
    service_ratio "<decimal>";
    V_In "<string> V";
    I_In "<string> A";
    VA_In "<string> VA";
    phases "[SNCBA]";
  }
~~~

# Description

TODO

## Properties

### `dc_dc_converter_type`
~~~
  enumeration {BUCK_BOOST, BOOST, BUCK} dc_dc_converter_type;
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

### `I_Out`
~~~
  complex I_Out[A];
~~~

TODO

### `Vdc`
~~~
  complex Vdc[V];
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

### `service_ratio`
~~~
  double service_ratio;
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

### `VA_In`
~~~
  complex VA_In[VA];
~~~

TODO

### `phases`
~~~
  set {S, N, C, B, A} phases;
~~~

TODO

# Example

~~~
  object dc_dc_converter {
    dc_dc_converter_type "0";
    generator_mode "0";
    V_Out "0+0i";
    I_Out "0+0i";
    Vdc "0+0i";
    VA_Out "0+0i";
    P_Out "0.0";
    Q_Out "0.0";
    service_ratio "0.0";
    V_In "0+0i";
    I_In "0+0i";
    VA_In "0+0i";
    phases "0";
  }
~~~

# See also
* [[/Module/Generators]]

