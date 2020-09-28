[[/GLM/Property/Python]] -- Python property

# Synopsis

GLM:

~~~
  class <class-name> {
    python <property-name>;
  }
  object <class-name> {
    <property-name> <type(<value>);
  }
~~~
  
# Description

The `python` property type support general python objects.  Although any type of object is supported, only those that implement the python `str` method as useful because GridLAB-D require the `str()` to generate the values for data exchange.

# Examples

~~~
class test
{
	python py_object;
}
object test
{
	py_object None;
}
object test
{
	py_object int(1);
}
object test
{
	py_object float(0.0);
}
object test
{
	py_object float(1.23);
}
object test
{
	py_object str('text');
}
object test
{
	py_object list([1,2,3]);
}
object test
{
	py_object dict(a=1,b=2,c=3);
}
~~~

