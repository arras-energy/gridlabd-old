[[/GLM/Global/Shell]] -- Global expansion of shell commands

# Synopsis

GLM:

~~~
${SHELL <expression>}
~~~

# Description

The shell command output is inserted into the GLM file at the location of the global expansion.

# Example

The following example prints the list `1 2 3`:

~~~
#print ${SHELL echo "1 2 3"}
~~~

# See also

* [[/GLM/Global/Python]]
