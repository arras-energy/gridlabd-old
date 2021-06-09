
<div class="cell markdown">

# GridLAB-D Geodata Subcommand Tutorial

</div>

<div class="cell markdown">

As of version 4.2.21, HiPAS GridLAB-D supports the handling of
geographic data. This document provides a tutorial for using the
`geodata` subcommand in GridLAB-D.

</div>

<div class="cell markdown">

## System Preparation

The following command checks that the version of GridLAB-D supports the
`geodata` subcommand.

</div>

<div class="cell code" execution_count="1">

``` python
bash% gridlabd --version='-ge 4.2.21'
```

</div>

<div class="cell markdown">

# General Usage

The `geodata` subcommand uses the general syntax
`gridlabd geodata OPTIONS DIRECTIVE [ARGUMENTS]`.

</div>

<div class="cell markdown">

## Creating location data

There are two basic types of geodata entities in GridLAB-D:

1.  an unordered collection of points each specified by a
    `latitude,longitude` tuple; and

2.  an ordered series of waypoints along a path specified by a sequence
    of `latitude,longitude` tuples.

The interpretation of an entity is left to the dataset processor, but it
can be specified using the `location` or `position` keys, for unordered
and ordered geodata entities, respectively. A geodata entity can be
converted from one to another by simply changing the key (see **Geodata
Indexing** below).

</div>

<div class="cell markdown">

The `create` directive is used to create a new geodata entity. The
general syntax is `gridlabd geodata create LOCATIONS ...`.

There are two methods of introducing locations. The first method
introduces one or more `latitude,longitude` tuples directly in the
command line. For example, the following command creates a geodata
entity with the approximate location of SLAC's main gate.

</div>

<div class="cell code" execution_count="2">

``` python
bash% gridlabd geodata create 37.415,-122.20
```

<div class="output stream stdout">

    id,latitude,longitude
    0,37.415,-122.2

</div>

</div>

<div class="cell markdown">

Multiple locations can be introduced by adding them to the command line,
for example:

</div>

<div class="cell code" execution_count="3">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.21
```

<div class="output stream stdout">

    id,latitude,longitude
    0,37.41,-122.2
    1,37.42,-122.21

</div>

</div>

<div class="cell markdown">

The second method uses an input file with locations and associated data,
such as this example CSV file:

</div>

<div class="cell code" execution_count="4">

``` python
bash% head -n 4 path_example.csv
```

<div class="output stream stdout">

    latitude,longitude,configuration,pole_height
    37.415045141688054,-122.2056472090359,flat3,18.0
    37.414698020593065,-122.20848749028133,sideT,20.0
    37.414454093051745,-122.21044282065421,sideT,21.0

</div>

</div>

<div class="cell markdown">

To use this file, the following command can be used:

</div>

<div class="cell code" execution_count="5">

``` python
bash% gridlabd geodata create path_example.csv | head -n 4
```

<div class="output stream stdout">

    id,latitude,longitude,configuration,pole_height
    0,37.41505,-122.20565,flat3,18.0
    1,37.4147,-122.20849,sideT,20.0
    2,37.41445,-122.21044,sideT,21.0

</div>

</div>

<div class="cell markdown">

Note that if multiple locations are provided they are sequenced in the
order in which they are presented, including if locations are provided
directly on the command line or from data files.

</div>

<div class="cell markdown">

If no location information is given, then the geodata is read from
`/dev/stdin`, e.g.,

</div>

<div class="cell code" execution_count="6">

``` python
bash% head -n 4 path_example.csv | cut -f1-2 -d, | gridlabd geodata create
```

<div class="output stream stdout">

    id,latitude,longitude
    0,37.41505,-122.20565
    1,37.4147,-122.20849
    2,37.41445,-122.21044

</div>

</div>

<div class="cell markdown">

Additional fields can be added using an expanded command-line syntax,
e.g.,

</div>

<div class="cell code" execution_count="7">

``` python
bash% gridlabd geodata create name=obj1+latitude=37.4205+longitude=-122.2046 name=obj2+latitude=37.5205+longitude=-122.3046
```

<div class="output stream stdout">

    id,name,latitude,longitude
    0,obj1,37.4205,-122.2046
    1,obj2,37.5205,-122.3046

</div>

</div>

<div class="cell markdown">

The default input delimiter is the `+` character. This can be changed
using the `--input_delimiter=STRING` option, e.g.,

</div>

<div class="cell code" execution_count="8">

``` python
bash% gridlabd geodata create --input_delimiter=',' 'name=obj1,latitude=37.4205,longitude=-122.2046' 'name=obj2,latitude=37.5205,longitude=-122.3046'
```

<div class="output stream stdout">

    id,name,latitude,longitude
    0,obj1,37.4205,-122.2046
    1,obj2,37.5205,-122.3046

</div>

</div>

<div class="cell markdown">

Note that the input delimiter must be set *before* it is used, and thus
it can be changed as command line data is presented.

</div>

<div class="cell markdown">

**Caveat**: The input delimiter can affect how strings are interpreted.
One common problem is setting the input delimiter to any character that
is typically found in an address and then attempting to reverse resolve
the address using the `address` dataset. This can result in corrupted
addresses being used.

</div>

<div class="cell markdown">

## Some useful options

</div>

<div class="cell markdown">

The default precision with which latitudes and longitudes are output is
5 decimals, which is approximately 1 meter resolution. You can change
the precision with which latitudes and longitudes are output using the
`-p` or `--precision` option, e.g.,

</div>

<div class="cell code" execution_count="9">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -p 4
```

<div class="output stream stdout">

    id,latitude,longitude
    0,37.41,-122.2
    1,37.42,-122.2
    2,37.42,-122.21

</div>

</div>

<div class="cell markdown">

This option actually changes the `precision.geolocation` option, which
can also be set using the direct option set syntax, e.g.,

</div>

<div class="cell code" execution_count="10">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 --precision.geolocation=4
```

<div class="output stream stdout">

    id,latitude,longitude
    0,37.41,-122.2
    1,37.42,-122.2
    2,37.42,-122.21

</div>

</div>

<div class="cell markdown">

The default field separator for RAW output is a space. You can change
this to any string using the `--fieldsep STRING` option, e.g.,

</div>

<div class="cell code" execution_count="11">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f RAW --fieldsep ','
```

<div class="output stream stdout">

    37.41,-122.2
    37.42,-122.2
    37.42,-122.21

</div>

</div>

<div class="cell markdown">

This is equivalent to directly setting the field separator option using
`--field_separator=','`.

</div>

<div class="cell markdown">

Similarly the default record separator for RAW output is a newline. You
can change this to any string using the `--recordsep STRING` option,
e.g.,

</div>

<div class="cell code" execution_count="12">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f RAW --fieldsep ',' --recordsep ';'
```

<div class="output stream stdout">

    37.41,-122.2;37.42,-122.2;37.42,-122.21

</div>

</div>

<div class="cell markdown">

This is equivalent to directly setting the record separator option using
`--record_separator=';'`.

</div>

<div class="cell markdown">

## Configurations

There are three locations where configuration settings are maintained:
(1) the system shared folder, (2) the user's home folder, and (3) the
local folder. These are consulted in this order so that the system
configuration overrides the default configuration, the user
configuration override the system, and the local configuration overrides
the user configuration.

By default the configuration files are named `geodata.conf`. The system
configuration is stored in `$GLD_ETC/geodata/geodata.conf` folder. The
user configuration is stored in `$HOME/.gridlabd/geodata/geodata.conf`
and the local configuration is stored in `$PWD/geodata.conf`.

Any additional configuration file name can be consulted using the
`-C FILENAME` or `--configfile FILENAME` option.

</div>

<div class="cell markdown">

You can manage the current configuration using the `config` directive,
e.g., to set the local configuration parameter `name` to `value`, use
the `set` option

</div>

<div class="cell code" execution_count="13">

``` python
bash% gridlabd geodata config set name local_value
```

<div class="output stream stdout">

    WARNING [geodata]: config file '/Users/dchassin/github/gridlabd/gldcore/geodata/docs/geodata.conf' is not found

</div>

</div>

<div class="cell markdown">

Note that if the file in which the parameter is stored does not already
exist, you will get a warning before it is created. This behavior can be
suppressed using the `-w` or `--warn` option, e.g.,

</div>

<div class="cell code" execution_count="14">

``` python
bash% gridlabd geodata config set name local_value -w
```

</div>

<div class="cell markdown">

To get the value, use the `get` option:

</div>

<div class="cell code" execution_count="15">

``` python
bash% gridlabd geodata config get name
```

<div class="output stream stdout">

    local_value

</div>

</div>

<div class="cell markdown">

To show all the configuration values, use the `show` option:

</div>

<div class="cell code" execution_count="16">

``` python
bash% gridlabd geodata config show
```

<div class="output stream stdout">

    geodata_url='http://geodata.gridlabd.us/'
    output_format='CSV'
    path_join='outer'
    column_names='{'ID': 'id', 'UUID': 'uuid', 'LAT': 'latitude', 'LON': 'longitude', 'DIST': 'distance', 'HEAD': 'heading', 'LOC': 'location', 'POS': 'position'}'
    uuid_type='4'
    name='local_value'

</div>

</div>

<div class="cell markdown">

To set a user configuration, use the `user.` prefix, e.g.,

</div>

<div class="cell code" execution_count="17">

``` python
bash% gridlabd geodata config set user.name user_value
bash% gridlabd geodata config show
```

<div class="output stream stdout">

    geodata_url='http://geodata.gridlabd.us/'
    output_format='CSV'
    path_join='outer'
    column_names='{'ID': 'id', 'UUID': 'uuid', 'LAT': 'latitude', 'LON': 'longitude', 'DIST': 'distance', 'HEAD': 'heading', 'LOC': 'location', 'POS': 'position'}'
    uuid_type='4'
    name='local_value'
    user.name='user_value'

</div>

</div>

<div class="cell markdown">

The same syntax is used for system configuration values, e.g.,

</div>

<div class="cell code" execution_count="18">

``` python
bash% gridlabd geodata config set system.name system_value
bash% gridlabd geodata config show
```

<div class="output stream stdout">

    geodata_url='http://geodata.gridlabd.us/'
    output_format='CSV'
    path_join='outer'
    column_names='{'ID': 'id', 'UUID': 'uuid', 'LAT': 'latitude', 'LON': 'longitude', 'DIST': 'distance', 'HEAD': 'heading', 'LOC': 'location', 'POS': 'position'}'
    uuid_type='4'
    name='local_value'
    user.name='user_value'
    system.name='system_value'

</div>

</div>

<div class="cell markdown">

To remove a value, use the `unset` option, e.g.,

</div>

<div class="cell code" execution_count="19">

``` python
bash% gridlabd geodata config unset name
bash% gridlabd geodata config show
```

<div class="output stream stdout">

    geodata_url='http://geodata.gridlabd.us/'
    output_format='CSV'
    path_join='outer'
    column_names='{'ID': 'id', 'UUID': 'uuid', 'LAT': 'latitude', 'LON': 'longitude', 'DIST': 'distance', 'HEAD': 'heading', 'LOC': 'location', 'POS': 'position'}'
    uuid_type='4'
    user.name='user_value'
    system.name='system_value'

</div>

</div>

<div class="cell markdown">

To remove all the local configuration values, simply delete the
`geodata.conf` file:

</div>

<div class="cell code" execution_count="20">

``` python
bash% rm geodata.conf
```

</div>

<div class="cell markdown">

## Geodata formatting

The output format can be changed using the `-f` or `--format` option.
The valid formats are `CSV`, `JSON`, `RAW`, `POS`, `GLM`, `FIELD`,
`PLOT`, `GDF`, `XLSX`, and `TABLE`. `CSV` format is the default, and it
can be explicitly specified as follows:

</div>

<div class="cell code" execution_count="21">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f CSV
```

<div class="output stream stdout">

    id,latitude,longitude
    0,37.41,-122.2
    1,37.42,-122.2
    2,37.42,-122.21

</div>

</div>

<div class="cell markdown">

JSON output looks like this:

</div>

