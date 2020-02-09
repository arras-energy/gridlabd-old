[[/Module/Market/Controller]] -- Class controller

# Synopsis

GLM:

~~~
  object controller {
    simple_mode "{DOUBLE_RAMP,WATERHEATER,HOUSE_PRECOOL,HOUSE_PREHEAT,HOUSE_COOL,HOUSE_HEAT,NONE}";
    bid_mode "{PROXY,OFF,ON}";
    use_override "{ON,OFF}";
    ramp_low "<decimal> degF";
    ramp_high "<decimal> degF";
    range_low "<decimal>";
    range_high "<decimal>";
    target "<string>";
    setpoint "<string>";
    demand "<string>";
    load "<string>";
    total "<string>";
    market "<string>";
    state "<string>";
    avg_target "<string>";
    std_target "<string>";
    bid_price "<decimal>";
    bid_quantity "<decimal>";
    set_temp "<decimal> degF";
    base_setpoint "<decimal> degF";
    market_price "<decimal>";
    period "<decimal> s";
    control_mode "{DEV_LEVEL,DOUBLE_RAMP,RAMP}";
    resolve_mode "{SLIDING,DEADBAND}";
    slider_setting "<decimal>";
    slider_setting_heat "<decimal>";
    slider_setting_cool "<decimal>";
    override "<string>";
    heating_range_high "<decimal> degF";
    heating_range_low "<decimal> degF";
    heating_ramp_high "<decimal>";
    heating_ramp_low "<decimal>";
    cooling_range_high "<decimal> degF";
    cooling_range_low "<decimal> degF";
    cooling_ramp_high "<decimal>";
    cooling_ramp_low "<decimal>";
    heating_base_setpoint "<decimal> degF";
    cooling_base_setpoint "<decimal> degF";
    deadband "<string>";
    heating_setpoint "<string>";
    heating_demand "<string>";
    cooling_setpoint "<string>";
    cooling_demand "<string>";
    sliding_time_delay "<decimal> s";
    use_predictive_bidding "<string>";
    device_actively_engaged "<decimal>";
    cleared_market "<integer>";
    locked "<integer>";
    p_ON "<decimal>";
    p_OFF "<decimal>";
    p_ONLOCK "<decimal>";
    p_OFFLOCK "<decimal>";
    delta_u "<decimal>";
    regulation_market_on "<string>";
    regulation_market_off "<string>";
    fast_regulation_signal "<decimal> s";
    market_price_on "<decimal>";
    market_price_off "<decimal>";
    period_on "<decimal> s";
    period_off "<decimal> s";
    regulation_period "<integer>";
    r1 "<decimal>";
    mu0 "<decimal>";
    mu1 "<decimal>";
    average_target "<string>";
    standard_deviation_target "<string>";
    bid_id "<integer>";
    bid_delay "<integer>";
    thermostat_state "<string>";
    proxy_average "<decimal>";
    proxy_standard_deviation "<decimal>";
    proxy_market_id "<integer>";
    proxy_market2_id "<integer>";
    proxy_clear_price "<decimal> $";
    proxy_clear_price2 "<decimal> $";
    proxy_price_cap "<decimal>";
    proxy_price_cap2 "<decimal>";
    proxy_market_unit "<string>";
    proxy_initial_price "<decimal>";
    proxy_marginal_fraction "<decimal>";
    proxy_marginal_fraction2 "<decimal>";
    proxy_clearing_quantity "<decimal>";
    proxy_clearing_quantity2 "<decimal>";
    proxy_seller_total_quantity "<decimal>";
    proxy_seller_total_quantity2 "<decimal>";
    proxy_margin_mode "{PROB,DENY,NORMAL}";
    proxy_clearing_type "{NULL,FAILURE,EXACT,MARGINAL_PRICE,MARGINAL_BUYER,MARGINAL_SELLER}";
    proxy_clearing_type2 "{NULL,FAILURE,EXACT,MARGINAL_PRICE,MARGINAL_BUYER,MARGINAL_SELLER}";
  }
