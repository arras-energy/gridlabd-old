[[/GLM/General/Collection]] -- Object collections

# Synopsis
GLM:
~~~
<property>=<value> [{and|AND|;} ... ] 
~~~

# Description

Several objects use collections as a property to help find objects that satisfy certain criteria.  Collections are typically built at initialization, although that can be run at any during a simulation if needed. 

A collection is specified as string with one or more filtering elements.  For example,
~~~
  class=house
~~~
would collect all the objects that are of class `house`.

Only properties that are invariant during a simulation may be used in a collection. The following properties are supported:

* `id`
* `class`
* `isa`
* `module`
* `groupid`
* `rank`
* `parent`
* `insvc`
* `name`
* `latitude`
* `longitude`
* `clock`
* `insvc`
* `outsvc`
* `flags`

The following operators are supported:

* AND (can also be written as "and" or ";")

Note that the "OR" operator is not supported at this time.

# See also

* [[/Module/Tape/Collector]]
* [[/Module/Mysql/Collector]]
* [[/Server/Find]]