<div class="cell code" execution_count="22">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f JSON
```

<div class="output stream stdout">

    {"0":{"latitude":37.41,"longitude":-122.2},"1":{"latitude":37.42,"longitude":-122.2},"2":{"latitude":37.42,"longitude":-122.21}}

</div>

</div>

<div class="cell markdown">

JSON output can be structured using the `pandas` `to_json()` `orient`
options `index`, `split`, `records`, `columns`, `values`, or `table`,
e.g.,

</div>

<div class="cell code" execution_count="23">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f JSON --json.orient=table
```

<div class="output stream stdout">

    {"schema":{"fields":[{"name":"id","type":"integer"},{"name":"latitude","type":"number"},{"name":"longitude","type":"number"}],"primaryKey":["id"],"pandas_version":"0.20.0"},"data":[{"id":0,"latitude":37.41,"longitude":-122.2},{"id":1,"latitude":37.42,"longitude":-122.2},{"id":2,"latitude":37.42,"longitude":-122.21}]}

</div>

</div>

<div class="cell markdown">

Other JSON options include `data_format`, `double_precision`,
`force_ascii`, and `date_unit`. See
<https://pandas.pydata.org/docs/reference/api/pandas.DataFrame.to_json.html>
for details.

</div>

<div class="cell markdown">

RAW output is generated as follows

</div>

<div class="cell code" execution_count="24">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f RAW
```

<div class="output stream stdout">

    37.41 -122.2
    37.42 -122.2
    37.42 -122.21

</div>

</div>

<div class="cell markdown">

TABLE output is generated for easy reading:

</div>

<div class="cell code" execution_count="25">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f TABLE
```

<div class="output stream stdout">

        latitude  longitude
    id                     
    0      37.41    -122.20
    1      37.42    -122.20
    2      37.42    -122.21

</div>

</div>

<div class="cell markdown">

Output formats typically can include an ordered field list, such as

</div>

<div class="cell code" execution_count="26">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f RAW:longitude,latitude --fieldsep ,
```

<div class="output stream stdout">

    -122.2,37.41
    -122.2,37.42
    -122.21,37.42

</div>

</div>

<div class="cell markdown">

## Geodata indexing

The output can be indexed using one of several standards keys, or keys
from the data. The default key is `id` which is the row number, as seen
in the examples so far. This key can be explicitly specified as follows:

</div>

<div class="cell code" execution_count="27">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -k id
```

<div class="output stream stdout">

    id,latitude,longitude
    0,37.41,-122.2
    1,37.42,-122.2
    2,37.42,-122.21

</div>

</div>

<div class="cell markdown">

The `location` key generates a geohash code:

</div>

<div class="cell code" execution_count="28">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -k location
```

<div class="output stream stdout">

    location,latitude,longitude,id
    9q9hg629j97y,37.41,-122.2,0
    9q9hgk0em9ef,37.42,-122.2,1
    9q9hgh17k9e4,37.42,-122.21,2

</div>

</div>

<div class="cell markdown">

The `position` key generates a distance index, treating the data rows as
a series of waypoints along a path. When this key is used, a distance
and a heading column are also generated.

</div>

<div class="cell code" execution_count="29">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -k position
```

<div class="output stream stdout">

    position,latitude,longitude,id,distance,heading
    0,37.41,-122.2,0,0.0,
    1112,37.42,-122.2,1,1112.0,0.0
    1995,37.42,-122.21,2,1995.0,270.0

</div>

</div>

<div class="cell markdown">

Any field or set of fields may be used for indexing, e.g.,

</div>

<div class="cell code" execution_count="30">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -k latitude,longitude
```

<div class="output stream stdout">

    latitude,longitude,id
    37.41,-122.2,0
    37.42,-122.2,1
    37.42,-122.21,2

</div>

</div>

<div class="cell markdown">

Universally unique identifiers can be also be generated using the
special `uuid` key, e.g.,

</div>

<div class="cell code" execution_count="31">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -k uuid
```

<div class="output stream stdout">

    uuid,latitude,longitude,id
    8cbf6a063af34edeb0221c430c9503bb,37.41,-122.2,0
    724b696aaea04b58959717e2197f2e86,37.42,-122.2,1
    c4e96aafe7c94a3783f05d68afcce029,37.42,-122.21,2

</div>

</div>

<div class="cell markdown">

The default `uuid` type is a random number, i.e., `--uuid_type=4`. Using
`--uuid_type=1` generates a unique identified based on the host,
sequence number, and time, e.g.,

</div>

<div class="cell code" execution_count="32">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -k uuid --uuid_type=1
```

<div class="output stream stdout">

    uuid,latitude,longitude,id
    8fd7f866c93c11eb8c1facde48001122,37.41,-122.2,0
    8fd7fab4c93c11eb8c1facde48001122,37.42,-122.2,1
    8fd7fae6c93c11eb8c1facde48001122,37.42,-122.21,2

</div>

</div>

<div class="cell markdown">

## Path waypoints

Waypoints can be generated along a path using the `-r` or `--resolution`
option. For example, 250-meter waypoints are generated using the
following syntax:

</div>

<div class="cell code" execution_count="33">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -r 250 -f TABLE
```

<div class="output stream stdout">

              latitude  longitude   id  distance  heading
    position                                             
    0         37.41000 -122.20000  0.0       0.0      NaN
    250       37.41225 -122.20000  NaN     250.0      0.0
    500       37.41450 -122.20000  NaN     500.0      0.0
    750       37.41674 -122.20000  NaN     750.0      0.0
    1000      37.41899 -122.20000  NaN    1000.0      0.0
    1112      37.42000 -122.20000  1.0    1112.0      0.0
    1362      37.42000 -122.20283  NaN    1362.0    270.0
    1612      37.42000 -122.20566  NaN    1612.0    270.0
    1862      37.42000 -122.20849  NaN    1862.0    270.0
    1995      37.42000 -122.21000  2.0    1995.0    270.0

</div>

</div>

<div class="cell markdown">

This is equivalent to setting the `resolution` option value, e.g.,

</div>

<div class="cell code" execution_count="34">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 --resolution=250 -f TABLE
```

<div class="output stream stdout">

              latitude  longitude   id  distance  heading
    position                                             
    0         37.41000 -122.20000  0.0       0.0      NaN
    250       37.41225 -122.20000  NaN     250.0      0.0
    500       37.41450 -122.20000  NaN     500.0      0.0
    750       37.41674 -122.20000  NaN     750.0      0.0
    1000      37.41899 -122.20000  NaN    1000.0      0.0
    1112      37.42000 -122.20000  1.0    1112.0      0.0
    1362      37.42000 -122.20283  NaN    1362.0    270.0
    1612      37.42000 -122.20566  NaN    1612.0    270.0
    1862      37.42000 -122.20849  NaN    1862.0    270.0
    1995      37.42000 -122.21000  2.0    1995.0    270.0

</div>

</div>

<div class="cell markdown">

The `distance` and `heading` columns are added to provide the distance
and heading to each waypoint from the last position. When using
waypoints, the key is automatically set to `position`, which provides
the distance from the first point. The use of other keys is not
supported. In addition, any waypoint added in the process is not
assigned a row `id` in order to protect the original row ids.

</div>

<div class="cell markdown">

To assign key values to the generated positions, use `--resolution_id`
options, e.g.,

</div>

<div class="cell code" execution_count="35">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f TABLE -r 250  --resolution_id
```

<div class="output stream stdout">

              latitude  longitude     id  distance  heading
    position                                               
    0         37.41000 -122.20000  0.000       0.0      NaN
    250       37.41225 -122.20000  0.125     250.0      0.0
    500       37.41450 -122.20000  0.251     500.0      0.0
    750       37.41674 -122.20000  0.376     750.0      0.0
    1000      37.41899 -122.20000  0.501    1000.0      0.0
    1112      37.42000 -122.20000  0.557    1112.0      0.0
    1362      37.42000 -122.20283  0.683    1362.0    270.0
    1612      37.42000 -122.20566  0.808    1612.0    270.0
    1862      37.42000 -122.20849  0.933    1862.0    270.0
    1995      37.42000 -122.21000  1.000    1995.0    270.0

</div>

</div>

<div class="cell markdown">

The new id values are generated as a fraction of the distance from the
first to the last waypoint.

</div>

<div class="cell markdown">

The precision of the new id values can be changed, e.g.,

</div>

<div class="cell code" execution_count="36">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f TABLE -r 250  --resolution_id --precision.resolution_id=5
```

<div class="output stream stdout">

              latitude  longitude       id  distance  heading
    position                                                 
    0         37.41000 -122.20000  0.00000       0.0      NaN
    250       37.41225 -122.20000  0.12531     250.0      0.0
    500       37.41450 -122.20000  0.25063     500.0      0.0
    750       37.41674 -122.20000  0.37594     750.0      0.0
    1000      37.41899 -122.20000  0.50125    1000.0      0.0
    1112      37.42000 -122.20000  0.55739    1112.0      0.0
    1362      37.42000 -122.20283  0.68271    1362.0    270.0
    1612      37.42000 -122.20566  0.80802    1612.0    270.0
    1862      37.42000 -122.20849  0.93333    1862.0    270.0
    1995      37.42000 -122.21000  1.00000    1995.0    270.0

</div>

</div>

<div class="cell markdown">

The index can be changed with the `-k` or `--key` options using a pipe.
Sometimes this is necessary when one option used implicitly precludes
the use of a desired key, such as when path resolution is used but
location is indexing is desired, e.g.,

</div>

<div class="cell code" execution_count="37">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -r 250 | gridlabd geodata create -k location -f TABLE
```

<div class="output stream stdout">

                  position  latitude  longitude   id  distance  heading
    location                                                           
    9q9hg629j97y         0  37.41000 -122.20000  0.0       0.0      NaN
    9q9hg68wjsgu       250  37.41225 -122.20000  NaN     250.0      0.0
    9q9hg70em87c       500  37.41450 -122.20000  NaN     500.0      0.0
    9q9hg788m9gu       750  37.41674 -122.20000  NaN     750.0      0.0
    9q9hg7btmt7c      1000  37.41899 -122.20000  NaN    1000.0      0.0
    9q9hgk0em9ef      1112  37.42000 -122.20000  1.0    1112.0      0.0
    9q9hghne79td      1362  37.42000 -122.20283  NaN    1362.0    270.0
    9q9hghhe39x6      1612  37.42000 -122.20566  NaN    1612.0    270.0
    9q9hgh47rc94      1862  37.42000 -122.20849  NaN    1862.0    270.0
    9q9hgh17k9e4      1995  37.42000 -122.21000  2.0    1995.0    270.0

</div>

</div>

<div class="cell markdown">

## Output

By default all output is written to `/dev/stdout`. The output can be
written to a file using the `-o` or `--output` option, e.g.,

</div>

