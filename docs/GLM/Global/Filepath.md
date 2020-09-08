[[/GLM/Global/Filepath]] -- Get the path portion of a filename variable

# Synopsis

GLM:

~~~
${FILEPATH variable}
~~~

# Description

The path portion of a pathname variable is inserted into the GLM file.

# Example

The following example prints the path to the current model file:

~~~
#print ${FILEPATH modelname}
~~~

# See also

* [[/GLM/Global/Filename]]
* [[/GLM/Global/Filetype]]
* [[/GLM/Global/Findfile]]
