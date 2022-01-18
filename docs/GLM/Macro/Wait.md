[[/Glm/Macro/Wait]] -- Macro to wait for asynchronous tasks to complete

# Synopsis

GLM:

~~~
#wait
~~~

# Description

The `#wait` macro wait on a task started by the `#start` macro in the background. The loader will continue when all the started tasks have exited. If any task exits with an error, the wait command fails and the GLM load will fail.

# Examples

The following command starts the GridLAB-D daemon process and waits for it to terminate.

~~~
#start gridlabd --daemon
#wait
~~~

# See also

* [[/GLM/Macro/Start]]