<div class="cell code" execution_count="38">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -o /tmp/test.csv
bash% cat /tmp/test.csv
```

<div class="output stream stdout">

    id,latitude,longitude
    0,37.41,-122.2
    1,37.42,-122.2
    2,37.42,-122.21

</div>

</div>

<div class="cell markdown">

GridLAB-D `geodata` entities can be output to GeoPandas using the `GDF`
output format. If the entity does not include any geometry data, it is
automatically created from the latitude and longitude fields, e.g.,

</div>

<div class="cell code" execution_count="39">

``` python
bash% gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -o /tmp/test.gdf -f GDF
```

</div>

<div class="cell markdown">

The following python code can be used to load the output file:

</div>

<div class="cell code" execution_count="40">

``` python
import geopandas
geopandas.read_file("/tmp/test.gdf")
```

<div class="output execute_result" execution_count="40">

       id  latitude  longitude                     geometry
    0   0     37.41    -122.20  POINT (-122.20000 37.41000)
    1   1     37.42    -122.20  POINT (-122.20000 37.42000)
    2   2     37.42    -122.21  POINT (-122.21000 37.42000)

</div>

</div>

<div class="cell markdown">

------------------------------------------------------------------------

# Datasets

The `merge` directive is used to merge a dataset into an existing
`geodata` entity. The general syntax is
`gridlabd geodata merge -D DATASET [OPTIONS ...]`

</div>

<div class="cell markdown">

## Distance

The `distance` dataset provide great-circle distance calculations. For
example, the following calculates the distance from the first point in
the series.

</div>

<div class="cell code" execution_count="41">

``` python
bash% gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f TABLE
```

<div class="output stream stdout">

        latitude  longitude  distance
    id                               
    0      37.41    -122.20       0.0
    1      37.42    -122.20    1112.0
    2      37.42    -122.21    1995.0

</div>

</div>

<div class="cell markdown">

The distance can be calculated relative to the previous location in the
series using the `--relative` options, e.g.,

</div>

<div class="cell code" execution_count="42" scrolled="true">

``` python
bash% gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21 --relative -f TABLE
```

<div class="output stream stdout">

        latitude  longitude  distance
    id                               
    0      37.41    -122.20       0.0
    1      37.42    -122.20       0.0
    2      37.42    -122.21       0.0

</div>

</div>

<div class="cell markdown">

The `distance` dataset support different units, include `meters` or `m`,
`kilometers` or `km`, `feet` or `ft`, `yards` or `yd`, `miles` or `mi`.
For example, the following calculates the distances in feet:

</div>

<div class="cell code" execution_count="43">

``` python
bash% gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21 --units=feet -f TABLE
```

<div class="output stream stdout">

        latitude  longitude  distance
    id                               
    0      37.41    -122.20       0.0
    1      37.42    -122.20    3648.0
    2      37.42    -122.21    6545.0

</div>

</div>

<div class="cell markdown">

Note that the default precision with which distances are calculated in 0
decimals. Thus the distance in `km` is rounded:

</div>

<div class="cell code" execution_count="44">

``` python
bash% gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21 --units=km -f TABLE # precision too low
```

<div class="output stream stdout">

        latitude  longitude  distance
    id                               
    0      37.41    -122.20       0.0
    1      37.42    -122.20       1.0
    2      37.42    -122.21       2.0

</div>

</div>

<div class="cell markdown">

The precision can be changed when necessary, e.g.,

</div>

<div class="cell code" execution_count="45">

``` python
bash% gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21 --units=km --precision.distance=2 -f TABLE
```

<div class="output stream stdout">

        latitude  longitude  distance
    id                               
    0      37.41    -122.20      0.00
    1      37.42    -122.20      1.11
    2      37.42    -122.21      2.00

</div>

</div>

<div class="cell markdown">

When `distance` is used with `--resolution` and `--relative`, the
waypoints are not considered, e.g.,

</div>

<div class="cell code" execution_count="46">

``` python
bash% gridlabd geodata -D distance merge 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f TABLE --table.max_rows=99 -r 250 --relative
```

<div class="output stream stdout">

              latitude  longitude   id  distance  heading
    position                                             
    0         37.41000 -122.20000  0.0       0.0      NaN
    250       37.41225 -122.20000  NaN     250.0      0.0
    500       37.41450 -122.20000  NaN     500.0      0.0
    750       37.41674 -122.20000  NaN     750.0      0.0
    1000      37.41899 -122.20000  NaN    1000.0      0.0
    1112      37.42000 -122.20000  1.0       0.0      0.0
    1362      37.42000 -122.20283  NaN     250.0    270.0
    1612      37.42000 -122.20566  NaN     500.0    270.0
    1862      37.42000 -122.20849  NaN     750.0    270.0
    1995      37.42000 -122.21000  2.0       0.0    270.0

</div>

</div>

<div class="cell markdown">

## Address

You can use the `address` dataset to perform address resolution
operations, both to and from latitude and longitude.

To obtain the address at a location, use the following command

</div>

<div class="cell code" execution_count="47">

``` python
bash% gridlabd geodata merge -D address 37.420,-122.20
```

<div class="output stream stdout">

    id,latitude,longitude,address
    0,37.42,-122.2,"Stanford Linear Accelerator Center National Accelerator Laboratory, Sand Hill Road, Menlo Park, San Mateo County, California, 94028, United States"

</div>

</div>

<div class="cell markdown">

To find the latitude and longitude of an address, use the command:

</div>

<div class="cell code" execution_count="48">

``` python
bash% gridlabd geodata merge -D address --reverse "2575 Sand Hill Rd, Menlo Park CA 94025"
```

<div class="output stream stdout">

    id,index,address,longitude,latitude
    0,0,"2575 Sand Hill Rd, Menlo Park CA 94025",-122.20457,37.42046

</div>

</div>

<div class="cell markdown">

Note that incomplete addresses are usually resolved but may not provided
the expected result. For example, the following address is not
sufficient for proper resolution, but provides a location nonetheless:

</div>

<div class="cell code" execution_count="49">

``` python
bash% gridlabd geodata merge -D address --reverse "Main St." # insufficient information for accurate results
```

<div class="output stream stdout">

    id,index,address,longitude,latitude
    0,0,Main St.,-79.30162,43.68904

</div>

</div>

<div class="cell markdown">

You can perform both operations in series to resolve the "official"
address from an unofficial but complete address:

</div>

<div class="cell code" execution_count="50">

``` python
bash% gridlabd geodata merge -D address --reverse "2575 Sand Hill Rd, Menlo Park CA" | gridlabd geodata merge -D address
```

<div class="output stream stdout">

    id,index,address,longitude,latitude
    0,0,"Stanford Linear Accelerator Center National Accelerator Laboratory, Sand Hill Road, Menlo Park, San Mateo County, California, 94028, United States",-122.20118,37.41546

</div>

</div>

<div class="cell markdown">

## Elevation

Elevation data can be obtained using the `elevation` data set, e.g.,

</div>

<div class="cell code" execution_count="51">

``` python
bash% gridlabd geodata merge -D elevation 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f TABLE
```

<div class="output stream stdout">

        latitude  longitude  elevation
    id                                
    0      37.41    -122.20       58.0
    1      37.42    -122.20       76.0
    2      37.42    -122.21      105.0

</div>

</div>

<div class="cell markdown">

Elevations can be calculated in other units using the `--units=UNIT`
options, e.g.,

</div>

<div class="cell code" execution_count="52">

``` python
bash% gridlabd geodata merge -D elevation 37.410,-122.20 37.420,-122.20 37.420,-122.21 --units=feet -f TABLE
```

<div class="output stream stdout">

        latitude  longitude  elevation
    id                                
    0      37.41    -122.20      190.0
    1      37.42    -122.20      249.0
    2      37.42    -122.21      344.0

</div>

</div>

<div class="cell markdown">

Valid units are `m` or `meters`, and `ft` or `feet`.

The precision of the elevations can be specified as well, e.g.,

</div>

<div class="cell code" execution_count="53">

``` python
bash% gridlabd geodata merge -D elevation 37.410,-122.20 37.420,-122.20 37.420,-122.21 --precision.elevation=2 --units=feet -f TABLE
```

<div class="output stream stdout">

        latitude  longitude  elevation
    id                                
    0      37.41    -122.20     190.29
    1      37.42    -122.20     249.34
    2      37.42    -122.21     344.49

</div>

</div>

<div class="cell markdown">

When using waypoints, the elevations are calculated along the path,
e.g.,

</div>

<div class="cell code" execution_count="54">

``` python
bash% gridlabd geodata merge -D elevation 37.410,-122.20 37.420,-122.20 37.420,-122.21 -r 250 -f TABLE
```

<div class="output stream stdout">

              latitude  longitude   id  distance  heading  elevation
    position                                                        
    0         37.41000 -122.20000  0.0       0.0      NaN       58.0
    250       37.41225 -122.20000  NaN     250.0      0.0       63.0
    500       37.41450 -122.20000  NaN     500.0      0.0       75.0
    750       37.41674 -122.20000  NaN     750.0      0.0       76.0
    1000      37.41899 -122.20000  NaN    1000.0      0.0       96.0
    1112      37.42000 -122.20000  1.0    1112.0      0.0       76.0
    1362      37.42000 -122.20283  NaN    1362.0    270.0       85.0
    1612      37.42000 -122.20566  NaN    1612.0    270.0       92.0
    1862      37.42000 -122.20849  NaN    1862.0    270.0      104.0
    1995      37.42000 -122.21000  2.0    1995.0    270.0      105.0

</div>

</div>

<div class="cell markdown">

## Vegetation

Vegetation data can be obtained using the `vegetation` dataset. This
dataset includes information from the California Forest Observatory
about canopy cover, height, and base. To access this data you must
subscribe to the service at <https://forestobservatory.com>. To register
you username and password, you must configure the dataset using the
following command:

</div>

<div class="cell code" execution_count="55">

``` python
bash% gridlabd geodata config set -w vegetation.username name@example.com
bash% gridlabd geodata config set vegetation.password password5839
```

</div>

<div class="cell markdown">

When you are registered you can merge vegetation data for geographic
locations, e.g.,

</div>

<div class="cell code" execution_count="56">

``` python
bash% gridlabd geodata merge -D vegetation 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f TABLE --vegetation.usecache=False
```

<div class="output stream stdout">

    2021-06-09 09:06:49,018 WARNING cfo.utils [_auth_request] | Authentication failed. Try and run .authenticate(ignore_temp=True)
    2021-06-09 09:06:49,018 WARNING cfo.utils [_auth_request] | b'{}\n'
    2021-06-09 09:06:49,019 WARNING cfo.utils [authenticate] | Authentication failed with status code 401
    WARNING [geodata/vegetation]: vegetation data provider authentication failed
        latitude  longitude  base  cover  height
    id                                          
    0      37.41    -122.20   2.0   0.17    11.0
    1      37.42    -122.20   1.0   0.18    15.0
    2      37.42    -122.21   2.0   0.01     0.0

</div>

</div>

<div class="cell markdown">

Vegetation data is always associated with a year, which is specified by
the `year` option, i.e.,

</div>

<div class="cell code" execution_count="57">

``` python
bash% gridlabd geodata merge -D vegetation 37.410,-122.20 37.420,-122.20 37.420,-122.21 --year=2020 -f TABLE
```

<div class="output stream stdout">

    2021-06-09 09:07:08,386 WARNING cfo.utils [_auth_request] | Authentication failed. Try and run .authenticate(ignore_temp=True)
    2021-06-09 09:07:08,386 WARNING cfo.utils [_auth_request] | b'{}\n'
    2021-06-09 09:07:08,386 WARNING cfo.utils [authenticate] | Authentication failed with status code 401
    WARNING [geodata/vegetation]: vegetation data provider authentication failed
        latitude  longitude  base  cover  height
    id                                          
    0      37.41    -122.20   2.0   0.17    11.0
    1      37.42    -122.20   1.0   0.18    15.0
    2      37.42    -122.21   2.0   0.01     0.0

</div>

</div>

<div class="cell markdown">

## Utility

Utility data can be obtained using the `utility` dataset. This dataset
includes information from the Homeland Infrastructure Foundation-Level
Data (HIFLD). Note that this dataset uses a very large datafile that can
take some time to download the first time.

The name of the utility at a location is the default data result, e.g.,

</div>

<div class="cell code" execution_count="58">

``` python
bash% gridlabd geodata merge -D utility 37.420,-122.20 -f TABLE
```

<div class="output stream stdout">

        latitude  longitude             NAME
    id                                      
    0      37.42     -122.2  PACIFIC GAS ...

</div>

</div>

<div class="cell markdown">

Other data is available (see
<https://hifld-geoplatform.opendata.arcgis.com/datasets/electric-retail-service-territories>
for detail). A list of data available can be obtained from the dataset
help, e.g., using `gridlabd geodata help utility`.

To get the utility's summer and winter peak load and number of customers
and the year for which the data is obtained, use the command

</div>

<div class="cell code" execution_count="59">

``` python
bash% gridlabd geodata merge -D utility 37.420,-122.20 --fields=WINTR_PEAK,SUMMR_PEAK,CUSTOMERS,YEAR -f TABLE
```

<div class="output stream stdout">

        latitude  longitude  WINTR_PEAK  SUMMR_PEAK  CUSTOMERS    YEAR
    id                                                                
    0      37.42     -122.2     12624.0     17263.0  5471786.0  2018.0

</div>

</div>

<div class="cell markdown">

The `utility` dataset also contains geometry data that can be used to
generate maps. This data is included when the `--geometry` option is
specified, e.g.,

</div>

<div class="cell code" execution_count="60">

``` python
bash% gridlabd geodata merge -D utility 37.420,-122.20 --geometry -f PLOT -o /tmp/utility.png --plot.figsize=10,5
```

</div>

<div class="cell markdown">

The following python code can be used to display the image in a Jupyter
notebook.

</div>

<div class="cell code" execution_count="61">

``` python
from IPython.display import Image
Image("/tmp/utility.png")
```

<div class="output execute_result" execution_count="61">

bash% [](1f3dc876b6d8f3e42eebe70dba2d97e427eb4b79.png)

</div>

</div>

<div class="cell markdown">

## Powerline

Powerline calculations can be performed using the `powerline` dataset.
This dataset calculates line sag, line sway, and line gallop.

To use this dataset additional data is usually required. When the needed
data is not available at a specific position or location, static values
can be provided using the available dataset options. Valid dataset
options include the following:

-   `--air_temperature=FLOAT` specified in Celsius, with a default of
    30 C.
-   `--wind_speed=FLOAT` specified in m/s, with a default is 0 m/s.
-   `--wind_direction=FLOAT` specified in degrees, with a default is 0
    degrees.
-   `--ice_thickness=FLOAT` specified in meters, with a default is 0 m.
-   `--power_flow=FLOAT` specified in Watts, with a default is 0 W.
-   `--global_horizontal_irradiance=FLOAT` specified in W/m^2, with a
    default of 1000 W/m^2.
-   `--ground_reflectance=FLOAT` specified per unit, with a default of
    0.3 pu.
-   `--ice_density=FLOAT` specified in kg/m^3, with a default of 915
    kg/m^3.
-   `--nominal_temperature=FLOAT` specified in Celcius, with a default
    of 15 C.
-   `--cable_type=STRING` specified as a string lookup in the cable type
    file, with no default value.
-   `--elevation=FLOAT` specified in m above sea level, with a default
    of 0 m.

For example, the cable type has no default and a reference to a known
cable type can be given on the command line, e.g.,

</div>

<div class="cell code" execution_count="62" scrolled="false">

``` python
bash% gridlabd geodata merge -D powerline path_example.csv --cable_type="TACSR/AC 610mm^2" -r 250 -f TABLE:linesag
```

<div class="output stream stdout">

    ERROR [geodata/powerline]: 'height'

</div>

</div>

<div class="cell markdown">

Known cable types are listed in the `geodata_powerline_cabletypes.csv`,
which contains the following fields:

</div>

<div class="cell code" execution_count="63">

``` python
import pandas
print("\n".join(pandas.read_csv("../geodata_powerline_cabletypes.csv").columns.to_list()))
```

<div class="output stream stdout">

    id
    diameter
    unit_weight
    rated_tensile_strength
    elasticity
    thermal_expansion
    conductor_crosssection_area
    resistivity
    nominal_resistance
    nomimal_temperature
    voltage_rating
    reflectivity
    emissivity

</div>

</div>

<div class="cell markdown">

The first record contains the following values

</div>

<div class="cell code" execution_count="64">

``` python
pandas.read_csv("../geodata_powerline_cabletypes.csv",nrows=1).transpose()
```

<div class="output execute_result" execution_count="64">

                                                0
    id                           TACSR/AC 610mm^2
    diameter                               0.0342
    unit_weight                             21.56
    rated_tensile_strength                  72000
    elasticity                           7.18e+10
    thermal_expansion                    2.06e-05
    conductor_crosssection_area         0.0006918
    resistivity                            0.0039
    nominal_resistance                   4.58e-05
    nomimal_temperature                        15
    voltage_rating                         230000
    reflectivity                              0.5
    emissivity                                0.6

</div>

</div>

<div class="cell markdown">

When needed data is not provided or the value provided is invalid, a
warning is output and the requested values are not calculated, e.g.,

</div>

<div class="cell code" execution_count="65">

``` python
bash% gridlabd geodata merge -D powerline path_example.csv -f TABLE:linesag # missing cable type
```

<div class="output stream stdout">

    WARNING [geodata/powerline]: cable_type not specified
    ERROR [geodata/powerline]: 'height'

</div>

</div>

<div class="cell code" execution_count="66">

``` python
bash% gridlabd geodata merge -D powerline path_example.csv -f TABLE:linesag --cable_type="My cabletype" # bad cable type
```

<div class="output stream stdout">

    WARNING [geodata/powerline]: cable_type='My cabletype' not found
    ERROR [geodata/powerline]: 'height'

</div>

</div>

<div class="cell markdown">

------------------------------------------------------------------------

# Getting help

You can obtain help using the `help` directive, e.g.,

</div>

<div class="cell code" execution_count="67" scrolled="false">

``` python
bash% gridlabd geodata help | head -n 10
```

<div class="output stream stdout">

    Help on module __main__:
    
    NAME
        __main__ - Syntax: gridlabd geodata OPTIONS DIRECTIVE [ARGUMENTS]
    
    DESCRIPTION
        The geodata command gathers and joins geographic data. The geodata subcommand
        uses directives that are documented in the DIRECTIVES section below.
        
        In general geodata is used to acquire geographic information at a location or

</div>

</div>

<div class="cell code" execution_count="68">

``` python
bash% gridlabd geodata help distance | head -n 10
```

<div class="output stream stdout">

    Help on module geodata_distance:
    
    NAME
        geodata_distance - GridLAB-D Geodata Distance Package
    
    DESCRIPTION
        The distance package computes the shortest distance between consecutive
        positions.
        
        OPTIONS

</div>

</div>

<div class="cell markdown">

------------------------------------------------------------------------

# Output Options

</div>

<div class="cell markdown">

## Verbose

</div>

<div class="cell markdown">

The `-v` or `--verbose` option can be used to get additional output
about the stages of processing, e.g.,

</div>

<div class="cell code" execution_count="69">

``` python
bash% gridlabd geodata merge -D distance 37,-122 38,-122 -v --units=km --precision.distance=3
```

<div class="output stream stdout">

    VERBOSE [geodata/distance]: verbose output enabled
    VERBOSE [geodata/distance]: arg=37,-122 --> append 1 rows from args
    VERBOSE [geodata/distance]: arg=38,-122 --> append 1 rows from args
    VERBOSE [geodata/distance]: arg=--units=km --> option ['units', 'km'] accepted
    VERBOSE [geodata/distance]: arg=--precision.distance=3 --> option ['precision.distance', '3'] accepted
    VERBOSE [geodata/distance]: 2 rows total received
    VERBOSE [geodata/distance]: creating row index
    VERBOSE [geodata/distance]: data = 
    --------------------------
       latitude  longitude  id
    0      37.0     -122.0   0
    1      38.0     -122.0   1
    --------------------------
    
    VERBOSE [geodata/distance]: options = {
        "verbose": true,
        "debug": false,
        "silent": false,
        "quiet": false,
        "warning": true,
        "key_index": "",
        "max_threads": 1,
        "config_file": "geodata.conf",
        "field_separator": " ",
        "record_separator": "\n",
        "input_delimiter": "+",
        "warning_as_error": false,
        "plot": {
            "figsize": [
                7.0,
                5.0
            ],
            "cmap": "",
            "categorical": false,
            "legend": false,
            "scheme": "",
            "k": 5,
            "vmin": NaN,
            "vmax": NaN,
            "aspect": "auto"
        },
        "show": {
            "block": true
        },
        "savefig": {
            "dpi": 300,
            "facecolor": "w",
            "edgecolor": "k",
            "orientation": "landscape",
            "format": "png",
            "transparent": false,
            "pad_inches": 0.1
        },
        "table": {
            "max_rows": 10,
            "max_columns": 10,
            "width": 80,
            "max_colwidth": 16
        },
        "precision": {
            "distance": 3,
            "heading": 1,
            "geolocation": 5,
            "id": 0,
            "resolution_id": 3
        },
        "resolution": 0,
        "resolution_id": false,
        "json": {
            "orient": "index",
            "data_format": "iso",
            "double_precision": 10,
            "force_ascii": true,
            "date_unit": "s"
        },
        "units": "km",
        "relative": false
    }
    VERBOSE [geodata/distance]: config = {
        "geodata_url": "http://geodata.gridlabd.us/",
        "output_format": "CSV",
        "path_join": "outer",
        "column_names": {
            "ID": "id",
            "UUID": "uuid",
            "LAT": "latitude",
            "LON": "longitude",
            "DIST": "distance",
            "HEAD": "heading",
            "LOC": "location",
            "POS": "position"
        },
        "uuid_type": 4,
        "vegetation.username": "name@example.com",
        "vegetation.password": "password5839",
        "method": "haversine"
    }
    id,latitude,longitude,distance
    0,37.0,-122.0,0.0
    1,38.0,-122.0,111.195

</div>

</div>

<div class="cell markdown">

## Silent

</div>

<div class="cell markdown">

The `-s` or `--silent` option suppresses error messages while still
returning an error condition, e.g.,

</div>

<div class="cell code" execution_count="70">

``` python
bash% gridlabd geodata merge -D distance 37,-122 38,-122 --badoption -s || echo "Exit code $?"
```

<div class="output stream stdout">

    Exit code 4

</div>

</div>

<div class="cell markdown">

## Warning

</div>

<div class="cell markdown">

The `-w` or `--warning` option suppresses warning messages, e.g.,

</div>

<div class="cell code" execution_count="71">

``` python
bash% gridlabd geodata merge -D distance 37,-122 38,-122 -r 100 -k location -w -f TABLE
```

<div class="output stream stdout">

                  latitude  longitude  id  distance
    location                                       
    9q94rzdk9gyt      37.0     -122.0   0       0.0
    9qc0pvdq1cqt      38.0     -122.0   1  111195.0

</div>

</div>

<div class="cell markdown">

If an error is desired instead of a warning, use the
`--warning_as_error` option, e.g.,

</div>

<div class="cell code" execution_count="72">

``` python
bash% gridlabd geodata merge -D distance 37,-122 38,-122 -r 100 -k location --warning_as_error # key warning is an error
```

<div class="output stream stdout">

    ERROR [geodata/distance]: using index 'location' cannot be used with '--resolution=100.0' option

</div>

</div>

<div class="cell markdown">

## Debug

</div>

<div class="cell markdown">

The `-d` or `--debug` option can be used to obtain details about why an
error occurred. For example, the following command has an invalid
`lat,lon` tuple and results in a simple error message:

</div>

<div class="cell code" execution_count="73">

``` python
bash% gridlabd geodata merge -D distance 37,-122 38,-122 --units=furlongs
```

<div class="output stream stdout">

    ERROR [geodata/distance]: unit 'furlongs' is not recognized

</div>

</div>

<div class="cell markdown">

Using the debug option provides a more detailed traceback of the error
that can be helpful in diagnostic the problem, particularly when it
originates in a dataset package. The call arguments, options, and
configuration is also output after the traceback information.

</div>

<div class="cell code" execution_count="74">

``` python
bash% gridlabd geodata merge -D distance 37,-122 38,-122 --units=furlongs -d # invalid unit
```

<div class="output stream stdout">

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.21-210608-develop_add_vegetation_geodata/bin/gridlabd-geodata", line 849, in merge
        result = MODULE.apply(data,options,config,warning)
      File "/usr/local/opt/gridlabd/4.2.21-210608-develop_add_vegetation_geodata/share/gridlabd/geodata_distance.py", line 119, in apply
        raise Exception(f"unit '{options['units']}' is not recognized")
    Exception: unit 'furlongs' is not recognized
    DEBUG [geodata/distance]: merge(args=['37,-122', '38,-122', '--units=furlongs']) --> data = 
    ----
    None
    ----
    options = {
        "verbose": false,
        "debug": true,
        "silent": false,
        "quiet": false,
        "warning": true,
        "key_index": "",
        "max_threads": 1,
        "config_file": "geodata.conf",
        "field_separator": " ",
        "record_separator": "\n",
        "input_delimiter": "+",
        "warning_as_error": false,
        "plot": {
            "figsize": [
                7.0,
                5.0
            ],
            "cmap": "",
            "categorical": false,
            "legend": false,
            "scheme": "",
            "k": 5,
            "vmin": NaN,
            "vmax": NaN,
            "aspect": "auto"
        },
        "show": {
            "block": true
        },
        "savefig": {
            "dpi": 300,
            "facecolor": "w",
            "edgecolor": "k",
            "orientation": "landscape",
            "format": "png",
            "transparent": false,
            "pad_inches": 0.1
        },
        "table": {
            "max_rows": 10,
            "max_columns": 10,
            "width": 80,
            "max_colwidth": 16
        },
        "precision": {
            "distance": 0,
            "heading": 1,
            "geolocation": 5,
            "id": 0,
            "resolution_id": 3
        },
        "resolution": 0,
        "resolution_id": false,
        "json": {
            "orient": "index",
            "data_format": "iso",
            "double_precision": 10,
            "force_ascii": true,
            "date_unit": "s"
        },
        "units": "furlongs",
        "relative": false
    }
    config = {
        "geodata_url": "http://geodata.gridlabd.us/",
        "output_format": "CSV",
        "path_join": "outer",
        "column_names": {
            "ID": "id",
            "UUID": "uuid",
            "LAT": "latitude",
            "LON": "longitude",
            "DIST": "distance",
            "HEAD": "heading",
            "LOC": "location",
            "POS": "position"
        },
        "uuid_type": 4,
        "vegetation.username": "name@example.com",
        "vegetation.password": "password5839",
        "method": "haversine"
    }

</div>

</div>

<div class="cell markdown">

Note that using the debug flag also enables traceback of warnings from
inside python modules, but may not traceback warnings from geodata
packages. If this occurs, use the `--warning_as_error` option.

</div>

<div class="cell markdown">

# Configurations versus Options

</div>

<div class="cell markdown">

Configurations can be set in advance using configuration files and the
`config` directive, or they can be specified at the command line.
Options can only be specified on the command line.

The current `geodata` configuration values are obtained using the
`--show_config` command line option:

</div>

<div class="cell code" execution_count="75">

``` python
bash% gridlabd geodata --show_config
```

<div class="output stream stdout">

    --geodata_url='http://geodata.gridlabd.us/'
    --output_format='CSV'
    --path_join='outer'
    --column_names.ID='id'
    --column_names.UUID='uuid'
    --column_names.LAT='latitude'
    --column_names.LON='longitude'
    --column_names.DIST='distance'
    --column_names.HEAD='heading'
    --column_names.LOC='location'
    --column_names.POS='position'
    --uuid_type=4
    --vegetation.username='name@example.com'
    --vegetation.password='password5839'

</div>

</div>

<div class="cell markdown">

This list will always include all the `system`, `user`, and `local`
configuration items.

</div>

<div class="cell markdown">

The valid `geodata` option values are obtained using a similar command
for options, e.g.,

</div>

<div class="cell code" execution_count="76">

``` python
bash% gridlabd geodata --show_options
```

<div class="output stream stdout">

    --verbose=False
    --debug=False
    --silent=False
    --quiet=False
    --warning=True
    --key_index=''
    --max_threads=1
    --config_file='geodata.conf'
    --field_separator=' '
    --record_separator='\n'
    --input_delimiter='+'
    --warning_as_error=False
    --plot.figsize=7.0.5.0
    --plot.cmap=''
    --plot.categorical=False
    --plot.legend=False
    --plot.scheme=''
    --plot.k=5
    --plot.vmin=nan
    --plot.vmax=nan
    --plot.aspect='auto'
    --show.block=True
    --savefig.dpi=300
    --savefig.facecolor='w'
    --savefig.edgecolor='k'
    --savefig.orientation='landscape'
    --savefig.format='png'
    --savefig.transparent=False
    --savefig.pad_inches=0.1
    --table.max_rows=10
    --table.max_columns=10
    --table.width=80
    --table.max_colwidth=16
    --precision.distance=0
    --precision.heading=1
    --precision.geolocation=5
    --precision.id=0
    --precision.resolution_id=3
    --resolution=0
    --resolution_id=False
    --json.orient='index'
    --json.data_format='iso'
    --json.double_precision=10
    --json.force_ascii=True
    --json.date_unit='s'

</div>

</div>

<div class="cell markdown">

------------------------------------------------------------------------

# GridLAB-D Models

</div>

<div class="cell markdown">

The `geodata` subcommand can be used from within GridLAB-D model files
(GLM) files. All GridLAB-D objects have a header attribute for latitude
and longitude, and thus can provide the data needed to perform `geodata`
create and merge operations.

The following step-by-step example illustrates how address resolution
can be performed on a GLM object.

</div>

<div class="cell markdown">

## Step 1: Add the needed geodata fields to the affected object classes

</div>

<div class="cell markdown">

    module residential;
    class house
    {
        char1024 address;
    }

</div>

<div class="cell markdown">

The `class` declaration adds the `address` field to the definition of
`house` provided by the `residential` module.

</div>

<div class="cell markdown">

## Step 2: Create the needed objects with the required data

</div>

<div class="cell markdown">

    object 
    {
        address "2575 Sand Hill Rd., Menlo Park, CA, 94025";
    }

</div>

<div class="cell markdown">

## Step 3: Write the CSV data file needed to create the `geodata` entity

</div>

<div class="cell markdown">

    #write /tmp/house_address.csv house:address

</div>

<div class="cell markdown">

The `#write` macro generates a CSV field of all the `house` objects with
only the `address` fields, which looks like this:

