[[/GLM/Global/Range]] -- Generate a range of values

# Synopsis

GLM:

~~~
${RANGE<delimiter><start>,<stop>,<increment>}
~~~

# Description

The `RANGE` pseudoglobal generates a range of value from `<start>` to `<stop>` stepping by `<increment>` separated by `<delimiter>`. The only allowed delimiters are a single space, a single comma, a single semicolon, or a single colon.

# Example

The following example prints the numbers from 1 to 10 delimited by colons:

~~~
#print ${RANGE:1,10}
~~~

# See also

* [[/GLM/Global/Shell]]
