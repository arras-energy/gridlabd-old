[[/Glm/Macro/Start]] -- Macro to asynchronously start a process

# Synopsis

GLM:

~~~
#start <command>
~~~

# Description

The `#start` macro run `<command>` in the background and immediately return. A zero exit code indicates that the process was started ok. A non-zero exit code will cause the GLM load to fail.

# Examples

The following command starts the GridLAB-D daemon process.

~~~
#start gridlabd --daemon
~~~

# See also

* [[/GLM/Macro/System]]
* [[/GLM/Macro/Exec]]

