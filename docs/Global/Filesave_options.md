[[filesave_options]] -- Global variable to control file save options

# Synopsis

Command line:
~~~
host% gridlabd -D filesave_options=[MODULES|PROPERTIES|CLASSES|GLOBALS|OBJECTS|SCHEDULES|FILTERS|SCRIPTS|CLOCK|ALL]
~~~
GLM file:
~~~
#set filesave_options=[MODULES|PROPERTIES|CLASSES|GLOBALS|OBJECTS|SCHEDULES|FILTERS|SCRIPTS|CLOCK|ALL]
~~~

# Description

The `filesave_options` global variable controls which portion of a GridLAB-D model are output when saving to a file.

# See also
* [[glm_save_options]]