</div>

<div class="cell markdown">

    id,class,name,address
    0,house,"house:0","2575 Sand Hill Rd., Menlo Park, CA"

</div>

<div class="cell markdown">

## Step 4: Run the desired `geodata` operation

</div>

<div class="cell markdown">

    #geodata merge -D address /tmp/house_address.csv --reverse --format GLM:@latitude,longitude -o /tmp/house_latlon.glm

</div>

<div class="cell markdown">

The command merges the `address` dataset into the data from the CSV
file, performing a reverse address resolution (i.e., address to
location), and outputs the result to a GLM file. The `GLM` format
options specified only the `latitude` and `longitude` fields are to be
generated. The `@` symbol reformats the GLM output using `modify`
directives instead of the default `object` directives. This is necessary
to update existing objects rather than creating new ones. The resulting
file looks like this:

</div>

<div class="cell markdown">

    modify house:0.longitude "-122.20118";
    modify house:0.latitude "37.41546";

</div>

<div class="cell markdown">

## Step 5: Include the updated values

</div>

<div class="cell markdown">

    #include /tmp/house_latlon.glm

</div>

<div class="cell markdown">

Now the object has the latitude and longitude data set based on the
address provided.

</div>

<div class="cell markdown">

## Example 1

The following example illustrates the result of using this method to
resolve the geolocation of a house using its address.

