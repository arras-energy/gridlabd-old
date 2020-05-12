[[/Module/Python/Tutorial]] -- Python module tutorial

# Introduction

The `python` module allow HiPAS GridLAB-D models to be manipulated using Python code, and GridLAB-D simulation of those models to be controlled.

There are three basic runtime architectures possible when using `python` with `gridlabd` at runtime, as shown in Figure 1. Which architecture you are using will depend on how the command line arguments are given.

[image:Tutorial_figure_1.png]
Figure 1: Runtime architectures for Python and GridLAB-D

When using `python` as a module in GridLAB-D (a), the command line arguments only includes the `gridlabd` model file, e.g., a `.glm` file. (Automatically converted files can also be used provided they generate a `.glm` file that includes the appropriate `python` module directives.)

When using `python` in a hybrid module structure (b), the command line arguments include both the `python` code and the `gridlabd` model.

When using the `python` module only (c), the command line arguments inlcude only the `python` code. (Automatically converted files can also be used provided they generate a `.py` file that includes the appropriate `gridlabd` module calls.)

# GridLAB-D Module

A GridLAB-D module is used when the command line includes only `.glm` files, e.g.,

~~~
bash$ gridlabd example.glm
~~~

When running `python` as a GridLAB-D module, the GLM file must contain a `module` directive to load the python module that implements your `python` code, e.g.,

~~~
module example_module;
~~~

In situations where you would like to define module globals for use in the simulation, you can add them to the module globals, e.g.,

~~~
module example_module
{
    example_global "Example";
}
~~~

# Hybrid Module

TODO

# Python Module

A Python module is run when it is given on the command file, such as

~~~
bash$ gridlabd example.py
~~~

The structure of this example is shown in Figure 2.

[image:Tutorial_figure_2.png]


