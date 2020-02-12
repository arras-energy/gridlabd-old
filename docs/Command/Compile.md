[[/Command/Compile]] -- Command line option to only Compile a model

# Synopsis

~~~
bash$ gridlabd -C filename [options ...]
bash$ gridlabd --compile filename [options ...]
~~~

# Description

The `--Compile` or `-C` command line option is used to compile the model specified in the `filename`.

The option is somewhat is misnomer in the sense that GridLAB-D not only compiles the model, but it also sets the initial conditions for the solvers.  For this reason, the option is particularly useful to generate models for other solvers to use.


# See also

* [[/Command/Compileonly]]
* [[/Global/Compileonly]]
* [[/Global/Initializeonly]]