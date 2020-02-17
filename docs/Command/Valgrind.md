[[/Command/Valgrind]] -- Run gridlabd with valgrind

# Synopsis

~~~
bash$ export VALGRIND_OPTIONS=...
bash$ gridlabd --valgrind ...
~~~

# Description

This command line enable the valgrind tool if it is installed on the host system. To install the tool on CentOS docker

~~~
bash$ sudo yum install valgrind
~~~

# See also

* [[/Command/Gdb]]
* [[/Command/Lldb]]
* [Valgrind Documentation](https://valgrind.org/docs/)
