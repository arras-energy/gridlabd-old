
The `python` module allows HiPAS GridLAB-D models to be manipulated using Python code, and GridLAB-D simulations of those models to be controlled directly from Python.

# Runtime Structure

There are three basic runtime architectures possible when using `python` with `gridlabd` at runtime, as shown in Figure 1. Which architecture you are using will depend on how the command line arguments are given.

[image:Tutorial_figure_1.png]
Figure 1: Runtime architectures for Python and GridLAB-D

When using `python` as a module in GridLAB-D (a), the command line arguments only includes the `gridlabd` model file, e.g., a `.glm` file. (Automatically converted files can also be used provided they generate a `.glm` file that includes the appropriate `python` module directives.)

When using `python` in a hybrid module structure (b), the command line arguments include both the `python` code and the `gridlabd` model.

When using the `python` module only (c), the command line arguments inlcude only the `python` code. (Automatically converted files can also be used provided they generate a `.py` file that includes the appropriate `gridlabd` module calls.)

---

# GridLAB-D Model

A GridLAB-D model structure is using a command line such as

~~~
bash$ gridlabd model.glm
~~~

The structure of this example is shown in Figure 2.

[image:Tutorial_figure_2.png]
Figure 2: Using a python module in a GridLAB-D model

The following sections explain how these three files generally interact.

## `model.glm`

The file `model.glm` loads the python handlers module, which implements the model's event handlers. In Figure 2 there are two kinds of event handlers.  The first is the global event handler `on_init(timestamp)` that is called by the core solver when the simulation initializes, if it is defined.  The available global event handlers that can be defined are

- `on_init(timestamp)`: This event handler is called when the simulation is initialized. The value of `timestamp` is the start time of the simulation.

- `on_precommit(timestamp)`: This event handler is called when the simulation solver is about to begin a series of object sychronization iterations. The value of `timestamp` is the clock time to which the simulation is synchronizing objects.

- `on_presync(timestamp)`: This event handler is called when the simulation is about to begin a solver iteration. The value of `timestamp` is the clock time to which the simulation is synchronizing objects.

- `on_sync(timestamp)`: This event handler is called when the simulation runs a solver iteration. The value of `timestamp` is the clock time to which the simulation is synchronizing objects.

- `on_postsync(timestamp)`: This event handler is called when the simulation has completed a solver iteration. The value of `timestamp` is the clock time to which the simulation is synchronizing objects.

- `on_commit(timestamp)`: This event handler is called when the simulation has completed a series of object synchronization iterations. The value of `timestamp` is the clock time to which the simulation is synchronizing objects.

- `on_term(timestamp)`: This is called when the simulation is terminated. The value of `timestamp` is set to the stop time of the simulation, or the time at which the simulation encountered an error, if any.

The second is the object event handler `commit(object,timestamp)` that is called whenever the object in which it is specified performs a `commit` operation. Only objects that define an `on_commit` property will call commit event handlers. To call a python event handler, the syntax must be `python:<module-name>.<handler-name>`. If the colon is omitted, the call will be made to an external python executable, rather than the handlers module.

The following object event handlers are allowed:

- `on_init python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation is initialized.

- `on_create python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation creates the object.

- `on_precommit python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation starts a clock synchronization on the object.

- `on_presync python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation start a solver iteration on the object.

- `on_sync python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation performs a solver iteration on the object.

- `on_postsync python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation ends a solver iteration on the object.

- `on_commit python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation completes a clock synchronization on the object. 

- `on_finalize python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation has completed updating the object.

- `on_term python:<module>:<handler>;`: Define this object property to call the `handler(object,timestamp)` in the specified `module` when the simulation is ready to destroy the object.

[code:/gldcore/link/python/examples/example_1/model.glm]

## `handlers.py`

The `handlers` module implements all the event handlers used by the model.  In Figure 2, the `on_init(<timestamp>)` handler is called automatically after the objects are initialized because it is defined in the `handlers` module. During this event, any operations that needs to be completed prior to the start of the simulation can be performed.

