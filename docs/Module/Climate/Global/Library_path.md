[[/Module/Climate/Global/Library_path]] -- Define the library path for climate and weather data

# Synopsis

GLM:

~~~
module climate
{
	library_path "/usr/local/share/gridlabd/US";
}
~~~

Shell:

~~~
bash$ gridlabd -D climate::library_path=/usr/local/share/gridlabd/US ...
~~~

# Description

The `library_path` module global specifies the search path climate and weather data files.  The default value is the system cache folder `/usr/local/share/gridlabd/US`.

# See also

* [[/GLM/Macro/Subcommands]]
* [[/Subcommand/Weather]]
