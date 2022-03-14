[[/Module/Resilience/Metrics]] -- Resilience collection

# Synopsis

GLM:

~~~
module resilience
{
	double default_impact_duration[h];
	char1024 metrics_cost_data;
	set {QUIET=65536, WARNING=131072, DEBUG=262144, VERBOSE=524288} message_flags; 
}
class metrics {
	double outage[unit*day]; 
	double cost[$]; 
	int32 impact; 
	char1024 report_file; 
	enumeration {ANNUALLY=0, SEASONALLY=1, MONTHLY=2, WEEKLY=3, DAILY=4} report_frequency; 
}
object metrics
{
	report_file "test_metrics_report.csv";
	report_frequency "MONTHLY";
}
~~~

# Description

The `metrics` objects collects resilience metrics data from objects that observe outage, repair cost, and emergency services impacts.

## Properties

### `double outage[unit*day]`

The total number of customer outage days. The initial value is 0.0 by default. Every child `outage` object will increase this property if it observes an outage.

### `double cost[$]`

The total repair cost. The initial value is 0.0 by default. Every child `cost` object will increase this property if it observes a repair.

### `int32 impact`

The total emergency services impacted more than 48 hours per event. The initial value is 0.0 by default. Every `impact` object will increase this property if it observes a prolonged outage of a designated meter.

### `char1024 report_file`

The file in which metric data is recorded.

### `enumeration {ANNUALLY=0, SEASONALLY=1, MONTHLY=2, WEEKLY=3, DAILY=4} report_frequency`

The frequency with which metric data is recorded.

## Globals

### `double default_impact_duration[h]`

Specifies the required duration of an emergency services outage to be considered.

### `char1024 metrics_cost_data`

Specifies the metrics cost data file to use when computing outage costs.  The file must be provided if `cost` metrics are used.

###	`set {QUIET=65536, WARNING=131072, DEBUG=262144, VERBOSE=524288} message_flags`

Specified the output message streams enabled in the module.

# See also

* [[/Module/Resilience/Cost]]
* [[/Module/Resilience/Impact]]
* [[/Module/Resilience/Outage]]

