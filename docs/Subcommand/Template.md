[[/Subcommand/Template]] -- template management subcommands

# Synopsis

Shell:

~~~
bash$ gridlabd template help
bash$ gridlabd template index <pattern>
bash$ gridlabd template list <pattern>
bash$ gridlabd template get <pattern>
bash$ gridlabd template submit <file>
bash$ gridlabd template delete <pattern>
bash$ gridlabd template info <pattern>
bash$ gridlabd template copy <name> [<file>]
bash$ gridlabd template clean
bash$ gridlabd template open <local>
bash$ gridlabd template session <local> [<pattern>]
bash$ gridlabd template add <local> <pattern>
bash$ gridlabd template save <local> ["<description>"]
bash$ gridlabd template submit <local>
bash$ gridlabd template config show
bash$ gridlabd template config reset
bash$ gridlabd template config get <name>
bash$ gridlabd template config set <name> <value>
bash$ gridlabd tempalte defaults <name>
~~~

# Description

Use the `template` subcommand to manage GridLAB-D's template files.  Templates may be either standalone `glm` files, or collections of files stored in `zip` file.  When a collection is used, the convention is to name the `zip` archive using the root of all the template files, e.g., `foo.zip` would contain files named `foo_1.glm`, `foo_2.csv`, and `foo_3.py`.

## `help`

~~~
  gridlabd template help
~~~

The `template help` subcommand obtains a list of the valid commands.

~~~
bash$ gridlabd template help
Syntax: gridlabd-template [options] <command> [...]
Options:
  -b|--branch <branch>      Change branch from which template is downloaded
  -c|--contenturl <url>     Change URL for downloading template
  -d|--debug                Enable debugging output
  -f|--format <format>      Change output format (e.g., json, glm, csv, default)
  -g|--githuburl <url>      Change URL for editing template
  -o|--organization <org>   Change organization of template
  -q|--quiet                Suppress unnecessary output to stderr
  -qq|--veryquiet           Suppress all output to stderr
  -r|--repository <repo>    Change repository name of template
  -u|--user <login>         Change user name for editing template
  -v|--verbose              Enable verbose output
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
bash$ gridlabd template index wood_pole
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

Change default template subcommand settings.

## `defaults`

~~~
  gridlabd template defaults <name>
~~~

Output the default values for template configuration CSV file used by the template `<name>`.
