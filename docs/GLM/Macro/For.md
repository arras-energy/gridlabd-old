[[/GLM/Macro/For]] -- For loop control macro

# Synopsis
GLM:
~~~
#for <var> in <list>
...
#done
~~~

# Description

The `#for` macro causes the parser to loop through a section of GLM multiple time with the specified global `<var>` set a each of the space-delimited entries in `<list>`.

# Example

The following example creates three different random number generators:
~~~
class random_source 
{
	randomvar value;
}
#for PDF in normal(1,0) uniform(0,1) triangle(-1,1)
object random-source 
{
	value "type:$PDF; refresh:1min";	
}
#done
~~~

# Caveat

For loops cannot be nested at this time.

# See also
* [[/GLM/General/Range]]