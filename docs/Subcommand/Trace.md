[[/Subcommand/Trace]] -- Trace gridlabd core images

# Synopsis

~~~
gridlabd trace help
gridlabd trace save <job>
gridlabd trace debug <job>
gridlabd trace dump <job>
~~~

# Description

The `trace` command is used to cause an immediate core memory dump of the specified job and perform various actions on the resulting file.

To get a list of jobs, see the [[/Command/Plist]] command line option.

## `save <job>`

The `save <job>` command save a core memory dump of the specified gridlabd job to be saved to a file.

## `debug <job>`

The `debug <job>` start the default system debugger on the specified gridlabd job.

## `dump`

The `dump <job>` command saves a core memory dump file and prints its contents to the screen.

# See also

* [[/Command/Plist]]