</div>

<div class="cell code" execution_count="77">

``` python
bash% cat example.glm
```

<div class="output stream stdout">

    module residential;
    class house
    {
        char1024 address;
    }
    
    object house
    {
        name "Building_53";
        address "2575 Sand Hill Rd., Menlo Park, CA";
    }
    #write /tmp/address.csv house:address
    #geodata merge -D address /tmp/address.csv --reverse --format GLM:@latitude,longitude -o /tmp/latlon.glm
    #include "/tmp/latlon.glm"

</div>

</div>

<div class="cell code" execution_count="78">

``` python
bash% gridlabd -I -w example.glm -o /tmp/example.json
bash% gridlabd json-get objects Building_53 </tmp/example.json | awk '/latitude|longitude|address/{print$0;}'
```

<div class="output stream stdout">

        "latitude": "37.415460",
        "longitude": "-122.201180",
        "address": "2575 Sand Hill Rd., Menlo Park, CA",

</div>

</div>

<div class="cell markdown">

# Example 2

The following example illustrates using pipes to chain merge commands.
In this case, the elevation data is generated at 25 meter resolution
along the path, and the result is passed to the distance calculation
which fills in the relative distance from the points in the original
dataset.

</div>

<div class="cell code" execution_count="79" scrolled="false">

``` python
bash% gridlabd geodata merge -D elevation path_example.csv -r 250 \
| gridlabd geodata merge -D distance -f TABLE --relative --table.width=999 --table.max_rows=999
```

<div class="output stream stdout">

          position  latitude  longitude configuration  pole_height  distance  heading  elevation
    id                                                                                          
    0.0          0  37.41505 -122.20565         flat3         18.0       0.0      NaN       88.0
    NaN        250  37.41471 -122.20845           NaN          NaN     250.0    263.0      100.0
    1.0        254  37.41470 -122.20849         sideT         20.0       0.0    263.0      100.0
    2.0        428  37.41445 -122.21044         sideT         21.0       0.0    262.7      111.0
    3.0        595  37.41422 -122.21231         sideT         18.0       0.0    263.0      110.0
    4.0        771  37.41398 -122.21427         sideT         20.0       0.0    263.0       98.0
    5.0        976  37.41370 -122.21657         sideT         18.0       0.0    263.1       91.0
    6.0       1128  37.41348 -122.21827         sideT         21.0       0.0    262.6       77.0
    7.0       1326  37.41321 -122.22048         sideT         18.0       0.0    263.0       91.0
    8.0       1490  37.41298 -122.22232         sideT         16.0       0.0    262.9       80.0
    9.0       1690  37.41271 -122.22456         sideT         21.0       0.0    263.1       79.0
    NaN       1940  37.41236 -122.22736           NaN          NaN     250.0    262.9       96.0
    10.0      1941  37.41236 -122.22736         sideT         18.0       0.0    262.9       96.0
    11.0      2123  37.41211 -122.22940         sideT         19.0       0.0    263.0       82.0
    12.0      2341  37.41181 -122.23184         3pole         15.0       0.0    263.0      103.0
    NaN       2591  37.41058 -122.23421           NaN          NaN     250.0    242.5       83.0
    13.0      2718  37.40995 -122.23541         3pole         16.0       0.0    242.5      129.0
    14.0      2941  37.40912 -122.23771         2pole         15.0       0.0    250.2      109.0
    NaN       3191  37.40818 -122.24028           NaN          NaN     250.0    249.9       89.0
    15.0      3268  37.40789 -122.24107         2pole         17.0       0.0    249.9      108.0
    16.0      3466  37.40715 -122.24311         2pole         15.0       0.0    250.1      115.0
    17.0      3653  37.40644 -122.24503         3pole         17.0       0.0    249.7      124.0
    18.0      3759  37.40562 -122.24443         vert3         17.0       0.0    143.8      127.0
    19.0      3878  37.40456 -122.24418         vert3         20.0       0.0    166.7      123.0
    20.0      4009  37.40339 -122.24429         vert3         20.0       0.0    185.4      117.0
    21.0      4115  37.40249 -122.24468         vert3         20.0       0.0    203.4      110.0
    22.0      4267  37.40136 -122.24566         vert3         22.0       0.0    220.9      106.0
    NaN       4517  37.40032 -122.24817           NaN          NaN     250.0    247.6      106.0
    23.0      4627  37.39987 -122.24927         3pole         16.0       0.0    247.6      116.0
    24.0      4793  37.39966 -122.25113         2pole         18.0       0.0    263.6      167.0
    NaN       5043  37.39950 -122.25395           NaN          NaN     250.0    266.8      206.0
    25.0      5248  37.39937 -122.25627         2pole         22.0       0.0    266.8      267.0
    26.0      5397  37.39928 -122.25795         3pole         15.0       0.0    266.9      285.0
    NaN       5647  37.39908 -122.26077           NaN          NaN     250.0    266.0      205.0
    NaN       5897  37.39888 -122.26359           NaN          NaN     500.0    266.0      308.0
    27.0      5977  37.39882 -122.26449         3pole         18.0       0.0    266.0      331.0
    28.0      6119  37.39871 -122.26610         2pole         15.0       0.0    266.1      352.0
    29.0      6250  37.39861 -122.26757         3pole         18.0       0.0    266.1      341.0
    NaN       6500  37.39667 -122.26901           NaN          NaN     250.0    216.5      343.0
    NaN       6750  37.39473 -122.27044           NaN          NaN     500.0    216.5      439.0
    30.0      6783  37.39448 -122.27063         3pole         21.0       0.0    216.5      444.0
    31.0      6971  37.39306 -122.27180         3pole         21.0       0.0    219.5      448.0
    32.0      7215  37.39248 -122.27446         2pole         17.0       0.0    257.7      505.0
    33.0      7463  37.39190 -122.27717         2pole         21.0       0.0    257.9      578.0
    34.0      7593  37.39161 -122.27860         2pole         23.0       0.0    258.5      615.0
    35.0      7840  37.39171 -122.28139         3pole         16.0       0.0    272.1      642.0
    NaN       8090  37.39048 -122.28376           NaN          NaN     250.0    242.6      548.0
    NaN       8340  37.38926 -122.28613           NaN          NaN     500.0    242.6      544.0
    36.0      8583  37.38806 -122.28844         tower         23.0       0.0    242.6      634.0

