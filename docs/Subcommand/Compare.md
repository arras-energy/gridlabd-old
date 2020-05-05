[[/Subcommand/Compare]] -- compare models

# Synopsis

GLM:

~~~
#compare [options] [constraints] file1 file2 ...
~~~

Shell:

~~~
bash$ gridlabd compare [options] file1 file2 ...
~~~

# Description

The `compare` subcommand compare two or more models.  If the models differ,
the exit code is non-zero.

## Options

Option affect how the output of generated.

### `-g|--glm`

The `-g` or `--glm` option enables verbose output in GLM syntax.

### `help`

The `-h` or `--help` option display a helpful message.

### `quiet`

The `-q` or `--quiet` option suppresses all output, warning, and error messages

### `verbose`

The `-v` or `--verbose` options enable output of individual differences.

## Constraints

Constraints affect how the comparisons are made by limiting comparisons to
those  that are included in the constraint, e.g., "object=name" will only
compare the object "name", and "object=name1,name2" will only compare objects 
those two objects.  If the value is preceeded by a hyphen, the group is excluded
from the comparison.  Note that inclusion starts with all classes being
excluded and exclusion starts with all classes being included. Therefore the
two constraints cannot be mixed.  

### `object`

The `object` constraint controls which objects are included or excluded from 
the comparisons.
