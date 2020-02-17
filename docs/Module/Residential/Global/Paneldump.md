[[/Residential/Paneldump]] -- Residential panel circuit dump

# Synopsis

~~~
module residential {
  paneldump_interval interval;
  paneldump_resolution resolution;
  paneldump_filename filename;
}
~~~

# Description

As of 4.2 the residential module supports dumping of the panel circuits in house objects.  To enable panel dump set the module global `paneldump_interval` to a non-zero value to indicate how often the interval energy measurement is sampled.  If `paneldump_interval` is negative, a power sample is taken whenever the magnitude of the complex difference from the last sample exceeds `paneldump_resolution`.  The panel dump filename is specified by `paneldump_filename` and is `paneldump.csv` by default.

## Example

The format of the panel dump file is illustrated by the following example:

~~~
timestamp,name,enduse,real,reactive
2020-04-01 01:00:00 PDT,house:1,LIGHTS,0.2584,0.0647611
2020-04-01 01:00:00 PDT,house:1,PLUGS,0.1224,0.059281
2020-04-01 01:00:00 PDT,house:1,FREEZER,0.1065,0.0515803
2020-04-01 01:00:00 PDT,house:1,REFRIGERATOR,0.1365,0.06611
2020-04-01 01:00:00 PDT,house:1,WATERHEATER,0.8,0
2020-04-01 01:00:58 PDT,house:1,HVAC,6.56996,1.64659
2020-04-01 01:04:39 PDT,house:1,HVAC,0,0
2020-04-01 01:13:47 PDT,house:1,HVAC,6.56996,1.64659
~~~
