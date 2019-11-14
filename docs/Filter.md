The external filter syntax has changed to make it compatible with the syntax for internal filters and property references.  The syntax for all properties that use filter inputs is
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
As a result of this change the syntax `value example(source.value)` is now interpreted as using the `value` member of the property `source` of the `destination` object instead of the `value` property of the `source` object. This also means that filters cannot come from anonymous objects (object names such as `<class>:<id>`).  All filter source objects must be named.  Omitting the filter source object name is interpreted as meaning `this` object.

# Resolution and saturation

In addition, filters now support resolution and saturation limits, e.g.,
~~~
filter delay(z,5min,10s,resolution=8,minimum=-2.5,maximum=2.5) = 1/z;
~~~
creates a filter with 8 bits of resolution (256 values) over a dynamic range of 5.0.