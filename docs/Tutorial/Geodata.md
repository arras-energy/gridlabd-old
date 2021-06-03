
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
    37.415045141688054,-122.2056472090359,flat3,15.0
    37.414698020593065,-122.20848749028133,sideT,15.0
    37.414454093051745,-122.21044282065421,sideT,15.0

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
    0,37.41505,-122.20565,flat3,15.0
    1,37.4147,-122.20849,sideT,15.0
    2,37.41445,-122.21044,sideT,15.0

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
    vegetation.username='name@example.com'
    vegetation.password='password5839'
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
    vegetation.username='name@example.com'
    vegetation.password='password5839'
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
    vegetation.username='name@example.com'
    vegetation.password='password5839'
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
    vegetation.username='name@example.com'
    vegetation.password='password5839'
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
    411455ae491e46b084f553cae4d8293c,37.41,-122.2,0
    1cf918a372d943be8ef1539fafddcab5,37.42,-122.2,1
    8d56c82786ae4fe3940fa3e107b96e63,37.42,-122.21,2

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
    62faf294c49111eb9a53989e633f97ae,37.41,-122.2,0
    62faf366c49111eb9a53989e633f97ae,37.42,-122.2,1
    62faf398c49111eb9a53989e633f97ae,37.42,-122.21,2

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
    500       37.41450 -122.20000  NaN     500.0      0.0       74.0
    750       37.41674 -122.20000  NaN     750.0      0.0       76.0
    1000      37.41899 -122.20000  NaN    1000.0      0.0       97.0
    1112      37.42000 -122.20000  1.0    1112.0      0.0       76.0
    1362      37.42000 -122.20283  NaN    1362.0    270.0       85.0
    1612      37.42000 -122.20566  NaN    1612.0    270.0       91.0
    1862      37.42000 -122.20849  NaN    1862.0    270.0      103.0
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
bash% gridlabd geodata merge -D vegetation 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f TABLE
```

<div class="output stream stdout">

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

              linesag
    position         
    0            15.0
    250          14.4
    254          15.0
    428          15.0
    595          15.0
    ...           ...
    7593         15.0
    7840         15.0
    8090        -31.2
    8340        -30.6
    8583         15.0
    
    [49 rows x 1 columns]

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
    rated_tensile_strength                72000.0
    elasticity                      71800000000.0
    thermal_expansion                    0.000021
    conductor_crosssection_area          0.000692
    resistivity                            0.0039
    nominal_resistance                   0.000046
    nomimal_temperature                      15.0
    voltage_rating                       230000.0
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
        linesag
    id         
    0       NaN
    1       NaN
    2       NaN
    3       NaN
    4       NaN
    ..      ...
    32      NaN
    33      NaN
    34      NaN
    35      NaN
    36      NaN

    [37 rows x 1 columns]

</div>

</div>

<div class="cell code" execution_count="66">

``` python
bash% gridlabd geodata merge -D powerline path_example.csv -f TABLE:linesag --cable_type="My cabletype" # bad cable type
```

<div class="output stream stdout">

    WARNING [geodata/powerline]: cable_type='My cabletype' not found
        linesag
    id         
    0       NaN
    1       NaN
    2       NaN
    3       NaN
    4       NaN
    ..      ...
    32      NaN
    33      NaN
    34      NaN
    35      NaN
    36      NaN

    [37 rows x 1 columns]

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
      File "/usr/local/opt/gridlabd/4.2.21-210530-develop_add_vegetation_geodata/bin/gridlabd-geodata", line 849, in merge
        result = MODULE.apply(data,options,config,warning)
      File "/usr/local/opt/gridlabd/4.2.21-210530-develop_add_vegetation_geodata/share/gridlabd/geodata_distance.py", line 119, in apply
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

<div class="cell code" execution_count="84" scrolled="false">

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
    2.0        428  37.41445 -122.21044         sideT         21.0       0.0    262.7      112.0
    3.0        595  37.41422 -122.21231         sideT         18.0       0.0    263.0      111.0
    4.0        771  37.41398 -122.21427         sideT         20.0       0.0    263.0       98.0
    5.0        976  37.41370 -122.21657         sideT         18.0       0.0    263.1       92.0
    6.0       1128  37.41348 -122.21827         sideT         21.0       0.0    262.6       79.0
    7.0       1326  37.41321 -122.22048         sideT         18.0       0.0    263.0       90.0
    8.0       1490  37.41298 -122.22232         sideT         16.0       0.0    262.9       78.0
    9.0       1690  37.41271 -122.22456         sideT         21.0       0.0    263.1       80.0
    NaN       1940  37.41236 -122.22736           NaN          NaN     250.0    262.9       92.0
    10.0      1941  37.41236 -122.22736         sideT         18.0       0.0    262.9       92.0
    11.0      2123  37.41211 -122.22940         sideT         19.0       0.0    263.0       81.0
    12.0      2341  37.41181 -122.23184         3pole         15.0       0.0    263.0      103.0
    NaN       2591  37.41058 -122.23421           NaN          NaN     250.0    242.5       83.0
    13.0      2718  37.40995 -122.23541         3pole         16.0       0.0    242.5      126.0
    14.0      2941  37.40912 -122.23771         2pole         15.0       0.0    250.2      121.0
    NaN       3191  37.40818 -122.24028           NaN          NaN     250.0    249.9       92.0
    15.0      3268  37.40789 -122.24107         2pole         17.0       0.0    249.9      106.0
    16.0      3466  37.40715 -122.24311         2pole         15.0       0.0    250.1      112.0
    17.0      3653  37.40644 -122.24503         3pole         17.0       0.0    249.7      125.0
    18.0      3759  37.40562 -122.24443         vert3         17.0       0.0    143.8      126.0
    19.0      3878  37.40456 -122.24418         vert3         20.0       0.0    166.7      123.0
    20.0      4009  37.40339 -122.24429         vert3         20.0       0.0    185.4      118.0
    21.0      4115  37.40249 -122.24468         vert3         20.0       0.0    203.4      106.0
    22.0      4267  37.40136 -122.24566         vert3         22.0       0.0    220.9      106.0
    NaN       4517  37.40032 -122.24817           NaN          NaN     250.0    247.6      106.0
    23.0      4627  37.39987 -122.24927         3pole         16.0       0.0    247.6      117.0
    24.0      4793  37.39966 -122.25113         2pole         18.0       0.0    263.6      172.0
    NaN       5043  37.39950 -122.25395           NaN          NaN     250.0    266.8      206.0
    25.0      5248  37.39937 -122.25627         2pole         22.0       0.0    266.8      264.0
    26.0      5397  37.39928 -122.25795         3pole         15.0       0.0    266.9      286.0
    NaN       5647  37.39908 -122.26077           NaN          NaN     250.0    266.0      212.0
    NaN       5897  37.39888 -122.26359           NaN          NaN     500.0    266.0      291.0
    27.0      5977  37.39882 -122.26449         3pole         18.0       0.0    266.0      322.0
    28.0      6119  37.39871 -122.26610         2pole         15.0       0.0    266.1      349.0
    29.0      6250  37.39861 -122.26757         3pole         18.0       0.0    266.1      326.0
    NaN       6500  37.39667 -122.26901           NaN          NaN     250.0    216.5      342.0
    NaN       6750  37.39473 -122.27044           NaN          NaN     500.0    216.5      436.0
    30.0      6783  37.39448 -122.27063         3pole         21.0       0.0    216.5      443.0
    31.0      6971  37.39306 -122.27180         3pole         21.0       0.0    219.5      448.0
    32.0      7215  37.39248 -122.27446         2pole         17.0       0.0    257.7      501.0
    33.0      7463  37.39190 -122.27717         2pole         21.0       0.0    257.9      573.0
    34.0      7593  37.39161 -122.27860         2pole         23.0       0.0    258.5      610.0
    35.0      7840  37.39171 -122.28139         3pole         16.0       0.0    272.1      640.0
    NaN       8090  37.39048 -122.28376           NaN          NaN     250.0    242.6      540.0
    NaN       8340  37.38926 -122.28613           NaN          NaN     500.0    242.6      547.0
    36.0      8583  37.38806 -122.28844         tower         23.0       0.0    242.6      632.0

</div>

</div>

<div class="cell markdown">

# Example 3

Ths following example illustrates merging elevation, vegetation, and
powerline data.

</div>

<div class="cell code" execution_count="95" scrolled="false">

``` python
bash% gridlabd geodata merge -D elevation path_example.csv -r 250 --resolution_id \
| gridlabd geodata merge -D vegetation \
| gridlabd geodata merge -D powerline --cable_type="TACSR/AC 610mm^2" --margin.vertical=10 -f TABLE:latitude,longitude,pole_height,elevation,linesag,cover,height,contact --table.width=999 --table.max_rows=999
```

<div class="output stream stdout">

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.21-210530-develop_add_vegetation_geodata/bin/gridlabd-geodata", line 1386, in <module>
        main(len(sys.argv),sys.argv)
      File "/usr/local/opt/gridlabd/4.2.21-210530-develop_add_vegetation_geodata/bin/gridlabd-geodata", line 1279, in main
        load_dataset([DATASET])
      File "/usr/local/opt/gridlabd/4.2.21-210530-develop_add_vegetation_geodata/bin/gridlabd-geodata", line 1104, in load_dataset
        MODULE = importlib.import_module(f"geodata_{DATASET}")
      File "/usr/local/Cellar/python@3.9/3.9.5/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
      File "<frozen importlib._bootstrap>", line 1030, in _gcd_import
      File "<frozen importlib._bootstrap>", line 1007, in _find_and_load
      File "<frozen importlib._bootstrap>", line 986, in _find_and_load_unlocked
      File "<frozen importlib._bootstrap>", line 680, in _load_unlocked
      File "<frozen importlib._bootstrap_external>", line 851, in exec_module
      File "<frozen importlib._bootstrap_external>", line 988, in get_code
      File "<frozen importlib._bootstrap_external>", line 918, in source_to_code
      File "<frozen importlib._bootstrap>", line 228, in _call_with_frames_removed
      File "/usr/local/opt/gridlabd/4.2.21-210530-develop_add_vegetation_geodata/share/gridlabd/geodata_powerline.py", line 576
        return result
    IndentationError: unexpected indent
    BrokenPipeError: [Errno 32] Broken pipe

    During handling of the above exception, another exception occurred:

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.21-210530-develop_add_vegetation_geodata/bin/gridlabd-geodata", line 1386, in <module>
        main(len(sys.argv),sys.argv)
      File "/usr/local/opt/gridlabd/4.2.21-210530-develop_add_vegetation_geodata/bin/gridlabd-geodata", line 1365, in main
        write_csv(data)
      File "/usr/local/opt/gridlabd/4.2.21-210530-develop_add_vegetation_geodata/bin/gridlabd-geodata", line 938, in write_csv
        data.to_csv(OUTPUT)
      File "/usr/local/lib/python3.9/site-packages/pandas/core/generic.py", line 3170, in to_csv
        formatter.save()
      File "/usr/local/lib/python3.9/site-packages/pandas/io/formats/csvs.py", line 227, in save
        f.close()
    BrokenPipeError: [Errno 32] Broken pipe

</div>

</div>

<div class="cell code">

``` python
```

</div>
