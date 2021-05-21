
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
    03e44f87675441178a6a675ccc066aa1,37.41,-122.2,0
    815642a94b9649cfa96f174dda71e214,37.42,-122.2,1
    9333825a5ef044f0b9cc5dec8b2a03f3,37.42,-122.21,2

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
    98be9e06ba5711eba3ffacde48001122,37.41,-122.2,0
    98be9ed8ba5711eba3ffacde48001122,37.42,-122.2,1
    98be9f00ba5711eba3ffacde48001122,37.42,-122.21,2

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
    1362      37.41899 -122.20283  NaN    1362.0    270.0
    1612      37.41899 -122.20566  NaN    1612.0    270.0
    1862      37.41899 -122.20849  NaN    1862.0    270.0
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
    1362      37.41899 -122.20283  NaN    1362.0    270.0
    1612      37.41899 -122.20566  NaN    1612.0    270.0
    1862      37.41899 -122.20849  NaN    1862.0    270.0
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
    1362      37.41899 -122.20283  0.683    1362.0    270.0
    1612      37.41899 -122.20566  0.808    1612.0    270.0
    1862      37.41899 -122.20849  0.933    1862.0    270.0
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
    1362      37.41899 -122.20283  0.68271    1362.0    270.0
    1612      37.41899 -122.20566  0.80802    1612.0    270.0
    1862      37.41899 -122.20849  0.93333    1862.0    270.0
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
    9q9hg5yt7tm9      1362  37.41899 -122.20283  NaN    1362.0    270.0
    9q9hg5ut3tr3      1612  37.41899 -122.20566  NaN    1612.0    270.0
    9q9hg5fmrv31      1862  37.41899 -122.20849  NaN    1862.0    270.0
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
bash% gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21
```

<div class="output stream stdout">

    id,latitude,longitude,distance
    0,37.41,-122.2,0.0
    1,37.42,-122.2,1112.0
    2,37.42,-122.21,1995.0

</div>

</div>

<div class="cell markdown">

The distance can be calculated relative to the previous location in the
series using the `--relative` options, e.g.,

</div>

<div class="cell code" execution_count="42" scrolled="true">

``` python
bash% gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21 --relative
```

<div class="output stream stdout">

    id,latitude,longitude,distance
    0,37.41,-122.2,0.0
    1,37.42,-122.2,0.0
    2,37.42,-122.21,0.0

</div>

</div>

<div class="cell markdown">

The `distance` dataset support different units, include `meters` or `m`,
`kilometers` or `km`, `feet` or `ft`, `yards` or `yd`, `miles` or `mi`.
For example, the following calculates the distances in feet:

</div>

<div class="cell code" execution_count="43">

``` python
bash% gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21 --units=feet
```

<div class="output stream stdout">

    id,latitude,longitude,distance
    0,37.41,-122.2,0.0
    1,37.42,-122.2,3648.0
    2,37.42,-122.21,6545.0

</div>

</div>

<div class="cell markdown">

Note that the default precision with which distances are calculated in 0
decimals. Thus the distance in `km` is rounded:

</div>

<div class="cell code" execution_count="44">

``` python
bash% gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21 --units=km # precision too low
```

<div class="output stream stdout">

    id,latitude,longitude,distance
    0,37.41,-122.2,0.0
    1,37.42,-122.2,1.0
    2,37.42,-122.21,2.0

</div>

</div>

<div class="cell markdown">

The precision can be changed when necessary, e.g.,

</div>

<div class="cell code" execution_count="45">

``` python
bash% gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21 --units=km --precision.distance=2
```

<div class="output stream stdout">

    id,latitude,longitude,distance
    0,37.41,-122.2,0.0
    1,37.42,-122.2,1.11
    2,37.42,-122.21,2.0

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
    1362      37.41899 -122.20283  NaN     274.0    270.0
    1612      37.41899 -122.20566  NaN     512.0    270.0
    1862      37.41899 -122.20849  NaN     758.0    270.0
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
bash% gridlabd geodata merge -D elevation 37.410,-122.20 37.420,-122.20 37.420,-122.21
```

<div class="output stream stdout">

    id,latitude,longitude,elevation
    0,37.41,-122.2,58.0
    1,37.42,-122.2,76.0
    2,37.42,-122.21,105.0

</div>

</div>

<div class="cell markdown">

Elevations can be calculated in other units using the `--units=UNIT`
options, e.g.,

</div>

<div class="cell code" execution_count="52">

``` python
bash% gridlabd geodata merge -D elevation 37.410,-122.20 37.420,-122.20 37.420,-122.21 --units=feet
```

<div class="output stream stdout">

    id,latitude,longitude,elevation
    0,37.41,-122.2,190.0
    1,37.42,-122.2,249.0
    2,37.42,-122.21,344.0

</div>

</div>

<div class="cell markdown">

Valid units are `m` or `meters`, and `ft` or `feet`.

The precision of the elevations can be specified as well, e.g.,

</div>

<div class="cell code" execution_count="53">

``` python
bash% gridlabd geodata merge -D elevation 37.410,-122.20 37.420,-122.20 37.420,-122.21 --precision.elevation=2 --units=feet
```

