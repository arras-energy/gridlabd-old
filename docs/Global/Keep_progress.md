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
reports.  If `TRUE`, then `CRLF` is output.

# Example

~~~
#set keep_progress=TRUE
~~~
