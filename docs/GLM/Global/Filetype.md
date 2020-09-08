[[/GLM/Global/Filetype]] -- Get the file type portion of a filename variable

# Synopsis

GLM:

~~~
${FILETYPE variable}
~~~

# Description

The file type (i.e., the extension) of a pathname variable is inserted into the GLM file.

# Example

The following example prints the extension of the current model file:

~~~
#print ${FILETYPE modelname}
~~~

# See also

* [[/GLM/Global/Filename]]
* [[/GLM/Global/Filepath]]
* [[/GLM/Global/Findfile]]