</div>

</div>

<div class="cell markdown">

# Example 3

Ths following example illustrates merging elevation, vegetation, and
powerline data.

</div>

<div class="cell code" execution_count="84" scrolled="false">

``` python
bash% gridlabd geodata merge -D elevation path_example.csv -r 30 --resolution_id \
| gridlabd geodata merge -D vegetation -w \
| gridlabd geodata merge -D powerline --cable_type="TACSR/AC 610mm^2" --margin.vertical=2 -f TABLE:latitude,longitude,pole_height,elevation,linesag,cover,height,contact --table.width=999 --table.max_rows=999
```

<div class="output stream stdout">

    2021-06-09 09:17:31,801 WARNING cfo.utils [_auth_request] | Authentication failed. Try and run .authenticate(ignore_temp=True)
    2021-06-09 09:17:31,801 WARNING cfo.utils [_auth_request] | b'{}\n'
    2021-06-09 09:17:31,801 WARNING cfo.utils [authenticate] | Authentication failed with status code 401
    id,position,latitude,longitude,configuration,pole_height,distance,heading,elevation,base,cover,height
    0.0,0,37.41505,-122.20565,flat3,18.0,0.0,,88.0,2.0,0.68,14.0
    0.003,30,37.41501,-122.20599,,,30.0,263.0,89.0,2.0,0.73,16.0
    0.006999999999999999,60,37.41497,-122.20632,,,60.0,263.0,89.0,2.0,0.74,16.0
    0.01,90,37.41493,-122.20666,,,90.0,263.0,90.0,2.0,0.72,17.0
    0.013999999999999999,120,37.41488,-122.20699,,,120.0,263.0,91.0,2.0,0.56,17.0
    0.017,150,37.41484,-122.20733,,,150.0,263.0,92.0,2.0,0.3,17.0
    0.021,180,37.4148,-122.20766,,,180.0,263.0,93.0,2.0,0.14,16.0
    0.024,210,37.41476,-122.208,,,210.0,263.0,95.0,2.0,0.07,10.0
    0.027999999999999997,240,37.41472,-122.20834,,,240.0,263.0,98.0,2.0,0.09,1.0
    0.03,254,37.4147,-122.20849,sideT,20.0,254.0,263.0,100.0,2.0,0.09,1.0
    0.033,284,37.41466,-122.20883,,,284.0,262.7,103.0,2.0,0.14,1.0
    0.037000000000000005,314,37.41461,-122.20916,,,314.0,262.7,99.0,2.0,0.16,1.0
    0.04,344,37.41457,-122.2095,,,344.0,262.7,98.0,2.0,0.14,1.0
    0.044000000000000004,374,37.41453,-122.20983,,,374.0,262.7,103.0,2.0,0.11,0.0
    0.047,404,37.41449,-122.21017,,,404.0,262.7,110.0,2.0,0.33,1.0
    0.05,428,37.41445,-122.21044,sideT,21.0,428.0,262.7,111.0,2.0,0.31,1.0
    0.053,458,37.41441,-122.21078,,,458.0,263.0,111.0,2.0,0.3,1.0
    0.057,488,37.41437,-122.21111,,,488.0,263.0,113.0,2.0,0.33,1.0
    0.06,518,37.41433,-122.21145,,,518.0,263.0,115.0,2.0,0.4,1.0
    0.064,548,37.41428,-122.21178,,,548.0,263.0,114.0,2.0,0.44,1.0
    0.067,578,37.41424,-122.21212,,,578.0,263.0,111.0,2.0,0.51,1.0
    0.069,595,37.41422,-122.21231,sideT,18.0,595.0,263.0,110.0,2.0,0.51,1.0
    0.073,625,37.41418,-122.21265,,,625.0,263.0,108.0,2.0,0.69,6.0
    0.076,655,37.41414,-122.21298,,,655.0,263.0,107.0,2.0,0.72,13.0
    0.08,685,37.4141,-122.21332,,,685.0,263.0,105.0,2.0,0.66,15.0
    0.083,715,37.41406,-122.21365,,,715.0,263.0,104.0,2.0,0.62,15.0
    0.087,745,37.41401,-122.21399,,,745.0,263.0,101.0,2.0,0.65,16.0
    0.09,771,37.41398,-122.21427,sideT,20.0,771.0,263.0,98.0,2.0,0.68,17.0
    0.09300000000000001,801,37.41394,-122.21461,,,801.0,263.1,96.0,2.0,0.65,18.0
    0.09699999999999999,831,37.4139,-122.21494,,,831.0,263.1,94.0,2.0,0.52,19.0
    0.1,861,37.41386,-122.21528,,,861.0,263.1,92.0,2.0,0.38,15.0
    0.10400000000000001,891,37.41382,-122.21561,,,891.0,263.1,91.0,2.0,0.2,9.0
    0.107,921,37.41378,-122.21595,,,921.0,263.1,89.0,2.0,0.1,4.0
    0.111,951,37.41373,-122.21628,,,951.0,263.1,90.0,2.0,0.07,2.0
    0.114,976,37.4137,-122.21657,sideT,18.0,976.0,263.1,91.0,2.0,0.06,1.0
    0.11699999999999999,1006,37.41366,-122.21691,,,1006.0,262.6,90.0,2.0,0.06,0.0
    0.121,1036,37.41361,-122.21724,,,1036.0,262.6,87.0,2.0,0.1,0.0
    0.124,1066,37.41357,-122.21758,,,1066.0,262.6,83.0,2.0,0.11,1.0
    0.128,1096,37.41353,-122.21791,,,1096.0,262.6,80.0,2.0,0.29,3.0
    0.131,1126,37.41348,-122.21825,,,1126.0,262.6,77.0,2.0,0.5,4.0
    0.131,1128,37.41348,-122.21827,sideT,21.0,1128.0,262.6,77.0,2.0,0.5,4.0
    0.135,1158,37.41344,-122.21861,,,1158.0,263.0,76.0,2.0,0.67,7.0
    0.138,1188,37.4134,-122.21894,,,1188.0,263.0,79.0,2.0,0.71,10.0
    0.142,1218,37.41336,-122.21928,,,1218.0,263.0,82.0,2.0,0.67,13.0
    0.145,1248,37.41332,-122.21961,,,1248.0,263.0,85.0,2.0,0.59,15.0
    0.149,1278,37.41327,-122.21995,,,1278.0,263.0,89.0,2.0,0.54,16.0
    0.152,1308,37.41323,-122.22028,,,1308.0,263.0,91.0,2.0,0.6,16.0
    0.154,1326,37.41321,-122.22048,sideT,18.0,1326.0,263.0,91.0,2.0,0.68,16.0
    0.158,1356,37.41317,-122.22082,,,1356.0,262.9,92.0,2.0,0.73,16.0
    0.161,1386,37.41313,-122.22115,,,1386.0,262.9,89.0,2.0,0.74,17.0
    0.165,1416,37.41308,-122.22149,,,1416.0,262.9,84.0,2.0,0.71,17.0
    0.168,1446,37.41304,-122.22182,,,1446.0,262.9,82.0,2.0,0.7,15.0
    0.172,1476,37.413,-122.22216,,,1476.0,262.9,81.0,2.0,0.68,13.0
    0.174,1490,37.41298,-122.22232,sideT,16.0,1490.0,262.9,80.0,2.0,0.62,12.0
    0.177,1520,37.41294,-122.22266,,,1520.0,263.1,78.0,2.0,0.57,11.0
    0.18100000000000002,1550,37.4129,-122.22299,,,1550.0,263.1,77.0,2.0,0.42,9.0
    0.184,1580,37.41286,-122.22333,,,1580.0,263.1,73.0,2.0,0.25,5.0
    0.188,1610,37.41282,-122.22366,,,1610.0,263.1,74.0,2.0,0.12,3.0
    0.191,1640,37.41278,-122.224,,,1640.0,263.1,78.0,2.0,0.08,1.0
    0.195,1670,37.41274,-122.22433,,,1670.0,263.1,79.0,1.0,0.06,1.0
    0.19699999999999998,1690,37.41271,-122.22456,sideT,21.0,1690.0,263.1,79.0,1.0,0.04,1.0
    0.2,1720,37.41267,-122.2249,,,1720.0,262.9,79.0,1.0,0.02,0.0
    0.204,1750,37.41263,-122.22523,,,1750.0,262.9,78.0,1.0,0.0,0.0
    0.207,1780,37.41258,-122.22557,,,1780.0,262.9,74.0,1.0,0.01,0.0
    0.21100000000000002,1810,37.41254,-122.2259,,,1810.0,262.9,74.0,1.0,0.02,0.0
    0.214,1840,37.4125,-122.22624,,,1840.0,262.9,79.0,1.0,0.06,1.0
    0.218,1870,37.41246,-122.22657,,,1870.0,262.9,86.0,1.0,0.1,1.0
    0.221,1900,37.41242,-122.22691,,,1900.0,262.9,93.0,2.0,0.19,2.0
    0.225,1930,37.41237,-122.22724,,,1930.0,262.9,95.0,2.0,0.29,4.0
    0.226,1941,37.41236,-122.22736,sideT,18.0,1941.0,262.9,96.0,2.0,0.29,4.0
    0.23,1971,37.41232,-122.2277,,,1971.0,263.0,95.0,2.0,0.34,2.0
    0.233,2001,37.41228,-122.22803,,,2001.0,263.0,92.0,2.0,0.41,3.0
    0.237,2031,37.41224,-122.22837,,,2031.0,263.0,87.0,2.0,0.37,4.0
    0.24,2061,37.4122,-122.2287,,,2061.0,263.0,82.0,2.0,0.26,4.0
    0.244,2091,37.41215,-122.22904,,,2091.0,263.0,83.0,2.0,0.09,2.0
    0.247,2121,37.41211,-122.22937,,,2121.0,263.0,82.0,2.0,0.07,2.0
    0.247,2123,37.41211,-122.2294,sideT,19.0,2123.0,263.0,82.0,2.0,0.07,2.0
    0.251,2153,37.41207,-122.22974,,,2153.0,263.0,82.0,2.0,0.05,1.0
    0.254,2183,37.41203,-122.23007,,,2183.0,263.0,83.0,1.0,0.07,3.0
    0.258,2213,37.41199,-122.23041,,,2213.0,263.0,84.0,1.0,0.1,3.0
    0.261,2243,37.41194,-122.23074,,,2243.0,263.0,85.0,2.0,0.13,4.0
    0.265,2273,37.4119,-122.23108,,,2273.0,263.0,89.0,2.0,0.12,4.0
    0.268,2303,37.41186,-122.23141,,,2303.0,263.0,99.0,2.0,0.11,4.0
    0.272,2333,37.41182,-122.23175,,,2333.0,263.0,103.0,2.0,0.13,2.0
    0.273,2341,37.41181,-122.23184,3pole,15.0,2341.0,263.0,103.0,2.0,0.15,1.0
    0.276,2371,37.41166,-122.23212,,,2371.0,242.5,102.0,2.0,0.07,1.0
    0.28,2401,37.41151,-122.23241,,,2401.0,242.5,96.0,2.0,0.08,1.0
    0.28300000000000003,2431,37.41137,-122.23269,,,2431.0,242.5,94.0,2.0,0.04,1.0
    0.287,2461,37.41122,-122.23298,,,2461.0,242.5,95.0,2.0,0.04,0.0
    0.29,2491,37.41107,-122.23326,,,2491.0,242.5,97.0,2.0,0.04,1.0
    0.294,2521,37.41092,-122.23354,,,2521.0,242.5,96.0,1.0,0.04,1.0
    0.297,2551,37.41077,-122.23383,,,2551.0,242.5,92.0,1.0,0.14,4.0
    0.301,2581,37.41063,-122.23411,,,2581.0,242.5,85.0,1.0,0.18,5.0
    0.304,2611,37.41048,-122.2344,,,2611.0,242.5,81.0,1.0,0.49,9.0
    0.308,2641,37.41033,-122.23468,,,2641.0,242.5,92.0,1.0,0.47,8.0
    0.311,2671,37.41018,-122.23496,,,2671.0,242.5,109.0,1.0,0.48,6.0
    0.315,2701,37.41003,-122.23525,,,2701.0,242.5,126.0,1.0,0.4,6.0
    0.317,2718,37.40995,-122.23541,3pole,16.0,2718.0,242.5,129.0,1.0,0.4,6.0
    0.32,2748,37.40984,-122.23572,,,2748.0,250.2,127.0,1.0,0.18,2.0
    0.324,2778,37.40973,-122.23603,,,2778.0,250.2,119.0,1.0,0.1,2.0
    0.327,2808,37.40962,-122.23634,,,2808.0,250.2,110.0,1.0,0.08,1.0
    0.331,2838,37.4095,-122.23665,,,2838.0,250.2,105.0,1.0,0.09,1.0
    0.33399999999999996,2868,37.40939,-122.23696,,,2868.0,250.2,104.0,1.0,0.14,1.0
    0.33799999999999997,2898,37.40928,-122.23727,,,2898.0,250.2,103.0,1.0,0.28,2.0
    0.341,2928,37.40917,-122.23757,,,2928.0,250.2,107.0,1.0,0.26,2.0
    0.34299999999999997,2941,37.40912,-122.23771,2pole,15.0,2941.0,250.2,109.0,1.0,0.42,5.0
    0.34600000000000003,2971,37.40901,-122.23802,,,2971.0,249.9,107.0,1.0,0.53,6.0
    0.35,3001,37.40889,-122.23833,,,3001.0,249.9,98.0,1.0,0.28,5.0
    0.353,3031,37.40878,-122.23864,,,3031.0,249.9,89.0,1.0,0.32,7.0
    0.35700000000000004,3061,37.40867,-122.23894,,,3061.0,249.9,87.0,2.0,0.42,11.0
    0.36,3091,37.40856,-122.23925,,,3091.0,249.9,86.0,2.0,0.3,11.0
    0.364,3121,37.40844,-122.23956,,,3121.0,249.9,85.0,2.0,0.26,9.0
    0.36700000000000005,3151,37.40833,-122.23987,,,3151.0,249.9,87.0,2.0,0.16,7.0
    0.371,3181,37.40822,-122.24018,,,3181.0,249.9,88.0,2.0,0.15,4.0
    0.374,3211,37.4081,-122.24049,,,3211.0,249.9,97.0,2.0,0.16,2.0
    0.37799999999999995,3241,37.40799,-122.24079,,,3241.0,249.9,105.0,2.0,0.19,1.0
    0.381,3268,37.40789,-122.24107,2pole,17.0,3268.0,249.9,108.0,2.0,0.14,1.0
    0.384,3298,37.40778,-122.24138,,,3298.0,250.1,109.0,2.0,0.09,1.0
    0.38799999999999996,3328,37.40767,-122.24169,,,3328.0,250.1,109.0,1.0,0.09,1.0
    0.391,3358,37.40755,-122.242,,,3358.0,250.1,111.0,1.0,0.11,1.0
    0.395,3388,37.40744,-122.24231,,,3388.0,250.1,111.0,1.0,0.17,1.0
    0.39799999999999996,3418,37.40733,-122.24261,,,3418.0,250.1,113.0,0.0,0.28,1.0
    0.402,3448,37.40722,-122.24292,,,3448.0,250.1,115.0,0.0,0.32,1.0
    0.40399999999999997,3466,37.40715,-122.24311,2pole,15.0,3466.0,250.1,115.0,0.0,0.32,1.0
    0.40700000000000003,3496,37.40704,-122.24342,,,3496.0,249.7,115.0,0.0,0.32,1.0
    0.41100000000000003,3526,37.40692,-122.24373,,,3526.0,249.7,117.0,1.0,0.33,1.0
    0.414,3556,37.40681,-122.24403,,,3556.0,249.7,119.0,1.0,0.22,1.0
    0.418,3586,37.40669,-122.24434,,,3586.0,249.7,120.0,1.0,0.16,1.0
    0.42100000000000004,3616,37.40658,-122.24465,,,3616.0,249.7,123.0,1.0,0.09,1.0
    0.425,3646,37.40647,-122.24496,,,3646.0,249.7,124.0,1.0,0.09,1.0
    0.426,3653,37.40644,-122.24503,3pole,17.0,3653.0,249.7,124.0,1.0,0.09,1.0
    0.429,3683,37.40621,-122.24486,,,3683.0,143.8,127.0,1.0,0.09,1.0
    0.433,3713,37.40597,-122.24469,,,3713.0,143.8,128.0,1.0,0.15,1.0
    0.436,3743,37.40574,-122.24452,,,3743.0,143.8,127.0,1.0,0.12,1.0
    0.43799999999999994,3759,37.40562,-122.24443,vert3,17.0,3759.0,143.8,127.0,1.0,0.12,1.0
    0.441,3789,37.40535,-122.24437,,,3789.0,166.7,126.0,1.0,0.08,2.0
    0.445,3819,37.40509,-122.2443,,,3819.0,166.7,124.0,1.0,0.08,2.0
    0.44799999999999995,3849,37.40482,-122.24424,,,3849.0,166.7,123.0,1.0,0.11,2.0
    0.452,3878,37.40456,-122.24418,vert3,20.0,3878.0,166.7,123.0,2.0,0.11,2.0
    0.455,3908,37.40429,-122.24421,,,3908.0,185.4,122.0,2.0,0.15,2.0
    0.45899999999999996,3938,37.40402,-122.24423,,,3938.0,185.4,120.0,2.0,0.15,2.0
    0.462,3968,37.40375,-122.24426,,,3968.0,185.4,118.0,2.0,0.23,3.0
    0.466,3998,37.40348,-122.24428,,,3998.0,185.4,117.0,2.0,0.31,4.0
    0.467,4009,37.40339,-122.24429,vert3,20.0,4009.0,185.4,117.0,2.0,0.31,3.0
    0.47100000000000003,4039,37.40313,-122.2444,,,4039.0,203.4,116.0,2.0,0.31,3.0
    0.474,4069,37.40288,-122.24451,,,4069.0,203.4,115.0,2.0,0.22,1.0
    0.478,4099,37.40262,-122.24462,,,4099.0,203.4,111.0,2.0,0.16,1.0
    0.479,4115,37.40249,-122.24468,vert3,20.0,4115.0,203.4,110.0,2.0,0.16,1.0
    0.483,4145,37.40227,-122.24487,,,4145.0,220.9,108.0,2.0,0.09,2.0
    0.486,4175,37.40205,-122.24507,,,4175.0,220.9,108.0,2.0,0.08,1.0
    0.49,4205,37.40182,-122.24526,,,4205.0,220.9,108.0,3.0,0.09,1.0
    0.493,4235,37.4016,-122.24545,,,4235.0,220.9,107.0,3.0,0.1,1.0
    0.49700000000000005,4265,37.40138,-122.24564,,,4265.0,220.9,106.0,3.0,0.15,1.0
    0.49700000000000005,4267,37.40136,-122.24566,vert3,22.0,4267.0,220.9,106.0,3.0,0.15,1.0
    0.501,4297,37.40124,-122.24596,,,4297.0,247.6,106.0,3.0,0.16,1.0
    0.504,4327,37.40111,-122.24626,,,4327.0,247.6,106.0,3.0,0.18,1.0
    0.508,4357,37.40099,-122.24656,,,4357.0,247.6,106.0,2.0,0.15,1.0
    0.511,4387,37.40086,-122.24687,,,4387.0,247.6,106.0,3.0,0.2,1.0
    0.515,4417,37.40074,-122.24717,,,4417.0,247.6,106.0,3.0,0.21,1.0
    0.518,4447,37.40061,-122.24747,,,4447.0,247.6,106.0,2.0,0.28,2.0
    0.522,4477,37.40049,-122.24777,,,4477.0,247.6,106.0,2.0,0.37,3.0
    0.525,4507,37.40036,-122.24807,,,4507.0,247.6,106.0,2.0,0.49,4.0
    0.529,4537,37.40024,-122.24837,,,4537.0,247.6,106.0,2.0,0.54,4.0
    0.532,4567,37.40012,-122.24867,,,4567.0,247.6,108.0,2.0,0.59,5.0
    0.536,4597,37.39999,-122.24898,,,4597.0,247.6,110.0,0.0,0.35,10.0
    0.539,4627,37.39987,-122.24927,3pole,16.0,4627.0,247.6,116.0,0.0,0.36,11.0
    0.5429999999999999,4657,37.39983,-122.24961,,,4657.0,263.6,123.0,0.0,0.39,10.0
    0.546,4687,37.39979,-122.24994,,,4687.0,263.6,134.0,0.0,0.43,10.0
    0.55,4717,37.39976,-122.25028,,,4717.0,263.6,146.0,0.0,0.47,9.0
    0.5529999999999999,4747,37.39972,-122.25061,,,4747.0,263.6,156.0,0.0,0.49,8.0
    0.557,4777,37.39968,-122.25095,,,4777.0,263.6,164.0,1.0,0.51,7.0
    0.5579999999999999,4793,37.39966,-122.25113,2pole,18.0,4793.0,263.6,167.0,1.0,0.51,7.0
    0.562,4823,37.39964,-122.25147,,,4823.0,266.8,166.0,1.0,0.49,6.0
    0.565,4853,37.39962,-122.25181,,,4853.0,266.8,163.0,1.0,0.46,5.0
    0.569,4883,37.3996,-122.25215,,,4883.0,266.8,166.0,1.0,0.43,5.0
    0.5720000000000001,4913,37.39958,-122.25249,,,4913.0,266.8,171.0,1.0,0.47,7.0
    0.5760000000000001,4943,37.39956,-122.25282,,,4943.0,266.8,178.0,1.0,0.52,7.0
    0.579,4973,37.39955,-122.25316,,,4973.0,266.8,186.0,1.0,0.55,7.0
    0.583,5003,37.39953,-122.2535,,,5003.0,266.8,194.0,1.0,0.59,7.0
    0.586,5033,37.39951,-122.25384,,,5033.0,266.8,203.0,1.0,0.65,8.0
    0.59,5063,37.39949,-122.25418,,,5063.0,266.8,210.0,1.0,0.71,9.0
    0.593,5093,37.39947,-122.25452,,,5093.0,266.8,219.0,1.0,0.73,11.0
    0.597,5123,37.39945,-122.25486,,,5123.0,266.8,227.0,1.0,0.69,10.0
    0.6,5153,37.39943,-122.2552,,,5153.0,266.8,233.0,1.0,0.6,9.0
    0.604,5183,37.39941,-122.25553,,,5183.0,266.8,244.0,1.0,0.54,6.0
    0.607,5213,37.39939,-122.25587,,,5213.0,266.8,256.0,1.0,0.42,6.0
    0.611,5243,37.39937,-122.25621,,,5243.0,266.8,265.0,1.0,0.31,8.0
    0.611,5248,37.39937,-122.25627,2pole,22.0,5248.0,266.8,267.0,1.0,0.31,8.0
    0.615,5278,37.39935,-122.25661,,,5278.0,266.9,274.0,1.0,0.29,9.0
    0.618,5308,37.39933,-122.25695,,,5308.0,266.9,281.0,1.0,0.31,9.0
    0.622,5338,37.39932,-122.25729,,,5338.0,266.9,284.0,1.0,0.32,10.0
    0.625,5368,37.3993,-122.25763,,,5368.0,266.9,284.0,1.0,0.3,11.0
    0.629,5397,37.39928,-122.25795,3pole,15.0,5397.0,266.9,285.0,1.0,0.27,11.0
    0.632,5427,37.39926,-122.25829,,,5427.0,266.0,283.0,1.0,0.26,10.0
    0.636,5457,37.39923,-122.25863,,,5457.0,266.0,274.0,1.0,0.24,10.0
    0.639,5487,37.39921,-122.25896,,,5487.0,266.0,262.0,1.0,0.24,12.0
    0.643,5517,37.39918,-122.2593,,,5517.0,266.0,248.0,1.0,0.25,10.0
    0.6459999999999999,5547,37.39916,-122.25964,,,5547.0,266.0,235.0,1.0,0.26,8.0
    0.65,5577,37.39914,-122.25998,,,5577.0,266.0,222.0,1.0,0.29,6.0
    0.653,5607,37.39911,-122.26032,,,5607.0,266.0,213.0,1.0,0.34,5.0
    0.657,5637,37.39909,-122.26066,,,5637.0,266.0,207.0,1.0,0.38,7.0
    0.66,5667,37.39907,-122.26099,,,5667.0,266.0,203.0,1.0,0.4,17.0
    0.664,5697,37.39904,-122.26133,,,5697.0,266.0,205.0,1.0,0.41,21.0
    0.667,5727,37.39902,-122.26167,,,5727.0,266.0,224.0,1.0,0.46,22.0
    0.6709999999999999,5757,37.39899,-122.26201,,,5757.0,266.0,247.0,1.0,0.54,21.0
    0.674,5787,37.39897,-122.26235,,,5787.0,266.0,262.0,1.0,0.66,21.0
    0.6779999999999999,5817,37.39895,-122.26269,,,5817.0,266.0,274.0,1.0,0.7,20.0
    0.6809999999999999,5847,37.39892,-122.26302,,,5847.0,266.0,285.0,2.0,0.73,17.0
    0.685,5877,37.3989,-122.26336,,,5877.0,266.0,296.0,2.0,0.76,16.0
    0.688,5907,37.39888,-122.2637,,,5907.0,266.0,312.0,2.0,0.8,16.0
    0.6920000000000001,5937,37.39885,-122.26404,,,5937.0,266.0,322.0,2.0,0.82,19.0
    0.695,5967,37.39883,-122.26438,,,5967.0,266.0,328.0,2.0,0.79,21.0
    0.696,5977,37.39882,-122.26449,3pole,18.0,5977.0,266.0,331.0,2.0,0.79,21.0
    0.7,6007,37.3988,-122.26483,,,6007.0,266.1,339.0,2.0,0.68,19.0
    0.703,6037,37.39877,-122.26517,,,6037.0,266.1,346.0,2.0,0.64,16.0
    0.7070000000000001,6067,37.39875,-122.26551,,,6067.0,266.1,350.0,2.0,0.58,12.0
    0.71,6097,37.39873,-122.26584,,,6097.0,266.1,352.0,2.0,0.53,11.0
    0.713,6119,37.39871,-122.2661,2pole,15.0,6119.0,266.1,352.0,2.0,0.52,12.0
    0.716,6149,37.39869,-122.26644,,,6149.0,266.1,352.0,2.0,0.55,13.0
    0.72,6179,37.39866,-122.26678,,,6179.0,266.1,351.0,2.0,0.59,15.0
    0.723,6209,37.39864,-122.26712,,,6209.0,266.1,348.0,2.0,0.6,15.0
    0.727,6239,37.39862,-122.26745,,,6239.0,266.1,343.0,2.0,0.6,14.0
    0.728,6250,37.39861,-122.26757,3pole,18.0,6250.0,266.1,341.0,2.0,0.6,14.0
    0.732,6280,37.39838,-122.26774,,,6280.0,216.5,323.0,1.0,0.57,10.0
    0.735,6310,37.39814,-122.26791,,,6310.0,216.5,309.0,1.0,0.48,8.0
    0.7390000000000001,6340,37.39791,-122.26809,,,6340.0,216.5,300.0,1.0,0.48,8.0
    0.742,6370,37.39768,-122.26826,,,6370.0,216.5,299.0,1.0,0.47,8.0
    0.746,6400,37.39745,-122.26843,,,6400.0,216.5,305.0,1.0,0.53,7.0
    0.7490000000000001,6430,37.39721,-122.2686,,,6430.0,216.5,317.0,1.0,0.68,13.0
    0.753,6460,37.39698,-122.26878,,,6460.0,216.5,327.0,1.0,0.68,13.0
    0.7559999999999999,6490,37.39675,-122.26895,,,6490.0,216.5,339.0,1.0,0.8,18.0
    0.76,6520,37.39652,-122.26912,,,6520.0,216.5,351.0,1.0,0.79,11.0
    0.763,6550,37.39628,-122.26929,,,6550.0,216.5,361.0,1.0,0.65,6.0
    0.767,6580,37.39605,-122.26946,,,6580.0,216.5,372.0,1.0,0.6,8.0
    0.77,6610,37.39582,-122.26964,,,6610.0,216.5,383.0,1.0,0.44,5.0
    0.774,6640,37.39559,-122.26981,,,6640.0,216.5,395.0,1.0,0.4,9.0
    0.777,6670,37.39535,-122.26998,,,6670.0,216.5,408.0,1.0,0.44,9.0
    0.7809999999999999,6700,37.39512,-122.27015,,,6700.0,216.5,421.0,1.0,0.41,11.0
    0.784,6730,37.39489,-122.27033,,,6730.0,216.5,432.0,1.0,0.45,8.0
    0.7879999999999999,6760,37.39466,-122.2705,,,6760.0,216.5,441.0,1.0,0.45,4.0
    0.79,6783,37.39448,-122.27063,3pole,21.0,6783.0,216.5,444.0,1.0,0.45,4.0
    0.794,6813,37.39425,-122.27082,,,6813.0,219.5,446.0,2.0,0.48,3.0
    0.797,6843,37.39403,-122.271,,,6843.0,219.5,445.0,2.0,0.52,5.0
    0.8009999999999999,6873,37.3938,-122.27119,,,6873.0,219.5,446.0,2.0,0.56,11.0
    0.804,6903,37.39358,-122.27137,,,6903.0,219.5,447.0,2.0,0.55,11.0
    0.8079999999999999,6933,37.39335,-122.27156,,,6933.0,219.5,448.0,2.0,0.68,22.0
    0.8109999999999999,6963,37.39313,-122.27175,,,6963.0,219.5,448.0,2.0,0.79,27.0
    0.812,6971,37.39306,-122.2718,3pole,21.0,6971.0,219.5,448.0,2.0,0.79,27.0
    0.816,7001,37.39299,-122.27213,,,7001.0,257.7,450.0,2.0,0.77,26.0
    0.8190000000000001,7031,37.39292,-122.27245,,,7031.0,257.7,451.0,2.0,0.79,25.0
    0.823,7061,37.39285,-122.27278,,,7061.0,257.7,458.0,2.0,0.81,30.0
    0.826,7091,37.39277,-122.27311,,,7091.0,257.7,466.0,2.0,0.83,33.0
    0.83,7121,37.3927,-122.27344,,,7121.0,257.7,477.0,2.0,0.85,35.0
    0.833,7151,37.39263,-122.27376,,,7151.0,257.7,489.0,2.0,0.86,33.0
    0.8370000000000001,7181,37.39256,-122.27409,,,7181.0,257.7,498.0,1.0,0.85,29.0
    0.84,7211,37.39249,-122.27442,,,7211.0,257.7,504.0,1.0,0.81,25.0
    0.841,7215,37.39248,-122.27446,2pole,17.0,7215.0,257.7,505.0,1.0,0.81,25.0
    0.8440000000000001,7245,37.39241,-122.27479,,,7245.0,257.9,499.0,1.0,0.76,21.0
    0.848,7275,37.39234,-122.27512,,,7275.0,257.9,500.0,1.0,0.73,19.0
    0.851,7305,37.39227,-122.27544,,,7305.0,257.9,514.0,1.0,0.73,18.0
    0.855,7335,37.3922,-122.27577,,,7335.0,257.9,529.0,1.0,0.75,19.0
    0.858,7365,37.39213,-122.2761,,,7365.0,257.9,541.0,1.0,0.77,20.0
    0.862,7395,37.39206,-122.27643,,,7395.0,257.9,555.0,1.0,0.79,22.0
    0.865,7425,37.39199,-122.27676,,,7425.0,257.9,566.0,1.0,0.81,23.0
    0.8690000000000001,7455,37.39192,-122.27708,,,7455.0,257.9,576.0,1.0,0.82,21.0
    0.87,7463,37.3919,-122.27717,2pole,21.0,7463.0,257.9,578.0,1.0,0.84,23.0
    0.873,7493,37.39183,-122.2775,,,7493.0,258.5,586.0,1.0,0.84,24.0
    0.877,7523,37.39177,-122.27783,,,7523.0,258.5,594.0,1.0,0.82,25.0
    0.88,7553,37.3917,-122.27816,,,7553.0,258.5,603.0,1.0,0.81,26.0
    0.883,7583,37.39163,-122.27849,,,7583.0,258.5,613.0,1.0,0.67,20.0
    0.885,7593,37.39161,-122.2786,2pole,23.0,7593.0,258.5,615.0,1.0,0.67,20.0
    0.888,7623,37.39162,-122.27894,,,7623.0,272.1,619.0,1.0,0.67,18.0
    0.892,7653,37.39163,-122.27928,,,7653.0,272.1,620.0,1.0,0.64,15.0
    0.895,7683,37.39165,-122.27962,,,7683.0,272.1,626.0,1.0,0.57,14.0
    0.899,7713,37.39166,-122.27996,,,7713.0,272.1,639.0,1.0,0.48,12.0
    0.902,7743,37.39167,-122.2803,,,7743.0,272.1,643.0,1.0,0.54,13.0
    0.9059999999999999,7773,37.39168,-122.28064,,,7773.0,272.1,643.0,1.0,0.61,15.0
    0.909,7803,37.3917,-122.28097,,,7803.0,272.1,643.0,1.0,0.68,16.0
    0.9129999999999999,7833,37.39171,-122.28131,,,7833.0,272.1,642.0,1.0,0.72,16.0
    0.9129999999999999,7840,37.39171,-122.28139,3pole,16.0,7840.0,272.1,642.0,1.0,0.72,16.0
    0.917,7870,37.39156,-122.28167,,,7870.0,242.6,633.0,1.0,0.79,16.0
    0.92,7900,37.39142,-122.28196,,,7900.0,242.6,621.0,1.0,0.79,16.0
    0.924,7930,37.39127,-122.28224,,,7930.0,242.6,608.0,1.0,0.75,17.0
    0.927,7960,37.39112,-122.28253,,,7960.0,242.6,596.0,2.0,0.74,17.0
    0.9309999999999999,7990,37.39097,-122.28281,,,7990.0,242.6,582.0,2.0,0.76,21.0
    0.934,8020,37.39083,-122.2831,,,8020.0,242.6,564.0,2.0,0.73,21.0
    0.938,8050,37.39068,-122.28338,,,8050.0,242.6,556.0,2.0,0.68,18.0
    0.941,8080,37.39053,-122.28367,,,8080.0,242.6,551.0,2.0,0.64,16.0
    0.945,8110,37.39038,-122.28395,,,8110.0,242.6,542.0,2.0,0.65,14.0
    0.948,8140,37.39024,-122.28423,,,8140.0,242.6,531.0,2.0,0.63,14.0
    0.9520000000000001,8170,37.39009,-122.28452,,,8170.0,242.6,523.0,2.0,0.63,14.0
    0.955,8200,37.38994,-122.2848,,,8200.0,242.6,522.0,2.0,0.62,14.0
    0.9590000000000001,8230,37.3898,-122.28509,,,8230.0,242.6,523.0,2.0,0.68,16.0
    0.9620000000000001,8260,37.38965,-122.28537,,,8260.0,242.6,529.0,2.0,0.68,16.0
    0.966,8290,37.3895,-122.28566,,,8290.0,242.6,537.0,2.0,0.73,22.0
    0.9690000000000001,8320,37.38935,-122.28594,,,8320.0,242.6,541.0,2.0,0.71,24.0
    0.973,8350,37.38921,-122.28623,,,8350.0,242.6,545.0,2.0,0.68,21.0
    0.976,8380,37.38906,-122.28651,,,8380.0,242.6,552.0,2.0,0.69,26.0
    0.98,8410,37.38891,-122.2868,,,8410.0,242.6,559.0,2.0,0.64,22.0
    0.983,8440,37.38876,-122.28708,,,8440.0,242.6,570.0,2.0,0.65,27.0
    0.987,8470,37.38862,-122.28736,,,8470.0,242.6,583.0,2.0,0.66,25.0
    0.99,8500,37.38847,-122.28765,,,8500.0,242.6,597.0,2.0,0.63,21.0
    0.9940000000000001,8530,37.38832,-122.28793,,,8530.0,242.6,614.0,2.0,0.64,18.0
    0.997,8560,37.38817,-122.28822,,,8560.0,242.6,627.0,2.0,0.58,12.0
    1.0,8583,37.38806,-122.28844,tower,23.0,8583.0,242.6,634.0,2.0,0.56,9.0

</div>

</div>

<div class="cell code">

``` python
```

</div>
