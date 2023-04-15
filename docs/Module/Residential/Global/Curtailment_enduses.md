[[/Module/Residential/Global/Curtailment_enduses]] -- Global enduse that are curtailable

# Synopsis

Shell:

~~~
bash$ gridlabd -D|--define residential::curtailment_enduses=LIGHTS|PLUGS|DISHWASHER|MICROWAVE|FREEZER|REFRIGERATION|RANGE|EVCHARGER|WATERHEATER|CLOTHESWASHER|DRYER|SUMP
~~~

GLM:

~~~
#set residential::curtailment_enduses=LIGHTS|PLUGS|DISHWASHER|MICROWAVE|FREEZER|REFRIGERATION|RANGE|EVCHARGER|WATERHEATER|CLOTHESWASHER|DRYER|SUMP
~~~

# Description

Specify whether the global enduse curtailment flag. The default value is `DISHWASHER|MICROWAVE|RANGE|EVCHARGER|CLOTHESWASHER|DRYER`.

# See also

* [[/Module/Residential/House]]
