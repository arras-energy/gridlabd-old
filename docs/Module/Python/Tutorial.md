[[/Module/Python/Tutorial]]


# Introduction

The `python` module allow HiPAS GridLAB-D models to be manipulated using Python code, and GridLAB-D simulation of those models to be controlled.

There are three basic runtime architectures possible when using `python` with `gridlabd` at runtime, as shown in Figure 1. Which architecture you are using will depend on how the command line arguments are given.

[image:Tutorial_figure_1.png]
Figure 1: Runtime architectures for Python and GridLAB-D

When using `python` as a module in GridLAB-D (a), the command line arguments only includes the `gridlabd` model file, e.g., a `.glm` file. (Automatically converted files can also be used provided they generate a `.glm` file that includes the appropriate `python` module directives.)

When using `python` in a hybrid module structure (b), the command line arguments include both the `python` code and the `gridlabd` model.

When using the `python` module only (c), the command line arguments inlcude only the `python` code. (Automatically converted files can also be used provided they generate a `.py` file that includes the appropriate `gridlabd` module calls.)


# Python Module

A Python module is run when it is given on the command file, such as

~~~
bash$ gridlabd main.py
~~~

The structure of this example is shown in Figure 2.

[image:Tutorial_figure_2.png]

The file `main.py` is loaded into the `python` main module.  This file loads the `gridlabd` module, and uses the `command` method to load the GridLAB-D model `model.glm`. 

The model loads the python module that implements that models event handlers. In this example there are two kinds of event handlers.  The first is the global event handler `on_init` that is called when the simulation initializes.  The second is the object event handler `on_commit` that is called whenever the object performs a `commit` operation.
