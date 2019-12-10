[[/Global/Filesave_options]] -- Global variable to control file save options

# Synopsis
GLM file:
~~~
#set filesave_options=[MODULES|PROPERTIES|CLASSES|GLOBALS|OBJECTS|SCHEDULES|FILTERS|SCRIPTS|CLOCK|ALL]
~~~
Shell:
~~~
host% gridlabd -D filesave_options=[MODULES|PROPERTIES|CLASSES|GLOBALS|OBJECTS|SCHEDULES|FILTERS|SCRIPTS|CLOCK|ALL]
~~~

# Description

The `filesave_options` global variable controls which portion of a GridLAB-D model are output when saving to a file.

# Example

~~~
#set filesave_options=GLOBALS|OBJECTS
~~~

# See also
* [[/Global/Glm_save_options]]

