[[/Global/Output_message_context]] -- Control context from which debug messages are allowed

# Synopsis

GLM:

~~~
#set output_message_context=ALL
~~~

Shell:

~~~
bash$ gridlabd -D output_message_context=ALL
bash$ gridlabd --define output_message_context=ALL
~~~

# Description

Control context from which core debug messages are allowed.  Valid message contexts are

 - `AGGREGATE`: aggregation debug messages only
 - `ALL`: all debug messages allowed
 - `CLASS`: class debug messages only
 - `CMDARG`: command line subsystem debug messages only
 - `COMPARE`: compare debug messages only
 - `CONVERT`: converter debug messages only
 - `CREATE`: create debug messages only
 - `DELTAMODE`: deltamode debug messages only
 - `ENDUSE`: enduse debug messages only
 - `ENVIRONMENT`: environment debug messages only
 - `EXCEPTION`: exception handling debug messages only
 - `EXEC`: exec debug messages only
 - `FIND`: find subsystem debug messages only
 - `GLOBALS`: globals debug messages only
 - `GUI`: graphical user interface debug messages only
 - `HTTP`: HTTP server debug messages only
 - `HTTPCLIENT`: HTTP client debug messages only
 - `INDEX`: index debug messages only
 - `INSTANCE`: instance manager debug messages only
 - `INTERPOLATE`: interpolation debug messages only
 - `JOB`: job control debug messages only
 - `KML`: KML interface debug messages only
 - `LEGAL`: legal debug messages only
 - `LINK`: link debug messages only
 - `LIST`: list debug messages only
 - `LOAD`: loader debug messages only
 - `LOADSHAPE`: loadshape debug messages only
 - `LOCALE`: locale debug messages only
 - `LOCK`: memory locking debug messages only
 - `MAIN`: main module debug messages only
 - `MATCH`: match subsystem debug messages only
 - `MATLAB`: Matlab interface debug messages only
 - `MODULE`: module debug messages only
 - `NONE`: no debug messages allowed
 - `OBJECT`: object debug messages only
 - `PROPERTY`: property debug messages only
 - `PYTHON`: python module debug messages only
 - `RANDOM`: random variable debug messages only
 - `REALTIME`: realtime subsystem debug messages only
 - `SANITIZE`: model sanitizer subsystem debug messages only
 - `SAVE`: save debug messages only
 - `SCHEDULE`: schedule debug messages only
 - `SERVER`: server subsystem debug messages only
 - `SETUP`: setup debug messages only
 - `STREAM`: streaming data debug messages only
 - `TEST`: testing subsystem debug messages only
 - `THREADPOOL`: thread pool debug messages only
 - `TIME`: time subsystem debug messages only
 - `TRANSFORM`: transform subsystem debug messages only
 - `UNIT`: unit manager subsystem debug messages only
 - `VALIDATE`: validation subsystem debug messages only
 - `VERSION`: version debug messages only
 - `XCORE`: X11 core debug messages only
 - `XML`: XML interface debug messages only

# Example

~~~
#set output_message_context=ALL
~~~
