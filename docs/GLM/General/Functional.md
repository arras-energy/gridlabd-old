[[/GLM/Property/Functional]] -- Functional property initialization

# Synopsis

GLM:

~~~
random.<distribution>(<parameters>)
~~~

# Description

Functional may be used to generate initial values.

For details on the supported distributions, see [[/GLM/General/Random values]]

# Example

~~~
class example
{
    double x;
}
object example 
{
    x random.uniform(0,100);
}
~~~

# See also

* [[/GLM/General/Random values]]
