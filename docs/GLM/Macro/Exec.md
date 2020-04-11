[[/Global/Exec]] -- Execute macro

# Synopsis

~~~
#exec <command>
~~~

# Description

The `#exec` macro is similar to the `#system` macro, except that it will cause the load to fail if the command has a non-zero exit code. Exec also redirect the command output through the normal GridLAB-D streams, i.e., `stdout` goes to normal message output and `stderr` goes to error message output.

# See also

* [[/Global/System]]
