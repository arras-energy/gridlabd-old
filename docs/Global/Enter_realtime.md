[[/Global/Enter_realtime]] -- Real-time entry time global variable

# Synopsis

GLM:

~~~
#set enter_realtime=NEVER
~~~

Shell:

~~~
bash$ gridlabd -D enter_realtime=NEVER
bash$ gridlabd --define enter_realtime=NEVER
~~~

# Description

The `enter_realtime` global variable allows the modeler to specify the clock time at which to switch the simulation to real-time mode.

# Example

~~~
#set enter_realtime=NEVER
~~~
