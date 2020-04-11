[[/Global/Literal_if]] -- Do not interpret LHS of `#if` macro as a variable name

# Synopsis

GLM:

~~~
#set literal_if=[TRUE,FALSE]
~~~

Shell:

~~~
bash$ gridlabd -D literal_if=FALSE
bash$ gridlabd --define literal_if=FALSE
~~~

# Description

Do not interpret LHS of #if macro as a variable name. The default value if `TRUE` meaning that the LHS is interpreted literally rather than being expanded as a variable name.

# Example

~~~
#set literal_if=FALSE
~~~

For example, when `literal_if` global is set to `FALSE` the if statement should be defined as follows: 
~~~
#set literal_if=FALSE
#define TEST_VARIABLE='on'
#if TEST_VARIABLE=='on'
#print "Test code success" //THIS CODE WILL EXECUTE
#endif 
~~~
When `literal_if` global is set to `TRUE` the if statement should be defined as follows:
~~~
#set literal_if=TRUE
#define TEST_VARIABLE='on'
#if ${TEST_VARIABLE}=='on'
#print "Test code success" //THIS CODE WILL EXECUTE
#endif 
~~~
