[[/Module/Powerflow/Global/Violation_active]] -- Count the currently active rating and limit violations

# Synopsis

GLM:

~~~
#print ${powerflow::violation_active}
#set powerflow::violation_active=0
~~~

# Description

When current, voltage, power, thermal, or control ratings or limits are violated a warning is normally generated on the warning stream.  If the module global `violation_record` is set to a valid filename, then no warning is generated, and instead a record is generated in CSV format to the specified filename.  Each time a violation is detected the global `powerflow::violation_active` is incremented. When the violation is not longer detected, the global `powerflow::violation_active` is decremented.

# See also

* [[/Module/Powerflow]]
* [[/Module/Powerflow/Global/Violation_record]]
* [[/Module/Powerflow/Global/Violation_count]]
