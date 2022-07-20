[[/python]] -- Python module

# Synopsis

Python 3.6 or greater:
~~~
  >>> import gridlabd
  >>> gridlabd.title()
  >>> gridlabd.version()
  >>> gridlabd.copyright()
  >>> gridlabd.credits()
  >>> gridlabd.license()
~~~
Output streams:
~~~
  >>> gridlabd.output(message)
  >>> gridlabd.debug(message)
  >>> gridlabd.warning(message)
  >>> gridlabd.error(message)
~~~
Simulation control:
~~~
  >>> gridlabd.command(argument)
  >>> gridlabd.start(mode)
  >>> gridlabd.wait()
  >>> gridlabd.cancel()
  >>> gridlabd.pause()
  >>> gridlabd.pauseat(datetime)
  >>> gridlabd.resume()
~~~
Model control:
~~~
  >>> gridlabd.module(name)
  >>> gridlabd.add(block,data)
  >>> gridlabd.load(filename)
  >>> gridlabd.save(filename)
~~~
Gets:
~~~
  >>> gridlabd.get(item)
  >>> gridlabd.get_class(name)
  >>> gridlabd.get_object(name)
  >>> gridlabd.get_global(name)
  >>> gridlabd.get_value(name,property)
  >>> gridlabd.get_schedule(name)
~~~
Sets:
~~~
  >>> gridlabd.set_global(name,value)
  >>> gridlabd.set_value(name,property,value)
~~~
Units:
~~~
  >>> gridlabd.convert_unit(string,to)
  >>> gridlabd.convert_unit(real,from,to)
~~~
Converters:
~~~
  >>> gridlabd.bool(string)
  >>> gridlabd.int16(string)
  >>> gridlabd.int32(string)
  >>> gridlabd.int64(string)
  >>> gridlabd.timestamp(string)
  >>> gridlabd.double(string)
  >>> gridlabd.complex(string)
~~~

# Description

The [[/python]] module `gridlabd` loads an instance of GridLAB-D into the current instance of python.  Only one instance of GridLAB-D may be loaded per instance of python.

A `.glm` file may load a Python module to implement event handlers.  The syntax for loading a Python module is the same as that for loading a GridLAB-D module:
~~~
  module module_name;
~~~

## Module variables 

If you want to set module variables that will be accessible through the module object in python, you can use the syntax
~~~
  module module_name {
    variable_name "initial value";
  }
~~~

In this case you can read or write the string using the python syntax
~~~
  >>> gridlabd.variable_name
~~~

## Module Event Handlers

The following module event handlers may be defined in the module.
~~~
  def on_init(t):
      # add your code here
      return True # return False to fail initialization

  def on_precommit(t):
      # add your code here
      return True # return False to fail initialization

  def on_commit(t):
      # add your code here
      return gridlabd.NEVER # return an integer value of t or later to cause iteration
                            # return an integer value less than t to stop simulation

  def on_term(t):
      # add your code here
      return None
~~~

The module event handlers are called automatically used when they are defined.  There is no need to specify them in the GLM file. Note that there are no module event handlers for sync events.

In addition, objects may call individual event handlers by defining the handler as follows:
~~~
  def event_handler(obj,t):
      # add your code here
      return gridlabd.NEVER # return an integer value of t or later to cause iteration
                            # return an integer value less than t to stop simulation

~~~
The object event handler are specified in a GLM file as follows:
~~~
  object my_class {
    on_init "python:module_name.init_handler";
    on_precommit "python:module_name.precommit_handler";
    on_presync "python:module_name.presync_handler";
    on_sync "python:module_name.sync_handler";
    on_postsync "python:module_name.postsync_handler";
    on_commit "python:module_name.commit_handler";
    on_term "python:module_name.term_handler";
  }
~~~

# Methods

The module supports the following methods to setup, access and control the simulation. For example list of available methods and variables, see the output of the module `help` command.

## `add(block,data)`

