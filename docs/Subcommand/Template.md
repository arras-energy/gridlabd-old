[[/Subcommand/Template]] -- template management subcommands

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

Use the `template` subcommand to manage GridLAB-D's template files.

## `help`

~~~
  gridlabd help
~~~

The `template help` subcommand obtains a list of the valid commands.

~~~
bash$ gridlabd template help
Syntax: gridlabd-template [options] <command> [...]
Options:
  -f|--format <format> Change output format (e.g., json, glm, csv, default)
Commands:
  help                 Get the list of template subcommands
  index <pattern>      Index of available template data matching <pattern> in archive
  list <pattern>       List of available downloaded template data matching <pattern>
  get <pattern>        Download template data matching <pattern> from archive 
  delete <pattern>     Delete downloaded template data matching <pattern>
  info <pattern>       Get information about template data in <pattern>
  copy <name> [<file>] Copy the contents of the template <name> into <file>
  clean                Clean up the local cache of the archive index
  open <local>         Open an editing session of template in <local>
  session <local> [<pattern>]  
                       List current sessions in <local> (active session is first)
  add <local> <pattern>
                       Add files <pattern> to the <local> repository
  save <local> ["<description>"]
                       Save the <local> template changes
  submit <local>       Submit the saved changes to the template
  config show          Show the current configuration
         reset         Reset the current configuration
         set <N> <V>   Set configuration variable name <N> to value <V>
         get <N> <V>   Get configuration variable name <N>
~~~

## `index`

~~~
  gridlabd template index <pattern>
~~~

The `index` command outputs a list of template files in the archive that match <pattern>.
  
~~~
bash$ gridlabd index wood_pole
wood_pole.glm
~~~

## `list`

~~~
  gridlabd template list <pattern>
~~~

The `list` command outputs a list of the template files that are available locally.

## `get`

~~~
  gridlabd template get <pattern>
~~~

The `get` command obtains a local copy of template files.

## `delete`

~~~
  gridlabd template delete <pattern>
~~~

The `delete` command removes local copies of template files.

## `info`

~~~
  gridlabd template info <pattern>
~~~

The `info` command outputs details about template files

## `copy`

~~~
  gridlabd template copy <name> [<file>]
~~~

Make a working copy of a template file.

## `clean`

~~~
  gridlabd template clean
~~~

Clean the local cache of the template index.

## `open`

~~~
  gridlabd template open <local>
~~~

Open a local working session of the template archive.

## `session`

~~~
  gridlabd template session <local> [<pattern>]
~~~

Output a list of the local working sessions. The first one is the active session.

## `add`

~~~
  gridlabd template add <local> <pattern>
~~~

Add the files matching _pattern_ to the current working session.

## `save`

~~~
  gridlabd template save <local> ["<description>"]
~~~

Add the current working session.

## `submit`

~~~
  gridlabd template submit <local>
~~~

Submit the local working session to the archive for review.

## `config`

~~~
  gridlabd template config {show|reset|get}
  gridlabd template config set <name> <value>
~~~

Manage the library manager configuration.
