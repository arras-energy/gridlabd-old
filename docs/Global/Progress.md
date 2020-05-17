[[/Global/Progress]] -- Progress tracking value

# Synopsis

~~~
bash$ gridlabd --server <modelname>.glm
bash$ curl http://localhost:6267/global/progress
~~~

# Description

This global obtains the simulation progress while it is running. The value is returned as a fractional quantity, i.e., in the range 0 to 1.
