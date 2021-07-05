[[/GLM/Global/Find]] -- Find objects

# Synopsis

GLM:

~~~
${FINDFILE criteria}
~~~

# Description

The objects that match the criteria are added to the list.

# Example

The following example prints the object of class `test`:

`/tmp/example.glm`
~~~
class test
{
    double x;
}
object test
{
    name my_test;
}
object test:..2
{
}
#print ${FIND class=test}
~~~

Running the example gives the following output:

~~~
$ gridlabd /tmp/example.glm
/tmp/test.glm(12): my_test test:1 test:2
~~~

# See also

* [[/GLM/Global/Shell]]