~~~

# Description

TODO

## Properties

### `simple_mode`
~~~
  enumeration {DOUBLE_RAMP, WATERHEATER, HOUSE_PRECOOL, HOUSE_PREHEAT, HOUSE_COOL, HOUSE_HEAT, NONE} simple_mode;
~~~

TODO

### `bid_mode`
~~~
  enumeration {PROXY, OFF, ON} bid_mode;
~~~

TODO

### `use_override`
~~~
  enumeration {ON, OFF} use_override;
~~~

TODO

### `ramp_low`
~~~
  double ramp_low[degF];
~~~

The comfort response below the setpoint

### `ramp_high`
~~~
  double ramp_high[degF];
~~~

The comfort response above the setpoint

### `range_low`
~~~
  double range_low;
~~~

The setpoint limit on the low side

### `range_high`
~~~
  double range_high;
~~~

The setpoint limit on the high side

### `target`
~~~
  char32 target;
~~~

The observed property (e.g., air temperature)

### `setpoint`
~~~
  char32 setpoint;
~~~

The controlled property (e.g., heating setpoint)

### `demand`
~~~
  char32 demand;
~~~

The controlled load when on

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

### `market`
~~~
  char32 market;
~~~

The market to bid into

### `state`
~~~
  char32 state;
~~~

The state property of the controlled load

### `avg_target`
~~~
  char32 avg_target;
~~~

TODO

### `std_target`
~~~
  char32 std_target;
~~~

TODO

### `bid_price`
~~~
  double bid_price;
~~~

The bid price

### `bid_quantity`
~~~
  double bid_quantity;
~~~

The bid quantity

### `set_temp`
~~~
  double set_temp[degF];
~~~

The reset value

### `base_setpoint`
~~~
  double base_setpoint[degF];
~~~

TODO

### `market_price`
~~~
  double market_price;
~~~

The current market clearing price seen by the controller.

### `period`
~~~
  double period[s];
~~~

Interval of time between market clearings

### `control_mode`
~~~
  enumeration {DEV_LEVEL, DOUBLE_RAMP, RAMP} control_mode;
~~~

TODO

### `resolve_mode`
~~~
  enumeration {SLIDING, DEADBAND} resolve_mode;
~~~

TODO

### `slider_setting`
~~~
  double slider_setting;
~~~

TODO

### `slider_setting_heat`
~~~
  double slider_setting_heat;
~~~

TODO

### `slider_setting_cool`
~~~
  double slider_setting_cool;
~~~

TODO

### `override`
~~~
  char32 override;
~~~

TODO

### `heating_range_high`
~~~
  double heating_range_high[degF];
~~~

TODO

### `heating_range_low`
~~~
  double heating_range_low[degF];
~~~

TODO

### `heating_ramp_high`
~~~
  double heating_ramp_high;
~~~

TODO

### `heating_ramp_low`
~~~
  double heating_ramp_low;
~~~

TODO

### `cooling_range_high`
~~~
  double cooling_range_high[degF];
~~~

TODO

### `cooling_range_low`
~~~
  double cooling_range_low[degF];
~~~

TODO

### `cooling_ramp_high`
~~~
  double cooling_ramp_high;
~~~

TODO

### `cooling_ramp_low`
~~~
  double cooling_ramp_low;
~~~

TODO

### `heating_base_setpoint`
~~~
  double heating_base_setpoint[degF];
~~~

TODO

### `cooling_base_setpoint`
~~~
  double cooling_base_setpoint[degF];
~~~

TODO

### `deadband`
~~~
  char32 deadband;
~~~

TODO

### `heating_setpoint`
~~~
  char32 heating_setpoint;
~~~

TODO

### `heating_demand`
~~~
  char32 heating_demand;
~~~

TODO

