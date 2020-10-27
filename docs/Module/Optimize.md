[[/Module/Optimize.md]] -- Module optimize

# Synopsis

GLM:

~~~
module optimize;
~~~

# Description

The `optimize` module provides general-purpose optimization strategies to iteratively find an extremum for an abritary object property based on constrained changes to one or more other object properties.

## `simple`

The [[/Module/Optimize/Simple]] optimizer using a simple linear search starting from the current state to achieve the desired objective.  This simple optimizer is one-dimensional and it can be extremely slow but it is guaranteed to find a local solution if one exists.

# See also

* [[/Module/Optimize/Simple]]
