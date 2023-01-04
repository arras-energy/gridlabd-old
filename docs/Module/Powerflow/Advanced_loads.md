[[/Module/Powerflow/Advanced_loads]] -- Advanced load models

# Synopsis

~~~
object building { ... }
object industrial { ... }
object public_service { ... }
object agricultural { ... }
~~~

# Description

Advanced load models are derived from the `load` object and include load models based on customer category.  The advanced load models support the following:

| Customer category                    | Metering | Thermal model | Load composition | Load schedule | Weather sensitivities
| ------------------------------------ | -------- | ------------- | ---------------- | ------------- | -------------------
| [[/Module/Powerflow/Building~]]       | Yes      | Yes           | No               | No            | Temperature, solar  
| [[/Module/Powerflow/Industrial~]]     | Yes      | No            | Yes              | Yes           | None
| [[/Module/Powerflow/Public service~]] | Yes      | No            | Yes              | Yes           | Any source threshold
| [[/Module/Powerflow/Agricultural~]]   | Yes      | No            | Yes              | Yes           | Any source linear function

All load models support 3-phase loads. Building load models can be single phase but unbalanced split phase load is not modeled.

Sensitivity implementations differ when supported.  For buildings, temperature and solar are inputs to the quasi-steady state thermal solution.  For public service loads, any input can be used as a threshold value to turn on or off loads according to a schedule.  For agricultural loads, any input can be used as a linear function input to increase or decrease loads according to a schedule.

# See also

* [[/Module/Powerflow/Building]]
* [[/Module/Powerflow/Industrial]]
* [[/Module/Powerflow/Public service]]
* [[/Module/Powerflow/Agricultural]]
