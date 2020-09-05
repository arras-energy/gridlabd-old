[[/GLM/Global/Filename]] -- Get the name portion of a filename variable

# Synopsis

GLM:

~~~
${FINDFILE variable}
~~~

# Description

The name portion of a pathname variable is inserted into the GLM file.

# Example

The following example prints the name of the current model file:

~~~
#print ${FILENAME modelname}
~~~

# See also

* [[/GLM/Global/Filepath]]
* [[/GLM/Global/Filetype]]
* [[/GLM/Global/Findfile]]
