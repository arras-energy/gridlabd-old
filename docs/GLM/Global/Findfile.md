[[/GLM/Global/Findfile]] -- Find a file in GLPATH

# Synopsis

GLM:

~~~
${FINDFILE filename}
~~~

# Description

The full pathname is inserted into the GLM file at the location of the global expansion.

# Example

The following example prints the full path name of a downloaded weather file:

~~~
#weather get Seattle
#setenv GLPATH=${GLPATH}:${GLPATH}/weather/US
#print ${FINDFILE WA-Seattle_Seattletacoma_Intl_A.tmy3}
~~~

# See also

* [[/GLM/Global/Shell]]
