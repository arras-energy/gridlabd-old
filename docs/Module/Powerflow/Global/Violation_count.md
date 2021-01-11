[[/Module/Powerflow/Global/Violation_count]] -- Count the number rating and limit violations

# Synopsis

GLM:

~~~
#print ${powerflow::violation_count}
#set powerflow::violation_count=0
~~~

# Description

When current, voltage, power, thermal, or control ratings or limits are violated a warning is normally generated on the warning stream.  If the module global `violation_record` is set to a valid filename, then no warning is generated, and instead a record is generated in CSV format to the specified filename.  Each time a violation is detected the global `powerflow::violation_count` is incremented.

# See also

* [[/Module/Powerflow]]
* [[/Module/Powerflow/Global/Violation_record]]
* [[/Module/Powerflow/Global/Violation_active]]
