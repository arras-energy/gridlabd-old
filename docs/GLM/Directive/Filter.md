[[/GLM/Directive/Filter]] -- Filter modeling directive

# Synopsis

GLM:

~~~
filter <name>(<domain>[,<timestep>[,<timeskew>[,<option>=<value>]]]) = <polynomial>/<polynomial>;
~~~

# Description

The filter directive defines a filter that can be used to connect a signal source property to a output signal property. 

Filter may be used to output values to an object property of type `double`.  Outputs are summed so that multiple filter may output to a single property, e.g.,

~~~
object example
{
  output1 filter11(input1);
  output1 filter21(input2);
  output2 filter12(input1);
  output2 filter22(input2);
}
~~~

Would represent a MIMO system with two inputs going to two outputs through 4 different filters with outputs summed.

## Name

Any unique alphabetic name may be used.

## Domain

Only discrete-time *z*-domain filters are supported.

## Timestep

Specifies the discrete-time filter timestep

## Timeskew

Specifies the time-shift for the discrete-time sampling.

## Resolution and saturation

In addition, filters now support resolution and saturation limits, e.g.,

~~~
filter delay(z,5min,10s,resolution=8,minimum=-2.5,maximum=2.5) = 1/z;
~~~

creates a filter with 8 bits of resolution (256 values) over a dynamic range of 5.0.

## Polynomial

The numerator and denominator are specified as a polynomial of the form $a_n z^n + a_{n-1} z^{n-1} + \cdots + a_1 z + a_0$, e.g.,

~~~
  an z^n + ... + a1 z + a0
~~~

The order of the numerator must be less than or equal to the order of the denominator. 

# Example

~~~
filter example(z,1) = z/(z+1);
object test {
  name "source";
  value 0.0;
}
object test {
  name "destination";
  value example(source:value);
}
~~~

# Caveat

The syntax is now different from GridLAB-D 4.0 because `value example(source.value)` is now interpreted as using the `value` member of the property `source` of the `destination` object instead of the `value` property of the `source` object. This also means that filters cannot come from anonymous objects (object names such as `<class>:<id>`).  All filter source objects must be named.  Omitting the filter source object name is interpreted as meaning `this` object.

