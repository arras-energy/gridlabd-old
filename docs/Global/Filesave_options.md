[[/Global/Filesave_options]] -- Global variable to control file save options

# Synopsis

GLM file:

~~~
#set filesave_options=[MODULES|PROPERTIES|CLASSES|GLOBALS|OBJECTS|SCHEDULES|FILTERS|SCRIPTS|CLOCK|ALLINITIAL|ALL]
~~~

Shell:

~~~
bash$ gridlabd -D filesave_options=[MODULES|PROPERTIES|CLASSES|GLOBALS|OBJECTS|SCHEDULES|FILTERS|SCRIPTS|CLOCK|ALLINITIAL|ALL]
~~~

# Description

The `filesave_options` global variable controls which portion of a GridLAB-D model are output when saving to a file.

~~~
#set filesave_options=ALLINITIAL
~~~
Ensures the JSON dump preserves the initial functions defined for the original model. This options should be enabled when running run/pause/resume type simulations using the JSON dump. Note, the default JSON dump preserves the last value of the property. 

# Example

~~~
#set filesave_options=GLOBALS|OBJECTS
~~~

# See also

* [[/Global/Glm_save_options]]

