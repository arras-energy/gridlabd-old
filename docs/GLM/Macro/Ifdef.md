[[/Glm/Macro/Ifdef]] -- Macro to conditionally process GLM lines when a global variable is defined

# Synopsis

GLM:

~~~
#ifdef <name>
...
[#else]
...
#endif
~~~

# Description

The `#ifdef` macro is used to conditionally process GLM lines when the variable `<name>` is defined.  This is the opposite of the `#ifndef` macro.

# Examples

~~~
#ifndef YES
#print yes
#else
#print no
#endif
~~~

# See also

* [[/Glm/Macro/If]]
* [[/Glm/Macro/Ifndef]]

