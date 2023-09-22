
The `python` module allows Arras Energy models to be manipulated using Python code, and GridLAB-D simulations of those models to be controlled directly from Python.

# Runtime Structure

There are three basic runtime architectures possible when using `python` with `gridlabd` at runtime, as shown in Figure 1. Which architecture you are using will depend on how the command line arguments are given.

[image:Tutorial_figure_1.png]
Figure 1: Runtime architectures for Python and GridLAB-D

When using `python` as a module in GridLAB-D (a), the command line arguments only includes the `gridlabd` model file, e.g., a `.glm` file. (Automatically converted files can also be used provided they generate a `.glm` file that includes the appropriate `python` module directives.)

When using `python` in a hybrid module structure (b), the command line arguments include both the `python` code and the `gridlabd` model.

When using the `python` module only (c), the command line arguments inlcude only the `python` code. (Automatically converted files can also be used provided they generate a `.py` file that includes the appropriate `gridlabd` module calls.)

---

# GridLAB-D Model

A GridLAB-D model structure is used when using a command line such as

~~~
bash$ gridlabd model.glm
~~~

The structure of this simulation is shown in Figure 2.

[image:Tutorial_figure_2.png]
Figure 2: Using a python module in a GridLAB-D model

The following sections explain how these two files generally interact.

## `model.glm`

The file `model.glm` loads the python handlers module, which implements the model's event handlers. In Figure 2 there are two kinds of event handlers.  The first is the global event handler `on_init(timestamp)` that is called by the core solver when the simulation initializes, if it is defined.  The available global event handlers that can be defined are

- `on_init(timestamp)`: This event handler is called when the simulation is initialized. The value of `timestamp` is the start time of the simulation. The `on_init` event handler must return `True` on success and `False` on failure.

- `on_precommit(timestamp)`: This event handler is called when the simulation solver is about to begin a series of object sychronization iterations. The value of `timestamp` is the clock time to which the simulation is synchronizing objects. The `on_precommit` event handler must return a timestamp.  If the time value returned is less than or equal to `timestamp`, the event handler fails.

- `on_presync(timestamp)`: This event handler is called when the simulation is about to begin a solver iteration. The value of `timestamp` is the clock time to which the simulation is synchronizing objects. If the time value returned is less than to `timestamp`, the event handler fails. If the time value returned is equal to `timestamp` the solver will iterate again with the same value as `timestamp`.  Returning a time value greater than `timestamp` will cause the solver call the event handler again *no later than* the time value.

- `on_sync(timestamp)`: This event handler is called when the simulation runs a solver iteration. The value of `timestamp` is the clock time to which the simulation is synchronizing objects. If the time value returned is equal to `timestamp` the solver will iterate again with the same value as `timestamp`.  Returning a time value greater than `timestamp` will cause the solver call the event handler again *no later than* the time value.

- `on_postsync(timestamp)`: This event handler is called when the simulation has completed a solver iteration. The value of `timestamp` is the clock time to which the simulation is synchronizing objects. If the time value returned is equal to `timestamp` the solver will iterate again with the same value as `timestamp`.  Returning a time value greater than `timestamp` will cause the solver call the event handler again *no later than* the time value.

- `on_commit(timestamp)`: This event handler is called when the simulation has completed a series of object synchronization iterations. The value of `timestamp` is the clock time to which the simulation is synchronizing objects. The `on_precommit` event handler must return a timestamp.  If the time value returned is less than or equal to `timestamp`, the event handler fails.

- `on_term(timestamp)`: This is called when the simulation is terminated. The value of `timestamp` is set to the stop time of the simulation, or the time at which the simulation encountered an error, if any. No return value is expected.

The second is the object event handler `commit(object,timestamp)` that is called whenever the object in which it is specified performs a `commit` operation. Only objects that define an `on_commit` property will call commit event handlers. To call a python event handler, the syntax must be `python:<module-name>.<handler-name>`. If the colon is omitted, the call will be made to an external python executable, rather than the handlers module.

The following object event handlers are allowed:

- `on_init python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation is initialized. The return value has the same meaning as the module `on_init` event handler.

- `on_create python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation creates the object. The return value has the same meaning as the module `on_init` event handler.

- `on_precommit python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation starts a clock synchronization on the object. The return value has the same meaning as the module `on_precommit` event handler.

- `on_presync python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation start a solver iteration on the object. The return value has the same meaning as the module `on_presync` event handler.

