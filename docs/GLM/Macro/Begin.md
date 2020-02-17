[[/GLM/Macro/Begin]] -- Macro to change language interpreter

# Synopsis

GLM

~~~
#begin <language>
<code>
#end
~~~

# Description

The #begin macro is used activate a new language interpreter. The new interpreter is active until the #end macro is encountered. 

## Built-in Languages

The following languages are currently provide through built-in support.

### `python`

The Python3 interpreter is linked to the GLM loader automatically when GridLAB-D is built.  The `gridlabd` module is also automatically loaded when the GridLAB-D Python interface is started.  For more information on the `gridlabd` module, see [[/Module/Python]]

# Example

The following example outputs the current version information.

~~~
#begin python
gridlabd.output(gridlabd.version())
#end
~~~

# See also

* [[/GLM/Global/Python]]
* [[/Module/Python]]
