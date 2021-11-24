[[/Glm/Macro/Ifmissing]] -- Macro to conditionally process GLM lines when a file missing

# Synopsis

GLM:

~~~
#ifmissing <path-name>
...
[#else]
...
#endif
~~~

or

~~~
#ifmissing "<path-name>"
...
[#else]
...
#endif
~~~

# Description

The `#ifmissing` macro is used to conditionally process GLM lines when a file is not found.

# Examples

~~~
#ifmissing "myfile.glm"
#print didn't find it
#endif
~~~

# See also

* [[/Glm/Macro/If]]
* [[/Glm/Macro/Ifexist]]

