[[/Module/Powerflow/Global/Violation_watchset]] -- Set of objects classes to watch for violations

# Synopsis

GLM:

~~~
module powerflow
{
	violation_watchset {NONE|LOAD|NODE|LINE|XFRM|VREG|LINK};
}
~~~

# Description

When `violation_watchset` is not `NONE` then if violations are being recorded, the parameter determine which classes of objects are considered when detecting and recording violations.

| Value  | Description 
| ------ | ----------- 
| `NONE` | No violations will be detected by the powerflow solver
| `LOAD` | Load bus voltage violations will be detected
| `NODE` | All node bus voltage violations will be detected
| `LINE` | Line current limit violations will be detected 
| `XFRM` | Transformer thermal limit violation will be detected
| `VREG` | Voltage regulator control limit violations will be detected
| `LINK` | All link limit violations will be detected

# See also

* [[/Module/Powerflow]]
* [[/Module/Powerflow/Global/Violation_active]]
* [[/Module/Powerflow/Global/Violation_count]]
* [[/Module/Powerflow/Global/Violation_record]]
