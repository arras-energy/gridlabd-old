[[/Module/Powerflow/Building]] - General building load model

# Synopsis

GLM:

~~~
object building
{
    phases "ABCDNS";
    nominal_voltage 480 V;
    timestep 60 s;
    output_timestep ${timestep} s;
    building_response <a0>,<a1>,<a2>;
    input "<object>.<property>,<channel>,<b0>,<b1>,<b2>,<d0>,<d1>[,<u0>,<u1>,<u2]";
    zip "<zp>,<ip>,<pp>,<zq>,<iq>,<pq>";
}
~~~

# Description

The `building` object implements a general building load model as a one or
more pair transfer functions from each input to the each of the output channels.

The static load model is of the form

$$
    y_1(t) = M(m(t)) ~ w(t) + y_0(m(t))
$$

where $M$ is the system mode matrix, and $w$ is the weather. If the system has
$K$ modes, then the matrix $M$ expresses the weather contribution of each
weather parameters to the total static load for each mode $m(t)$. If the
weather contains four variables, then the matrix $M$ is $2\times4$ such that it
yields the real and reactive power contributions to the load in the current
mode given the current weather.

The dynamic model is of the form

$$
    \dot x = A x + B u
$$

$$
    y_2(t) = C x + D u
$$

where all the matrix and variables are time-varying, but the matrices $A$, $B$,
$C$, and $D$ vary slowly relative to the state variable $x$.

The total load is $y(t) = y_1(t) + y_2(t)$.

## Inputs

Inputs are specified with the source object and property and the target channel, 
i.e., 0 for air temperature and 1 for mass temperature.  The values of `b` specify
the coefficients for the numerator of the transfer function, and the values of `d`
specify the coefficients for the output.  The values of `u` provide the historical
values to facilitate initialization.

## Building Response

The state response at the time $t$ to an input is given by the equation

~~~
    x[t] = b[0]/a[0]*u[t] + b[1]/a[0]*u[t-1] + b[2]/a[0]*u[t-2] 
                          - a[1]/a[0]*x[t-1] - a[2]/a[0]*x[t-2]
~~~

The building response is specified using a 2x3 matrix with row 1 for the air
temperature and row 2 for the mass temperature.

## Outputs

The outputs are given by the contribution of each input

~~~
    y[channel][t] = d[channel]*u[t]
~~~

with channel 0 for real power and channel 1 for reactive power. 

The values of `zip` are scalars for the power contributions in units of `kW`
from the output `y` to each component of load.

# Example

~~~
class weather
{
    double temperature[degF];
}
object weather
{
    temperature 50 degF;
}
module powerflow;
object building
{
    phases ABC;
    nominal_voltage 480;
    timestep 60;
    building_response "0,0,1;0,0,1";
    input "weather.temperature,0,0,0,1,1,1";
    input "weather.temperature,1,0,0,1,1,1";
    zip "1,0,0;0.1,0,0";
}
~~~

# See also

* [[/Module/Powerflow/Load]]