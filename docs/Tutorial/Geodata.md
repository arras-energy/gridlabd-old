::: {.cell .markdown}
# GridLAB-D GeoData Subcommand

As of version 4.2.21, HiPAS GridLAB-D supports the handling of
geographic data.

This command checks the version of GridLAB-D and loads the python module
we will use.
:::

::: {.cell .code execution_count="1"}
``` {.python}
!gridlabd --version='-ge 4.2.21'
from IPython.display import Image
import pandas, geopandas
```
:::

::: {.cell .markdown}
The `geodata` subcommand uses the general syntax
`gridlabd geodata OPTIONS DIRECTIVE [ARGUMENTS]`.
:::

::: {.cell .markdown}
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
:::

::: {.cell .markdown}
The `create` directive is used to create a new geodata entity. The
general syntax is `gridlabd geodata create LOCATIONS ...`.

There are two methods of introducing locations. The first method
introduces one or more `latitude,longitude` tuples directly in the
command line. For example, the following command creates a geodata
entity with the approximate location of SLAC\'s main gate.
:::

::: {.cell .code execution_count="2"}
``` {.python}
!gridlabd geodata create 37.415,-122.20
```

::: {.output .stream .stdout}
    id,latitude,longitude
    0,37.415,-122.2
:::
:::

::: {.cell .markdown}
Multiple locations can be introduced by adding them to the command line,
for example:
:::

