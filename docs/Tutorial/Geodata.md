
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

<div class="cell code" execution_count="87">

``` python
bash% gridlabd --version='-ge 4.2.26'
bash% gridlabd requirements
```

<div class="output stream stdout">

    DEPRECATION: Configuring installation scheme with distutils config files is deprecated and will no longer work in the near future. If you are using a Homebrew or Linuxbrew Python, please see discussion at https://github.com/Homebrew/homebrew-core/issues/76621
    Requirement already satisfied: matplotlib_inline in /usr/local/lib/python3.9/site-packages (0.1.2)
    Requirement already satisfied: traitlets in /usr/local/lib/python3.9/site-packages (from matplotlib_inline) (5.0.5)
    Requirement already satisfied: ipython-genutils in /usr/local/lib/python3.9/site-packages (from traitlets->matplotlib_inline) (0.2.0)

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

<div class="cell code" execution_count="86">

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

<div class="output stream stdout">

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

<div class="output stream stdout">

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

</div>

</div>

<div class="cell markdown">

To get the value, use the `get` option:

</div>

<div class="cell code" execution_count="15">

``` python
bash% gridlabd geodata config get name
```

<div class="output stream stdout">

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'
    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'
    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'
    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

<div class="output stream stdout">

    rm: geodata.conf: No such file or directory

</div>

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'
    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'
    cat: /tmp/test.csv: No such file or directory

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

<div class="output stream stdout">

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

</div>

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'
    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

<div class="output stream stdout">

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'
    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

</div>

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

<div class="output stream stdout">

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

</div>

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

<div class="output error" ename="FileNotFoundError"
evalue="No such file or directory: '/tmp/utility.png'">

    ---------------------------------------------------------------------------
    TypeError                                 Traceback (most recent call last)
    /usr/local/Cellar/jupyterlab/3.1.4/libexec/lib/python3.9/site-packages/IPython/core/display.py in _data_and_metadata(self, always_both)
       1299         try:
    -> 1300             b64_data = b2a_base64(self.data).decode('ascii')
       1301         except TypeError:

    TypeError: a bytes-like object is required, not 'str'

    During handling of the above exception, another exception occurred:

    FileNotFoundError                         Traceback (most recent call last)
    /usr/local/Cellar/jupyterlab/3.1.4/libexec/lib/python3.9/site-packages/IPython/core/formatters.py in __call__(self, obj, include, exclude)
        968 
        969             if method is not None:
    --> 970                 return method(include=include, exclude=exclude)
        971             return None
        972         else:

    /usr/local/Cellar/jupyterlab/3.1.4/libexec/lib/python3.9/site-packages/IPython/core/display.py in _repr_mimebundle_(self, include, exclude)
       1288         if self.embed:
       1289             mimetype = self._mimetype
    -> 1290             data, metadata = self._data_and_metadata(always_both=True)
       1291             if metadata:
       1292                 metadata = {mimetype: metadata}

    /usr/local/Cellar/jupyterlab/3.1.4/libexec/lib/python3.9/site-packages/IPython/core/display.py in _data_and_metadata(self, always_both)
       1300             b64_data = b2a_base64(self.data).decode('ascii')
       1301         except TypeError:
    -> 1302             raise FileNotFoundError(
       1303                 "No such file or directory: '%s'" % (self.data))
       1304         md = {}

    FileNotFoundError: No such file or directory: '/tmp/utility.png'

</div>

