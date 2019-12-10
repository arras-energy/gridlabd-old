[[/GLM/General/Collection]] -- Object collections

# Synopsis
GLM:
~~~
<property><comparison><value> [{and|AND|;} ... ] 
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

## Search criteria

Object searches are usually expressed using a search criteria, such as
~~~
object class { 
  group "<property> <comparison> <value>";  
  // ..  
}
~~~
where `class` is the class of object that uses the `group` property (e.g., `collector`, `histogram`), `property` is the object property name to match against (e.g., `name`, `class`, `parent`), `comparison` is the comparison operator (e.g., ==, \<, !\~), and `value` the value to match against.

## Operators

  - `!=` : Not equal, e.g., `property!=value`  
  - `<=` : Less than or equal, e.g., `property<=value`
  - `>=` : Greater than or equal, e.g., `property>=value`
  - `!~` : Not like, e.g., `property`!`pattern`
  - `=` : Equal

# Caveats

The `OR` operator is not supported at this time.

Multiple search criteria can be indicated using `and`/`or` as appropriate. Parenthetical operators are not supported.

The implementation of the `and` and `or` operators is incomplete and not mathematically correct. Any logical statement joined with an `and` will remove all objects not identified with that operation from the working set. Any logical statement joined with an `or` will add all objects that match that operation to the working set. There is no sense of operator precedence, and operators are processed from left to right.

Date and time values must be fully qualified absolute date/time stamps using the appropriate timezone. Relative time can also be given using `s`, `m`, `h`, `d`, or `w` suffixes as desired, e.g., `1800s` to indicate 30 minutes.

# See also

* [[/Module/Tape/Collector]]
* [[/Module/Mysql/Collector]]
* [[/Server/Find]]
