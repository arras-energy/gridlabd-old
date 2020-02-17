[[/Glm/Macro/System]] -- Macro to run a command in the host system environment

# Synopsis

GLM:

~~~
#system <command>
~~~

# Description

The `#system` macro executes `<command>` in the system environment.  If the command does not specify the full path to an executable, then it must be found in the `PATH` environment.  

# Examples

~~~
#system pwd ; ls -l
~~~

# Caveats

The return code is ignored. If you need to have the GLM load fail when the return code is non-zero, use the `#exec` macro instead.

# See also

* [[/GLM/Macro/Exec]]