### `cooling_setpoint`
~~~
  char32 cooling_setpoint;
~~~

TODO

### `cooling_demand`
~~~
  char32 cooling_demand;
~~~

TODO

### `sliding_time_delay`
~~~
  double sliding_time_delay[s];
~~~

Time interval desired for the sliding resolve mode to change from cooling or heating to off

### `use_predictive_bidding`
~~~
  bool use_predictive_bidding;
~~~

TODO

### `device_actively_engaged`
~~~
  double device_actively_engaged;
~~~

TODO

### `cleared_market`
~~~
  int32 cleared_market;
~~~

TODO

### `locked`
~~~
  int32 locked;
~~~

TODO

### `p_ON`
~~~
  double p_ON;
~~~

TODO

### `p_OFF`
~~~
  double p_OFF;
~~~

TODO

### `p_ONLOCK`
~~~
  double p_ONLOCK;
~~~

TODO

### `p_OFFLOCK`
~~~
  double p_OFFLOCK;
~~~

TODO

### `delta_u`
~~~
  double delta_u;
~~~

TODO

### `regulation_market_on`
~~~
  char32 regulation_market_on;
~~~

The willing to change state from ON->OFF market to bid into for regulation case

### `regulation_market_off`
~~~
  char32 regulation_market_off;
~~~

The willing to change state from OFF->ON market to bid into for regulation case

### `fast_regulation_signal`
~~~
  double fast_regulation_signal[s];
~~~

