[[/Subcommand/Geodata]] -- Process geographic data

# Synopsis

GLM:
~~~
#geodata OPTIONS DIRECTIVE [ARGUMENTS]
~~~

Shell:
~~~
% gridlabd geodata OPTIONS DIRECTIVE [ARGUMENTS]
~~~

# Description

The geodata command gathers and fuses geographic data. The geodata subcommand uses directives that are documented in the DIRECTIVES section below.

In general geodata is used to acquire geographic information at a location or along a specified path. This information includes ground elevation, vegetation characteristics, weather, census, building, and transportation data.  The specific data sets and their origins are described in the DATASETS section below.

Some datasets require data acquired from online source. The default cache folder is /usr/local/share/gridlabd/geodata.

System, user, and local configurations may be saved.  The default config file `$HOME/.gridlabd/geodata-config`.

Some datasets may be processes in parallel.  The default maximum thread count is 1.

# Options

## `-v|--verbose`

Enables verbose output.

## `-q|--quiet`

Disables normal output.

## `-s|--silent`

Disables error output.

## `-d|--debug`

Enables debugging output, i.e., errors are exceptions with traceback.

## `-w|--warning`

Disables warning output.

## `-c|--cache FOLDER`

Changes cache folder.

## `-C|--config FILE`

Changes config file

## `-T|--threadcount THREADS`

Change maximum thread count

## `-D|--dataset NAME`

Specifies the dataset from which the geographic information is to be acquired. See the DATASETS section below for details.

## `-o|--output CSVOUT`

Normally the output is written to /dev/stdout. When the [-f|--fuse] option is specified, the input file name is used.  When CSVOUT is specified the output data is written to the specified file. If the output file already exists, it is overwritten with the new data.

## `-r|--resolution] METERS`

Specifies the resolution in meters of the data to be generated along the path. If omitted, the output is generated only at the locations listed in the input. This option has no meaning for the location directive.

## `[-f|--fuse]`

Enable fusing new data with existing data in the output.

# Directives

The following directives are available:

## `config get PARAMETER`
## `config set PARAMETER VALUE`
## `config show [PATTERN]`

The config directive gets, sets, and show values in the config file.

## `location LATITUDE LONGITUDE [ARGUMENTS]`

The location directive performs the geodata operation only at the  location specified.

## `path CSVIN [ARGUMENTS]`

The path directive performs the geodata operation along the path specified in the CSVIN file.  If the `[-r|--resoluation METERS]` option is used, the data is generated at the specified resolution. Otherwise the data is generated only at the points specified in CSVIN.

# Datasets

The following datasets are available. To specify a layer, use the syntax
"NAME.LAYER".  Multiple datasets and layers may be specified using a
comma-delimited list.

## `distance`

The distance dataset provides simple distance calculations using the haversine
formule for `(latitude,longitude)` tuples.

## `elevation`

The elevation dataset provides 1 arcsecond resolution ground elevation data
from the USGS.

## `vegetation`

The vegetation dataset provides 7 layers of data about vegetation.  Available
layers are "canopy_cover", "canopy_height", "base_height", "bulk_density",
"layer_count", "fuel_density", and "surface_fuels".. You must subscribe to the
Forest Observatory service to use this dataset.

## `weather`

The weather dataset provides historical, current, and short term forecasts of
dry bulk, wet bulb, solar, and wind data from NOAA.

## `census`

The census dataset provides economic and population data from the US Census
Bureau.

## `building`

The building dataset provides build environment data from NREL. Available
layers are "building_type" and "building_size".

## `transportation`

The transportation dataset provides mobility data from traffic data services.
You must subscribe to the TrafficView service to use this dataset.

# Examples

The following command obtains the distance between two locations:

~~~
% gridlabd geodata -D distance location 37.415045141688054,-122.2056472090359 37.388063971857704,-122.28844288884694
latitude,longitude,distance
37.415045141688054,-122.2056472090359,0.0
37.388063971857704,-122.28844288884694,7905.040879872745
~~~

The following command obtains the coordinates for the points between two locations at 1 km resolution:

~~~
% gridlabd geodata -D distance location 37.415045141688054,-122.2056472090359 37.388063971857704,-122.28844288884694 -r 1000
latitude,longitude,distance
37.415045141688054,-122.2056472090359,0.0
37.4116319816247,-122.21612099166008,1000.0
37.40821882156135,-122.22659477428425,1000.0
37.404805661497996,-122.23706855690843,1000.0
37.40139250143464,-122.2475423395326,1000.0
37.39797934137129,-122.25801612215677,1000.0
37.39456618130794,-122.26848990478095,1000.0
37.391153021244584,-122.27896368740512,1000.0
37.388063971857704,-122.28844288884694,905.0408798727449
~~~
