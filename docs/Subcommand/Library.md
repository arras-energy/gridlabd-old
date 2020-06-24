[[/Subcommand/Library]] -- library management subcommands

# Synopsis

Shell:

~~~
bash$ gridlabd help
bash$ gridlabd index <pattern>
bash$ gridlabd list <pattern>
bash$ gridlabd get <pattern>
bash$ gridlabd submit <file>
bash$ gridlabd delete <pattern>
bash$ gridlabd info <pattern>
bash$ gridlabd copy <name> [<file>]
bash$ gridlabd clean
bash$ gridlabd open <local>
bash$ gridlabd session <local> [<pattern>]
bash$ gridlabd add <local> <pattern>
bash$ gridlabd save <local> ["<description>"]
bash$ gridlabd submit <local>
bash$ gridlabd config show
bash$ gridlabd config reset
bash$ gridlabd config get <name>
bash$ gridlabd config set <name> <value>
~~~

# Description

Use the `library` subcommand to manage GridLAB-D's library files.

## `help`

~~~
  gridlabd help
~~~

The `library help` subcommand obtains a list of the valid commands.

~~~
bash$ gridlabd library help
Syntax: gridlabd-library [options] <command> [...]
Options:
  -f|--format <format> Change output format (e.g., json, glm, csv, default)
Commands:
  help                 Get the list of library subcommands
  index <pattern>      Index of available library data matching <pattern> in archive
  list <pattern>       List of available downloaded library data matching <pattern>
  get <pattern>        Download library data matching <pattern> from archive 
  delete <pattern>     Delete downloaded library data matching <pattern>
  info <pattern>       Get information about library data in <pattern>
  copy <name> [<file>] Copy the contents of the library <name> into <file>
  clean                Clean up the local cache of the archive index
  open <local>         Open an editing session of library in <local>
  session <local> [<pattern>]  
                       List current sessions in <local> (active session is first)
  add <local> <pattern>
                       Add files <pattern> to the <local> repository
  save <local> ["<description>"]
                       Save the <local> library changes
  submit <local>       Submit the saved changes to the library
  config show          Show the current configuration
         reset         Reset the current configuration
         set <N> <V>   Set configuration variable name <N> to value <V>
         get <N> <V>   Get configuration variable name <N>
~~~

## `index`

~~~
  gridlabd library index <pattern>
~~~

The `index` command outputs a list of library files in the archive that match <pattern>.
  
~~~
bash$ gridlabd index wood_pole
wood_pole.glm
~~~

## `list`

~~~
  gridlabd library list <pattern>
~~~

The `list` command outputs a list of the library files that are available locally.

## `get`

~~~
  gridlabd library get <pattern>
~~~

The `get` command obtains a local copy of library files.

## `delete`

~~~
  gridlabd library delete <pattern>
~~~

The `delete` command removes local copies of library files.

## `info`

~~~
  gridlabd library info <pattern>
~~~

The `info` command outputs details about library files

## `copy`

~~~
  gridlabd library copy <name> [<file>]
~~~

Make a working copy of a library file.

## `clean`

~~~
  gridlabd library clean
~~~

Clean the local cache of the library index.

## `open`

~~~
  gridlabd library open <local>
~~~

Open a local working session of the library archive.

## `session`

~~~
  gridlabd library session <local> [<pattern>]
~~~

Output a list of the local working sessions. The first one is the active session.

## `add`

~~~
  gridlabd library add <local> <pattern>
~~~

Add the files matching _pattern_ to the current working session.

## `save`

~~~
  gridlabd library save <local> ["<description>"]
~~~

Add the current working session.

## `submit`

~~~
  gridlabd library submit <local>
~~~

Submit the local working session to the archive for review.

## `config`

~~~
  gridlabd library config {show|reset|get}
  gridlabd library config set <name> <value>
~~~

Manage the library manager configuration.
