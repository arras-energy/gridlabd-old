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
