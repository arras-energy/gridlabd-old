[[/Module/Market]] -- Module market

# Synopsis
GLM:
~~~
  object supervisory_control {
    unit "<string>";
    period "<decimal> s";
    market_id "<integer>";
    nominal_frequency "<decimal> Hz";
    droop "<decimal> %";
    frequency_deadband "<decimal> Hz";
    PFC_mode "{OVER_UNDER_FREQUENCY,UNDER_FREQUENCY,OVER_FREQUENCY}";
    bid_sort_mode "{VOLTAGE_EXTREMES,VOLTAGE_DEVIAION_FROM_NOMINAL,POWER_DECREASING,POWER_INCREASING,NONE}";
  }
~~~

# Description

TODO

## Properties

### `unit`
~~~
  char32 unit;
~~~

Unit of quantity

### `period`
~~~
  double period[s];
~~~

Interval of time between market clearings

### `market_id`
~~~
  int32 market_id;
~~~

Unique identifier of market clearing

### `nominal_frequency`
~~~
  double nominal_frequency[Hz];
~~~

Nominal frequency

### `droop`
~~~
  double droop[%];
~~~

Droop value for the supervisor

### `frequency_deadband`
~~~
  double frequency_deadband[Hz];
~~~

Frequency deadband for assigning trigger frequencies

### `PFC_mode`
~~~
  enumeration {OVER_UNDER_FREQUENCY, UNDER_FREQUENCY, OVER_FREQUENCY} PFC_mode;
~~~

Operation mode of the primary frequency controller

### `bid_sort_mode`
~~~
  enumeration {VOLTAGE_EXTREMES, VOLTAGE_DEVIAION_FROM_NOMINAL, POWER_DECREASING, POWER_INCREASING, NONE} bid_sort_mode;
~~~

Determines how the bids into the market is sorted to contruct the PF curve

# Example

~~~
  object supervisory_control {
    unit "";
    period "0.0";
    market_id "0";
    nominal_frequency "0.0";
    droop "0.0";
    frequency_deadband "0.0";
    PFC_mode "0";
    bid_sort_mode "0";
  }
~~~

# See also
* [[/Module/Market]]

