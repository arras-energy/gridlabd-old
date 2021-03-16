[[/Command/Template]] -- load a GLM template

# Synopsis

Shell:

~~~
$ gridlabd ... [-t|--template] <template-name> ...
~~~

# Description

The `-t` or `--template` command line option directs the loader to load a template GLM file from the current organization's template folder.  The standard location for a template is in the folder `/usr/local/share/gridlabd/template/$ORGANIZATION/<template-name>/<template-name>.glm`.

## `ORGANIZAITON`

The environment variable `ORGANIZATION` must be set in order for the template option to function properly. 

# Example

The following example loads the IEEE-13 bus model and run the ICA analysis template.

~~~
$ export ORGANIZATION=$(gridlabd template config get ORGANIZATION)
$ gridlabd template get ica_analysis
$ gridlabd IEEE-13.glm -t ica_analysis
~~~

# See also

* [[/Subcommand/Template]]