The regulation signal that the controller compares against (i.e., how often is there a control action

### `market_price_on`
~~~
  double market_price_on;
~~~

The current market clearing price seen by the controller in ON->OFF regulation market

### `market_price_off`
~~~
  double market_price_off;
~~~

The current market clearing price seen by the controller  in OFF->ON regulation market

### `period_on`
~~~
  double period_on[s];
~~~

Interval of time between market clearings in ON->OFF regulation market

### `period_off`
~~~
  double period_off[s];
~~~

Interval of time between market clearings in  OFF->ON regulation market

### `regulation_period`
~~~
  int32 regulation_period;
~~~

Fast regulation signal period

### `r1`
~~~
  double r1;
~~~

Temporary diagnostic variable

### `mu0`
~~~
  double mu0;
~~~

Temporary diagnostic variable

### `mu1`
~~~
  double mu1;
~~~

Temporary diagnostic variable

### `average_target`
~~~
  char32 average_target;
~~~

TODO

### `standard_deviation_target`
~~~
  char32 standard_deviation_target;
~~~

TODO

### `bid_id`
~~~
  int64 bid_id;
~~~

TODO

### `bid_delay`
~~~
  int32 bid_delay;
~~~

TODO

### `thermostat_state`
~~~
  char32 thermostat_state;
~~~

The name of the thermostat state property within the object being controlled

### `proxy_average`
~~~
  double proxy_average;
~~~

TODO

### `proxy_standard_deviation`
~~~
  double proxy_standard_deviation;
~~~

TODO

### `proxy_market_id`
~~~
  int64 proxy_market_id;
~~~

TODO

### `proxy_market2_id`
~~~
  int64 proxy_market2_id;
~~~

TODO

### `proxy_clear_price`
~~~
  double proxy_clear_price[$];
~~~

TODO

### `proxy_clear_price2`
~~~
  double proxy_clear_price2[$];
~~~

TODO

### `proxy_price_cap`
~~~
  double proxy_price_cap;
~~~

TODO

### `proxy_price_cap2`
~~~
  double proxy_price_cap2;
~~~

TODO

### `proxy_market_unit`
~~~
  char32 proxy_market_unit;
~~~

TODO

### `proxy_initial_price`
~~~
  double proxy_initial_price;
~~~

TODO

### `proxy_marginal_fraction`
~~~
  double proxy_marginal_fraction;
~~~

TODO

### `proxy_marginal_fraction2`
~~~
  double proxy_marginal_fraction2;
~~~

TODO

### `proxy_clearing_quantity`
~~~
  double proxy_clearing_quantity;
~~~

TODO

### `proxy_clearing_quantity2`
~~~
  double proxy_clearing_quantity2;
~~~

TODO

### `proxy_seller_total_quantity`
~~~
  double proxy_seller_total_quantity;
~~~

TODO

### `proxy_seller_total_quantity2`
~~~
  double proxy_seller_total_quantity2;
~~~

TODO

### `proxy_margin_mode`
~~~
  enumeration {PROB, DENY, NORMAL} proxy_margin_mode;
~~~

TODO

### `proxy_clearing_type`
~~~
  enumeration {NULL, FAILURE, EXACT, MARGINAL_PRICE, MARGINAL_BUYER, MARGINAL_SELLER} proxy_clearing_type;
~~~

TODO

### `proxy_clearing_type2`
~~~
  enumeration {NULL, FAILURE, EXACT, MARGINAL_PRICE, MARGINAL_BUYER, MARGINAL_SELLER} proxy_clearing_type2;
~~~

TODO

# Example

~~~
  object controller {
    simple_mode "0";
    bid_mode "0";
    use_override "0";
    ramp_low "0.0";
    ramp_high "0.0";
    range_low "0.0";
    range_high "0.0";
    target "";
    setpoint "";
    demand "";
    load "";
    total "";
    market "";
    state "";
    avg_target "";
    std_target "";
    bid_price "0.0";
    bid_quantity "0.0";
    set_temp "0.0";
    base_setpoint "0.0";
    market_price "0.0";
    period "0.0";
    control_mode "0";
    resolve_mode "0";
    slider_setting "0.0";
    slider_setting_heat "0.0";
    slider_setting_cool "0.0";
    override "";
    heating_range_high "0.0";
    heating_range_low "0.0";
    heating_ramp_high "0.0";
    heating_ramp_low "0.0";
    cooling_range_high "0.0";
    cooling_range_low "0.0";
    cooling_ramp_high "0.0";
    cooling_ramp_low "0.0";
    heating_base_setpoint "0.0";
    cooling_base_setpoint "0.0";
    deadband "";
    heating_setpoint "";
    heating_demand "";
    cooling_setpoint "";
    cooling_demand "";
    sliding_time_delay "0.0";
    use_predictive_bidding "FALSE";
    device_actively_engaged "0.0";
    cleared_market "0";
    locked "0";
    p_ON "0.0";
    p_OFF "0.0";
    p_ONLOCK "0.0";
    p_OFFLOCK "0.0";
    delta_u "0.0";
    regulation_market_on "";
    regulation_market_off "";
    fast_regulation_signal "0.0";
    market_price_on "0.0";
    market_price_off "0.0";
    period_on "0.0";
    period_off "0.0";
    regulation_period "0";
    r1 "0.0";
    mu0 "0.0";
    mu1 "0.0";
    average_target "";
    standard_deviation_target "";
    bid_id "0";
    bid_delay "0";
    thermostat_state "";
    proxy_average "0.0";
    proxy_standard_deviation "0.0";
    proxy_market_id "0";
    proxy_market2_id "0";
    proxy_clear_price "0.0";
    proxy_clear_price2 "0.0";
    proxy_price_cap "0.0";
    proxy_price_cap2 "0.0";
    proxy_market_unit "";
    proxy_initial_price "0.0";
    proxy_marginal_fraction "0.0";
    proxy_marginal_fraction2 "0.0";
    proxy_clearing_quantity "0.0";
    proxy_clearing_quantity2 "0.0";
    proxy_seller_total_quantity "0.0";
    proxy_seller_total_quantity2 "0.0";
    proxy_margin_mode "0";
    proxy_clearing_type "0";
    proxy_clearing_type2 "0";
  }
~~~

# See also
* [[/Module/Market]]

