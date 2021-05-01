[[/Subcommand/Require]] -- Require a module or package

# Synopsis

GLM:

~~~
#require [-v|--verbose] [-r|--requirements filename] [modules ...]
~~~

Shell:
~~~
bash$ gridlabd require [-v|--verbose] [-r|--requirements filename] [modules ...]
~~~

# Description

The `require` subcommand verifies that one or more python modules are installed and/or meet minimum version requirements.

If no options are given, then the current requirements are output.

## Options

### `-v` or `--verbose`

Enables verbose output in the event that a module is installed.

### `-r filename` or `--requirements filename`

Specifies that a requirements file should be used to specify one or more module version requirements.

# See also

* [[/Subcommand/Python]]
