[[/Converters/Import/Mdb_files]] -- MDB file import converters

# Synopsis

~~~
#input "filename.mdb" [-t <type> [options]]
~~~

# Description

There are many different types of CSV files that can be imported. Each is documented separately according to the type of data it delivers to the GLM loader.

By default the all tables contained in the MDB file are extracted into CSV files in the working folder.

## Import types

### `CYME`

CYME databases can be converted to GLM using the `-t cyme` option.  Additional options can be provided as a python dictionary, e.g., `{'name':'value',...}` and to define GLM global variables.

# Caveat

The `MDB` converters require the `mdbtools` be installed on the host system.

# See also

* [[/Converters/Input/Csv_files]]
