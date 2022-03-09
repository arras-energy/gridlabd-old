[[/Module/Resilience/Outage]] -- Resilience outage measurement

# Synopsis

GLM:

~~~
class outage {
    char256 asset_class;
    double value[unit.h];
}
object outage 
{
    asset_class ASSET_CLASS;
    value INITIAL_VALUE unit.h;
}
~~~

# Description

The `outage` object monitors all the powerflow objects of ASSET_CLASS for violations of current, voltage, power, thermal, or control limits.  If a violation is detected parent object's `outage` metric is incremented by the duration of the outage in hours. 

## Properties

### `asset_class node`

The powerflow module asset class which is monitored for violations of current, voltage, power, thermal, or control limits.

### `double value[unit.h]`

The total number of asset.hours of outage observed.


# See also

* [[/Module/Resilience/Cost]]
* [[/Module/Resilience/Metrics]]
* [[/Module/Resilience/Impact]]
