[[/GLM/Macro/Subcommand]] -- Subcommand macros

# Synopsis

GLM:

~~~
#<subcommand> <options> ...
~~~

# Description

All subcommands can be executed by passing through the macro interpreter.  The general syntax takes the macro and prepends `gridlabd-` before calling the subcommand script installed in `/usr/local/opt/gridlabd/share/gridlabd`.

The stdout stream is piped to the GridLAB-D output stream and stderr is piped to the GridLAB-D error stream.

# Example

~~~
#aws s3 ls
~~~

# See also

* [[/Subcommand/Aws]]
* [[/Subcommand/Docker]]
* [[/Subcommand/Gdb]]
* [[/Subcommand/Git]]
* [[/Subcommand/Help]]
* [[/Subcommand/Library]]
* [[/Subcommand/Lldb]]
* [[/Subcommand/Manual]]
* [[/Subcommand/Python]]
* [[/Subcommand/Valgrind]]
* [[/Subcommand/Validate]]
* [[/Subcommand/Version]]
* [[/Subcommand/Weather]]
