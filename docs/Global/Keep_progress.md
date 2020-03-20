[[/Global/Keep_progress]] -- Keep progress enable flag

# Synopsis

GLM:

~~~
#set keep_progress=TRUE
~~~

Shell:

~~~
bash$ gridlabd -D keep_progress=TRUE
bash$ gridlabd --define keep_progress=TRUE
~~~

# Description

Keep progress enable flag. By default this is `FALSE`, which indicates that only `CR` is output between progress 
reports.  If `TRUE`, then `CRLF` is output. Note, disabling the `supress_repeat_messages` flag allows the display of every line of progress.

# Example

~~~
#set supress_repeat_messages=FALSE
#set keep_progress=TRUE
~~~

# See also

* [[/Global/Suppress_repeat_messages]]


