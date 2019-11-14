[[Python subcommand]] - subcommand to run Python in a GridLAB-D environment

# Synopsis
~~~
  host% gridlabd python options
~~~
where `options` are the python command options

# Description

Normally GridLAB-D requires certain environment variables to be set to function properly.  If you are running Python code, and would like to use the GridLAB-D module, you can run python inside that GridLAB-D environment using the `python` subcommand.

# Example
~~~
  host% gridlabd python
  Python 3.7.2 (default, Feb 12 2019, 08:15:36) 
  [Clang 10.0.0 (clang-1000.11.45.5)] on darwin
  Type "help", "copyright", "credits" or "license" for more information.
  >>> import os
  >>> os.environ["GLPATH"]
  '/usr/local/lib/gridlabd:/usr/local/share/gridlabd'
~~~