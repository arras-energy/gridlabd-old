
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
bash% gridlabd --version='-ge 4.2.26'
bash% gridlabd requirements
```

<div class="output stream stdout">

    
    Usage:   
      /usr/local/opt/python@3.9/bin/python3.9 -m pip <command> [options]
    
    no such option: -r

</div>

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
bash% gridlabd geodata config set name local_value -w
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
    814feaf2794e4dc08a25eddc4a68f043,37.41,-122.2,0
    0f1c64584e604b2cac8c69eb40135b70,37.42,-122.2,1
    adc8ba4186874fbb9a7ff18fe1b9eaad,37.42,-122.21,2

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
    0705a628020d11ecbde8d0817adb129d,37.41,-122.2,0
    0705a6e6020d11ecbde8d0817adb129d,37.42,-122.2,1
    0705a718020d11ecbde8d0817adb129d,37.42,-122.21,2

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
    0,0,Main St.,114.04407,22.31315

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
    0,0,"54 - Stanford Research Computing Facility, 2575, Sand Hill Road, Stanford Hills, Menlo Park, San Mateo County, California, 94305, United States",-122.20118,37.41546

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
bash% gridlabd geodata merge -D powerline path_example.csv --cable_type="TACSR/AC 610mm^2" -r 50 -f TABLE:linesag
```

<div class="output stream stdout">

    WARNING [geodata/powerline]: cannot run function CONTACT and 'height' is missing or invalid
              linesag
    position         
    0            18.0
    50           12.2
    100           9.4
    150           9.7
    200          13.0
    ...           ...
    8390        -16.6
    8440         -8.9
    8490          0.6
    8540         12.0
    8583         23.0
    
    [191 rows x 1 columns]

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
    WARNING [geodata/powerline]: cannot run function CONTACT and 'height' is missing or invalid
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
    WARNING [geodata/powerline]: cannot run function CONTACT and 'height' is missing or invalid
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
        "filter": "",
        "select": "",
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
      File "/usr/local/opt/gridlabd/4.2.26-210820-develop_add_cyme_converter/bin/gridlabd-geodata", line 854, in merge
        result = MODULE.apply(data,options,config,warning)
      File "/usr/local/opt/gridlabd/4.2.26-210820-develop_add_cyme_converter/share/gridlabd/geodata_distance.py", line 119, in apply
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
        "filter": "",
        "select": "",
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
    --filter=''
    --select=''

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

<div class="cell code" execution_count="80" scrolled="false">

``` python
bash% gridlabd geodata merge -D elevation path_example.csv -r 30 --resolution_id \
| gridlabd geodata merge -D vegetation -w | grep -v WARNING \
| gridlabd geodata merge -D powerline --cable_type="TACSR/AC 610mm^2" --margin.vertical=2 -f TABLE:latitude,longitude,pole_height,elevation,linesag,cover,height,contact --table.width=999 --table.max_rows=999
```

