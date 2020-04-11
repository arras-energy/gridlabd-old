[[/Global/Ignore_errors]] -- Global variable that controls whether errors cause immediate exit

# Synopsis

Shell:

~~~
bash$ gridlabd -D ignore_errors=[FALSE|TRUE] ...
bash$ gridlabd --define ignore_errors=[FALSE|TRUE] ...
~~~

GLM:

~~~
  #set ignore_errors=[FALSE|TRUE]  
~~~

# Description

As of 4.2, when an error occurs the simulation automatically exits the main loop, i.e., the default value of `ignore_errors` is `FALSE`.  However, in some instances errors are not a reason to exit, either because they aren't really errors or because circumstances demand the simulation continue in spite of the error, e.g., a real-time simulation is running.  In such cases, the global variable `ignore_errors` may be set to `TRUE` to prevent the simulation from exiting when an error occurs.
