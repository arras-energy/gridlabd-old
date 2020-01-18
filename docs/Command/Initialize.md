[[/Command/Initialize]] -- Command line option to only initialize a model

# Synopsis
~~~
bash$ gridlabd -I filename [options ...]
bash$ gridlabd --initialize filename [options ...]
~~~

# Description

The `--initialize` or `-I` command line option is used to compile and initialize the model specified in the `filename`.

The option is somewhat is misnomer in the sense that GridLAB-D not only creates the model and sets initial conditions for the solvers, but it also attempts to obtain the first solution in time.  For this reason, the option is particularly useful to validate a model.

# See also

* [[/Command/Compileonly]]
* [[/Global/Compileonly]]
* [[/Global/Initializeonly]]
