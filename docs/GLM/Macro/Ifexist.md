[[/Glm/Macro/Ifexist]] -- Macro to conditionally process GLM lines when a file exists

# Synopsis
GLM:
~~~
#ifexist <path-name>
...
[#else]
...
#endif
~~~
or
~~~
#ifexist "<path-name>"
...
[#else]
...
#endif
~~~

# Description

The `#ifexist` macro is used to conditionally process GLM lines when a file is found.

# Examples

~~~
#ifexist "myfile.glm"
#print found it
#endif
~~~

# Caveats

1. TODO enumerate known issues using Ifexist

# See also
* [[/Glm/Macro/If]]

