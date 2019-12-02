[[/GLM/Property/Randomvar]] -- Random variables

# Synopsis
GLM:
~~~
class <class-name> {
    randomvar <property-name>;
}
object <class-name> {
    <property-name> {
        type:<distribution>(<parameters>); 
        [min:<lower-bound>;] 
        [max:<upper-bound>;] 
        [refresh: <update-rate>] 
        [state:<seed>] 
        [correlation:[<object>.]<property>[*<scale>[+<zero>]]]
        };
}
~~~

# Description

The `randomvar` property type implements a pseudo-random value that changes periodically according to the `<distribution>` given. The meaning of the `<parameters>` depends on the `<distribution>` chosen. The distributed may be truncated at the `<lower-bound>` and `<upper-bound>`. The frequency of the updates is given by the `<update-rate>`.

Correlated random variables are generated using the equation 
$$
    x = (x_0-zero) \times scale + zero + x_1
$$
where $x_0$ is the remote variable from `<object>.<property>` and $x_1$ is the local random variable.

For details on the supported distributions, see [[/GLM/General/Random values]]

# Example

The following example generates a random Gaussian truncated to the range (-5,15) time series with a new sample every minute with a mean 5.0 and standard deviation of 3.0.
~~~
class example {
    randomvar x;
}
module tape;
object example {
    x {type:normal(5,3); min=-5; max=15; refresh=1min};
    object recorder {
        property "x";
        file "example.csv";
        interval -1;
    };
}
~~~

# See also
* [[/GLM/General/Random values]]
