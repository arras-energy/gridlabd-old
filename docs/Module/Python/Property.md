[[/GLM/Module/Python/Property]] -- Direct property access python objects

# Synopsis

Python:

~~~
import gridlabd
property.__init__(object=<int>, property=<str>)
property.__init__(object=<str>, property=<str>)
property.get_initial()
property.get_name()
property.get_unit()
property.get_value()
property.rlock()
property.set_name(<str>)
property.set_object(<int>)
property.set_object(<str>)
property.set_value(<str>)
property.set_value(<long>)
property.set_value(<float>)
property.set_value(<complex>)
property.set_value(<object>)
property.unlock()
property.wlock()
~~~

# Description

The `property` object provides direct high-performance access to GridLAB-D object properties.  Access is provided through strong-typed accessors that a tired to the GridLAB-D property types.

Objects can be identified by number (int) or name (str).  Properties may only be identified by name (str).

Values can be provided either as a string or a value that is compatible with the internal property in GridLAB-D.  When the value is provided as a string, it is parsed in the same manner as a GLM value.

Values can be retrieved only as the python type associated with the GridLAB-D property type.  To retrieve the value as a string, you must use the `str()` method.  In this case, the value is converted using GridLAB-D's output conversion methods for the property type.

# Example

The following example illustrates access to various object property types.

`example.glm`:

~~~
module example;
class example_class
{
	python my_python;
	int16 my_int16;
	int32 my_int32;
	int64 my_int64;
	double my_double;
	complex my_complex;
	char32 my_char32;
	char256 my_char256;
	char1024 my_char1024;
	randomvar my_randomvar;
	object my_object;
	double a_speed[m/s];
}
object example_class
{
	name "example_object"
	my_python "['test',12.34,{'this':'is,'an':'example'}";
}
~~~

`example.py`:

~~~
~~~