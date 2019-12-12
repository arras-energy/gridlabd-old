[[/Glm/Macro/Setenv]] -- Macro to set an environment variable

# Synopsis
GLM:
~~~
#setenv <name>=<value>
~~~
Shell:
~~~
bash$ export <name>=<value> ; gridlabd ...
~~~

# Description

The `#setenv` macro sets the environment variable `<name>` to `<value>`.

When GridLAB-D evaluates a global variable, and the global variable is not found, then GridLAB-D will attempt to resolve the name using the environment variables.

# Examples

~~~
#setenv NAME=value
~~~

# Caveats

Some environment variables, such a `LD_LIBRARY_PATH` present a security risk and cannot be set inside a GLM file.  In such cases, the environment variable must be set by the shell that calls GridLAB-D.

# See also
* [[/Glm/Macro/Define]]
* [[/Glm/Macro/Set]]

