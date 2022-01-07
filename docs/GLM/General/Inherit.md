[[/GLM/General/Inherit.md]] -- Inherit property values

# Synopsis

GLM:

~~~
object <class>
{
    <property-name> inherit;
    <property_name> @<object-name>;
}
~~~

# Description

The `inherit` property value causes the value to be copied from the parent object. This requires that the parent already be defined.  The alternative syntax `@<name>` inherits the property value from the named object.

# Example

This example copies the value of `x` from object `test1` into object `test2`:

~~~
class test
{
    double x;
}
object test
{
    name test1;
    x 1.23;
}
object test
{
    name test2;
    x @test1;
}
~~~
