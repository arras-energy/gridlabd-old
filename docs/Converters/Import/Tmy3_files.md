[[/Converters/Input/Tmy3]] -- TMY3 file input converter

# Synopsis

~~~
#input "filename.tmy3" [-p|--property NAME=VALUE] [...]
~~~

# Description

The TMY3 converter creates the GLM objects needed to load the specified TMY3 data into the current model.

The GLM contents is always generated as follows:

~~~
#weather get <filename>.tmy3
module climate;
object climate 
{
	tmyfile "<filename>.tmy3";
	NAME "VALUE";
	...
}
~~~

# Caveat

The [[/Subcommand/Weather]] operation requires internet access to GitHub to download the specified weather file.  To obtain a list a known TMY3 files, use the `gridlabd weather index [pattern]` command.  To obtain a list of downlaoded TMY3 files, use the `gridlabd weather list [pattern]` command.

# See also

* [[/Converters/Input/Csv_files]]
* [[/Subcommand/Weather]]