<div class="output stream stdout">

    id,latitude,longitude,elevation
    0,37.41,-122.2,190.29
    1,37.42,-122.2,249.34
    2,37.42,-122.21,344.49

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
    1362      37.41899 -122.20283  NaN    1362.0    270.0       82.0
    1612      37.41899 -122.20566  NaN    1612.0    270.0       92.0
    1862      37.41899 -122.20849  NaN    1862.0    270.0      103.0
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
bash% gridlabd geodata merge -D vegetation 37.410,-122.20 37.420,-122.20 37.420,-122.21
```

<div class="output stream stdout">

    id,latitude,longitude,base,cover,height
    0,37.41,-122.2,1.0,5,0.0
    1,37.42,-122.2,1.0,18,3.0
    2,37.42,-122.21,1.0,75,4.0

</div>

</div>

<div class="cell markdown">

Vegetation data is always associated with a year, which is specified by
the `year` option, i.e.,

</div>

<div class="cell code" execution_count="57">

``` python
bash% gridlabd geodata merge -D vegetation 37.410,-122.20 37.420,-122.20 37.420,-122.21 --year=2020
```

<div class="output stream stdout">

    id,latitude,longitude,base,cover,height
    0,37.41,-122.2,1.0,5,0.0
    1,37.42,-122.2,1.0,18,3.0
    2,37.42,-122.21,1.0,75,4.0

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
bash% gridlabd geodata merge -D utility 37.420,-122.20 
```

<div class="output stream stdout">

    id,latitude,longitude,NAME
    0,37.42,-122.2,PACIFIC GAS & ELECTRIC CO.

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
bash% gridlabd geodata merge -D utility 37.420,-122.20 --fields=WINTR_PEAK,SUMMR_PEAK,CUSTOMERS,YEAR
```

<div class="output stream stdout">

    id,latitude,longitude,WINTR_PEAK,SUMMR_PEAK,CUSTOMERS,YEAR
    0,37.42,-122.2,12624.0,17263.0,5471786.0,2018.0

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
    0          15.000000
    250         4.016087
    254        15.000000
    428        15.000000
    595        15.000000
    ...              ...
    7593       15.000000
    7840       15.000000
    8090     -483.425003
    8340     -454.023317
    8583       15.000000
    
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
bash% gridlabd geodata merge -D distance 37,-122 38,-122 -r 100 -k location -w
```

<div class="output stream stdout">

    location,latitude,longitude,id,distance
    9q94rzdk9gyt,37.0,-122.0,0,0.0
    9qc0pvdq1cqt,38.0,-122.0,1,111195.0

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
      File "/usr/local/opt/gridlabd/4.2.21-210519-develop_add_vegetation_geodata/bin/gridlabd-geodata", line 849, in merge
        result = MODULE.apply(data,options,config,warning)
      File "/usr/local/opt/gridlabd/4.2.21-210519-develop_add_vegetation_geodata/share/gridlabd/geodata_distance.py", line 119, in apply
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
along the path,

</div>

<div class="cell code" execution_count="91" scrolled="false">

``` python
bash% gridlabd geodata merge -D elevation path_example.csv -r 25 | gridlabd geodata merge -D distance -f TABLE --relative --table.width=999 --table.max_rows=999
```

