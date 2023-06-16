[[/Converters/Import/Mdb_files]] -- MDB file import converters

# Synopsis

Shell:
~~~
bash$ gridlabd [-D mdb_load_options='-t TYPE [OPTIONS ...]] FILENAME.mdb ...
~~~

GLM:

~~~
#input "filename.mdb" [-t <type> [options]]
~~~

# Description

There are many different types of MDB files that can be imported. Each is documented separately according to the type of data it delivers to the GLM loader.

By default only required tables defined in thc converter will be extracted into CSV files in the table storage folder.

## Import types

### `-t cyme [OPTIONS ...]`

CYME databases can be converted to GLM model using the `-t cyme` option. Additional options can be provided as an equation using option `-p`, e.g., `-p name=value ...` and to define GLM global variables.

Options:

- `config=CSV`: specify the name of CSV for converter configuration

- `cyme-tables`: print out the required CYME tables

- `data-dir=DIR`: specify the directory for storing CSV tables (default /tmp/gridlabd/mdb-cyme2glm/INPUT_FILE_NAME)

- `equipment_mdb=MDB`: specify the MDB for equipment data (default None)

- `network-ID=ID`: specify the network ID list to be converted (default [])

- `coordinateCSV=CSV`: specify the name of CSV for saving the node geographic location data (default None)


### `-t table [OPTIONS ...]`

CYME databases can able be converted to GLM object using the `-t table -f object` option or converted to GLM player object using the `-t table -f player` option. Additional options can be provided as an equation using option `-p`, e.g., `-p name=value ...` and to define GLM global variables.

Options:

- `table=TABLE`: specify the table to read

- `class=CLASS`: specify the class name to use if not found in columns (default None)

- `modules=MODULE`: specify module to load if not found in columns (default [])

- `columns=COLUMN:PROPERTY[,...]`: specify column mappings (default {})

- `dtypes=PROPERTY:TYPE[,...]`: specify property types (default {})

- `chunksize=INTEGER`: specify the chunksize for processing data (default 1000)

## Config.csv Options

Settings in the `config.csv` file that affect the converter include:

~~~
<setting1>,<value1>
<setting2>,<value2>
...
<settingN>,<valueN>
~~~

### `GLM_NETWORK_PREFIX`

Each network in the CYME database will be output in a separate GLM file using the name of the network with the network prefix.

### `GLM_NETWORK_MATCHES`

The network pattern matching uses POSIX regular expressions to match network names starting with the first character of the network name.  Here are some useful examples:

  - `abc`: match all network names that start with the string "abc"
  - `abc$`: match the network name "abc" only
  - `.*`: match all network names
  - `.*abc`: match all networks containing the string "abc"
  - `.*abc$`: match all networks ending with the string "abc"
  - `[0-9]`: match all networks starting with the digits 0 through 9
  - `.*[0-9]`: match all networks containing the digits 0 through 9
  - `.*[0-9]$`: match all networks ending with the digits 0 through 9

For details on POSIX pattern matching see [POSIX Regular Expression Documentation](https://en.wikibooks.org/wiki/Regular_Expressions/POSIX_Basic_Regular_Expressions).

### `GLM_NOMINAL_VOLTAGE`

The nominal voltage must be specified either in the `config.csv` or in the included GLM file.

### `GLM_INCLUDE` 

A single `#include` macro may be added after the `#define` specified by `GLM_DEFINE`.  This allows the define statement to control the behavior of the include file.

### `GLM_DEFINE`

A single `#define` may be specified to alter the behavior of the include file, object definitions, and modify statements.

### `GLM_MODIFY`

A single CSV file may be processed after the GLM objects are created to enable modification of object properties, if desired.  The format of the modification file is as follows:

~~~
<object1>,<property1>,<value1>
<object2>,<property2>,<value2>
...
<objectN>,<propertyN>,<valueN>
~~~

### `GLM_ASSUMPTIONS`

During the conversion process, some assumption may be made to generate a working GridLAB-D model.  These assumptions will be disposed of according to setting of `GLM_ASSUMPTIONS`:

  - `ignore`: ignore the assumptions generated
  - `include`: add the assumptions to the GLM file so they can be modified manually
  - `save`: save the assumption to GLM file for later processig
  - `warn`: save the assumptions to a CSV file and generate a warning message

### `GLM_VOLTAGE_FIX`

Once conversion of the mdb file is finished the converter can attempt to fix the nominal voltages across the network such that connections have the same voltage at each connection. `True` or `False`

### `GLM_PHASE_FIX`

Once conversion of the mdb file is finished the converter can attempt to fix the phases across the network such that connections have the phase at each connection. `True` or `False`

### `GLM_INPUT_LINE_LENGTH_UNIT`

Set the line length unit that is used within the input MDB file. Options are `m` or `km`. Default `m`.

### `GLM_OUTPUT`

Sets the path for any outputs. Defaults to `stdout`



### `ERROR_OUTPUT`

Sets the path for any errors. Defaults to `stderr`

### `WARNING_OUTPUT`

Sets the path for any warnings. Defaults to `stderr`

### `WARNING`

Option to display any warnings during the conversion. Defaults to `True`

### `DEBUG`

Option to display any debug outputs during the conversion. Defaults to `False`

### `QUIET`

Option to run converter without any error messages. Defaults to `False`

### `VERBOSE`

Option to run converter with verbose output messages. Defaults to `False`




# Example

Shell:
~~~
bash$ gridlabd -D mdb_load_options='-t cyme -o ./OUTPUT_FILE.glm -p config=CSV -p data-dir=DIR -p equipment_mdb=MDB -p network-ID="ID1 ID2 ..." -p coordinateCSV=CSV' ./INPUT_FILE.mdb

bash$ gridlabd -D mdb_load_options='-t table -f object -o ./OUTPUT_FILE.glm -p table=TABLE -p class=CLASS -p modules=MODULES' ./INPUT_FILE.mdb

bash$ gridlabd -D mdb_load_options='-t table -f player -o ./OUTPUT_FILE.glm -p table=AMI -p csvfile=ami -p properties=power' ./INPUT_FILE.mdb
~~~

# Caveat

The `MDB` converters require the `mdbtools` be installed on the host system.

# See also

* [[/Converters/Input/Csv_files]]






