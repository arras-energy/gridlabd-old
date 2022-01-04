[[/Global/Maximum_runtime]] -- Maximum runtime for a simulation

# Synopsis

GLM:

~~~
#set maximum_runtime=0
~~~

Shell:

~~~
bash$ gridlabd -D maximum_runtime=0
bash$ gridlabd --define maximum_runtime=0
~~~

# Description

Maximum run time for a simulation in seconds.  Simulations that run longer than the alloted time will exit with an error code 11 (TMERR). A value of `0` indicates that there is no limit to the run time.

# Example

~~~
#set maximum_runtime=60
~~~
