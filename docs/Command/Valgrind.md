[[valgrind]] -- Run gridlabd with valgrind

# Synopsis

~~~
host% export VALGRIND_OPTIONS=...
host% gridlabd --valgrind ...
~~~

# Description

This command line enable the valgrind tool if it is installed on the host system. To install the tool on CentOS docker

~~~
host% sudo yum install valgrind
~~~

# See also
* --[[gdb]] command line options
* --[[lldb]] command line option
