[[/Subcommand/Geodata]] -- Process geographic data

# Synopsis

GLM:
~~~
#geodata OPTIONS DIRECTIVE [LOCATION|DATAFILE ...] [ARGUMENTS]
~~~

Shell:
~~~
% gridlabd geodata OPTIONS DIRECTIVE [ARGUMENTS]
~~~

# Description

The geodata command gathers and joins geographic data. The geodata subcommand uses directives that are documented in the DIRECTIVES section below.

In general geodata is used to acquire geographic information at a location or along a specified path. This information includes ground elevation, vegetation characteristics, weather, census, building, and transportation data.  The specific data sets and their origins are described in the DATASETS section below.

Some datasets require data acquired from online source. The default cache folder is /usr/local/share/gridlabd/geodata.

System, user, and local configurations may be saved.  The default config file `$HOME/.gridlabd/geodata-config`.

Some datasets may be processes in parallel.  The default maximum thread count is 1.

# Options

## `-C|--configfile FILE`

Changes config file

## `-d|--debug`

Enables debugging output, i.e., errors are exceptions with traceback.

## `-D|--dataset NAME`

Specifies the dataset from which the geographic information is to be acquired. See the DATASETS section below for details.

## `-f|--format {CSV,JSON,RAW,POS,TABLE,GLM,GDF,XLSX}[:FIELDS]`

Specified the output format.  See `gridlabd geodata help` for details.

## `--fieldsep STRING`

Set the RAW output field separator.

## `--filter=CALLS`

The `--filter=CALLS` option applies the pandas dataframe calls to the resulting data before returning the result.  The CALLS may be a comma separate value list of valid dataframe function calls, e.g., `dropna()`, `drop(0)`, or `gt(0)`.  This may be used in conjunction with `--select` to remove rows that fail to meet certain criteria. Multiple filters are semicolon-delimited

## `-j|--join {inner,outer,left,right}`

Controls how the dataset is merged with the input path. If [-j|--join {inner,outer,left,right}] is specified, the input is joined with the dataset in the manner specified, i.e., "left" uses the input index, "right" uses the dataset's index, "outer" forms the union of both indexes and sorts it, and "inner" forms the intersection of both indexes, preserving the order of the input index.

## `-k|--key FIELD1[,FIELD2[,...]]`

Set the index to the specified field(s).

## `-o|--output CSVOUT`

Normally the output is written to /dev/stdout. When CSVOUT is specified the output data is written to the specified file. If the output file already exists, it is overwritten with the new data.

## `-p|--precision DIGITS`

Set the latitude and longitude precision. The default is 5, which is about 1 meter.

## `-r|--resolution METERS`

Specifies the resolution in meters of the data to be generated along the path. If omitted, the output is generated only at the locations listed in the input. This option has no meaning for the location directive.

## `--recordsep STRING`

Set the RAW output record separator.

## `-s|--silent`

Disables error output.

## `--select=COLUMNS`

Selects rows with columns that evaluate to `True`. Multiple colums are comma-delimited.

## `--show_config`

Displays the current configuration values.

## `--show_options`

Displays the current option values.

## `-v|--verbose`

Enables verbose output.

## `-w|--warning`

Disables warning output.

# Directives

The following directives are available:

## `config get PARAMETER`
## `config set PARAMETER VALUE`
## `config show [PATTERN]`

The config directive gets, sets, and show values in the config file.

# Datasets

The following datasets are available. To specify a layer, use the syntax
"NAME.LAYER".  Multiple datasets and layers may be specified using a
comma-delimited list.

## `distance`

The `distance` dataset provides simple distance calculations using the haversine
formule for `(latitude,longitude)` tuples.

## `elevation`

The `elevation` dataset provides 1 arcsecond resolution ground elevation data
from the USGS. The default data source is located at `http://geodata.gridlabd.us/elevation/10m`.

## `address`

The `address` dataset provides address resolution for the United States. The
data is obtained from Nominatim using the geocode library.

## `utility`

The `utility` dataset delivers utility service territories and information
provided by ArcGis for the Homeland Infrastructure Foundation-Level Data
(HIFLD) website.

## `vegetation`

The `vegetation` dataset provides 7 layers of data about vegetation.  Available
layers are "canopy_cover", "canopy_height", "base_height", "bulk_density",
"layer_count", "fuel_density", and "surface_fuels".. You must subscribe to the
Forest Observatory service to use this dataset.

## `weather`

The `weather` dataset provides historical, current, and short term forecasts of
dry bulk, wet bulb, solar, and wind data from NOAA.

## `census`

The `census` dataset provides economic and population data from the US Census
Bureau.

## `building`

The `building` dataset provides build environment data from NREL. Available
layers are "building_type" and "building_size".

## `transportation`

The `transportation` dataset provides mobility data from traffic data services.
You must subscribe to the TrafficView service to use this dataset.

# Examples

The following command obtains the distance between two locations:

~~~
% gridlabd geodata merge -d -D distance location 37.4150,-122.2056 37.3880,-122.2884
latitude,longitude,distance
37.415,-122.2056,0.0
37.388,-122.2884,7906.193731232153
~~~

The following command obtains the coordinates for the points between two locations at 1 km resolution:

~~~
% gridlabd geodata merge -d -D distance location 37.4150,-122.2056 37.3880,-122.2884 -r 1000
latitude,longitude,distance
37.415,-122.2056,0.0
37.41158495593229,-122.21607280180764,1000.0
37.40816991186458,-122.22654560361528,1000.0
37.404754867796875,-122.23701840542292,1000.0
37.40133982372917,-122.24749120723055,1000.0
37.39792477966146,-122.25796400903819,1000.0
37.39450973559375,-122.26843681084583,1000.0
37.39109469152604,-122.27890961265346,1000.0
37.388,-122.2884,906.1937312321534
~~~
