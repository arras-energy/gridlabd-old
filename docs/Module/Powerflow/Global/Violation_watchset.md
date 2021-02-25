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

# See also

* [[/Module/Powerflow]]
* [[/Module/Powerflow/Global/Violation_active]]
* [[/Module/Powerflow/Global/Violation_count]]
* [[/Module/Powerflow/Global/Violation_record]]