The `add` method constructs a GLM directive of type `block` using `data`. The `block` may be one of `clock`, `global`, `module`, `class`, and `object`.  Data is provided as a python `dict` object, with `parameter:value` pairs for each entry in the block.  Generally, the following syntax

~~~
>>> add('block',{'var1':'value1', 'var2':'value2', ...})
~~~

results in the following GLM code

~~~
block {
  var1 value1;
  var2 value2;
  ...
}
~~~

Some block use one of the data items specially, e.g.,

~~~
>>> add('object',{'class':'my_class', 'name':'my_name', ...})
~~~

result in the code

~~~
object my_class {
  name my_name;
  ...
}
~~~

The return value from the `add` method is the name of the temporary GLM file constructed.  When you are done constructing the GLM file, you can use the `command(glm_name)` method to load the model.

Note: the `add` method is not useful after the simulation is started because you cannot load additional models using `command` after initialization is completed.

## `command(argument)`

The `command` method sends a command line argument to the gridlabd instance.  Command line arguments are processed immediately in the order they are received.

Note: that you cannot use `command` after initialization is completed.

## `load(file)`

The `load` method processes a file as if it were passed using the `command(file)` method.  Input files may be `.glm` or `.py` files.  All other file formats are supported provided there is a converter from that format to `.glm` installed in the `/usr/local/share/gridlabd` folder.

## `start(mode)`

The `start` method completes processing of the command line arguments, initializes the simulation, and starts running the clock. Valid modes are 

* `thread`: The simulation starts immediately as a separate thread. The call returns immediately.

* `pause`: The simulation is initialized as a separate thread but does not start. The call returns immediately.

* `wait`: The simulation starts in the same thread. The call does not return until the simulation is done.

## `wait()`

The `wait` method waits for the running simulation to stop.

## `cancel()`

The `cancel` method cancels the current simulation.

## `pause()`

The `pause` method immediately pauses the simulation.

## `pauseat(datetime)`

The `pauseat` method pauses the simulation at the specified `datetime`. If the simulation is already paused at an earlier, it resumes automatically until the specified `datetime` is reached.

## `resume()`

The simulation is resumed with no specified stop time.

## `get(item)`

Gets a list of items, where item is one of `globals`, `modules`, `classes`, or `objects`.

Note: the `get` command only works after the initialization has started.

## `get_class(name)`

Gets a description of the class named.

Note: the `get_class` command only works after the initialization has started.

## `get_object(name)`

Gets a description of the object named.

Note: the `get_object` command only works after the initialization has started.

## `get_global(name)`

Get the global variable `name`. The value will be a string formatted by GridLAB-D.

Note: the `get_global` command only works after the initialization has started.

## `set_global(name,value)`

Set the global variable `name` to `value`. The value must be a string that can be interpreted by GridLAB-D.

Note: the `set_global` command only works after the initialization has started.

## `get_value(name,property)`

Get the value of the property of an object. The value will be a string formatted by GridLAB-D.

Note: the `get_value` command only works after the initialization has started.

## `set_value(name,property,value)`

Set the value of the property of an object. The value must be a string that can be interpreted by GridLAB-D.

Note: the `set_value` command only works after the initialization has started.

## `save(file)` 

Saves the full model to the file.  The currently supported formats are `.glm`, `.xml`, and `.json`.  For GLM files, the save content is controlled by the [[/glm_save_options]] global variable. Other formats are supported if there is a converter from JSON to that format installed in the `/usr/local/share/gridlabd` folder.

# Example

The following model is `test.glm`:
~~~
  module test;
  clock {
  	starttime '2018-01-01 00:00:00';
  	stoptime '2018-01-01 01:00:00';
  }
  class test {
  	randomvar x[MW];
  }
  object test {
  	name "my_test";
  	x "type:lognormal(0,1); refresh:5min";
  	on_commit python:test.commit;
  }
~~~

