# Syntax
~~~~
#insert name(var1=value1, var2=value2, ..., varN=valueN)
~~~~
The `#insert` macro allows a GLM file to be included with parameters set as global variables during the include.  The macro is equivalent to
~~~~
#include using(var1=value1, var2=value2, ..., varN=valueN) "_name_.glm"
~~~~
