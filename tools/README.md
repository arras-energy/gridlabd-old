# Tools

The tools folder contains tools are python modules that do not require write access to `gridlabd` shared folders for caching and sharing files among multiple users.  

The general paradigm for running is a tools is either from the command line, e.g.,

~~~
gridlabd python -m TOOLNAME [ARGUMENTS ...]
~~~

or from a loader macro, e.g.,

~~~
#python -m TOOLNAME [ARGUMENTS ...]
~~~

Following standard programming convention for Python script is sufficient for creating tools. It is also good practice to include argument processing that support `-h`, `--help`, and `help`. This will enable user to run the command

~~~
gridlabd python -m TOOLNAME help
~~~

to obtain help on tool usage.
