[[/Global/Maximum_runtime]] -- Maximum runtime for a simulation

# Synopsis

GLM:

~~~
#set maximum_runtime=60
~~~

Shell:

~~~
bash$ gridlabd -D maximum_runtime=60
bash$ gridlabd --define maximum_runtime=60
~~~

# Description

Maximum run time for a simulation in seconds.  Simulations that run longer than the alloted time will exit with an error code 11 (TMERR).

# Example

~~~
#set maximum_synctime=60
~~~
