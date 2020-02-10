[[/Glm/Macro/Ifndef]] -- Macro to conditionally process GLM lines when a global variable is not defined

# Synopsis

GLM:

~~~
#ifndef <name>
...
[#else]
...
#endif
~~~

# Description

The `#ifndef` macro is the opposite of the `#ifdef` macro.

# Examples

~~~
#ifndef YES
#print no
#else
#print yes
#endif
~~~

# See also

* [[/Glm/Macro/If]]
* [[/Glm/Macro/Ifdef]]

