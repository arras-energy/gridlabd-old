[[/Module/Powerflow/Global/Message_flags]] -- Module powerflow global variable message_flags

# Synopsis

Shell:

~~~
bash$ gridlabd -D|--define message_flags=<value>
~~~

GLM:

~~~
  #set message_flags=[NONE|QUIET|WARNING|VERBOSE|DEBUG|ALL]
~~~

# Description

The `message_flags` module global controls the default message handling for objects handled by the module.  The default value is `NONE`.  `ALL` enables the `VERBOSE` and `DEBUG` streams for objects with the `flags` set to `VERBOSE` or `DEBUG`, respectively.

Note the `VERBOSE` and `DEBUG` streams are only enabled when the `verbose` and `debug` global variables are set to `TRUE`, respectively.

# See also

* [[/Module/Powerflow]]
