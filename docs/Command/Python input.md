[[/Command/Python input]] -- Command line option for running Python scripts

# Synopsis

~~~
bash$ gridlabd [...] <script-name>.py [...]
bash$ gridlabd [...] '<script-name>.py <arg1> <arg2> ... <argN>' [...]
~~~

# Description

GridLAB-D supports Python scripts on the command line, e.g.,

~~~
bash$ gridlabd my-script.py
~~~

will run the script `my-script.py` in the current GridLAB-D Python environment.

If you wish to include command line arguments for the python script, then you should enclose the arguments in the python script name argument, e.g.,

~~~
bash$ gridlabd 'my-script.py my-arg1 my-arg2'
~~~

# Caveat

Only `python3` is supported, and the Python executable must installed (or linked) in the `/usr/local/bin` folder.

# See also

* [[/Module/Python]]
* [[/Subcommand/Python]]
