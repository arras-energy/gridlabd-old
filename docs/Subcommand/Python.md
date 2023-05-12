[[/Subcommand/Python]] -- subcommand to run Python in a GridLAB-D environment

# Synopsis

Shell:

~~~
bash$ gridlabd python <options> ...
~~~

# Description

Normally GridLAB-D requires certain environment variables to be set to function properly.  If you are running Python code, and would like to use the GridLAB-D module, you can run Python inside that GridLAB-D environment using the `python` subcommand, where `<options>` are the Python command options

# Caveat

Only `python3` is supported, and the executable must be located at `/usr/local/opt/gridlabd/<pkg>/bin/python3`.

# Example

~~~
bash$ gridlabd python
  Python 3.7.2 (default, Feb 12 2019, 08:15:36) 
  [Clang 10.0.0 (clang-1000.11.45.5)] on darwin
  Type "help", "copyright", "credits" or "license" for more information.
  >>> import os
  >>> os.environ["GLPATH"]
  '/usr/local/opt/gridlabdlib/gridlabd:/usr/local/opt/gridlabd/share/gridlabd'
~~~

# See also

* [[/Command/Validation]]
* [[/Module/Python]]