[[/Module/Powerflow/Voltdump]] -- Voltage dump object

# Synopsis
GLM
~~~
  object voltdump {
    filemode "w";
    filename "";
    interval "60";
    maxcount "0";
  }
~~~

# Description

The voltdump object produces a dump of the voltages at the nodes. This can be used to determine the cause of convergence problems.

## Properties

### `filemode`
~~~
  char8 filemode;
~~~
Specifies whether the dump is rewritten each time a new timestamp is generated.  Use `w` to the have object write a new dump file each time. Use `a` to append to the new dump to the end of file when a new timestamp is generated.

### `interval`
~~~
interval 
~~~
Sets the interval at which the dump is executed.

# See also

* [[/Module/Powerflow/Currdump]]
