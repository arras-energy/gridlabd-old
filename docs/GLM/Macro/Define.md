[[/Glm/Macro/Define]] -- Macro to define a new global variable

# Synopsis
GLM:
~~~
#define <variable-name>=<value>
~~~

# Description

The `#define` macro is used to define a new variable.

# Examples

~~~
#define NAME=value
~~~

# Caveats

Normally, one can only define a variable that isn't already defined.  If you need to loosen this restriction, use the `strictnames` global variable. Otherwise, you must use `#set` to set the value of an existing variable.

# See also
* [[/GLM/Macro/Set]]
* [[/Global/Strictnames]]

