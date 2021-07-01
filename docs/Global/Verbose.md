[[/Global/Verbose]] -- Verbose enable flag

# Synopsis

GLM:

~~~
#set verbose=FALSE
~~~

Shell:

~~~
bash$ gridlabd -D verbose=FALSE
bash$ gridlabd --define verbose=FALSE
~~~

# Description

Verbose enable flag. Set to `TRUE` to enable. To enable object level verbose 

~~~
#set verbose=TRUE

module <module_name> {
  message_flags VERBOSE;
}

object <object_name> {
  flags VERBOSE;
}
~~~

# Example

~~~
#set verbose=FALSE
~~~