<div class="output error" ename="FileNotFoundError"
evalue="No such file or directory: '/tmp/utility.png'">

    ---------------------------------------------------------------------------
    TypeError                                 Traceback (most recent call last)
    /usr/local/Cellar/jupyterlab/3.1.4/libexec/lib/python3.9/site-packages/IPython/core/display.py in _data_and_metadata(self, always_both)
       1299         try:
    -> 1300             b64_data = b2a_base64(self.data).decode('ascii')
       1301         except TypeError:

    TypeError: a bytes-like object is required, not 'str'

    During handling of the above exception, another exception occurred:

    FileNotFoundError                         Traceback (most recent call last)
    /usr/local/Cellar/jupyterlab/3.1.4/libexec/lib/python3.9/site-packages/IPython/core/formatters.py in __call__(self, obj)
        343             method = get_real_method(obj, self.print_method)
        344             if method is not None:
    --> 345                 return method()
        346             return None
        347         else:

    /usr/local/Cellar/jupyterlab/3.1.4/libexec/lib/python3.9/site-packages/IPython/core/display.py in _repr_png_(self)
       1318     def _repr_png_(self):
       1319         if self.embed and self.format == self._FMT_PNG:
    -> 1320             return self._data_and_metadata()
       1321 
       1322     def _repr_jpeg_(self):

    /usr/local/Cellar/jupyterlab/3.1.4/libexec/lib/python3.9/site-packages/IPython/core/display.py in _data_and_metadata(self, always_both)
       1300             b64_data = b2a_base64(self.data).decode('ascii')
       1301         except TypeError:
    -> 1302             raise FileNotFoundError(
       1303                 "No such file or directory: '%s'" % (self.data))
       1304         md = {}

    FileNotFoundError: No such file or directory: '/tmp/utility.png'

</div>

<div class="output execute_result" execution_count="61">

    <IPython.core.display.Image object>

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

</div>

</div>

<div class="cell code" execution_count="66">

``` python
bash% gridlabd geodata merge -D powerline path_example.csv -f TABLE:linesag --cable_type="My cabletype" # bad cable type
```

<div class="output stream stdout">

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

</div>

</div>

<div class="cell code" execution_count="68">

``` python
bash% gridlabd geodata help distance | head -n 10
```

<div class="output stream stdout">

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'
    Exit code 1

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

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

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'
    ERROR    [INIT] : GldMain::subcommand(format='%s/gridlabd-%s',...): command '/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata merge -D address /tmp/address.csv --reverse --format GLM:@latitude,longitude -o /tmp/latlon.glm
    ' returns code 1
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

<div class="cell code" execution_count="79" scrolled="true">

``` python
bash% gridlabd geodata merge -D elevation path_example.csv -r 250 \
| gridlabd geodata merge -D distance -f TABLE --relative --table.width=999 --table.max_rows=999
```

<div class="output stream stdout">

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'
    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

</div>

</div>

<div class="cell markdown">

# Example 3

Ths following example illustrates merging elevation, vegetation, and
powerline data.

</div>

<div class="cell code" execution_count="80" scrolled="true">

``` python
bash% gridlabd geodata merge -D elevation path_example.csv -r 30 --resolution_id \
| gridlabd geodata merge -D vegetation -w | grep -v WARNING \
| gridlabd geodata merge -D powerline --cable_type="TACSR/AC 610mm^2" --margin.vertical=2 -f TABLE:latitude,longitude,pole_height,elevation,linesag,cover,height,contact --table.width=999 --table.max_rows=999
```

<div class="output stream stdout">

    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'
    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'
    Traceback (most recent call last):
      File "/usr/local/opt/gridlabd/4.2.25-210808-develop_add_model_browser/bin/gridlabd-geodata", line 358, in <module>
        import matplotlib.pyplot as plt
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 2336, in <module>
        switch_backend(rcParams["backend"])
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 276, in switch_backend
        class backend_mod(matplotlib.backend_bases._Backend):
      File "/usr/local/lib/python3.9/site-packages/matplotlib/pyplot.py", line 277, in backend_mod
        locals().update(vars(importlib.import_module(backend_name)))
      File "/usr/local/Cellar/python@3.9/3.9.6/Frameworks/Python.framework/Versions/3.9/lib/python3.9/importlib/__init__.py", line 127, in import_module
        return _bootstrap._gcd_import(name[level:], package, level)
    ModuleNotFoundError: No module named 'matplotlib_inline'

</div>

</div>

<div class="cell code">

``` python
```

</div>
