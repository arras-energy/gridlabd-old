[[/Module/Market/Double_controller]] -- Class double_controller

# Synopsis

GLM:

~~~
  object double_controller {
    thermostat_mode "{COOL,HEAT,OFF,INVALID}";
    last_mode "{COOL,HEAT,OFF,INVALID}";
    resolve_mode "{STICKY,DEADBAND,NONE}";
    setup_mode "{HOUSE,NONE}";
    bid_mode "{OFF,ON}";
    last_mode_timer "<integer>";
    cool_ramp_low "<decimal>";
    cool_ramp_high "<decimal>";
    cool_range_low "<decimal>";
    cool_range_high "<decimal>";
    cool_set_base "<decimal>";
    cool_setpoint "<decimal>";
    heat_ramp_low "<decimal>";
    heat_ramp_high "<decimal>";
    heat_range_low "<decimal>";
    heat_range_high "<decimal>";
    heat_set_base "<decimal>";
    heat_setpoint "<decimal>";
    temperature_name "<string>";
    cool_setpoint_name "<string>";
    cool_demand_name "<string>";
    heat_setpoint_name "<string>";
    heat_demand_name "<string>";
    load_name "<string>";
    total_load_name "<string>";
    deadband_name "<string>";
    state_name "<string>";
    market "<string>";
    market_period "<decimal>";
    bid_price "<decimal>";
    bid_quant "<decimal>";
    load "<string>";
    total "<string>";
    last_price "<decimal>";
    temperature "<decimal>";
    cool_bid "<decimal>";
    heat_bid "<decimal>";
    cool_demand "<decimal>";
    heat_demand "<decimal>";
    price "<decimal>";
    avg_price "<decimal>";
    stdev_price "<decimal>";
  }
~~~

# Description

TODO

## Properties

### `thermostat_mode`

~~~
  enumeration {COOL, HEAT, OFF, INVALID} thermostat_mode;
~~~

TODO

### `last_mode`

~~~
  enumeration {COOL, HEAT, OFF, INVALID} last_mode;
~~~

TODO

### `resolve_mode`

~~~
  enumeration {STICKY, DEADBAND, NONE} resolve_mode;
~~~

TODO

### `setup_mode`

~~~
  enumeration {HOUSE, NONE} setup_mode;
~~~

TODO

### `bid_mode`

~~~
  enumeration {OFF, ON} bid_mode;
~~~

TODO

### `last_mode_timer`

~~~
  int64 last_mode_timer;
~~~

TODO

### `cool_ramp_low`

~~~
  double cool_ramp_low;
~~~

TODO

### `cool_ramp_high`

~~~
  double cool_ramp_high;
~~~

TODO

### `cool_range_low`

~~~
  double cool_range_low;
~~~

TODO

### `cool_range_high`

~~~
  double cool_range_high;
~~~

TODO

### `cool_set_base`

~~~
  double cool_set_base;
~~~

TODO

### `cool_setpoint`

~~~
  double cool_setpoint;
~~~

TODO

### `heat_ramp_low`

~~~
  double heat_ramp_low;
~~~

TODO

### `heat_ramp_high`

~~~
  double heat_ramp_high;
~~~

TODO

### `heat_range_low`

~~~
  double heat_range_low;
~~~

TODO

### `heat_range_high`

~~~
  double heat_range_high;
~~~

TODO

### `heat_set_base`

~~~
  double heat_set_base;
~~~

TODO

### `heat_setpoint`

~~~
  double heat_setpoint;
~~~

TODO

### `temperature_name`

~~~
  char32 temperature_name;
~~~

TODO

### `cool_setpoint_name`

~~~
  char32 cool_setpoint_name;
~~~

TODO

### `cool_demand_name`

~~~
  char32 cool_demand_name;
~~~

TODO

### `heat_setpoint_name`

~~~
  char32 heat_setpoint_name;
~~~

TODO

### `heat_demand_name`

~~~
  char32 heat_demand_name;
~~~

TODO

### `load_name`

~~~
  char32 load_name;
~~~

TODO

### `total_load_name`

~~~
  char32 total_load_name;
~~~

TODO

### `deadband_name`

~~~
  char32 deadband_name;
~~~

TODO

### `state_name`

~~~
  char32 state_name;
~~~

TODO

### `market`

~~~
  object market;
~~~

The market to bid into

### `market_period`

~~~
  double market_period;
~~~

TODO

### `bid_price`

~~~
  double bid_price;
~~~

The bid price

### `bid_quant`

~~~
  double bid_quant;
~~~

The bid quantity

### `load`

~~~
  char32 load;
~~~

The current controlled load

### `total`

~~~
  char32 total;
~~~

The uncontrolled load (if any)

### `last_price`

~~~
  double last_price;
~~~

TODO

### `temperature`

~~~
  double temperature;
~~~

TODO

### `cool_bid`

~~~
  double cool_bid;
~~~

TODO

### `heat_bid`

~~~
  double heat_bid;
~~~

TODO

### `cool_demand`

~~~
  double cool_demand;
~~~

TODO

### `heat_demand`

~~~
  double heat_demand;
~~~

TODO

### `price`

~~~
  double price;
~~~

TODO

### `avg_price`

~~~
  double avg_price;
~~~

TODO

### `stdev_price`

~~~
  double stdev_price;
~~~

TODO

# Example

~~~
  object double_controller {
    thermostat_mode "0";
    last_mode "0";
    resolve_mode "0";
    setup_mode "0";
    bid_mode "0";
    last_mode_timer "0";
    cool_ramp_low "0.0";
    cool_ramp_high "0.0";
    cool_range_low "0.0";
    cool_range_high "0.0";
    cool_set_base "0.0";
    cool_setpoint "0.0";
    heat_ramp_low "0.0";
    heat_ramp_high "0.0";
    heat_range_low "0.0";
    heat_range_high "0.0";
    heat_set_base "0.0";
    heat_setpoint "0.0";
    temperature_name "";
    cool_setpoint_name "";
    cool_demand_name "";
    heat_setpoint_name "";
    heat_demand_name "";
    load_name "";
    total_load_name "";
    deadband_name "";
    state_name "";
    market_period "0.0";
    bid_price "0.0";
    bid_quant "0.0";
    load "";
    total "";
    last_price "0.0";
    temperature "0.0";
    cool_bid "0.0";
    heat_bid "0.0";
    cool_demand "0.0";
    heat_demand "0.0";
    price "0.0";
    avg_price "0.0";
    stdev_price "0.0";
  }
~~~

# See also

* [[/Module/Market]]

