[[/GLM/Macro/Begin]] -- Macro to change language interpreter

# Synopsis
GLM
~~~
#begin <language>
<code>
#end
~~~

# Description

The #begin macro is used activate a new language interpreter. The new interpreter is active until the #end macro is encountered. The languages currently supported are
* python

# Example

The following example outputs the current version information.
~~~
#begin python
gridlabd.output(gridlabd.version())
#end
~~~

# See also
* [[/GLM/Global/Python]]
