[[/Global/Rusage_rate]] -- Specify the sample rate at which system resource usage data is collected

# Synopsis

GLM:

~~~
#set rusage_rate=<integer>
~~~

Shell:

~~~
bash$ gridlabd -D rusage_rate=1
bash$ gridlabd --define rusage_rate=1
~~~

# Description

Specifies the rate at which system resource usage data is collected. The default value is `0` which means no sampling is performed.  The sampling rate is specified in seconds of the simulation clock [[/Global/Clock]].

# See also

* [[/Command/Rusage]]
* [[/Global/Clock]]
* [[/Global/Rusage_data]]
* [[/Global/Rusage_file]]