The following is the file `test.py`:
~~~
  def on_init(t) :
  	gridlabd.output("timestamp,x")
  	return True
  
  def on_sync(t) :
  	gridlabd.output("%s,%s"%(gridlabd.get_global("clock"),gridlabd.get_object("my_test")["x"]))
  	return gridlabd.NEVER
  
  def commit(obj,t) :
  	gridlabd.debug("%s.commit(obj='%s',t=%d) ok" % (gridlabd.__name__,obj,t))
  	return gridlabd.NEVER
~~~
The following example runs the model in Python:
~~~
bash$ python3
  Python 3.7.0 (default, Aug 22 2018, 15:22:29) 
  [Clang 8.0.0 (clang-800.0.42.1)] on darwin
  Type "help", "copyright", "credits" or "license" for more information.
  >>> import gridlabd
  >>> gridlabd.command('test')
  1
  >>> gridlabd.start('wait')
  timestamp,x
  2018-01-01 00:00:00 UTC,0.825548
  2018-01-01 00:05:00 UTC,1.472663
  2018-01-01 00:10:00 UTC,0.654999
  2018-01-01 00:15:00 UTC,0.671511
  2018-01-01 00:20:00 UTC,0.576646
  2018-01-01 00:25:00 UTC,6.734517
  2018-01-01 00:30:00 UTC,1.034309
  2018-01-01 00:35:00 UTC,2.144641
  2018-01-01 00:40:00 UTC,8.558224
  2018-01-01 00:45:00 UTC,1.453279
  2018-01-01 00:50:00 UTC,0.251099
  2018-01-01 00:55:00 UTC,2.778129
  2018-01-01 01:00:00 UTC,0.603100
  4362511024
  >>> quit()
~~~
The following example records all house temperature when the model commit if performed:

The model is `test_house.glm`:
~~~
  module my_test;
  clock {
  	timezone "US/CA/San Francisco";
  	starttime "2019-01-01 00:00:00";
  	stoptime "2019-02-01 00:00:00";
  }
  module powerflow;
  module residential;
  object triplex_meter:..10 {
  	nominal_voltage 120;
  	phases AS;
  	object house:..4 {
  		heating_setpoint 68;
  		cooling_setpoint 76;
  		on_commit "python:my_test.record_house";
  	};
  }
~~~
The module is `my_test.py`:
~~~
  import os
  
  def find(criteria) :
  	finder = criteria.split("=")
  	if len(finder) < 2 :
  		raise Exception("find(criteria='key=value'): criteria syntax error")
  	objects = gridlabd.get("objects")
  	result = []
  	for name in objects :
 		item = gridlabd.get_object(name)
  		if finder[0] in item and item[finder[0]] == finder[1] :
  			if "name" in item.keys() :
  				result.append(item["name"])
   			else :
  				result.append("%s:%s" % (item["class"],item["id"]))
  	return result
  
  def on_init(t):
  	houses = find("class=house")
  	global recorder
  	recorder = open("house.csv","w")
  	recorder.write("name,datetime,temperature\n")
  	return True
  
  def record_house(name,t) :
  	global recorder
  	if recorder :
  		house = gridlabd.get_object(name)
  		recorder.write("%s,%s,%s\n" % (house["name"],house["clock"],house["air_temperature"]))
  	return True
  
  def on_term(t):
  	recorder.close()
~~~
The run script is `test_house.py`:
~~~
  import gridlabd
  gridlabd.command("test_house.glm")
  gridlabd.start("wait")
~~~
The first few lines of output are in `house.csv`:
~~~
  name,datetime,temperature
  house:49,2019-01-01 00:00:00 PST,+71.3573
  house:48,2019-01-01 00:00:00 PST,+72.1508
  house:47,2019-01-01 00:00:00 PST,+67.3659
  house:46,2019-01-01 00:00:00 PST,+67.1166
  house:44,2019-01-01 00:00:00 PST,+76.6451
  house:43,2019-01-01 00:00:00 PST,+75.8254
  house:42,2019-01-01 00:00:00 PST,+75.6871
  house:41,2019-01-01 00:00:00 PST,+71.4308
  house:39,2019-01-01 00:00:00 PST,+73.763
~~~

# See also

* [[/Python subcommand]]