<div class="output stream stdout">

          position  latitude  longitude configuration  pole_height  distance  heading  elevation
    id                                                                                          
    0.0          0  37.41505 -122.20565         flat3         15.0       0.0      NaN       88.0
    NaN         25  37.41502 -122.20593           NaN          NaN      25.0    263.0       88.0
    NaN         50  37.41498 -122.20621           NaN          NaN      50.0    263.0       85.0
    NaN         75  37.41495 -122.20649           NaN          NaN      75.0    263.0       85.0
    NaN        100  37.41491 -122.20677           NaN          NaN     100.0    263.0       86.0
    NaN        125  37.41488 -122.20705           NaN          NaN     125.0    263.0       87.0
    NaN        150  37.41484 -122.20733           NaN          NaN     150.0    263.0       89.0
    NaN        175  37.41481 -122.20761           NaN          NaN     175.0    263.0       92.0
    NaN        200  37.41477 -122.20789           NaN          NaN     200.0    263.0       94.0
    NaN        225  37.41474 -122.20817           NaN          NaN     225.0    263.0       96.0
    NaN        250  37.41471 -122.20845           NaN          NaN     250.0    263.0      100.0
    1.0        254  37.41470 -122.20849         sideT         15.0       0.0    263.0      100.0
    NaN        279  37.41467 -122.20873           NaN          NaN      21.0    262.7      103.0
    NaN        304  37.41463 -122.20901           NaN          NaN      47.0    262.7       97.0
    NaN        329  37.41460 -122.20929           NaN          NaN      72.0    262.7       96.0
    NaN        354  37.41456 -122.20957           NaN          NaN      97.0    262.7       98.0
    NaN        379  37.41453 -122.20984           NaN          NaN     121.0    262.7      101.0
    NaN        404  37.41449 -122.21012           NaN          NaN     146.0    262.7      109.0
    2.0        428  37.41445 -122.21044         sideT         15.0       0.0    262.7      112.0
    NaN        453  37.41446 -122.21040           NaN          NaN       4.0    263.0      112.0
    NaN        478  37.41442 -122.21068           NaN          NaN      21.0    263.0      114.0
    NaN        503  37.41439 -122.21096           NaN          NaN      46.0    263.0      115.0
    NaN        528  37.41435 -122.21124           NaN          NaN      72.0    263.0      117.0
    NaN        553  37.41432 -122.21152           NaN          NaN      96.0    263.0      118.0
    NaN        578  37.41428 -122.21180           NaN          NaN     122.0    263.0      117.0
    3.0        595  37.41422 -122.21231         sideT         15.0       0.0    263.0      111.0
    NaN        620  37.41425 -122.21208           NaN          NaN      21.0    263.0      114.0
    NaN        645  37.41422 -122.21236           NaN          NaN       4.0    263.0      111.0
    NaN        670  37.41418 -122.21264           NaN          NaN      29.0    263.0      109.0
    NaN        695  37.41415 -122.21292           NaN          NaN      54.0    263.0      110.0
    NaN        720  37.41411 -122.21320           NaN          NaN      80.0    263.0      108.0
    NaN        745  37.41408 -122.21348           NaN          NaN     104.0    263.0      106.0
    NaN        770  37.41404 -122.21376           NaN          NaN     130.0    263.0      104.0
    4.0        771  37.41398 -122.21427         sideT         15.0       0.0    263.0       98.0
    NaN        796  37.41401 -122.21404           NaN          NaN      21.0    263.1      101.0
    NaN        821  37.41398 -122.21432           NaN          NaN       4.0    263.1       98.0
    NaN        846  37.41394 -122.21460           NaN          NaN      29.0    263.1       96.0
    NaN        871  37.41391 -122.21488           NaN          NaN      54.0    263.1       95.0
    NaN        896  37.41387 -122.21516           NaN          NaN      80.0    263.1       93.0
    NaN        921  37.41384 -122.21544           NaN          NaN     104.0    263.1       92.0
    NaN        946  37.41381 -122.21572           NaN          NaN     129.0    263.1       91.0
    NaN        971  37.41377 -122.21600           NaN          NaN     155.0    263.1       90.0
    5.0        976  37.41370 -122.21657         sideT         15.0       0.0    263.1       92.0
    NaN       1001  37.41374 -122.21628           NaN          NaN      26.0    262.6       90.0
    NaN       1026  37.41370 -122.21656           NaN          NaN       1.0    262.6       92.0
    NaN       1051  37.41366 -122.21684           NaN          NaN      24.0    262.6       92.0
    NaN       1076  37.41363 -122.21712           NaN          NaN      49.0    262.6       90.0
    NaN       1101  37.41359 -122.21740           NaN          NaN      74.0    262.6       86.0
    NaN       1126  37.41355 -122.21768           NaN          NaN      99.0    262.6       84.0
    6.0       1128  37.41348 -122.21827         sideT         15.0       0.0    262.6       79.0
    NaN       1153  37.41352 -122.21796           NaN          NaN      28.0    263.0       81.0
    NaN       1178  37.41349 -122.21823           NaN          NaN       4.0    263.0       79.0
    NaN       1203  37.41345 -122.21851           NaN          NaN      21.0    263.0       76.0
    NaN       1228  37.41342 -122.21879           NaN          NaN      46.0    263.0       76.0
    NaN       1253  37.41338 -122.21907           NaN          NaN      72.0    263.0       78.0
    NaN       1278  37.41335 -122.21935           NaN          NaN      96.0    263.0       81.0
    NaN       1303  37.41332 -122.21963           NaN          NaN     121.0    263.0       87.0
    7.0       1326  37.41321 -122.22048         sideT         15.0       0.0    263.0       90.0
    NaN       1351  37.41328 -122.21991           NaN          NaN      51.0    262.9       88.0
    NaN       1376  37.41325 -122.22019           NaN          NaN      26.0    262.9       89.0
    NaN       1401  37.41321 -122.22047           NaN          NaN       1.0    262.9       90.0
    NaN       1426  37.41318 -122.22075           NaN          NaN      24.0    262.9       91.0
    NaN       1451  37.41314 -122.22103           NaN          NaN      49.0    262.9       92.0
    NaN       1476  37.41311 -122.22131           NaN          NaN      74.0    262.9       89.0
    8.0       1490  37.41298 -122.22232         sideT         15.0       0.0    262.9       78.0
    NaN       1515  37.41307 -122.22159           NaN          NaN      65.0    263.1       85.0
    NaN       1540  37.41304 -122.22187           NaN          NaN      40.0    263.1       81.0
    NaN       1565  37.41300 -122.22215           NaN          NaN      15.0    263.1       80.0
    NaN       1590  37.41297 -122.22243           NaN          NaN      10.0    263.1       78.0
    NaN       1615  37.41294 -122.22271           NaN          NaN      35.0    263.1       78.0
    NaN       1640  37.41290 -122.22299           NaN          NaN      60.0    263.1       78.0
    NaN       1665  37.41287 -122.22327           NaN          NaN      85.0    263.1       77.0
    NaN       1690  37.41284 -122.22355           NaN          NaN     110.0    263.1       71.0
    9.0       1690  37.41271 -122.22456         sideT         15.0       0.0    263.1       80.0
    NaN       1715  37.41280 -122.22383           NaN          NaN      65.0    262.9       74.0
    NaN       1740  37.41277 -122.22411           NaN          NaN      40.0    262.9       78.0
    NaN       1765  37.41273 -122.22439           NaN          NaN      15.0    262.9       79.0
    NaN       1790  37.41270 -122.22467           NaN          NaN      10.0    262.9       80.0
    NaN       1815  37.41266 -122.22495           NaN          NaN      35.0    262.9       81.0
    NaN       1840  37.41263 -122.22523           NaN          NaN      60.0    262.9       80.0
    NaN       1865  37.41259 -122.22551           NaN          NaN      85.0    262.9       80.0
    NaN       1890  37.41256 -122.22579           NaN          NaN     110.0    262.9       74.0
    NaN       1915  37.41252 -122.22607           NaN          NaN     135.0    262.9       72.0
    NaN       1940  37.41249 -122.22635           NaN          NaN     160.0    262.9       77.0
    10.0      1941  37.41236 -122.22736         sideT         15.0       0.0    262.9       92.0
    NaN       1966  37.41245 -122.22663           NaN          NaN      65.0    263.0       80.0
    NaN       1991  37.41242 -122.22691           NaN          NaN      40.0    263.0       85.0
    NaN       2016  37.41238 -122.22719           NaN          NaN      15.0    263.0       89.0
    NaN       2041  37.41235 -122.22747           NaN          NaN      10.0    263.0       92.0
    NaN       2066  37.41231 -122.22775           NaN          NaN      35.0    263.0       94.0
    NaN       2091  37.41228 -122.22803           NaN          NaN      60.0    263.0       94.0
    NaN       2116  37.41225 -122.22830           NaN          NaN      84.0    263.0       91.0
    11.0      2123  37.41211 -122.22940         sideT         15.0       0.0    263.0       81.0
    NaN       2148  37.41221 -122.22858           NaN          NaN      73.0    263.0       84.0
    NaN       2173  37.41218 -122.22886           NaN          NaN      48.0    263.0       82.0
    NaN       2198  37.41214 -122.22914           NaN          NaN      23.0    263.0       81.0
    NaN       2223  37.41211 -122.22942           NaN          NaN       2.0    263.0       81.0
    NaN       2248  37.41207 -122.22970           NaN          NaN      27.0    263.0       81.0
    NaN       2273  37.41204 -122.22998           NaN          NaN      52.0    263.0       82.0
    NaN       2298  37.41201 -122.23026           NaN          NaN      77.0    263.0       82.0
    NaN       2323  37.41197 -122.23054           NaN          NaN     102.0    263.0       83.0
    12.0      2341  37.41181 -122.23184         3pole         15.0       0.0    263.0      103.0
    NaN       2366  37.41185 -122.23078           NaN          NaN      94.0    242.5       83.0
    NaN       2391  37.41172 -122.23102           NaN          NaN      73.0    242.5       85.0
    NaN       2416  37.41160 -122.23125           NaN          NaN      57.0    242.5       85.0
    NaN       2441  37.41148 -122.23149           NaN          NaN      48.0    242.5       88.0
    NaN       2466  37.41135 -122.23173           NaN          NaN      52.0    242.5       87.0
    NaN       2491  37.41123 -122.23196           NaN          NaN      65.0    242.5       90.0
    NaN       2516  37.41111 -122.23220           NaN          NaN      84.0    242.5       85.0
    NaN       2541  37.41098 -122.23244           NaN          NaN     106.0    242.5       90.0
    NaN       2566  37.41086 -122.23267           NaN          NaN     129.0    242.5       95.0
    NaN       2591  37.41074 -122.23291           NaN          NaN     152.0    242.5       93.0
    NaN       2616  37.41061 -122.23315           NaN          NaN     177.0    242.5       96.0
    NaN       2641  37.41049 -122.23338           NaN          NaN     200.0    242.5       89.0
    NaN       2666  37.41037 -122.23362           NaN          NaN     224.0    242.5       88.0
    NaN       2691  37.41024 -122.23386           NaN          NaN     250.0    242.5       78.0
    NaN       2716  37.41012 -122.23409           NaN          NaN     274.0    242.5       79.0
    13.0      2718  37.40995 -122.23541         3pole         15.0       0.0    242.5      126.0
    NaN       2743  37.41003 -122.23435           NaN          NaN      94.0    250.2       85.0
    NaN       2768  37.40994 -122.23461           NaN          NaN      71.0    250.2      109.0
    NaN       2793  37.40984 -122.23487           NaN          NaN      49.0    250.2      116.0
    NaN       2818  37.40975 -122.23512           NaN          NaN      34.0    250.2      122.0
    NaN       2843  37.40966 -122.23538           NaN          NaN      32.0    250.2      124.0
    NaN       2868  37.40956 -122.23564           NaN          NaN      48.0    250.2      121.0
    NaN       2893  37.40947 -122.23590           NaN          NaN      69.0    250.2      117.0
    NaN       2918  37.40938 -122.23615           NaN          NaN      91.0    250.2      120.0
    14.0      2941  37.40912 -122.23771         2pole         15.0       0.0    250.2      121.0
    NaN       2966  37.40928 -122.23641           NaN          NaN     116.0    249.9      114.0
    NaN       2991  37.40919 -122.23667           NaN          NaN      92.0    249.9      113.0
    NaN       3016  37.40910 -122.23693           NaN          NaN      69.0    249.9      124.0
    NaN       3041  37.40900 -122.23718           NaN          NaN      49.0    249.9      118.0
    NaN       3066  37.40891 -122.23744           NaN          NaN      33.0    249.9      116.0
    NaN       3091  37.40881 -122.23770           NaN          NaN      34.0    249.9      121.0
    NaN       3116  37.40872 -122.23795           NaN          NaN      49.0    249.9      112.0
    NaN       3141  37.40862 -122.23821           NaN          NaN      71.0    249.9      103.0
    NaN       3166  37.40853 -122.23847           NaN          NaN      94.0    249.9       90.0
    NaN       3191  37.40844 -122.23873           NaN          NaN     118.0    249.9       86.0
    NaN       3216  37.40834 -122.23898           NaN          NaN     142.0    249.9       88.0
    NaN       3241  37.40825 -122.23924           NaN          NaN     166.0    249.9       96.0
    NaN       3266  37.40815 -122.23950           NaN          NaN     191.0    249.9       95.0
    15.0      3268  37.40789 -122.24107         2pole         15.0       0.0    249.9      106.0
    NaN       3293  37.40806 -122.23975           NaN          NaN     118.0    250.1       91.0
    NaN       3318  37.40797 -122.24001           NaN          NaN      94.0    250.1       97.0
    NaN       3343  37.40787 -122.24027           NaN          NaN      71.0    250.1       97.0
    NaN       3368  37.40778 -122.24053           NaN          NaN      49.0    250.1      100.0
    NaN       3393  37.40769 -122.24078           NaN          NaN      34.0    250.1      104.0
    NaN       3418  37.40759 -122.24104           NaN          NaN      33.0    250.1      103.0
    NaN       3443  37.40750 -122.24130           NaN          NaN      48.0    250.1      105.0
    16.0      3466  37.40715 -122.24311         2pole         15.0       0.0    250.1      112.0
    NaN       3491  37.40741 -122.24156           NaN          NaN     140.0    249.7      104.0
    NaN       3516  37.40731 -122.24181           NaN          NaN     116.0    249.7      104.0
    NaN       3541  37.40722 -122.24207           NaN          NaN      92.0    249.7      109.0
    NaN       3566  37.40712 -122.24232           NaN          NaN      70.0    249.7      109.0
    NaN       3591  37.40703 -122.24258           NaN          NaN      49.0    249.7      108.0
    NaN       3616  37.40693 -122.24284           NaN          NaN      34.0    249.7      115.0
    NaN       3641  37.40684 -122.24309           NaN          NaN      35.0    249.7      111.0
    17.0      3653  37.40644 -122.24503         3pole         15.0       0.0    249.7      125.0
    NaN       3678  37.40664 -122.24295           NaN          NaN     185.0    143.8      118.0
    NaN       3703  37.40645 -122.24281           NaN          NaN     196.0    143.8      118.0
    NaN       3728  37.40625 -122.24267           NaN          NaN     210.0    143.8      121.0
    NaN       3753  37.40606 -122.24253           NaN          NaN     225.0    143.8      125.0
    18.0      3759  37.40562 -122.24443         vert3         15.0       0.0    143.8      126.0
    NaN       3784  37.40584 -122.24247           NaN          NaN     175.0    166.7      125.0
    NaN       3809  37.40562 -122.24242           NaN          NaN     178.0    166.7      129.0
    NaN       3834  37.40540 -122.24237           NaN          NaN     184.0    166.7      128.0
    NaN       3859  37.40517 -122.24232           NaN          NaN     193.0    166.7      125.0
    19.0      3878  37.40456 -122.24418         vert3         15.0       0.0    166.7      123.0
    NaN       3903  37.40495 -122.24234           NaN          NaN     168.0    185.4      124.0
    NaN       3928  37.40473 -122.24236           NaN          NaN     162.0    185.4      124.0
    NaN       3953  37.40450 -122.24238           NaN          NaN     159.0    185.4      121.0
    NaN       3978  37.40428 -122.24240           NaN          NaN     160.0    185.4      122.0
    NaN       4003  37.40405 -122.24242           NaN          NaN     165.0    185.4      122.0
    20.0      4009  37.40339 -122.24429         vert3         15.0       0.0    185.4      118.0
    NaN       4034  37.40384 -122.24251           NaN          NaN     165.0    203.4      120.0
    NaN       4059  37.40363 -122.24261           NaN          NaN     151.0    203.4      120.0
    NaN       4084  37.40342 -122.24270           NaN          NaN     140.0    203.4      119.0
    NaN       4109  37.40320 -122.24279           NaN          NaN     134.0    203.4      118.0
    21.0      4115  37.40249 -122.24468         vert3         15.0       0.0    203.4      106.0
    NaN       4140  37.40302 -122.24295           NaN          NaN     164.0    220.9      117.0
    NaN       4165  37.40283 -122.24311           NaN          NaN     144.0    220.9      118.0
    NaN       4190  37.40265 -122.24327           NaN          NaN     126.0    220.9      117.0
    NaN       4215  37.40246 -122.24343           NaN          NaN     110.0    220.9      114.0
    NaN       4240  37.40228 -122.24359           NaN          NaN      99.0    220.9      114.0
    NaN       4265  37.40209 -122.24375           NaN          NaN      93.0    220.9      106.0
    22.0      4267  37.40136 -122.24566         vert3         15.0       0.0    220.9      106.0
    NaN       4292  37.40199 -122.24401           NaN          NaN     162.0    247.6      105.0
    NaN       4317  37.40189 -122.24426           NaN          NaN     137.0    247.6      104.0
    NaN       4342  37.40178 -122.24451           NaN          NaN     112.0    247.6      105.0
    NaN       4367  37.40168 -122.24476           NaN          NaN      87.0    247.6      106.0
    NaN       4392  37.40157 -122.24501           NaN          NaN      62.0    247.6      105.0
    NaN       4417  37.40147 -122.24526           NaN          NaN      37.0    247.6      105.0
    NaN       4442  37.40137 -122.24551           NaN          NaN      13.0    247.6      106.0
    NaN       4467  37.40126 -122.24576           NaN          NaN      14.0    247.6      106.0
    NaN       4492  37.40116 -122.24601           NaN          NaN      38.0    247.6      106.0
    NaN       4517  37.40106 -122.24627           NaN          NaN      63.0    247.6      106.0
    NaN       4542  37.40095 -122.24652           NaN          NaN      89.0    247.6      106.0
    NaN       4567  37.40085 -122.24677           NaN          NaN     113.0    247.6      106.0
    NaN       4592  37.40074 -122.24702           NaN          NaN     139.0    247.6      106.0
    NaN       4617  37.40064 -122.24727           NaN          NaN     163.0    247.6      106.0
    23.0      4627  37.39987 -122.24927         3pole         15.0       0.0    247.6      117.0
    NaN       4652  37.40061 -122.24755           NaN          NaN     173.0    263.6      106.0
    NaN       4677  37.40058 -122.24783           NaN          NaN     150.0    263.6      106.0
    NaN       4702  37.40055 -122.24811           NaN          NaN     127.0    263.6      106.0
    NaN       4727  37.40051 -122.24839           NaN          NaN     105.0    263.6      106.0
    NaN       4752  37.40048 -122.24867           NaN          NaN      86.0    263.6      107.0
    NaN       4777  37.40045 -122.24895           NaN          NaN      70.0    263.6      108.0
    24.0      4793  37.39966 -122.25113         2pole         15.0       0.0    263.6      172.0
    NaN       4818  37.40044 -122.24923           NaN          NaN     189.0    266.8      110.0
    NaN       4843  37.40042 -122.24952           NaN          NaN     165.0    266.8      112.0
    NaN       4868  37.40040 -122.24980           NaN          NaN     143.0    266.8      118.0
    NaN       4893  37.40039 -122.25008           NaN          NaN     123.0    266.8      128.0
    NaN       4918  37.40037 -122.25036           NaN          NaN     104.0    266.8      137.0
    NaN       4943  37.40036 -122.25065           NaN          NaN      89.0    266.8      142.0
    NaN       4968  37.40034 -122.25093           NaN          NaN      78.0    266.8      143.0
    NaN       4993  37.40032 -122.25121           NaN          NaN      74.0    266.8      146.0
    NaN       5018  37.40031 -122.25149           NaN          NaN      79.0    266.8      143.0
    NaN       5043  37.40029 -122.25177           NaN          NaN      90.0    266.8      142.0
    NaN       5068  37.40028 -122.25206           NaN          NaN     107.0    266.8      153.0
    NaN       5093  37.40026 -122.25234           NaN          NaN     126.0    266.8      159.0
    NaN       5118  37.40024 -122.25262           NaN          NaN     147.0    266.8      161.0
    NaN       5143  37.40023 -122.25290           NaN          NaN     169.0    266.8      162.0
    NaN       5168  37.40021 -122.25319           NaN          NaN     192.0    266.8      172.0
    NaN       5193  37.40020 -122.25347           NaN          NaN     215.0    266.8      181.0
    NaN       5218  37.40018 -122.25375           NaN          NaN     239.0    266.8      190.0
    NaN       5243  37.40016 -122.25403           NaN          NaN     262.0    266.8      198.0
    25.0      5248  37.39937 -122.25627         2pole         15.0       0.0    266.8      264.0
    NaN       5273  37.40015 -122.25432           NaN          NaN     193.0    266.9      207.0
    NaN       5298  37.40013 -122.25460           NaN          NaN     170.0    266.9      217.0
    NaN       5323  37.40012 -122.25488           NaN          NaN     148.0    266.9      224.0
    NaN       5348  37.40010 -122.25516           NaN          NaN     127.0    266.9      226.0
    NaN       5373  37.40009 -122.25544           NaN          NaN     109.0    266.9      229.0
    26.0      5397  37.39928 -122.25795         3pole         15.0       0.0    266.9      286.0
    NaN       5422  37.40007 -122.25573           NaN          NaN     215.0    266.0      234.0
    NaN       5447  37.40005 -122.25601           NaN          NaN     192.0    266.0      240.0
    NaN       5472  37.40003 -122.25629           NaN          NaN     169.0    266.0      251.0
    NaN       5497  37.40001 -122.25657           NaN          NaN     146.0    266.0      259.0
    NaN       5522  37.39999 -122.25685           NaN          NaN     125.0    266.0      272.0
    NaN       5547  37.39997 -122.25714           NaN          NaN     105.0    266.0      277.0
    NaN       5572  37.39995 -122.25742           NaN          NaN      88.0    266.0      277.0
    NaN       5597  37.39993 -122.25770           NaN          NaN      76.0    266.0      278.0
    NaN       5622  37.39991 -122.25798           NaN          NaN      70.0    266.0      279.0
    NaN       5647  37.39989 -122.25826           NaN          NaN      73.0    266.0      278.0
    NaN       5672  37.39987 -122.25855           NaN          NaN      84.0    266.0      273.0
    NaN       5697  37.39985 -122.25883           NaN          NaN     100.0    266.0      266.0
    NaN       5722  37.39983 -122.25911           NaN          NaN     119.0    266.0      258.0
    NaN       5747  37.39981 -122.25939           NaN          NaN     140.0    266.0      246.0
    NaN       5772  37.39979 -122.25967           NaN          NaN     162.0    266.0      232.0
    NaN       5797  37.39977 -122.25996           NaN          NaN     186.0    266.0      219.0
    NaN       5822  37.39975 -122.26024           NaN          NaN     209.0    266.0      213.0
    NaN       5847  37.39973 -122.26052           NaN          NaN     232.0    266.0      203.0
    NaN       5872  37.39971 -122.26080           NaN          NaN     256.0    266.0      201.0
    NaN       5897  37.39969 -122.26108           NaN          NaN     280.0    266.0      199.0
    NaN       5922  37.39967 -122.26137           NaN          NaN     305.0    266.0      201.0
    NaN       5947  37.39965 -122.26165           NaN          NaN     329.0    266.0      206.0
    NaN       5972  37.39963 -122.26193           NaN          NaN     354.0    266.0      219.0
    27.0      5977  37.39882 -122.26449         3pole         15.0       0.0    266.0      322.0
    NaN       6002  37.39961 -122.26221           NaN          NaN     220.0    266.1      235.0
    NaN       6027  37.39959 -122.26249           NaN          NaN     196.0    266.1      249.0
    NaN       6052  37.39958 -122.26277           NaN          NaN     174.0    266.1      261.0
    NaN       6077  37.39956 -122.26306           NaN          NaN     151.0    266.1      283.0
    NaN       6102  37.39954 -122.26334           NaN          NaN     129.0    266.1      291.0
    28.0      6119  37.39871 -122.26610         2pole         15.0       0.0    266.1      349.0
    NaN       6144  37.39952 -122.26362           NaN          NaN     237.0    266.1      306.0
    NaN       6169  37.39950 -122.26390           NaN          NaN     213.0    266.1      320.0
    NaN       6194  37.39948 -122.26418           NaN          NaN     190.0    266.1      331.0
    NaN       6219  37.39946 -122.26447           NaN          NaN     166.0    266.1      338.0
    NaN       6244  37.39944 -122.26475           NaN          NaN     144.0    266.1      345.0
    29.0      6250  37.39861 -122.26757         3pole         15.0       0.0    266.1      326.0
    NaN       6275  37.39925 -122.26489           NaN          NaN     247.0    216.5      345.0
    NaN       6300  37.39905 -122.26504           NaN          NaN     229.0    216.5      345.0
    NaN       6325  37.39886 -122.26518           NaN          NaN     213.0    216.5      338.0
    NaN       6350  37.39867 -122.26532           NaN          NaN     199.0    216.5      345.0
    NaN       6375  37.39847 -122.26547           NaN          NaN     186.0    216.5      336.0
    NaN       6400  37.39828 -122.26561           NaN          NaN     177.0    216.5      318.0
    NaN       6425  37.39808 -122.26575           NaN          NaN     171.0    216.5      318.0
    NaN       6450  37.39789 -122.26590           NaN          NaN     168.0    216.5      299.0
    NaN       6475  37.39770 -122.26604           NaN          NaN     169.0    216.5      281.0
    NaN       6500  37.39750 -122.26618           NaN          NaN     174.0    216.5      280.0
    NaN       6525  37.39731 -122.26633           NaN          NaN     181.0    216.5      280.0
    NaN       6550  37.39712 -122.26647           NaN          NaN     192.0    216.5      288.0
    NaN       6575  37.39692 -122.26661           NaN          NaN     206.0    216.5      295.0
    NaN       6600  37.39673 -122.26676           NaN          NaN     221.0    216.5      298.0
    NaN       6625  37.39653 -122.26690           NaN          NaN     239.0    216.5      307.0
    NaN       6650  37.39634 -122.26705           NaN          NaN     257.0    216.5      322.0
    NaN       6675  37.39615 -122.26719           NaN          NaN     276.0    216.5      322.0
    NaN       6700  37.39595 -122.26733           NaN          NaN     297.0    216.5      338.0
    NaN       6725  37.39576 -122.26748           NaN          NaN     317.0    216.5      348.0
    NaN       6750  37.39557 -122.26762           NaN          NaN     338.0    216.5      352.0
    NaN       6775  37.39537 -122.26776           NaN          NaN     361.0    216.5      366.0
    30.0      6783  37.39448 -122.27063         3pole         15.0       0.0    216.5      443.0
    NaN       6808  37.39518 -122.26792           NaN          NaN     252.0    219.5      381.0
    NaN       6833  37.39500 -122.26807           NaN          NaN     233.0    219.5      395.0
    NaN       6858  37.39481 -122.26823           NaN          NaN     215.0    219.5      395.0
    NaN       6883  37.39462 -122.26838           NaN          NaN     199.0    219.5      409.0
    NaN       6908  37.39443 -122.26854           NaN          NaN     185.0    219.5      416.0
    NaN       6933  37.39424 -122.26869           NaN          NaN     173.0    219.5      420.0
    NaN       6958  37.39406 -122.26885           NaN          NaN     164.0    219.5      424.0
    31.0      6971  37.39306 -122.27180         3pole         15.0       0.0    219.5      448.0
    NaN       6996  37.39400 -122.26912           NaN          NaN     259.0    257.7      427.0
    NaN       7021  37.39394 -122.26939           NaN          NaN     234.0    257.7      429.0
    NaN       7046  37.39388 -122.26967           NaN          NaN     209.0    257.7      434.0
    NaN       7071  37.39382 -122.26994           NaN          NaN     185.0    257.7      436.0
    NaN       7096  37.39376 -122.27021           NaN          NaN     161.0    257.7      438.0
    NaN       7121  37.39370 -122.27049           NaN          NaN     136.0    257.7      440.0
    NaN       7146  37.39364 -122.27076           NaN          NaN     112.0    257.7      442.0
    NaN       7171  37.39358 -122.27103           NaN          NaN      89.0    257.7      446.0
    NaN       7196  37.39352 -122.27130           NaN          NaN      68.0    257.7      447.0
    32.0      7215  37.39248 -122.27446         2pole         15.0       0.0    257.7      501.0
    NaN       7240  37.39346 -122.27158           NaN          NaN     277.0    257.9      447.0
    NaN       7265  37.39340 -122.27185           NaN          NaN     252.0    257.9      448.0
    NaN       7290  37.39334 -122.27212           NaN          NaN     228.0    257.9      449.0
    NaN       7315  37.39329 -122.27240           NaN          NaN     203.0    257.9      450.0
    NaN       7340  37.39323 -122.27267           NaN          NaN     179.0    257.9      452.0
    NaN       7365  37.39317 -122.27294           NaN          NaN     155.0    257.9      460.0
    NaN       7390  37.39311 -122.27322           NaN          NaN     130.0    257.9      472.0
    NaN       7415  37.39305 -122.27349           NaN          NaN     107.0    257.9      474.0
    NaN       7440  37.39299 -122.27376           NaN          NaN      84.0    257.9      484.0
    33.0      7463  37.39190 -122.27717         2pole         15.0       0.0    257.9      573.0
    NaN       7488  37.39294 -122.27404           NaN          NaN     300.0    258.5      493.0
    NaN       7513  37.39288 -122.27431           NaN          NaN     275.0    258.5      502.0
    NaN       7538  37.39283 -122.27459           NaN          NaN     250.0    258.5      511.0
    NaN       7563  37.39277 -122.27486           NaN          NaN     226.0    258.5      503.0
    NaN       7588  37.39272 -122.27513           NaN          NaN     202.0    258.5      499.0
    34.0      7593  37.39161 -122.27860         2pole         15.0       0.0    258.5      610.0
    NaN       7618  37.39273 -122.27542           NaN          NaN     307.0    272.1      506.0
    NaN       7643  37.39274 -122.27570           NaN          NaN     285.0    272.1      521.0
    NaN       7668  37.39275 -122.27598           NaN          NaN     264.0    272.1      535.0
    NaN       7693  37.39276 -122.27627           NaN          NaN     242.0    272.1      545.0
    NaN       7718  37.39277 -122.27655           NaN          NaN     222.0    272.1      553.0
    NaN       7743  37.39278 -122.27683           NaN          NaN     203.0    272.1      559.0
    NaN       7768  37.39279 -122.27711           NaN          NaN     186.0    272.1      554.0
    NaN       7793  37.39280 -122.27740           NaN          NaN     170.0    272.1      558.0
    NaN       7818  37.39281 -122.27768           NaN          NaN     156.0    272.1      562.0
    35.0      7840  37.39171 -122.28139         3pole         15.0       0.0    272.1      640.0
    NaN       7865  37.39268 -122.27792           NaN          NaN     325.0    242.6      578.0
    NaN       7890  37.39256 -122.27815           NaN          NaN     301.0    242.6      581.0
    NaN       7915  37.39244 -122.27839           NaN          NaN     277.0    242.6      597.0
    NaN       7940  37.39232 -122.27863           NaN          NaN     253.0    242.6      603.0
    NaN       7965  37.39219 -122.27886           NaN          NaN     230.0    242.6      612.0
    NaN       7990  37.39207 -122.27910           NaN          NaN     206.0    242.6      619.0
    NaN       8015  37.39195 -122.27934           NaN          NaN     183.0    242.6      626.0
    NaN       8040  37.39182 -122.27958           NaN          NaN     160.0    242.6      621.0
    NaN       8065  37.39170 -122.27981           NaN          NaN     140.0    242.6      631.0
    NaN       8090  37.39158 -122.28005           NaN          NaN     119.0    242.6      625.0
    NaN       8115  37.39146 -122.28029           NaN          NaN     101.0    242.6      630.0
    NaN       8140  37.39133 -122.28052           NaN          NaN      88.0    242.6      618.0
    NaN       8165  37.39121 -122.28076           NaN          NaN      79.0    242.6      623.0
    NaN       8190  37.39109 -122.28100           NaN          NaN      77.0    242.6      616.0
    NaN       8215  37.39097 -122.28123           NaN          NaN      83.0    242.6      616.0
    NaN       8240  37.39084 -122.28147           NaN          NaN      97.0    242.6      609.0
    NaN       8265  37.39072 -122.28171           NaN          NaN     114.0    242.6      592.0
    NaN       8290  37.39060 -122.28195           NaN          NaN     133.0    242.6      584.0
    NaN       8315  37.39047 -122.28218           NaN          NaN     155.0    242.6      567.0
    NaN       8340  37.39035 -122.28242           NaN          NaN     176.0    242.6      559.0
    NaN       8365  37.39023 -122.28266           NaN          NaN     199.0    242.6      550.0
    NaN       8390  37.39011 -122.28289           NaN          NaN     222.0    242.6      547.0
    NaN       8415  37.38998 -122.28313           NaN          NaN     246.0    242.6      528.0
    NaN       8440  37.38986 -122.28337           NaN          NaN     270.0    242.6      522.0
    NaN       8465  37.38974 -122.28361           NaN          NaN     294.0    242.6      522.0
    NaN       8490  37.38962 -122.28384           NaN          NaN     318.0    242.6      517.0
    NaN       8515  37.38949 -122.28408           NaN          NaN     343.0    242.6      522.0
    NaN       8540  37.38937 -122.28432           NaN          NaN     367.0    242.6      534.0
    NaN       8565  37.38925 -122.28455           NaN          NaN     391.0    242.6      543.0
    36.0      8583  37.38806 -122.28844         tower         15.0       0.0    242.6      632.0

</div>

</div>

<div class="cell code">

``` python
```

</div>
