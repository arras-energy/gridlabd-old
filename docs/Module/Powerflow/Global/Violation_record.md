[[/Module/Powerflow/Global/Violation_record]] -- Enable recording of rating and limit violations

# Synopsis

Shell:

~~~
bash$ gridlabd -D|--define powerflow::violation_record=<filename>
~~~

GLM:

~~~
module powerflow 
{
	violation_record "<filename>";
}
~~~

# Description

When current, voltage, power, thermal, or control ratings or limits are violated a warning is normally generated on the warning stream.  If the module global `violation_record` is set to a valid filename, then no warning is generated, and instead a record is generated in CSV format to the specified filename.  The general format of a record is as follows

| Timestamp | Object | Type | Description |
| --------- | ------ | ---- | ----------- |
| `<datetime>` | `<object-name>` | `<violation-type>` | `<descriptive-message>` |

Valid violation types are the set

| Type | Value | Description |
| ---- | ----- | ----------- |
| NONE | 0     | No violation was reported |
| CURRENT | 1 | A current limit was violated |
| VOLTAGE | 2 | A voltage limit was violated |
| POWER | 4 | A power limit was violated |
| THERMAL | 8 | A thermal limit was violated |
| CONTROL | 16 | A control limit was violated |

# See also

* [[/Module/Powerflow]]
* [[/Module/Powerflow/Global/Violation_active]]
* [[/Module/Powerflow/Global/Violation_count]]
* [[/Module/Powerflow/Global/Violation_watchset]]