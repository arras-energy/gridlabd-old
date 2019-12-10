[[/Glm/Macro/If]] -- Conditional test macro

# Synopsis
GLM:
~~~
#if <value1> <comparison> <value2>
...
[#else]
...
#endif
~~~

# Description

The `#if` macro introduces a conditional section of a GLM file.  The `#else` and `#endif` macros are used to introduce the alternative section and terminate the conditional section.

# Examples

~~~
#ifdef ${VAR} == yes
#print VAR == "yes"
#else
#print VAR != "yes"
#endif
~~~

# Caveats

Prior to version 4.2.1, the `value1` is interpreted as a variable name by default.  This behavior is controlled by `literal_if`.  As of version 4.2.1, the value of `literal_if` is `true`. To employ the behavior prior to version 4.2.1, set `literal_if` to `false`. If the variable is undefined, you may wish to use the `relax_undefined_if` global variable to relax the constraint that the variable must be defined to complete the test.

# See also
* [[/Global/Literal_if]]
* [[/Global/Relax_undefined_if]]
