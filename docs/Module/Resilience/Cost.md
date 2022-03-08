[[/Module/Resilience/Cost]] -- Resilience cost measurement

# Synopsis

GLM:

~~~
module resilience
{
    metrics_cost_data "metrics_cost_data.csv";    
}
class cost {
    char256 asset_class;
    double value[$];
}
object cost 
{
    asset_class ASSET_CLASS;
    value INITIAL_VALUE $;
}
~~~

# Description

The `cost` object compute the cost of outages on the `powerflow` class `ASSET_CLASS` for violations of current, voltage, power, thermal, or control limits.  If a violation is detected the cost impact is calculated and added to the parent metric object's `cost` metric. 

Cost data for each asset class must be provided in the `metrics_cost_data` file.  The format of the metric cost data must be as follows:

~~~
classname,violation,fixedcost,variablecost
CLASS_1,VIOLATIONS_1,FIXEDCOST_1,VARIABLECOST_1
CLASS_2,VIOLATIONS_2,FIXEDCOST_2,VARIABLECOST_2
...
CLASS_N,VIOLATIONS_N,FIXEDCOST_N,VARIABLECOST_N
~~~

where the `classname` must be a `powerflow` module class, the `violations` field be a valid `powerflow_object` keyword value for the `violation_detected` property.  Values for multiple violations can be summed, e.g., `CURRENT|VOLTAGE|POWER` is represented as `7`.  The `fixedcost` field is given is `$` for each violation event, regardless of duration.  The `variablecost` field is given in `$/h`.

## Properties

### `char256 asset_class`

The powerflow class which is monitored for violations of current, voltage, power, thermal, or control limits.

### `double value[$]`

The total number of events observed.

# See also

* [[/Module/Resilience/Impact]]
* [[/Module/Resilience/Metrics]]
* [[/Module/Resilience/Outage]]
