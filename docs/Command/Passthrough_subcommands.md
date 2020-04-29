[[/GLM/Macro/Passthrough_subcommands]] -- Passthrough subcommands

# Synopsis

GLM:

~~~
#aws [options ...]
#docker [options ...]
#gdb [options ...]
#git [options ...]
#help [options ...]
#library [options ...]
#lldb [options ...]
#manual [options ...]
#python [options ...]
#valgrind [options ...]
#validate [options ...]
#version [options ...]
#weather [options ...]
~~~

# Description

All subcommands can be executed by passing through the macro interpreter.  The general syntax takes the macro and prepends `gridlabd-` before calling the subcommand script installed in `/usr/local/share/gridlabd`.

The stdout stream is piped to the GridLAB-D output stream and stderr is piped to the GridLAB-D error stream.

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
