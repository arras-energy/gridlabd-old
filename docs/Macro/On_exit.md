[[Macro/On_exit]] - on_exit macro

# Synopsis
~~~
  #on_exit exitcode command
~~~

# Description

When this macro is encountered, an exit handler is added when the simulation exits with `exitcode`.  Exit handlers are run only when the simulation has completed all post simulation processing and is about to exit.  The exit process is suspended until the result of the exit handler(s) are obtained.

When the value `-1` is used for the `exitcode`, any non-zero exit condition will trigger the exit handler.

## Example

The following example run a series of models with VARIABLE incremented for each run
~~~
  // set the initial value to 0
  #ifndef VARIABLE
  #define VARIABLE=0
  #endif
  // print the value
  #system echo ${VARIABLE}
  // run the next value, stopping at 9
  #if VARIABLE < 9
  #on_exit 0 ${exename} -D VARIABLE=$((${VARIABLE}+1)) ${modelname} &
  #endif
~~~
which produces the output
~~~
  0
  1
  2
  3
  4
  5
  6
  7
  8
  9
~~~