<div class="output stream stdout">

           latitude  longitude  pole_height  elevation  linesag  cover  height  contact
    id                                                                                 
    0.000  37.41505 -122.20565         18.0       88.0     18.0   0.68    14.0     0.00
    0.003  37.41501 -122.20599          NaN       89.0     14.5   0.73    16.0     0.73
    0.007  37.41497 -122.20632          NaN       89.0     13.2   0.74    16.0     0.74
    0.010  37.41493 -122.20666          NaN       90.0     12.0   0.72    17.0     0.72
    0.014  37.41488 -122.20699          NaN       91.0     11.8   0.56    17.0     0.56
    0.017  37.41484 -122.20733          NaN       92.0     12.8   0.30    17.0     0.30
    0.021  37.41480 -122.20766          NaN       93.0     14.8   0.14    16.0     0.14
    0.024  37.41476 -122.20800          NaN       95.0     17.0   0.07    10.0     0.00
    0.028  37.41472 -122.20834          NaN       98.0     19.3   0.09     1.0     0.00
    0.030  37.41470 -122.20849         20.0      100.0     20.0   0.09     1.0     0.00
    0.033  37.41466 -122.20883          NaN      103.0     15.9   0.14     1.0     0.00
    0.037  37.41461 -122.20916          NaN       99.0     20.1   0.16     1.0     0.00
    0.040  37.41457 -122.20950          NaN       98.0     22.7   0.14     1.0     0.00
    0.044  37.41453 -122.20983          NaN      103.0     20.5   0.11     0.0     0.00
    0.047  37.41449 -122.21017          NaN      110.0     17.7   0.33     1.0     0.00
    0.050  37.41445 -122.21044         21.0      111.0     21.0   0.31     1.0     0.00
    0.053  37.41441 -122.21078          NaN      111.0     17.2   0.30     1.0     0.00
    0.057  37.41437 -122.21111          NaN      113.0     12.8   0.33     1.0     0.00
    0.060  37.41433 -122.21145          NaN      115.0      9.7   0.40     1.0     0.00
    0.064  37.41428 -122.21178          NaN      114.0     10.9   0.44     1.0     0.00
    0.067  37.41424 -122.21212          NaN      111.0     15.5   0.51     1.0     0.00
    0.069  37.41422 -122.21231         18.0      110.0     18.0   0.51     1.0     0.00
    0.073  37.41418 -122.21265          NaN      108.0     15.1   0.69     6.0     0.00
    0.076  37.41414 -122.21298          NaN      107.0     12.6   0.72    13.0     0.72
    0.080  37.41410 -122.21332          NaN      105.0     12.3   0.66    15.0     0.66
    0.083  37.41406 -122.21365          NaN      104.0     12.3   0.62    15.0     0.62
    0.087  37.41401 -122.21399          NaN      101.0     15.7   0.65    16.0     0.65
    0.090  37.41398 -122.21427         20.0       98.0     20.0   0.68    17.0     0.00
    0.093  37.41394 -122.21461          NaN       96.0     17.1   0.65    18.0     0.65
    0.097  37.41390 -122.21494          NaN       94.0     15.5   0.52    19.0     0.52
    0.100  37.41386 -122.21528          NaN       92.0     15.1   0.38    15.0     0.38
    0.104  37.41382 -122.21561          NaN       91.0     14.8   0.20     9.0     0.00
    0.107  37.41378 -122.21595          NaN       89.0     16.8   0.10     4.0     0.00
    0.111  37.41373 -122.21628          NaN       90.0     17.0   0.07     2.0     0.00
    0.114  37.41370 -122.21657         18.0       91.0     18.0   0.06     1.0     0.00
    0.117  37.41366 -122.21691          NaN       90.0     13.9   0.06     0.0     0.00
    0.121  37.41361 -122.21724          NaN       87.0     13.3   0.10     0.0     0.00
    0.124  37.41357 -122.21758          NaN       83.0     15.1   0.11     1.0     0.00
    0.128  37.41353 -122.21791          NaN       80.0     17.3   0.29     3.0     0.00
    0.131  37.41348 -122.21825          NaN       77.0     20.9   0.50     4.0     0.00
    0.131  37.41348 -122.21827         21.0       77.0     21.0   0.50     4.0     0.00
    0.135  37.41344 -122.21861          NaN       76.0     20.2   0.67     7.0     0.00
    0.138  37.41340 -122.21894          NaN       79.0     16.7   0.71    10.0     0.00
    0.142  37.41336 -122.21928          NaN       82.0     14.4   0.67    13.0     0.67
    0.145  37.41332 -122.21961          NaN       85.0     13.3   0.59    15.0     0.59
    0.149  37.41327 -122.21995          NaN       89.0     12.5   0.54    16.0     0.54
    0.152  37.41323 -122.22028          NaN       91.0     14.8   0.60    16.0     0.60
    0.154  37.41321 -122.22048         18.0       91.0     18.0   0.68    16.0     0.00
    0.158  37.41317 -122.22082          NaN       92.0     11.6   0.73    16.0     0.73
    0.161  37.41313 -122.22115          NaN       89.0     10.6   0.74    17.0     0.74
    0.165  37.41308 -122.22149          NaN       84.0     12.8   0.71    17.0     0.71
    0.168  37.41304 -122.22182          NaN       82.0     13.5   0.70    15.0     0.70
    0.172  37.41300 -122.22216          NaN       81.0     14.5   0.68    13.0     0.68
    0.174  37.41298 -122.22232         16.0       80.0     16.0   0.62    12.0     0.00
    0.177  37.41294 -122.22266          NaN       78.0     15.1   0.57    11.0     0.00
    0.181  37.41290 -122.22299          NaN       77.0     14.5   0.42     9.0     0.00
    0.184  37.41286 -122.22333          NaN       73.0     18.0   0.25     5.0     0.00
    0.188  37.41282 -122.22366          NaN       74.0     17.8   0.12     3.0     0.00
    0.191  37.41278 -122.22400          NaN       78.0     15.9   0.08     1.0     0.00
    0.195  37.41274 -122.22433          NaN       79.0     18.1   0.06     1.0     0.00
    0.197  37.41271 -122.22456         21.0       79.0     21.0   0.04     1.0     0.00
    0.200  37.41267 -122.22490          NaN       79.0     18.6   0.02     0.0     0.00
    0.204  37.41263 -122.22523          NaN       78.0     18.4   0.00     0.0     0.00
    0.207  37.41258 -122.22557          NaN       74.0     22.2   0.01     0.0     0.00
    0.211  37.41254 -122.22590          NaN       74.0     23.1   0.02     0.0     0.00
    0.214  37.41250 -122.22624          NaN       79.0     20.2   0.06     1.0     0.00
    0.218  37.41246 -122.22657          NaN       86.0     16.3   0.10     1.0     0.00
    0.221  37.41242 -122.22691          NaN       93.0     13.6   0.19     2.0     0.00
    0.225  37.41237 -122.22724          NaN       95.0     16.9   0.29     4.0     0.00
    0.226  37.41236 -122.22736         18.0       96.0     18.0   0.29     4.0     0.00
    0.230  37.41232 -122.22770          NaN       95.0     13.6   0.34     2.0     0.00
    0.233  37.41228 -122.22803          NaN       92.0     12.5   0.41     3.0     0.00
    0.237  37.41224 -122.22837          NaN       87.0     14.6   0.37     4.0     0.00
    0.240  37.41220 -122.22870          NaN       82.0     18.1   0.26     4.0     0.00
    0.244  37.41215 -122.22904          NaN       83.0     16.8   0.09     2.0     0.00
    0.247  37.41211 -122.22937          NaN       82.0     18.9   0.07     2.0     0.00
    0.247  37.41211 -122.22940         19.0       82.0     19.0   0.07     2.0     0.00
    0.251  37.41207 -122.22974          NaN       82.0     17.6   0.05     1.0     0.00
    0.254  37.41203 -122.23007          NaN       83.0     16.5   0.07     3.0     0.00
    0.258  37.41199 -122.23041          NaN       84.0     16.5   0.10     3.0     0.00
    0.261  37.41194 -122.23074          NaN       85.0     17.6   0.13     4.0     0.00
    0.265  37.41190 -122.23108          NaN       89.0     17.1   0.12     4.0     0.00
    0.268  37.41186 -122.23141          NaN       99.0     11.5   0.11     4.0     0.00
    0.272  37.41182 -122.23175          NaN      103.0     13.3   0.13     2.0     0.00
    0.273  37.41181 -122.23184         15.0      103.0     15.0   0.15     1.0     0.00
    0.276  37.41166 -122.23212          NaN      102.0     12.9   0.07     1.0     0.00
    0.280  37.41151 -122.23241          NaN       96.0     16.7   0.08     1.0     0.00
    0.283  37.41137 -122.23269          NaN       94.0     17.4   0.04     1.0     0.00
    0.287  37.41122 -122.23298          NaN       95.0     16.0   0.04     0.0     0.00
    0.290  37.41107 -122.23326          NaN       97.0     14.5   0.04     1.0     0.00
    0.294  37.41092 -122.23354          NaN       96.0     17.0   0.04     1.0     0.00
    0.297  37.41077 -122.23383          NaN       92.0     23.4   0.14     4.0     0.00
    0.301  37.41063 -122.23411          NaN       85.0     33.5   0.18     5.0     0.00
    0.304  37.41048 -122.23440          NaN       81.0     41.8   0.49     9.0     0.00
    0.308  37.41033 -122.23468          NaN       92.0     35.8   0.47     8.0     0.00
    0.311  37.41018 -122.23496          NaN      109.0     24.8   0.48     6.0     0.00
    0.315  37.41003 -122.23525          NaN      126.0     14.9   0.40     6.0     0.00
    0.317  37.40995 -122.23541         16.0      129.0     16.0   0.40     6.0     0.00
    0.320  37.40984 -122.23572          NaN      127.0     11.5   0.18     2.0     0.00
    0.324  37.40973 -122.23603          NaN      119.0     14.1   0.10     2.0     0.00
    0.327  37.40962 -122.23634          NaN      110.0     18.8   0.08     1.0     0.00
    0.331  37.40950 -122.23665          NaN      105.0     20.7   0.09     1.0     0.00
    0.334  37.40939 -122.23696          NaN      104.0     19.8   0.14     1.0     0.00
    0.338  37.40928 -122.23727          NaN      103.0     20.0   0.28     2.0     0.00
    0.341  37.40917 -122.23757          NaN      107.0     16.4   0.26     2.0     0.00
    0.343  37.40912 -122.23771         15.0      109.0     15.0   0.42     5.0     0.00
    0.346  37.40901 -122.23802          NaN      107.0     12.3   0.53     6.0     0.00
    0.350  37.40889 -122.23833          NaN       98.0     17.5   0.28     5.0     0.00
    0.353  37.40878 -122.23864          NaN       89.0     23.7   0.32     7.0     0.00
    0.357  37.40867 -122.23894          NaN       87.0     24.0   0.42    11.0     0.00
    0.360  37.40856 -122.23925          NaN       86.0     24.1   0.30    11.0     0.00
    0.364  37.40844 -122.23956          NaN       85.0     25.3   0.26     9.0     0.00
    0.367  37.40833 -122.23987          NaN       87.0     24.4   0.16     7.0     0.00
    0.371  37.40822 -122.24018          NaN       88.0     25.5   0.15     4.0     0.00
    0.374  37.40810 -122.24049          NaN       97.0     19.6   0.16     2.0     0.00
    0.378  37.40799 -122.24079          NaN      105.0     15.5   0.19     1.0     0.00
    0.381  37.40789 -122.24107         17.0      108.0     17.0   0.14     1.0     0.00
    0.384  37.40778 -122.24138          NaN      109.0     13.3   0.09     1.0     0.00
    0.388  37.40767 -122.24169          NaN      109.0     11.8   0.09     1.0     0.00
    0.391  37.40755 -122.24200          NaN      111.0      9.6   0.11     1.0     0.00
    0.395  37.40744 -122.24231          NaN      111.0     10.6   0.17     1.0     0.00
    0.398  37.40733 -122.24261          NaN      113.0     10.8   0.28     1.0     0.00
    0.402  37.40722 -122.24292          NaN      115.0     12.3   0.32     1.0     0.00
    0.404  37.40715 -122.24311         15.0      115.0     15.0   0.32     1.0     0.00
    0.407  37.40704 -122.24342          NaN      115.0     13.4   0.32     1.0     0.00
    0.411  37.40692 -122.24373          NaN      117.0     11.2   0.33     1.0     0.00
    0.414  37.40681 -122.24403          NaN      119.0     10.1   0.22     1.0     0.00
    0.418  37.40669 -122.24434          NaN      120.0     11.4   0.16     1.0     0.00
    0.421  37.40658 -122.24465          NaN      123.0     11.9   0.09     1.0     0.00
    0.425  37.40647 -122.24496          NaN      124.0     15.7   0.09     1.0     0.00
    0.426  37.40644 -122.24503         17.0      124.0     17.0   0.09     1.0     0.00
    0.429  37.40621 -122.24486          NaN      127.0     12.7   0.09     1.0     0.00
    0.433  37.40597 -122.24469          NaN      128.0     12.1   0.15     1.0     0.00
    0.436  37.40574 -122.24452          NaN      127.0     15.2   0.12     1.0     0.00
    0.438  37.40562 -122.24443         17.0      127.0     17.0   0.12     1.0     0.00
    0.441  37.40535 -122.24437          NaN      126.0     15.3   0.08     2.0     0.00
    0.445  37.40509 -122.24430          NaN      124.0     16.3   0.08     2.0     0.00
    0.448  37.40482 -122.24424          NaN      123.0     17.9   0.11     2.0     0.00
    0.452  37.40456 -122.24418         20.0      123.0     20.0   0.11     2.0     0.00
    0.455  37.40429 -122.24421          NaN      122.0     17.1   0.15     2.0     0.00
    0.459  37.40402 -122.24423          NaN      120.0     16.7   0.15     2.0     0.00
    0.462  37.40375 -122.24426          NaN      118.0     17.8   0.23     3.0     0.00
    0.466  37.40348 -122.24428          NaN      117.0     19.4   0.31     4.0     0.00
    0.467  37.40339 -122.24429         20.0      117.0     20.0   0.31     3.0     0.00
    0.471  37.40313 -122.24440          NaN      116.0     16.8   0.31     3.0     0.00
    0.474  37.40288 -122.24451          NaN      115.0     15.4   0.22     1.0     0.00
    0.478  37.40262 -122.24462          NaN      111.0     18.7   0.16     1.0     0.00
    0.479  37.40249 -122.24468         20.0      110.0     20.0   0.16     1.0     0.00
    0.483  37.40227 -122.24487          NaN      108.0     18.8   0.09     2.0     0.00
    0.486  37.40205 -122.24507          NaN      108.0     16.9   0.08     1.0     0.00
    0.490  37.40182 -122.24526          NaN      108.0     16.4   0.09     1.0     0.00
    0.493  37.40160 -122.24545          NaN      107.0     18.4   0.10     1.0     0.00
    0.497  37.40138 -122.24564          NaN      106.0     21.7   0.15     1.0     0.00
    0.497  37.40136 -122.24566         22.0      106.0     22.0   0.15     1.0     0.00
    0.501  37.40124 -122.24596          NaN      106.0     17.3   0.16     1.0     0.00
    0.504  37.40111 -122.24626          NaN      106.0     13.4   0.18     1.0     0.00
    0.508  37.40099 -122.24656          NaN      106.0     10.5   0.15     1.0     0.00
    0.511  37.40086 -122.24687          NaN      106.0      8.5   0.20     1.0     0.00
    0.515  37.40074 -122.24717          NaN      106.0      7.4   0.21     1.0     0.00
    0.518  37.40061 -122.24747          NaN      106.0      7.3   0.28     2.0     0.00
    0.522  37.40049 -122.24777          NaN      106.0      8.1   0.37     3.0     0.00
    0.525  37.40036 -122.24807          NaN      106.0      9.9   0.49     4.0     0.00
    0.529  37.40024 -122.24837          NaN      106.0     12.5   0.54     4.0     0.00
    0.532  37.40012 -122.24867          NaN      108.0     14.1   0.59     5.0     0.00
    0.536  37.39999 -122.24898          NaN      110.0     16.8   0.35    10.0     0.00
    0.539  37.39987 -122.24927         16.0      116.0     16.0   0.36    11.0     0.00
    0.543  37.39983 -122.24961          NaN      123.0     10.5   0.39    10.0     0.39
    0.546  37.39979 -122.24994          NaN      134.0      4.5   0.43    10.0     0.43
    0.550  37.39976 -122.25028          NaN      146.0      1.3   0.47     9.0     0.47
    0.553  37.39972 -122.25061          NaN      156.0      3.6   0.49     8.0     0.49
    0.557  37.39968 -122.25095          NaN      164.0     12.3   0.51     7.0     0.00
    0.558  37.39966 -122.25113         18.0      167.0     18.0   0.51     7.0     0.00
    0.562  37.39964 -122.25147          NaN      166.0     19.0   0.49     6.0     0.00
    0.565  37.39962 -122.25181          NaN      163.0     23.1   0.46     5.0     0.00
    0.569  37.39960 -122.25215          NaN      166.0     22.0   0.43     5.0     0.00
    0.572  37.39958 -122.25249          NaN      171.0     20.0   0.47     7.0     0.00
    0.576  37.39956 -122.25282          NaN      178.0     16.8   0.52     7.0     0.00
    0.579  37.39955 -122.25316          NaN      186.0     13.7   0.55     7.0     0.00
    0.583  37.39953 -122.25350          NaN      194.0     11.5   0.59     7.0     0.00
    0.586  37.39951 -122.25384          NaN      203.0      9.4   0.65     8.0     0.65
    0.590  37.39949 -122.25418          NaN      210.0     10.2   0.71     9.0     0.71
    0.593  37.39947 -122.25452          NaN      219.0     10.1   0.73    11.0     0.73
    0.597  37.39945 -122.25486          NaN      227.0     12.0   0.69    10.0     0.00
    0.600  37.39943 -122.25520          NaN      233.0     16.8   0.60     9.0     0.00
    0.604  37.39941 -122.25553          NaN      244.0     17.4   0.54     6.0     0.00
    0.607  37.39939 -122.25587          NaN      256.0     18.4   0.42     6.0     0.00
    0.611  37.39937 -122.25621          NaN      265.0     23.4   0.31     8.0     0.00
    0.611  37.39937 -122.25627         22.0      267.0     22.0   0.31     8.0     0.00
    0.615  37.39935 -122.25661          NaN      274.0     14.4   0.29     9.0     0.00
    0.618  37.39933 -122.25695          NaN      281.0      8.2   0.31     9.0     0.31
    0.622  37.39932 -122.25729          NaN      284.0      7.5   0.32    10.0     0.32
    0.625  37.39930 -122.25763          NaN      284.0     11.2   0.30    11.0     0.30
    0.629  37.39928 -122.25795         15.0      285.0     15.0   0.27    11.0     0.00
    0.632  37.39926 -122.25829          NaN      283.0     12.8   0.26    10.0     0.00
    0.636  37.39923 -122.25863          NaN      274.0     18.3   0.24    10.0     0.00
    0.639  37.39921 -122.25896          NaN      262.0     27.7   0.24    12.0     0.00
    0.643  37.39918 -122.25930          NaN      248.0     39.6   0.25    10.0     0.00
    0.646  37.39916 -122.25964          NaN      235.0     51.4   0.26     8.0     0.00
    0.650  37.39914 -122.25998          NaN      222.0     63.9   0.29     6.0     0.00
    0.653  37.39911 -122.26032          NaN      213.0     73.1   0.34     5.0     0.00
    0.657  37.39909 -122.26066          NaN      207.0     80.0   0.38     7.0     0.00
    0.660  37.39907 -122.26099          NaN      203.0     85.7   0.40    17.0     0.00
    0.664  37.39904 -122.26133          NaN      205.0     86.1   0.41    21.0     0.00
    0.667  37.39902 -122.26167          NaN      224.0     70.2   0.46    22.0     0.00
    0.671  37.39899 -122.26201          NaN      247.0     51.2   0.54    21.0     0.00
    0.674  37.39897 -122.26235          NaN      262.0     40.8   0.66    21.0     0.00
    0.678  37.39895 -122.26269          NaN      274.0     34.2   0.70    20.0     0.00
    0.681  37.39892 -122.26302          NaN      285.0     29.2   0.73    17.0     0.00
    0.685  37.39890 -122.26336          NaN      296.0     25.1   0.76    16.0     0.00
    0.688  37.39888 -122.26370          NaN      312.0     16.8   0.80    16.0     0.80
    0.692  37.39885 -122.26404          NaN      322.0     15.2   0.82    19.0     0.82
    0.695  37.39883 -122.26438          NaN      328.0     18.4   0.79    21.0     0.79
    0.696  37.39882 -122.26449         18.0      331.0     18.0   0.79    21.0     0.79
    0.700  37.39880 -122.26483          NaN      339.0     10.2   0.68    19.0     0.68
    0.703  37.39877 -122.26517          NaN      346.0      5.4   0.64    16.0     0.64
    0.707  37.39875 -122.26551          NaN      350.0      5.4   0.58    12.0     0.58
    0.710  37.39873 -122.26584          NaN      352.0      9.2   0.53    11.0     0.53
    0.713  37.39871 -122.26610         15.0      352.0     15.0   0.52    12.0     0.00
    0.716  37.39869 -122.26644          NaN      352.0     10.6   0.55    13.0     0.55
    0.720  37.39866 -122.26678          NaN      351.0      8.7   0.59    15.0     0.59
    0.723  37.39864 -122.26712          NaN      348.0     10.4   0.60    15.0     0.60
    0.727  37.39862 -122.26745          NaN      343.0     15.6   0.60    14.0     0.60
    0.728  37.39861 -122.26757         18.0      341.0     18.0   0.60    14.0     0.00
    0.732  37.39838 -122.26774          NaN      323.0     35.5   0.57    10.0     0.00
    0.735  37.39814 -122.26791          NaN      309.0     49.9   0.48     8.0     0.00
    0.739  37.39791 -122.26809          NaN      300.0     60.0   0.48     8.0     0.00
    0.742  37.39768 -122.26826          NaN      299.0     62.8   0.47     8.0     0.00
    0.746  37.39745 -122.26843          NaN      305.0     59.4   0.53     7.0     0.00
    0.749  37.39721 -122.26860          NaN      317.0     50.8   0.68    13.0     0.00
    0.753  37.39698 -122.26878          NaN      327.0     45.0   0.68    13.0     0.00
    0.756  37.39675 -122.26895          NaN      339.0     37.9   0.80    18.0     0.00
    0.760  37.39652 -122.26912          NaN      351.0     31.5   0.79    11.0     0.00
    0.763  37.39628 -122.26929          NaN      361.0     28.2   0.65     6.0     0.00
    0.767  37.39605 -122.26946          NaN      372.0     24.4   0.60     8.0     0.00
    0.770  37.39582 -122.26964          NaN      383.0     21.5   0.44     5.0     0.00
    0.774  37.39559 -122.26981          NaN      395.0     18.2   0.40     9.0     0.00
    0.777  37.39535 -122.26998          NaN      408.0     15.1   0.44     9.0     0.00
    0.781  37.39512 -122.27015          NaN      421.0     12.4   0.41    11.0     0.41
    0.784  37.39489 -122.27033          NaN      432.0     12.8   0.45     8.0     0.00
    0.788  37.39466 -122.27050          NaN      441.0     15.7   0.45     4.0     0.00
    0.790  37.39448 -122.27063         21.0      444.0     21.0   0.45     4.0     0.00
    0.794  37.39425 -122.27082          NaN      446.0     16.3   0.48     3.0     0.00
    0.797  37.39403 -122.27100          NaN      445.0     15.9   0.52     5.0     0.00
    0.801  37.39380 -122.27119          NaN      446.0     14.7   0.56    11.0     0.00
    0.804  37.39358 -122.27137          NaN      447.0     14.7   0.55    11.0     0.00
    0.808  37.39335 -122.27156          NaN      448.0     16.1   0.68    22.0     0.68
    0.811  37.39313 -122.27175          NaN      448.0     19.7   0.79    27.0     0.79
    0.812  37.39306 -122.27180         21.0      448.0     21.0   0.79    27.0     0.79
    0.816  37.39299 -122.27213          NaN      450.0     19.4   0.77    26.0     0.77
    0.819  37.39292 -122.27245          NaN      451.0     20.5   0.79    25.0     0.79
    0.823  37.39285 -122.27278          NaN      458.0     17.3   0.81    30.0     0.81
    0.826  37.39277 -122.27311          NaN      466.0     15.0   0.83    33.0     0.83
    0.830  37.39270 -122.27344          NaN      477.0     11.4   0.85    35.0     0.85
    0.833  37.39263 -122.27376          NaN      489.0      8.3   0.86    33.0     0.86
    0.837  37.39256 -122.27409          NaN      498.0     10.3   0.85    29.0     0.85
    0.840  37.39249 -122.27442          NaN      504.0     17.2   0.81    25.0     0.81
    0.841  37.39248 -122.27446         17.0      505.0     17.0   0.81    25.0     0.81
    0.844  37.39241 -122.27479          NaN      499.0     23.8   0.76    21.0     0.00
    0.848  37.39234 -122.27512          NaN      500.0     26.1   0.73    19.0     0.00
    0.851  37.39227 -122.27544          NaN      514.0     17.5   0.73    18.0     0.73
    0.855  37.39220 -122.27577          NaN      529.0     10.4   0.75    19.0     0.75
    0.858  37.39213 -122.27610          NaN      541.0      8.9   0.77    20.0     0.77
    0.862  37.39206 -122.27643          NaN      555.0      7.8   0.79    22.0     0.79
    0.865  37.39199 -122.27676          NaN      566.0     12.4   0.81    23.0     0.81
    0.869  37.39192 -122.27708          NaN      576.0     20.1   0.82    21.0     0.82
    0.870  37.39190 -122.27717         21.0      578.0     21.0   0.84    23.0     0.84
    0.873  37.39183 -122.27750          NaN      586.0     14.8   0.84    24.0     0.84
    0.877  37.39177 -122.27783          NaN      594.0     13.0   0.82    25.0     0.82
    0.880  37.39170 -122.27816          NaN      603.0     14.6   0.81    26.0     0.81
    0.883  37.39163 -122.27849          NaN      613.0     20.0   0.67    20.0     0.67
    0.885  37.39161 -122.27860         23.0      615.0     23.0   0.67    20.0     0.00
    0.888  37.39162 -122.27894          NaN      619.0     17.4   0.67    18.0     0.67
    0.892  37.39163 -122.27928          NaN      620.0     16.0   0.64    15.0     0.64
    0.895  37.39165 -122.27962          NaN      626.0     10.6   0.57    14.0     0.57
    0.899  37.39166 -122.27996          NaN      639.0     -0.6   0.48    12.0     0.48
    0.902  37.39167 -122.28030          NaN      643.0     -1.7   0.54    13.0     0.54
    0.906  37.39168 -122.28064          NaN      643.0      2.3   0.61    15.0     0.61
    0.909  37.39170 -122.28097          NaN      643.0      7.2   0.68    16.0     0.68
    0.913  37.39171 -122.28131          NaN      642.0     14.5   0.72    16.0     0.72
    0.913  37.39171 -122.28139         16.0      642.0     16.0   0.72    16.0     0.72
    0.917  37.39156 -122.28167          NaN      633.0     17.6   0.79    16.0     0.79
    0.920  37.39142 -122.28196          NaN      621.0     22.5   0.79    16.0     0.00
    0.924  37.39127 -122.28224          NaN      608.0     29.1   0.75    17.0     0.00
    0.927  37.39112 -122.28253          NaN      596.0     35.3   0.74    17.0     0.00
    0.931  37.39097 -122.28281          NaN      582.0     44.2   0.76    21.0     0.00
    0.934  37.39083 -122.28310          NaN      564.0     57.7   0.73    21.0     0.00
    0.938  37.39068 -122.28338          NaN      556.0     61.9   0.68    18.0     0.00
    0.941  37.39053 -122.28367          NaN      551.0     63.7   0.64    16.0     0.00
    0.945  37.39038 -122.28395          NaN      542.0     70.2   0.65    14.0     0.00
    0.948  37.39024 -122.28423          NaN      531.0     79.3   0.63    14.0     0.00
    0.952  37.39009 -122.28452          NaN      523.0     86.0   0.63    14.0     0.00
    0.955  37.38994 -122.28480          NaN      522.0     86.4   0.62    14.0     0.00
    0.959  37.38980 -122.28509          NaN      523.0     85.4   0.68    16.0     0.00
    0.962  37.38965 -122.28537          NaN      529.0     80.1   0.68    16.0     0.00
    0.966  37.38950 -122.28566          NaN      537.0     73.4   0.73    22.0     0.00
    0.969  37.38935 -122.28594          NaN      541.0     71.4   0.71    24.0     0.00
    0.973  37.38921 -122.28623          NaN      545.0     70.0   0.68    21.0     0.00
    0.976  37.38906 -122.28651          NaN      552.0     66.2   0.69    26.0     0.00
    0.980  37.38891 -122.28680          NaN      559.0     63.2   0.64    22.0     0.00
    0.983  37.38876 -122.28708          NaN      570.0     56.7   0.65    27.0     0.00
    0.987  37.38862 -122.28736          NaN      583.0     48.7   0.66    25.0     0.00
    0.990  37.38847 -122.28765          NaN      597.0     40.6   0.63    21.0     0.00
    0.994  37.38832 -122.28793          NaN      614.0     30.1   0.64    18.0     0.00
    0.997  37.38817 -122.28822          NaN      627.0     24.4   0.58    12.0     0.00
    1.000  37.38806 -122.28844         23.0      634.0     23.0   0.56     9.0     0.00

</div>

</div>

<div class="cell code">

``` python
```

</div>
