[[Events]] -- Object event handlers

Objects can be assigned external event handlers using the `on_*` load method.  The following events may be handled: `init`, `precommit`, `presync`, `sync`, `postsync`, `commit`, `finalize`.  The syntax for implementing event handlers is
~~~
  object class {
    on_init "command";
    on_precommit "command";
    on_presync "command";
    on_sync "command";
    on_postsync "command";
    on_commit "command";
    on_finalize "command";
  }
~~~
where "command" is the event command. 

## Python support

If the command is to be processed in the Python environment, use the "python:" prefix, e.g.,
~~~
  object name {
    on_sync "python:module.method";
  }
~~~
In this case, the function `method` in the python3 `module` must be defined as follows:
~~~
def method(name,t1):
  // process object name at timestamp t1
  // return t2 for time of upcoming event
  // return gridlabd.NEVER for no upcoming event
  // return gridlabd.INVALID for error
~~~

## Shell environment

The following global variables are copied to the shell environment when the script is called

* ''clock'': <tt>CLOCK</tt> is the global clock
* ''hostname'': <tt>HOSTNAME</tt> is the server hostname
* ''server_portnum'': <tt>PORT</tt> is the server port number
* ''object_name'': <tt>OBJECT</tt> is the object name

# Example
~~~
  module residential;
  #set suppress_repeat_messages=0
  #option server
  module tape;
  clock {
  	starttime '2000-01-01 00:00:00';
  	stoptime '2000-01-01 01:00:00';
  }
  object house {
  	name house_1;
  	on_init "echo init clock=$CLOCK hostname=$HOSTNAME port=$PORT object=$OBJECT";
  	on_precommit "echo precommit clock=$CLOCK hostname=$HOSTNAME port=$PORT object=$OBJECT";
  	on_presync "echo presync clock=$CLOCK hostname=$HOSTNAME port=$PORT object=$OBJECT";
  	on_sync "echo sync clock=$CLOCK hostname=$HOSTNAME port=$PORT object=$OBJECT";
  	on_postsync "echo postsync clock=$CLOCK hostname=$HOSTNAME port=$PORT object=$OBJECT";
  	on_commit "echo commit clock=$CLOCK hostname=$HOSTNAME port=$PORT object=$OBJECT";
  	on_finalize "echo finalize clock=$CLOCK hostname=$HOSTNAME port=$PORT object=$OBJECT";
  	object recorder {
  		property air_temperature;
  		interval 300;
  		file air_temperature.csv;
  	};
  }
~~~