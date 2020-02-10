[[/Module/Powerflow/Frequency_gen]] -- Class frequency_gen

# Synopsis

GLM:

~~~
  object frequency_gen {
    Frequency_Mode "{AUTO,OFF}";
    Frequency "<decimal> Hz";
    FreqChange "<decimal> Hz/s";
    Deadband "<decimal> Hz";
    Tolerance "<decimal> %";
    M "<decimal> pu*s";
    D "<decimal> %";
    Rated_power "<decimal> W";
    Gen_power "<decimal> W";
    Load_power "<decimal> W";
    Gov_delay "<decimal> s";
    Ramp_rate "<decimal> W/s";
    Low_Freq_OI "<decimal> Hz";
    High_Freq_OI "<decimal> Hz";
    avg24 "<decimal> Hz";
    std24 "<decimal> Hz";
    avg168 "<decimal> Hz";
    std168 "<decimal> Hz";
    Num_Resp_Eqs "<integer>";
  }
~~~

# Description

TODO

## Properties

### `Frequency_Mode`

~~~
  enumeration {AUTO, OFF} Frequency_Mode;
~~~

Frequency object operations mode

### `Frequency`

~~~
  double Frequency[Hz];
~~~

Instantaneous frequency value

### `FreqChange`

~~~
  double FreqChange[Hz/s];
~~~

Frequency change from last timestep

### `Deadband`

~~~
  double Deadband[Hz];
~~~

Frequency deadband of the governor

### `Tolerance`

~~~
  double Tolerance[%];
~~~

% threshold a power difference must be before it is cared about

### `M`

~~~
  double M[pu*s];
~~~

Inertial constant of the system

### `D`

~~~
  double D[%];
~~~

Load-damping constant

### `Rated_power`

~~~
  double Rated_power[W];
~~~

Rated power of system (base power)

### `Gen_power`

~~~
  double Gen_power[W];
~~~

Mechanical power equivalent

### `Load_power`

~~~
  double Load_power[W];
~~~

Last sensed load value

### `Gov_delay`

~~~
  double Gov_delay[s];
~~~

Governor delay time

### `Ramp_rate`

~~~
  double Ramp_rate[W/s];
~~~

Ramp ideal ramp rate

### `Low_Freq_OI`

~~~
  double Low_Freq_OI[Hz];
~~~

Low frequency setpoint for GFA devices

### `High_Freq_OI`

~~~
  double High_Freq_OI[Hz];
~~~

High frequency setpoint for GFA devices

### `avg24`

~~~
  double avg24[Hz];
~~~

Average of last 24 hourly instantaneous measurements

### `std24`

~~~
  double std24[Hz];
~~~

Standard deviation of last 24 hourly instantaneous measurements

### `avg168`

~~~
  double avg168[Hz];
~~~

Average of last 168 hourly instantaneous measurements

### `std168`

~~~
  double std168[Hz];
~~~

Standard deviation of last 168 hourly instantaneous measurements

### `Num_Resp_Eqs`

~~~
  int32 Num_Resp_Eqs;
~~~

Total number of equations the response can contain

# Example

~~~
  object frequency_gen {
    Frequency_Mode "0";
    Frequency "0.0";
    FreqChange "0.0";
    Deadband "0.0";
    Tolerance "0.0";
    M "0.0";
    D "0.0";
    Rated_power "0.0";
    Gen_power "0.0";
    Load_power "0.0";
    Gov_delay "0.0";
    Ramp_rate "0.0";
    Low_Freq_OI "0.0";
    High_Freq_OI "0.0";
    avg24 "0.0";
    std24 "0.0";
    avg168 "0.0";
    std168 "0.0";
    Num_Resp_Eqs "0";
  }
~~~

# See also

* [[/Module/Powerflow]]

