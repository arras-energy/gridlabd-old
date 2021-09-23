[[/Command/Profile]] -- Control performance profiling of core and modules while simulation runs

# Synopsis

~~~
bash$ gridlabd --profile[=FORMAT]                                           
~~~

# Description

Control performance profiling of core and modules while simulation runs.  Support formats are `TEXT`, `CSV`, and `JSON`.  The default format is `TEXT`.  Use the [[/Command/Redirect]] option to write the output to a file.

When the profiler is enabled, the object profile is also output to `object_profile.txt`.

# See also

* [[/Command/Redirect]]
