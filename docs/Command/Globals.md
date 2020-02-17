[[/Command/Globals]] -- Displays a sorted list of all global variables

# Synopsis

~~~
bash$ gridlabd --globals                                               
~~~

# Description

The `--globals` command line option dumps the globals.  If `<name>` is given, only the value of the named global is dumped.

If used alone on the command line, the default value of the global is dumped.  If used after a file is loaded, the value of the global after the load is complete is dumped.

# Example
~~~
bash$ gridlabd --globals=strictnames
~~~
