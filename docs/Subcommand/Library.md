[[library]] -- library management subcommands

# Synopsis
~~~
host% gridlabd help
host% gridlabd index <pattern>
host% gridlabd list <pattern>
host% gridlabd get <pattern>
host% gridlabd submit <file>
host% gridlabd delete <pattern>
host% gridlabd info <pattern>
host% gridlabd copy <name> [<file>]
host% gridlabd clean
host% gridlabd open <local>
host% gridlabd session <local> [<pattern>]
host% gridlabd add <local> <pattern>
host% gridlabd save <local> ["<description>"]
host% gridlabd submit <local>
host% gridlabd config show
host% gridlabd config reset
host% gridlabd config get <name>
host% gridlabd config set <name> <value>
~~~

# Description

Use the [[library]] subcommand to manage GridLAB-D's library files.

## Help

The [[library help]] subcommand obtains a list of the valid commands.
~~~
host% gridlabd library help
Syntax: gridlabd-library <command> [...]
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

## Index _pattern_

The `index` command outputs a list of library files in the archive that match <pattern>.  
~~~
host% gridlabd index wood_pole
wood_pole.glm
~~~

## List _pattern_

The `list` command outputs a list of the library files that are available locally.

## Get _pattern_

The `get` command obtains a local copy of library files.

## Delete _pattern_

The `delete` command removes local copies of library files.

## Info _pattern_

The `info` command outputs details about library files

## Copy _name_ [_file_]

Make a working copy of a library file.

## Clean

Clean the local cache of the library index.

## Open _local_

Open a local working session of the library archive.

## Session _local_ [_pattern_]

Output a list of the local working sessions. The first one is the active session.

## Add _local_ _pattern_

Add the files matching _pattern_ to the current working session.

## Save _local_ ["_description_"]

Add the current working session.

## Submit _local_

Submit the local working session to the archive for review.

## Config {show|reset|get _name_|set _name_ _value_}

Manage the library manager configuration.
