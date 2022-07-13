[[/Global/Deltamode_allowed]] -- Global to control use of delta mode simulations

# Synopsis

GLM:

~~~
#set deltamode_allowed=[FALSE|TRUE]
~~~

Command line:

~~~
bash$ gridlabd -D deltamode_allowed=[FALSE|TRUE]
~~~

# Description

In version 4.3 delta mode is not allowed by default.  

# Example

The following will enable delta mode in a model 

~~~
#set deltamode_allowed=TRUE
~~~
