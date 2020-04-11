[[/GLM/Property/Randomvar]] -- Random variables

# Synopsis

GLM:

~~~
class <class-name> {
    randomvar <property-name>;
}
object <class-name> {
    <property-name> {
        <value> | type:<distribution>(<parameters>); 
        [min:<lower-bound>;] 
        [max:<upper-bound>;] 
        [refresh: <update-rate>] 
        [state:<seed>] 
        [correlation:[<object-name>.]<property-name>[*<scale>[+<bias>]]]
        [integrate;]
        };
}
~~~

# Description

The `randomvar` property type implements a pseudo-random value that changes periodically according to the `<distribution>` given. The meaning of the `<parameters>` depends on the `<distribution>` chosen. The distributed may be truncated at the `<lower-bound>` and `<upper-bound>`. The frequency of the updates is given by the `<update-rate>`.

Correlated random variables are generated using the equation 
$$
    x = x_1 + x_0 \times scale + bias
$$
where $x_0$ is the current value obtained from `<object-name>.<property-name>` and $x_1$ is the sample from `<distribution>(<parameters>)`.

If `integrate` is specified, each new random value is added the current value.

For details on the supported distributions, see [[/GLM/General/Random values]]

# Example

The following example generates two standard random Gaussian that are 90% anti-correlated.

~~~
class example {
    randomvar x;
    randomvar y;
}
module tape;
object example {
    name my_object;
    x {type:normal(0.0,1.0): min:-3.0, max:+3.0; refresh:1.0min;};
    y {type:normal(0.0,0.1); min:-3.0; max:+3.0; refresh:1.0min; correlation:my_object.x*-0.9};
    object recorder {
        property "x,y";
        file "example.csv";
        interval -1;
    };
}
~~~

# Caveat

The order in which the random variables is defined is important to creating correlations.  As a result, the `correlation` specification must refer to an existing random variable.  In addition, because each random variable may depend on only one other random variable, the standard method of creating $N>2$ correlated random variables is not supported directly.  Instead, a cascade of $N=2$ random variables must be defined.  This can be accomplished by performing Gauss elimination on the Cholesky decomposition of the covariance matrix to obtain a bi-diagonal matrix of the form

~~~
(     s[1]^2          0          0          0          0 )
(   s[1]s[2]     s[2]^2          0          0          0 )
(          0   s[2]s[3]     s[3]^2          0          0 )
(          0          0          .          .          0 )
(          0          0          0 s[N-1]s[N]     s[N]^2 )
~~~

# See also

* [[/GLM/General/Random values]]
