[[/Global/Pythonpath]] - Python module search path

# Synopsis

GLM:

~~~
#set pythonpath=<path>
~~~

Shell:

~~~
bash$ gridlabd -D|--define pythonpath=<path> ...
~~~

# Remarks

Path folders are separated by colon, i.e., `dir1:dir2:dir3`.

The `pythonpath` global is used to find python module when the `module` directive is used.

# See also

* [[/GLM/Directive/Module]]