The `commit(object,timestamp)` handler is also called for each object that defines the `on_commit` roperty.  During these events data that needs to be collected can gathered from specific object.  Similarly, a `precommit` event can be used to make changes to objects before the next simulation clock update.

Any handler that returns an object that evaluates to `False` will halt the simulation with an error. If the event handlers is associated with a specific object, the error will be attributed to that object.

[code:/gldcore/link/python/examples/example_1/handlers.py]

---

# Hybrid Module

A hybrid module structure is used when it is given on the command file, such as

~~~
bash$ gridlabd main.py
~~~

The structure of this example is shown in Figure 3.

[image:Tutorial_figure_3.png]
Figure 3: Hybrid Python and GridLAB-D modules

The following sections explain how these three files generally interact.

## `main.py`

The file `main.py` is loaded into the `python` main module.  After loading the needed Python modules this file loads the `gridlabd` module, Part 1 sets up the variables used to collect data from the simulation.

Part 2 uses the `command()` method to load the GridLAB-D model `model.glm` into the simulator, e.g., `command("model.glm")`. After the model is loaded using the `command()` method, the simulation can be started using the `start()` method.  The most common approach is to wait for the simulation to complete, e.g., using `start("wait")`.

In Part 3 the results are extracted and processed when the simulation is done.

[code:/gldcore/link/python/examples/example_2/main.py]

## `model.glm`

The model loads the python handlers module that implements that model's event handlers. In this example there are two kinds of event handlers.  The first is the global event handler `on_init()` that is implicitly called when the simulation initializes.  The second is the object event handler `commit()` that is called whenever the object performs a `commit` operation. 

Only objects that define an `on_commit` property will call event handlers. To call a python event handler, the syntax must be `python:<module-name>.<handler-name>`. If the colon is omitted, the call will be made to an external python executable, rather than the handlers module.

[code:/gldcore/link/python/examples/example_2/model.glm]

## `handlers.py`

The handlers module implements all the event handlers used by the model.  In this case, the `on_init(<timestamp>)` handler is called automatically after the objects are initialized when it is defined in the handlers module. During this event, any operations that needs to be completed prior to the start of the simulation can be performed.

The `commit(<object>,<timestamp>)` handler is called for each object the defines `commit` using its `on_commit` property.  During these events data that needs to be collected can gathered from specific object.  Similarly, a `precommit` event can be used to make changes to objects before the next simulation clock update.

Any handler that returns something that evaluates to `False` will halt the simulation.

[code:/gldcore/link/python/examples/example_2/handlers.py]

---

# Python Module

A hybrid module structure is used when it is given on the command file, such as

~~~
bash$ gridlabd main.py
~~~

The structure of this example is shown in Figure 4.

[image:Tutorial_figure_4.png]
Figure 4: Runtime architectures for Python and GridLAB-D

The following sections explain how these three files generally interact.

## `main.py`

The file `main.py` is loaded into the `python` main module.  After loading the needed Python modules this file loads the `gridlabd` module, Part 1 sets up the variables used to collect data from the simulation.

Part 2 uses the `command()` method to load the GridLAB-D model `model.glm` into the simulator, e.g., `command("model.glm")`. After the model is loaded using the `command()` method, the simulation can be started using the `start()` method.  The most common approach is to wait for the simulation to complete, e.g., using `start("wait")`.

In Part 3 the results are extracted and processed when the simulation is done.

[code:/gldcore/link/python/examples/example_3/main.py]

## `model.glm`

The model loads the python handlers module that implements that model's event handlers. In this example there are two kinds of event handlers.  The first is the global event handler `on_init()` that is implicitly called when the simulation initializes.  The second is the object event handler `commit()` that is called whenever the object performs a `commit` operation. 

Only objects that define an `on_commit` property will call event handlers. To call a python event handler, the syntax must be `python:<module-name>.<handler-name>`. If the colon is omitted, the call will be made to an external python executable, rather than the handlers module.

[code:/gldcore/link/python/examples/example_3/model.glm]