- `on_sync python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation performs a solver iteration on the object. The return value has the same meaning as the module `on_sync` event handler.

- `on_postsync python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation ends a solver iteration on the object. The return value has the same meaning as the module `on_postsync` event handler. 

- `on_commit python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation completes a clock synchronization on the object.  The return value has the same meaning as the module `on_commit` event handler.

- `on_finalize python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation has completed updating the object.  The return value has the same meaning as the module `on_term` event handler.

- `on_term python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation is ready to destroy the object.  The return value has the same meaning as the module `on_term` event handler.

## `handlers.py`

The `handlers` module implements all the event handlers used by the model.  In Figure 2, the `on_init(<timestamp>)` handler is called automatically after the objects are initialized because it is defined in the `handlers` module. During this event, any operations that needs to be completed prior to the start of the simulation can be performed.

The `commit(object,timestamp)` handler is also called for each object that defines the `on_commit` roperty.  During these events data that needs to be collected can gathered from specific object.  Similarly, a `precommit` event can be used to make changes to objects before the next simulation clock update.

Any handler that returns an object that evaluates to `False` will halt the simulation with an error. If the event handlers is associated with a specific object, the error will be attributed to that object.

## Example

The following example generates a plot of a random number sequence generated by a test class. The values are recorded in a CSV file, which is used to generate a plot when the simulation is done.

### `model.glm`:

The model file loads the Python handlers module, sets up the clock, defines the class for the random number generator, defines the test object that generates the random numbers, loads the tape module, and defines the recorder that collects the random number generators into a CSV file.

[code:/python/examples/example_1/model.glm]

### `handlers.py`:

The handlers file defines the `on_term` event handler that loads the resulting CSV file, generates the plot, and saves it in `example.png`.

[code:/python/examples/example_1/handlers.py]

---

# Hybrid Module

A hybrid module structure is used when using a command line such as

~~~
bash$ gridlabd main.py model.glm
~~~

The structure of this simulation is shown in Figure 3.

[image:Tutorial_figure_3.png]
Figure 3: Using a hygrid python module and GridLAB-D model simulation

The following sections explain how these three files generally interact.

## `main.py`

The file `main.py` is loaded into the `python` main module.  At this point any python code may be executed, as well as defining classes, methods, and global python data structures.

## `model.glm`

The model loads the python handlers module implements that model's event handlers. The model can then load other modules, and define classes and objects needed in the simulation.

## `handlers.py`

The handlers module implements all the event handlers used by the model.  Any handler that returns an object that evaluates to `False` in Python will halt the simulation.

## Example

The following example generates a plot of a random number sequence generated by a test class. The values are collected by an object event handler. The plot is generated by a module event handler.

### `main.py`:

The main file verifies the version of Python.

[code:/python/examples/example_2/main.py]

### `model.glm`:

The model file loads the handlers and sets some global variables that will be used when generating the plot.  The clock is then set, the test class defined, and the object that generates the random numbers is instantiated, with an `on_commit` event handler to permit sampling the random numbers when they are generated.

[code:/python/examples/example_2/model.glm]

### `handlers.py`:

The handlers module defines an `on_init` module event handler to set up the data collection system. It also defines an `on_term` module event handler to generate the plot when the simulation is done. Finally, it defines a `commit` handler to collect the random numbers generated by the test object.

[code:/python/examples/example_2/handlers.py]

---

# Python Module

A Python module structure is used when a using a command line such as

~~~
bash$ gridlabd main.py
~~~

The structure of this simulation is shown in Figure 4.

[image:Tutorial_figure_4.png]
Figure 4: Using a GridLAB-D in a Python module

The following sections explain how these two files generally interact.

## `main.py`

The file `main.py` loads the `gridlabd` module and uses the `command()` method to load the GridLAB-D model `model.glm` into the simulator. After the model is loaded the simulation can be started using the `start()` method.


## `model.glm`

The GridLAB-D model defines the objects to be implemented in the simulation. 

## Example

The following example illustrates how to use GridLAB-D in a python module.  

### `main.py`:

The main module imports the required modules, loads the GridLAB-D model, and runs it, waiting for it to complete.  After the simulation is done, the module loads the output data and plots it.

[code:/python/examples/example_3/main.py]

### `model.glm`:

The model file sets the clock, creates the test class, loads the tape module, and defines the recorder to generate the output data.

[code:/python/examples/example_3/model.glm]


