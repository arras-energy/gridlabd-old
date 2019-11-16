[[Deltamode_allowed]] -- Global to control use of delta mode simulations

# Synopsis

GLM:
~~~
#set deltamode_allowed=[FALSE|TRUE]
~~~
Command line:
~~~
host% gridlabd -D deltamode_allowed=[FALSE|TRUE]
~~~

# Remarks

In version 4.2 delta mode is not allowed by default.  To enable delta mode in a model you must use
~~~
#set deltamode_allowed=TRUE
~~~
