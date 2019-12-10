[[/Glm/Macro/Set]] -- Macro to set a global variable

# Synopsis
GLM:
~~~
#set <name>=<value>
~~~
Shell:
~~~
bash$ gridlabd -D <name>=<value> ...
bash$ gridlabd --define <name>=<value> ...
~~~

# Description

The `#set` macro sets the value of an existing global variable.  If the variable is not defined, the `#set` macro will fail.

# Examples

~~~
#set name=value
~~~

# Caveats

Normally, one can only set a variable that is already defined.  If you need to loosen this restriction, use the `strictnames` global variable. Otherwise, you must use `#define` to define the value of a new variable.

# See also
* [[/Glm/Macro/Define]]
* [[/Global/Strictnames]]