::: {.cell .code execution_count="3"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.21
```

::: {.output .stream .stdout}
    id,latitude,longitude
    0,37.41,-122.2
    1,37.42,-122.21
:::
:::

::: {.cell .markdown}
The second method uses an input file with locations and associated data,
such as this example CSV file:
:::

::: {.cell .code execution_count="4"}
``` {.python}
!head -n 4 path_example.csv
```

::: {.output .stream .stdout}
    latitude,longitude,configuration,pole_height
    37.415045141688054,-122.2056472090359,flat3,15.0
    37.414698020593065,-122.20848749028133,sideT,15.0
    37.414454093051745,-122.21044282065421,sideT,15.0
:::
:::

::: {.cell .markdown}
To use this file, the following command can be used:
:::

::: {.cell .code execution_count="5"}
``` {.python}
!gridlabd geodata create path_example.csv | head -n 4
```

::: {.output .stream .stdout}
    id,latitude,longitude,configuration,pole_height
    0,37.41505,-122.20565,flat3,15.0
    1,37.4147,-122.20849,sideT,15.0
    2,37.41445,-122.21044,sideT,15.0
:::
:::

::: {.cell .markdown}
Note that if multiple locations are provided they are sequenced in the
order in which they are presented, including if locations are provided
directly on the command line or from data files.
:::

::: {.cell .markdown}
If no location information is given, then the geodata is read from
`/dev/stdin`, e.g.,
:::

::: {.cell .code execution_count="6"}
``` {.python}
!head -n 4 path_example.csv | cut -f1-2 -d, | gridlabd geodata create
```

::: {.output .stream .stdout}
    id,latitude,longitude
    0,37.41505,-122.20565
    1,37.4147,-122.20849
    2,37.41445,-122.21044
:::
:::

::: {.cell .markdown}
Additional fields can be added using an expanded command-line syntax,
e.g.,
:::

::: {.cell .code execution_count="7"}
``` {.python}
!gridlabd geodata create name=obj1+latitude=37.4205+longitude=-122.2046 name=obj2+latitude=37.5205+longitude=-122.3046
```

::: {.output .stream .stdout}
    id,name,latitude,longitude
    0,obj1,37.4205,-122.2046
    1,obj2,37.5205,-122.3046
:::
:::

::: {.cell .markdown}
The default input delimiter is the `+` character. This can be changed
using the `--input_delimiter=STRING` option, e.g.,
:::

::: {.cell .code execution_count="8"}
``` {.python}
!gridlabd geodata create --input_delimiter=',' 'name=obj1,latitude=37.4205,longitude=-122.2046' 'name=obj2,latitude=37.5205,longitude=-122.3046'
```

::: {.output .stream .stdout}
    id,name,latitude,longitude
    0,obj1,37.4205,-122.2046
    1,obj2,37.5205,-122.3046
:::
:::

::: {.cell .markdown}
Note that the input delimiter must be set *before* it is used, and thus
it can be changed as command line data is presented.
:::

::: {.cell .markdown}
**Caveat**: The input delimiter can affect how strings are interpreted.
One common problem is the setting the input delimiter to any character
that is typically found in an address and then attempting to reverse
resolve the address using the `address` dataset. This can result in
corrupted addresses being use.
:::

::: {.cell .markdown}
## Some useful options
:::

::: {.cell .markdown}
The default precision with which latitudes and longitudes are output is
5 decimals, which is approximately 1 meter resolution. You can change
the precision with which latitudes and longitudes are output using the
`-p` or `--precision` option, e.g.,
:::

::: {.cell .code execution_count="9"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -p 4
```

::: {.output .stream .stdout}
    id,latitude,longitude
    0,37.41,-122.2
    1,37.42,-122.2
    2,37.42,-122.21
:::
:::

::: {.cell .markdown}
This option actually changes the `precision.geolocation` option, which
can also be set using the direct option set syntax, e.g.,
:::

::: {.cell .code execution_count="10"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 --precision.geolocation=4
```

::: {.output .stream .stdout}
    id,latitude,longitude
    0,37.41,-122.2
    1,37.42,-122.2
    2,37.42,-122.21
:::
:::

::: {.cell .markdown}
The default field separator for RAW output is a space. You can change
this to any string using the `--fieldsep STRING` option, e.g.,
:::

::: {.cell .code execution_count="11"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f RAW --fieldsep ','
```

::: {.output .stream .stdout}
    37.41,-122.2
    37.42,-122.2
    37.42,-122.21
:::
:::

::: {.cell .markdown}
This is equivalent to directly setting the field separator option using
`--field_separator=,`.
:::

::: {.cell .markdown}
Similarly the default record separator for RAW output is a newline. You
can change this to any string using the `--recordsep STRING` option,
e.g.,
:::

::: {.cell .code execution_count="12"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f RAW --fieldsep ',' --recordsep ';'
```

::: {.output .stream .stdout}
    37.41,-122.2;37.42,-122.2;37.42,-122.21
:::
:::

::: {.cell .markdown}
This is equivalent to directly setting the record separator option using
`--record_separator=4`.
:::

::: {.cell .markdown}
## Configurations

There are three locations where configuration settings are maintained:
(1) the system shared folder, (2) the user\'s home folder, and (3) the
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
:::

::: {.cell .markdown}
You can manage the current configuration using the `config` directive,
e.g., to set the local configuration parameter `name` to `value`, use
the `set` option
:::

::: {.cell .code execution_count="13"}
``` {.python}
!gridlabd geodata config set name local_value
```

::: {.output .stream .stdout}
    WARNING [geodata]: config file '/Users/david/Documents/github/slacgismo/gridlabd/gldcore/geodata/docs/geodata.conf' is not found
:::
:::

::: {.cell .markdown}
Note that if the file in which the parameter is stored does not already
exist, you will get a warning before it is created.
:::

::: {.cell .markdown}
To get the value, use the `get` option:
:::

::: {.cell .code execution_count="14"}
``` {.python}
!gridlabd geodata config get name
```

::: {.output .stream .stdout}
    local_value
:::
:::

::: {.cell .markdown}
To show all the configuration values, use the `show` option:
:::

::: {.cell .code execution_count="15"}
``` {.python}
!gridlabd geodata config show
```

::: {.output .stream .stdout}
    geodata_url='http://geodata.gridlabd.us/'
    output_format='CSV'
    path_join='outer'
    column_names='{'ID': 'id', 'UUID': 'uuid', 'LAT': 'latitude', 'LON': 'longitude', 'DIST': 'distance', 'HEAD': 'heading', 'LOC': 'location', 'POS': 'position'}'
    uuid_type='4'
    name='local_value'
:::
:::

::: {.cell .markdown}
To set a user configuration, use the `user.` prefix, e.g.,
:::

::: {.cell .code execution_count="16"}
``` {.python}
!gridlabd geodata config set user.name user_value
!gridlabd geodata config show
```

::: {.output .stream .stdout}
    geodata_url='http://geodata.gridlabd.us/'
    output_format='CSV'
    path_join='outer'
    column_names='{'ID': 'id', 'UUID': 'uuid', 'LAT': 'latitude', 'LON': 'longitude', 'DIST': 'distance', 'HEAD': 'heading', 'LOC': 'location', 'POS': 'position'}'
    uuid_type='4'
    name='local_value'
    user.name='user_value'
:::
:::

::: {.cell .markdown}
The same syntax is used for system configuration values, e.g.,
:::

::: {.cell .code execution_count="17"}
``` {.python}
!gridlabd geodata config set system.name system_value
!gridlabd geodata config show
```

::: {.output .stream .stdout}
    geodata_url='http://geodata.gridlabd.us/'
    output_format='CSV'
    path_join='outer'
    column_names='{'ID': 'id', 'UUID': 'uuid', 'LAT': 'latitude', 'LON': 'longitude', 'DIST': 'distance', 'HEAD': 'heading', 'LOC': 'location', 'POS': 'position'}'
    uuid_type='4'
    name='local_value'
    user.name='user_value'
    system.name='system_value'
:::
:::

::: {.cell .markdown}
To remove a value, use the `unset` option, e.g.,
:::

::: {.cell .code execution_count="18"}
``` {.python}
!gridlabd geodata config unset name
!gridlabd geodata config show
```

::: {.output .stream .stdout}
    geodata_url='http://geodata.gridlabd.us/'
    output_format='CSV'
    path_join='outer'
    column_names='{'ID': 'id', 'UUID': 'uuid', 'LAT': 'latitude', 'LON': 'longitude', 'DIST': 'distance', 'HEAD': 'heading', 'LOC': 'location', 'POS': 'position'}'
    uuid_type='4'
    user.name='user_value'
    system.name='system_value'
:::
:::

::: {.cell .markdown}
To remove all the local configuration values, simply delete the
`geodata.conf` file:
:::

::: {.cell .code execution_count="19"}
``` {.python}
!rm geodata.conf
```
:::

::: {.cell .markdown}
## Geodata formatting

The output format can be changed using the `-f` or `--format` option.
The valid formats are `CSV`, `JSON`, `RAW`, `POS`, `GLM`, `FIELD`,
`PLOT`, `GDF`, `XLSX`, and `TABLE`. `CSV` format is the default, and it
can be explicitly specified as follows:
:::

::: {.cell .code execution_count="20"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f CSV
```

::: {.output .stream .stdout}
    id,latitude,longitude
    0,37.41,-122.2
    1,37.42,-122.2
    2,37.42,-122.21
:::
:::

::: {.cell .markdown}
JSON output looks like this:
:::

::: {.cell .code execution_count="21"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f JSON
```

::: {.output .stream .stdout}
    {"0":{"latitude":37.41,"longitude":-122.2},"1":{"latitude":37.42,"longitude":-122.2},"2":{"latitude":37.42,"longitude":-122.21}}
:::
:::

::: {.cell .markdown}
JSON output can be structured using the `pandas` `to_json()` `orient`
options `index`, `split`, `records`, `columns`, `values`, or `table`,
e.g.,
:::

::: {.cell .code execution_count="22"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f JSON --json.orient=table
```

::: {.output .stream .stdout}
    {"schema":{"fields":[{"name":"id","type":"integer"},{"name":"latitude","type":"number"},{"name":"longitude","type":"number"}],"primaryKey":["id"],"pandas_version":"0.20.0"},"data":[{"id":0,"latitude":37.41,"longitude":-122.2},{"id":1,"latitude":37.42,"longitude":-122.2},{"id":2,"latitude":37.42,"longitude":-122.21}]}
:::
:::

::: {.cell .markdown}
Other JSON options include `data_format`, `double_precision`,
`force_ascii`, and `date_unit`. See
<https://pandas.pydata.org/docs/reference/api/pandas.DataFrame.to_json.html>
for details.
:::

::: {.cell .markdown}
RAW output is generated as follows
:::

::: {.cell .code execution_count="23"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f RAW
```

::: {.output .stream .stdout}
    37.41 -122.2
    37.42 -122.2
    37.42 -122.21
:::
:::

::: {.cell .markdown}
TABLE output is generated for easy reading:
:::

::: {.cell .code execution_count="24"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f TABLE
```

::: {.output .stream .stdout}
        latitude  longitude
    id                     
    0      37.41    -122.20
    1      37.42    -122.20
    2      37.42    -122.21
:::
:::

::: {.cell .markdown}
Output formats typically can include an ordered field list, such as
:::

::: {.cell .code execution_count="25"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -f RAW:longitude,latitude --fieldsep ,
```

::: {.output .stream .stdout}
    -122.2,37.41
    -122.2,37.42
    -122.21,37.42
:::
:::

::: {.cell .markdown}
## Geodata indexing

The output can be indexed using one of several standards keys, or keys
from the data. The default key is `id` which is the row number, as seen
in the examples so far. This key can be explicitly specified as follows:
:::

::: {.cell .code execution_count="26"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -k id
```

::: {.output .stream .stdout}
    id,latitude,longitude
    0,37.41,-122.2
    1,37.42,-122.2
    2,37.42,-122.21
:::
:::

::: {.cell .markdown}
The `location` key generates a geohash code:
:::

::: {.cell .code execution_count="27"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -k location
```

::: {.output .stream .stdout}
    location,latitude,longitude,id
    9q9hg629j97y,37.41,-122.2,0
    9q9hgk0em9ef,37.42,-122.2,1
    9q9hgh17k9e4,37.42,-122.21,2
:::
:::

::: {.cell .markdown}
The `position` key generates a distance index, treating the data rows as
a series of waypoints along a path. When this key is used, a distance
and a heading column are also generated.
:::

::: {.cell .code execution_count="28"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -k position
```

::: {.output .stream .stdout}
    position,latitude,longitude,id,distance,heading
    0,37.41,-122.2,0,0.0,
    1112,37.42,-122.2,1,1112.0,0.0
    1995,37.42,-122.21,2,1995.0,270.0
:::
:::

::: {.cell .markdown}
Any field or set of fields may be used for indexing, e.g.,
:::

::: {.cell .code execution_count="29"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -k latitude,longitude
```

::: {.output .stream .stdout}
    latitude,longitude,id
    37.41,-122.2,0
    37.42,-122.2,1
    37.42,-122.21,2
:::
:::

::: {.cell .markdown}
Universally unique identifiers can be also be generated using the
special `uuid` key, e.g.,
:::

::: {.cell .code execution_count="30"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -k uuid
```

::: {.output .stream .stdout}
    uuid,latitude,longitude,id
    4166371352c24373bdc724163dc98632,37.41,-122.2,0
    227580798edb42a4a9047e83fa1795a5,37.42,-122.2,1
    2d926493235142d4b1cb93fbd866cfc5,37.42,-122.21,2
:::
:::

::: {.cell .markdown}
The default `uuid` type is a random number, i.e., `--uuid_type=4`. Using
`--uuid_type=1` generates a unique identified based on the host,
sequence number, and time, e.g.,
:::

::: {.cell .code execution_count="31"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -k uuid --uuid_type=1
```

::: {.output .stream .stdout}
    uuid,latitude,longitude,id
    5aea2b2ab5cc11eb8508acde48001122,37.41,-122.2,0
    5aea2bc0b5cc11eb8508acde48001122,37.42,-122.2,1
    5aea2bf2b5cc11eb8508acde48001122,37.42,-122.21,2
:::
:::

::: {.cell .markdown}
## Path waypoints

Waypoints can be generated along a path using the `-r` or `--resolution`
option. For example, 250-meter waypoints are generated using the
following syntax:
:::

::: {.cell .code execution_count="32"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -r 250 -f TABLE
```

::: {.output .stream .stdout}
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
:::
:::

::: {.cell .markdown}
This is equivalent to setting the `resolution` option value, e.g.,
:::

::: {.cell .code execution_count="33"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 --resolution=250 -f TABLE
```

::: {.output .stream .stdout}
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
:::
:::

::: {.cell .markdown}
The `distance` and `heading` columns are added to provide the distance
and heading to each waypoint from the last position. When using
waypoints, the key is automatically set to `position`, which provides
the distance from the first point. The use of other keys is not
supported. In addition, any waypoint added in the process is not
assigned a row `id` in order to protect the original row ids.

The index can be changed with the `-k` or `--key` options using a pipe.
Sometimes this is necessary when one option used implicitly precludes
the use of a desired key, such as when path resolution is used but
location is indexing is desired, e.g.,
:::

::: {.cell .code execution_count="34"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -r 250 | gridlabd geodata create -k location -f TABLE
```

::: {.output .stream .stdout}
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
:::
:::

::: {.cell .markdown}
## Output

By default all output is written to `/dev/stdout`. The output can be
written to a file using the `-o` or `--output` option, e.g.,
:::

::: {.cell .code execution_count="35"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -o /tmp/test.csv
!cat /tmp/test.csv
```

::: {.output .stream .stdout}
    id,latitude,longitude
    0,37.41,-122.2
    1,37.42,-122.2
    2,37.42,-122.21
:::
:::

::: {.cell .markdown}
GridLAB-D `geodata` entities can be output to GeoPandas using the `GDF`
output format. If the entity does not include any geometry data, it is
automatically created from the latitude and longitude fields, e.g.,
:::

::: {.cell .code execution_count="36"}
``` {.python}
!gridlabd geodata create 37.410,-122.20 37.420,-122.20 37.420,-122.21 -o /tmp/test.gdf -f GDF
geopandas.read_file("/tmp/test.gdf")
```

::: {.output .execute_result execution_count="36"}
```{=html}
<div>
<style scoped>
    .dataframe tbody tr th:only-of-type {
        vertical-align: middle;
    }

    .dataframe tbody tr th {
        vertical-align: top;
    }

    .dataframe thead th {
        text-align: right;
    }
</style>
<table border="1" class="dataframe">
  <thead>
    <tr style="text-align: right;">
      <th></th>
      <th>id</th>
      <th>latitude</th>
      <th>longitude</th>
      <th>geometry</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>0</th>
      <td>0</td>
      <td>37.41</td>
      <td>-122.20</td>
      <td>POINT (-122.20000 37.41000)</td>
    </tr>
    <tr>
      <th>1</th>
      <td>1</td>
      <td>37.42</td>
      <td>-122.20</td>
      <td>POINT (-122.20000 37.42000)</td>
    </tr>
    <tr>
      <th>2</th>
      <td>2</td>
      <td>37.42</td>
      <td>-122.21</td>
      <td>POINT (-122.21000 37.42000)</td>
    </tr>
  </tbody>
</table>
</div>
```
:::
:::

::: {.cell .markdown}

------------------------------------------------------------------------

# Datasets

The `merge` directive is used to merge a dataset into an existing
`geodata` entity. The general syntax is
`gridlabd geodata merge -D DATASET [OPTIONS ...]`
:::

::: {.cell .markdown}
## Distance

The `distance` dataset provide great-circle distance calculations. For
example, the following calculates the distance from the first point in
the series.
:::

::: {.cell .code execution_count="37"}
``` {.python}
!gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21
```

::: {.output .stream .stdout}
    id,latitude,longitude,distance
    0,37.41,-122.2,0.0
    1,37.42,-122.2,593.0
    2,37.42,-122.21,1260.0
:::
:::

::: {.cell .markdown}
The distance can be calculated relative to the previous location in the
series using the `--relative` options, e.g.,
:::

::: {.cell .code execution_count="38" scrolled="true"}
``` {.python}
!gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21 --relative
```

::: {.output .stream .stdout}
    id,latitude,longitude,distance
    0,37.41,-122.2,0.0
    1,37.42,-122.2,593.0
    2,37.42,-122.21,667.0
:::
:::

::: {.cell .markdown}
The `distance` dataset support different units, include `meters` or `m`,
`kilometers` or `km`, `feet` or `ft`, `yards` or `yd`, `miles` or `mi`.
For example, the following calculates the distances in feet:
:::

::: {.cell .code execution_count="39"}
``` {.python}
!gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21 --units=feet
```

::: {.output .stream .stdout}
    id,latitude,longitude,distance
    0,37.41,-122.2,0.0
    1,37.42,-122.2,1944.0
    2,37.42,-122.21,4134.0
:::
:::

::: {.cell .markdown}
Note that the default precision with which distances are calculated in 0
decimals. Thus the distance in `km` is rounded:
:::

::: {.cell .code execution_count="40"}
``` {.python}
!gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21 --units=km # precision too low
```

::: {.output .stream .stdout}
    id,latitude,longitude,distance
    0,37.41,-122.2,0.0
    1,37.42,-122.2,1.0
    2,37.42,-122.21,1.0
:::
:::

::: {.cell .markdown}
The precision can be changed when necessary, e.g.,
:::

::: {.cell .code execution_count="41"}
``` {.python}
!gridlabd geodata merge -D distance 37.410,-122.20 37.420,-122.20 37.420,-122.21 --units=km --precision.distance=2
```

::: {.output .stream .stdout}
    id,latitude,longitude,distance
    0,37.41,-122.2,0.0
    1,37.42,-122.2,0.59
    2,37.42,-122.21,1.26
:::
:::

::: {.cell .markdown}
## Address

You can use the `address` dataset to perform address resolution
operations, both to and from latitude and longitude.

To obtain the address at a location, use the following command
:::

::: {.cell .code execution_count="42"}
``` {.python}
!gridlabd geodata merge -D address 37.420,-122.20
```

::: {.output .stream .stdout}
    id,latitude,longitude,address
    0,37.42,-122.2,"Stanford Linear Accelerator Center National Accelerator Laboratory, Sand Hill Road, Menlo Park, San Mateo County, California, 94028, United States"
:::
:::

::: {.cell .markdown}
To find the latitude and longitude of an address, use the command:
:::

::: {.cell .code execution_count="43"}
``` {.python}
!gridlabd geodata merge -D address --reverse "2575 Sand Hill Rd, Menlo Park CA 94025"
```

::: {.output .stream .stdout}
    id,index,address,longitude,latitude
    0,0,"2575 Sand Hill Rd, Menlo Park CA 94025",-122.20457,37.42046
:::
:::

::: {.cell .markdown}
Note that incomplete addresses are usually resolved but may not provided
the expected result. For example, the following address is not
sufficient for proper resolution, but provides a location nonetheless:
:::

::: {.cell .code execution_count="44"}
``` {.python}
!gridlabd geodata merge -D address --reverse "Main St." # insufficient information for accurate results
```

::: {.output .stream .stdout}
    id,index,address,longitude,latitude
    0,0,Main St.,-79.30162,43.68904
:::
:::

::: {.cell .markdown}
You can perform both operations in series to resolve the \"official\"
address from an unofficial but complete address:
:::

::: {.cell .code execution_count="45"}
``` {.python}
!gridlabd geodata merge -D address --reverse "2575 Sand Hill Rd, Menlo Park CA" | gridlabd geodata merge -D address
```

::: {.output .stream .stdout}
    id,index,address,longitude,latitude
    0,0,"Stanford Linear Accelerator Center National Accelerator Laboratory, Sand Hill Road, Menlo Park, San Mateo County, California, 94028, United States",-122.20118,37.41546
:::
:::

::: {.cell .markdown}
## Elevation

Elevation data can be obtained using the `elevation` data set, e.g.,
:::

::: {.cell .code execution_count="46"}
``` {.python}
!gridlabd geodata merge -D elevation 37.410,-122.20 37.420,-122.20 37.420,-122.21
```

::: {.output .stream .stdout}
    id,latitude,longitude,elevation
    0,37.41,-122.2,58.0
    1,37.42,-122.2,76.0
    2,37.42,-122.21,105.0
:::
:::

::: {.cell .markdown}
Elevations can be calculated in other units using the `--units=UNIT`
options, e.g.,
:::

::: {.cell .code execution_count="47"}
``` {.python}
!gridlabd geodata merge -D elevation 37.410,-122.20 37.420,-122.20 37.420,-122.21 --units=feet
```

::: {.output .stream .stdout}
    id,latitude,longitude,elevation
    0,37.41,-122.2,190.0
    1,37.42,-122.2,249.0
    2,37.42,-122.21,344.0
:::
:::

::: {.cell .markdown}
Valid units are `m` or `meters`, and `ft` or `feet`.

The precision of the elevations can be specified as well, e.g.,
:::

::: {.cell .code execution_count="48"}
``` {.python}
!gridlabd geodata merge -D elevation 37.410,-122.20 37.420,-122.20 37.420,-122.21 --precision.elevation=2 --units=feet
```

::: {.output .stream .stdout}
    id,latitude,longitude,elevation
    0,37.41,-122.2,190.29
    1,37.42,-122.2,249.34
    2,37.42,-122.21,344.49
:::
:::

::: {.cell .markdown}
## Utility

Utility data can be obtained using the `utility` dataset. This dataset
includes information from the Homeland Infrastructure Foundation-Level
Data (HIFLD). Note that this dataset uses a very large datafile that can
take some time to download the first time.

The name of the utility at a location is the default data result, e.g.,
:::

::: {.cell .code execution_count="49"}
``` {.python}
!gridlabd geodata merge -D utility 37.420,-122.20 
```

::: {.output .stream .stdout}
    id,latitude,longitude,NAME
    0,37.42,-122.2,PACIFIC GAS & ELECTRIC CO.
:::
:::

::: {.cell .markdown}
Other data is available (see
<https://hifld-geoplatform.opendata.arcgis.com/datasets/electric-retail-service-territories>
for detail). A list of data available can be obtained from the dataset
help, e.g., using `gridlabd geodata help utility`.

To get the utility\'s summer and winter peak load and number of
customers and the year for which the data is obtained, use the command
:::

::: {.cell .code execution_count="50"}
``` {.python}
!gridlabd geodata merge -D utility 37.420,-122.20 --fields=WINTR_PEAK,SUMMR_PEAK,CUSTOMERS,YEAR
```

::: {.output .stream .stdout}
    id,latitude,longitude,WINTR_PEAK,SUMMR_PEAK,CUSTOMERS,YEAR
    0,37.42,-122.2,12624.0,17263.0,5471786.0,2018.0
:::
:::

::: {.cell .markdown}
The `utility` dataset also contains geometry data that can be used to
generate maps. This data is included when the `--geometry` option is
specified, e.g.,
:::

::: {.cell .code execution_count="51"}
``` {.python}
!gridlabd geodata merge -D utility 37.420,-122.20 --geometry -f PLOT -o /tmp/utility.png --plot.figsize=10,5
Image("/tmp/utility.png")
```

::: {.output .execute_result execution_count="51"}
![](1f3dc876b6d8f3e42eebe70dba2d97e427eb4b79.png)
:::
:::

::: {.cell .markdown}
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
-   `--global_horizontal_irradiance=FLOAT` specified in W/m\^2, with a
    default of 1000 W/m\^2.
-   `--ground_reflectance=FLOAT` specified per unit, with a default of
    0.3 pu.
-   `--ice_density=FLOAT` specified in kg/m\^3, with a default of 915
    kg/m\^3.
-   `--nominal_temperature=FLOAT` specified in Celcius, with a default
    of 15 C.
-   `--cable_type=STRING` specified as a string lookup in the cable type
    file, with no default value.
-   `--elevation=FLOAT` specified in m above sea level, with a default
    of 0 m.

For example, the cable type has no default and a reference to a known
cable type can be given on the command line, e.g.,
:::

::: {.cell .code execution_count="52" scrolled="false"}
``` {.python}
!gridlabd geodata merge -D powerline path_example.csv --cable_type="TACSR/AC 610mm^2" -r 250 -f TABLE:linesag
```

::: {.output .stream .stdout}
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
:::
:::

::: {.cell .markdown}
Known cable types are listed in the `geodata_powerline_cabletypes.csv`,
which contains the following fields:
:::

::: {.cell .code execution_count="53"}
``` {.python}
print("\n".join(pandas.read_csv("../geodata_powerline_cabletypes.csv").columns.to_list()))
```

::: {.output .stream .stdout}
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
:::
:::

::: {.cell .markdown}
The first record contains the following values
:::

::: {.cell .code execution_count="54"}
``` {.python}
pandas.read_csv("../geodata_powerline_cabletypes.csv",nrows=1).transpose()
```

::: {.output .execute_result execution_count="54"}
```{=html}
<div>
<style scoped>
    .dataframe tbody tr th:only-of-type {
        vertical-align: middle;
    }

    .dataframe tbody tr th {
        vertical-align: top;
    }

    .dataframe thead th {
        text-align: right;
    }
</style>
<table border="1" class="dataframe">
  <thead>
    <tr style="text-align: right;">
      <th></th>
      <th>0</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>id</th>
      <td>TACSR/AC 610mm^2</td>
    </tr>
    <tr>
      <th>diameter</th>
      <td>0.0342</td>
    </tr>
    <tr>
      <th>unit_weight</th>
      <td>21.56</td>
    </tr>
    <tr>
      <th>rated_tensile_strength</th>
      <td>72000.0</td>
    </tr>
    <tr>
      <th>elasticity</th>
      <td>71800000000.0</td>
    </tr>
    <tr>
      <th>thermal_expansion</th>
      <td>0.000021</td>
    </tr>
    <tr>
      <th>conductor_crosssection_area</th>
      <td>0.000692</td>
    </tr>
    <tr>
      <th>resistivity</th>
      <td>0.0039</td>
    </tr>
    <tr>
      <th>nominal_resistance</th>
      <td>0.000046</td>
    </tr>
    <tr>
      <th>nomimal_temperature</th>
      <td>15.0</td>
    </tr>
    <tr>
      <th>voltage_rating</th>
      <td>230000.0</td>
    </tr>
    <tr>
      <th>reflectivity</th>
      <td>0.5</td>
    </tr>
    <tr>
      <th>emissivity</th>
      <td>0.6</td>
    </tr>
  </tbody>
</table>
</div>
```
:::
:::

::: {.cell .markdown}
When needed data is not provided or the value provided is invalid, a
warning is output and the requested values are not calculated, e.g.,
:::

::: {.cell .code execution_count="55"}
``` {.python}
!gridlabd geodata merge -D powerline path_example.csv -f TABLE:linesag # missing cable type
```

::: {.output .stream .stdout}
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
:::
:::

::: {.cell .code execution_count="56"}
``` {.python}
!gridlabd geodata merge -D powerline path_example.csv -f TABLE:linesag --cable_type="My cabletype" # bad cable type
```

::: {.output .stream .stdout}
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
:::
:::

::: {.cell .markdown}

------------------------------------------------------------------------

# Getting help

You can obtain help using the `help` directive, e.g.,
:::

::: {.cell .code execution_count="57" scrolled="false"}
``` {.python}
!gridlabd geodata help | head -n 10
```

::: {.output .stream .stdout}
    Help on module __main__:
    
    NAME
        __main__ - Syntax: gridlabd geodata OPTIONS DIRECTIVE [ARGUMENTS]
    
    DESCRIPTION
        The geodata command gathers and joins geographic data. The geodata subcommand
        uses directives that are documented in the DIRECTIVES section below.
        
        In general geodata is used to acquire geographic information at a location or
:::
:::

::: {.cell .code execution_count="58"}
``` {.python}
!gridlabd geodata help distance | head -n 10
```

::: {.output .stream .stdout}
    Help on module geodata_distance:
    
    NAME
        geodata_distance - GridLAB-D Geodata Distance Package
    
    DESCRIPTION
        The distance package computes the shortest distance between consecutive
        positions.
        
        OPTIONS
:::
:::

::: {.cell .markdown}

------------------------------------------------------------------------

# Output Options
:::

::: {.cell .markdown}
## Verbose
:::

::: {.cell .markdown}
The `-v` or `--verbose` option can be used to get additional output
about the stages of processing, e.g.,
:::

::: {.cell .code execution_count="59"}
``` {.python}
!gridlabd geodata merge -D distance 37,-122 38,-122 -v --units=km --precision.distance=3
```

::: {.output .stream .stdout}
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
            "id": 0
        },
        "resolution": 0,
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
        "method": "haversine"
    }
    id,latitude,longitude,distance
    0,37.0,-122.0,0.0
    1,38.0,-122.0,58.924
:::
:::

::: {.cell .markdown}
## Silent
:::

::: {.cell .markdown}
The `-s` or `--silent` option suppresses error messages while still
returning an error condition, e.g.,
:::

::: {.cell .code execution_count="60"}
``` {.python}
!gridlabd geodata merge -D distance 37,-122 38,-122 --badoption -s || echo "Exit code $?"
```

::: {.output .stream .stdout}
    Exit code 4
:::
:::

::: {.cell .markdown}
## Warning
:::

::: {.cell .markdown}
The `-w` or `--warning` option suppresses warning messages, e.g.,
:::

::: {.cell .code execution_count="61"}
``` {.python}
!gridlabd geodata merge -D distance 37,-122 38,-122 -r 100 -k location -w
```

::: {.output .stream .stdout}
    location,latitude,longitude,id,distance
    9q94rzdk9gyt,37.0,-122.0,0,0.0
    9qc0pvdq1cqt,38.0,-122.0,1,58924.0
:::
:::

::: {.cell .markdown}
If an error is desired instead of a warning, use the
`--warning_as_error` option, e.g.,
:::

::: {.cell .code execution_count="62"}
``` {.python}
!gridlabd geodata merge -D distance 37,-122 38,-122 -r 100 -k location --warning_as_error # key warning is an error
```

::: {.output .stream .stdout}
    ERROR [geodata/distance]: using index 'location' cannot be used with '--resolution=100.0' option
:::
:::

::: {.cell .markdown}
## Debug
:::

::: {.cell .markdown}
The `-d` or `--debug` option can be used to obtain details about why an
error occurred. For example, the following command has an invalid
`lat,lon` tuple and results in a simple error message:
:::

::: {.cell .code execution_count="63"}
``` {.python}
!gridlabd geodata merge -D distance 37,-122 38,-122 --units=furlongs
```

::: {.output .stream .stdout}
    ERROR [geodata/distance]: unit 'furlongs' is not recognized
:::
:::

::: {.cell .markdown}
Using the debug option provides a more detailed traceback of the error
that can be helpful in diagnostic the problem, particularly when it
originates in a dataset package. The call arguments, options, and
configuration is also output after the traceback information.
:::

::: {.cell .code execution_count="64"}
``` {.python}
!gridlabd geodata merge -D distance 37,-122 38,-122 --units=furlongs -d # invalid unit
```

::: {.output .stream .stdout}
    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.21-210515-develop_geodata_subcommand/bin/gridlabd-geodata", line 844, in merge
        result = MODULE.apply(data,options,config,warning)
      File "/usr/local/opt/gridlabd/4.2.21-210515-develop_geodata_subcommand/share/gridlabd/geodata_distance.py", line 113, in apply
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
            "id": 0
        },
        "resolution": 0,
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
        "method": "haversine"
    }
:::
:::

::: {.cell .markdown}
Note that using the debug flag also enables traceback of warnings from
inside python modules, but may not traceback warnings from geodata
packages. If this occurs, use the `--warning_as_error` option.
:::

::: {.cell .markdown}
# Configurations versus Options
:::

::: {.cell .markdown}
Configurations can be set in advance using configuration files and the
`config` directive, or they can be specified at the command line.
Options can only be specified on the command line.

The current `geodata` configuration values are obtained using the
`--show_config` command line option:
:::

::: {.cell .code execution_count="65"}
``` {.python}
!gridlabd geodata --show_config
```

::: {.output .stream .stdout}
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
:::
:::

::: {.cell .markdown}
This list will always include all the `system`, `user`, and `local`
configuration items.
:::

::: {.cell .markdown}
The valid `geodata` option values are obtained using a similar command
for options, e.g.,
:::

::: {.cell .code execution_count="66"}
``` {.python}
!gridlabd geodata --show_options
```

::: {.output .stream .stdout}
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
    --resolution=0
    --json.orient='index'
    --json.data_format='iso'
    --json.double_precision=10
    --json.force_ascii=True
    --json.date_unit='s'
:::
:::

::: {.cell .markdown}

------------------------------------------------------------------------

# GridLAB-D Models
:::

::: {.cell .markdown}
The `geodata` subcommand can be used from within GridLAB-D model files
(GLM) files. All GridLAB-D objects have a header attribute for latitude
and longitude, and thus can provide the data needed to perform `geodata`
create and merge operations.

The following step-by-step example illustrates how address resolution
can be performed on a GLM object.
:::

::: {.cell .markdown}
## Step 1: Add the needed geodata fields to the affected object classes
:::

::: {.cell .raw}
```{=ipynb}
module residential;
class house
{
    char1024 address;
}
```
:::

::: {.cell .markdown}
The `class` declaration adds the `address` field to the definition of
`house` provided by the `residential` module.
:::

::: {.cell .markdown}
## Step 2: Create the needed objects with the required data
:::

::: {.cell .raw}
```{=ipynb}
object 
{
    address "2575 Sand Hill Rd., Menlo Park, CA, 94025";
}
```
:::

::: {.cell .markdown}
## Step 3: Write the CSV data file needed to create the `geodata` entity
:::

::: {.cell .raw}
```{=ipynb}
#write /tmp/house_address.csv house:address
```
:::

::: {.cell .markdown}
The `#write` macro generates a CSV field of all the `house` objects with
only the `address` fields, which looks like this:
:::

::: {.cell .raw}
```{=ipynb}
id,class,name,address
0,house,"house:0","2575 Sand Hill Rd., Menlo Park, CA"
```
:::

::: {.cell .markdown}
## Step 4: Run the desired `geodata` operation
:::

::: {.cell .raw}
```{=ipynb}
#geodata merge -D address /tmp/house_address.csv --reverse --format GLM:@latitude,longitude -o /tmp/house_latlon.glm
```
:::

::: {.cell .markdown}
The command merges the `address` dataset into the data from the CSV
file, performing a reverse address resolution (i.e., address to
location), and outputs the result to a GLM file. The `GLM` format
options specified only the `latitude` and `longitude` fields are to be
generated. The `@` symbol reformats the GLM output using `modify`
directives instead of the default `object` directives. This is necessary
to update existing objects rather than creating new ones. The resulting
file looks like this:
:::

::: {.cell .raw}
```{=ipynb}
modify house:0.longitude "-122.20118";
modify house:0.latitude "37.41546";
```
:::

::: {.cell .markdown}
## Step 5: Include the updated values
:::

::: {.cell .raw}
```{=ipynb}
#include /tmp/house_latlon.glm
```
:::

::: {.cell .markdown}
Now the object have the latitude and longitude data set based on the
address provided.
:::
