[[/Global/Object_scan]] -- Format for reading anonymous object names

# Synopsis

GLM:

~~~
#set object_scan=%[^:]:%d
~~~

Shell:

~~~
bash$ gridlabd -D object_scan=%[^:]:%d
bash$ gridlabd --define object_scan=%[^:]:%d
~~~

# Description

Format for reading anonymous object names

# Example

~~~
#set object_scan=%[^:]:%d
~~~
