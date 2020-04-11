[[/Glm/Macro/Sleep]] -- Macro to sleep the GLM load for 

# Synopsis

GLM:

~~~
#sleep <milliseconds>
~~~

# Description

The `#sleep` macro causes the GLM loader to pause for `<milliseconds>`.

# Examples

The following pauses the loader for 1 second.

~~~
#sleep 1000
~~~

# Caveats

If the sleep value is invalid, no error is returned and the loader does not pause. If a signal was received that was routed to a signal-catching function for the duration indicated, no error is returned, and the sleep process is not completed.

