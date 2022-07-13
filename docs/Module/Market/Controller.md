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

The controller is loosely based upon the design used in the Olympic Peninsula Project. This controller provides price-responsive controls (or other control inputs) to individual objects, typically appliances, within GridLAB-D. The controller compares the current price signal to the average market price, each delivered by the auction object, and bids the appliance’s current demand as a function of price back into the auction. After the market clears all bids within the system and determines the next market price, the controller modifies the appliance’s set points to reflect operation at the new current price, often related to the standard deviation from the average set point. The set point that is modified depends upon the object to which the controller is modifying. At this time, only devices with continuous temperature set points may be used with the controller object. As this object is expanded, additional controls may added that align with the general design principles.

## Properties

### `simple_mode`

~~~
  enumeration {DOUBLE_RAMP, WATERHEATER, HOUSE_PRECOOL, HOUSE_PREHEAT, HOUSE_COOL, HOUSE_HEAT, NONE} simple_mode;
~~~

This will set all of the default parameters for the controller object to automatically control certain pre-defined objects. When using this function, only the properties pertaining to the auction object will need to be set.

### `bid_mode`

~~~
  enumeration {PROXY, OFF, ON} bid_mode;
~~~

This value is used to turn the bidding strategies on or off. Note: Not currently operational.

### `use_override`

~~~
  enumeration {ON, OFF} use_override;
~~~

This value enforced a bidding strategy by commanding a unit to turn on when the bid is "won", or turn off when the bid is "lost", overriding the standard controls of the unit.

### `setpoint`, `{heating,cooling}_setpoint`

~~~
  double base_setpoint[degF];
  double heating_setpoint[degF];
  double cooling_setpoint[degF];
~~~

The name of the set point to be modified by the controller object. Within the HVAC system, this would include heating_setpoint or cooling_setpoint. Heating and cooling versions of variable are used in DOUBLE_RAMP mode.

### `base_setpoint`, `{heating,cooling}_base_setpoint`

~~~
  double base_setpoint[degF];
  double heating_base_setpoint[degF];
  double cooling_base_setpoint[degF];
~~~

This is the temperature set point of the system were there no controller present, or the original set point prior to the controller's input. Future implementations will allow this to control set points other than the temperature. No limit to value. Heating and cooling versions are used in the double_ramp mode.

### `range_{low,high}`, `{heating,cooling}_range_{low,high}`

~~~
  double range_low[degF];
  double range_high[degF];
  double range_heating_low[degF];
  double range_heating_high[degF];
  double range_cooling_low[degF];
  double range_cooling_high[degF];
~~~

These are the maximum bounds of variability allowed by the controller. For example, the heating_setpoint may vary +/- 5 degrees, but no more. These are relative to the base_setpoint (+5 F), not absolute (72 F). Range_high must be zero or greater and range_low must be zero or less. Heating and cooling versions are used in the double_ramp mode.

### `ramp_{low,high}`, `{heating,cooling}_ramp_{low,high}`

~~~
  double ramp_low[degF];
  double ramp_high[degF];
  double ramp_heating_low[degF];
  double ramp_heating_high[degF];
  double ramp_cooling_low[degF];
  double ramp_cooling_high[degF];
~~~

This specifies the slope of the linear control algorithm as a function of the average price, the current price, and the standard deviation from the average, and determines the controllers operation and bid. This will be further discussed later. No limit to value. Heating and cooling versions are used in the double_ramp mode.

### `target`

~~~
  char32 target;
~~~

The observed property (e.g., air temperature)

### `demand`, `{heating,cooling}_demand`


~~~
  char32 demand;
  char32 heating_demand;
  char32 cooling_demand;
~~~

The property name within the parent object that specifies the amount of power demanded by the controllable object at that time. For HVAC systems, this is heating_demand or cooling_demand. The heating and cooling versions are used in double_ramp mode.

### `load`

~~~
  char32 load;
~~~

The property name within the parent object that specifies the amount of power actually being used by the controllable object at the specified time. For HVAC systems, this is `hvac_load`.

### `total`

~~~
  char32 total;
~~~

The property name within the parent object that specifies, if any, all uncontrollable loads within that object in addition to the controllable load. For the HVAC model, this includes such things as circulation fan power or standby power settings, and is specified with total_load. It does not include additional panel demand from other appliances.

### `market`

~~~
  char32 market;
~~~

This references the market that provides the price signal to the controller, and generates the rolling average and standard deviations seen by the object. This is also the object into which the controller will bid its price. It is typically specified as an auction or stubauction object, and is typically referenced by the name of the object.

### `state`

~~~
  char32 state;
~~~

The property name within the parent object that specifies the current conditional state of the controllable object. For the HVAC system, this signifies on or off, however, future implementations may include multi-state objects.

### `average_target`, `avg_target`

~~~
  char32 average_target;
  char32 avg_target;
~~~

This value points to the property within the auction object which will be used to provide the rolling average price. This is usually determined by a rolling 24 hour average (avg24), a rolling 3-day (avg72), or a rolling week (avg168). Future implementations will allow this rolling average to be determined at any window length. Future implementations will also include the ability to look at variables other than average price and standard deviation.

### `standard_deviation_target`, `std_target`

~~~
  char32 standard_deviation_target;
  char32 std_target;
~~~

Similar to average_target, but specifies the rolling standard deviation.

### `bid_price`

~~~
  double bid_price;
~~~

This specifies the bidding price for the controller at the given operating points. Must be between negative and positive price cap, or will be cut off by the auction. This is typically a calculated value.

### `bid_quantity`

~~~
  double bid_quantity;
~~~

This specifies the amount of power demanded by the object at the determined bid_price. Must be a non-zero positive number. This is typically a calculated value.

### `set_temp`

~~~
  double set_temp[degF];
~~~

This specifies the final determined temperature of the controlled set point after the market has been cleared. Future implementations will allow for multi-state objects to be controlled.

### `market_price`

~~~
  double market_price;
~~~

The current market clearing price seen by the controller.

### `period`

~~~
  double period[s];
~~~

The period of time for which the controller operates. This signals how often the controller will update the state of the set point and how often the controller will bid into the market. Ideally, this should be identical to, or a multiple of, the auction object’s time period. While this is not required, if the supply bid and demand bids do not coincide, odd behavior may occur. Must be a positive, non-zero value.

### `control_mode`

~~~
  enumeration {DEV_LEVEL, DOUBLE_RAMP, RAMP} control_mode;
~~~

This specifies between the various control modes available. These is further described in the [[/Module/Market]] documentation.

### `resolve_mode`

~~~
  enumeration {SLIDING, DEADBAND} resolve_mode;
~~~

In certain control modes, multiple set points are controlled simultaneously. This specifies how to resolve a conflict between multiple control modes. This will be described in more detail, but will include deadband and sliding resolution modes. When multiple control set points are controlled, typically variables such as range and ramp will need to be specified multiple times, independent of each other.

### `slider_setting`, `slider_setting_{heat,cool}`

~~~
  double slider_setting;
  double slider_setting_heat;
  double slider_setting_cool;
~~~

These variables are simplified means of assigning value to ramp_low, ramp_high, range_low, and range_high, where 1 is an approximation of the most responsive level. The heat and cool versions are used in the double_ramp mode to specify both sides of the curve.

### `override`

~~~
  char32 override;
~~~

Used in conjunction with OVERRIDE mode, and assigns a property in the parent object which follow the override rules to short-circuit normal operation.

### `deadband`

~~~
  char32 deadband;
~~~

This is used to point the object property that contains the deadband variable. This is used in DEADBAND resolve_mode.

### `sliding_time_delay`

~~~
  double sliding_time_delay[s];
~~~

This value will allow the user to set a time delay within the sliding resolution mode. It will determine how long the controller stores the previous state when transitions only occur between HEAT/COOL and OFF. At the end of the time delay, the controller will update to the current system mode. If a transition occurs between HEAT <-> COOL (directly or indirectly), then the resolution should be updated to the current state and the time delay re-set.

### `use_predictive_bidding`

~~~
  bool use_predictive_bidding;
~~~

Not used at this time.

### `device_actively_engaged`

~~~
  double device_actively_engaged;
~~~

Not used at this time.

### `cleared_market`

~~~
  int32 cleared_market;
~~~

Not used at this time.

### `locked`

~~~
  int32 locked;
~~~

Not used at this time.

### `p_ON`

~~~
  double p_ON;
~~~

Not used at this time.

### `p_OFF`

~~~
  double p_OFF;
~~~

Not used at this time.

### `p_ONLOCK`

~~~
  double p_ONLOCK;
~~~

Not used at this time.

### `p_OFFLOCK`

~~~
  double p_OFFLOCK;
~~~

Not used at this time.

### `delta_u`

~~~
  double delta_u;
~~~

Not used at this time.

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

### `bid_id`

~~~
  int64 bid_id;
~~~

Not used at this time.

### `bid_delay`

~~~
  int32 bid_delay;
~~~

Not used at this time.

### `thermostat_state`

~~~
  char32 thermostat_state;
~~~

The name of the thermostat state property within the object being controlled

### `proxy_average`

~~~
  double proxy_average;
~~~

Not used at this time.

### `proxy_standard_deviation`

~~~
  double proxy_standard_deviation;
~~~

Not used at this time.

### `proxy_market_id`

~~~
  int64 proxy_market_id;
~~~

Not used at this time.

### `proxy_market2_id`

~~~
  int64 proxy_market2_id;
~~~

Not used at this time.

### `proxy_clear_price`

~~~
  double proxy_clear_price[$];
~~~

Not used at this time.

### `proxy_clear_price2`

~~~
  double proxy_clear_price2[$];
~~~

Not used at this time.

### `proxy_price_cap`

~~~
  double proxy_price_cap;
~~~

Not used at this time.

### `proxy_price_cap2`

~~~
  double proxy_price_cap2;
~~~

Not used at this time.

### `proxy_market_unit`

~~~
  char32 proxy_market_unit;
~~~

Not used at this time.

### `proxy_initial_price`

~~~
  double proxy_initial_price;
~~~

Not used at this time.

### `proxy_marginal_fraction`

~~~
  double proxy_marginal_fraction;
~~~

Not used at this time.

### `proxy_marginal_fraction2`

~~~
  double proxy_marginal_fraction2;
~~~

Not used at this time.

### `proxy_clearing_quantity`

~~~
  double proxy_clearing_quantity;
~~~

Not used at this time.

### `proxy_clearing_quantity2`

~~~
  double proxy_clearing_quantity2;
~~~

Not used at this time.

### `proxy_seller_total_quantity`

~~~
  double proxy_seller_total_quantity;
~~~

Not used at this time.

### `proxy_seller_total_quantity2`

~~~
  double proxy_seller_total_quantity2;
~~~

Not used at this time.

### `proxy_margin_mode`

~~~
  enumeration {PROB, DENY, NORMAL} proxy_margin_mode;
~~~

Not used at this time.

### `proxy_clearing_type`

~~~
  enumeration {NULL, FAILURE, EXACT, MARGINAL_PRICE, MARGINAL_BUYER, MARGINAL_SELLER} proxy_clearing_type;
~~~

Not used at this time.

### `proxy_clearing_type2`

~~~
  enumeration {NULL, FAILURE, EXACT, MARGINAL_PRICE, MARGINAL_BUYER, MARGINAL_SELLER} proxy_clearing_type2;
~~~

Not used at this time.

# Example

Assume an auction setup of:

~~~
class auction {
    double current_price_mean_24h;
    double current_price_stdev_24h;
    double my_avg;
    double my_std;
}
object auction {
    name Market_1;
    period 300;
    unit kW;
    capacity_reference_object Substation_Transformer;
    capacity_reference_property power_out_real;
    max_capacity_reference_bid_quantity 1200; //Defaults to 1200 kW
    init_price 0.10;
    init_stdev 0.03;
    my_avg 0.15;
    my_std 0.05;
    warmup 0;
     object player {
         file price.player;
         loop 10;
         property capacity_reference_bid_price;
     };    
}
~~~

Then, a bidding controller for an HVAC system in DOUBLE_RAMP and SLIDING modes, which bids 60 seconds prior to the market closing and uses the previous 24 hours of cleared prices to determine that statistics for responsiveness, could be setup as:

~~~
object controller {
    name testController_1;
    parent house_1;
    market Market_1;
    control_mode DOUBLE_RAMP;
    resolve_mode SLIDING;
    bid_mode ON;
    heating_base_setpoint 65;
    cooling_base_setpoint 75;
    target air_temperature;
    deadband thermostat_deadband;
    average_target current_price_mean_24h;
    standard_deviation_target current_price_stdev_24h;
    period 300;
    cooling_setpoint cooling_setpoint;
    heating_setpoint heating_setpoint;
    heating_demand last_heating_load;
    cooling_demand last_cooling_load;
    bid_delay 30;
    heating_range_high 0.265;
    cooling_range_high 0.442;
    heating_range_low -0.442;
    cooling_range_low -0.265;
    heating_ramp_high -2.823;
    cooling_ramp_high 2.823;
    heating_ramp_low -2.823;
    cooling_ramp_low 2.823;
    total total_load;
    load hvac_load;
    state power_state;
}
~~~

A similar HVAC controller for RAMP mode, controlling only the cooling load and bidding immediately prior to market closing, but uses predefined values for average and standard deviation, would be:

~~~
object controller {
    name testController_3;
    market Market_1;
    parent house_3;
    bid_mode ON;
    control_mode RAMP;
    base_setpoint 75;
    setpoint cooling_setpoint;
    target air_temperature;
    deadband thermostat_deadband;
    average_target my_avg;
    standard_deviation_target my_std;
    period 300;
    demand last_cooling_load;
    range_high 0.431;
    range_low -0.258;
    ramp_high 2.828;
    ramp_low 2.828;
    //slider_setting 0.2; //This could replace range_high,range_low, ramp_high, and ramp_low.
    total total_load;
    load hvac_load;
    state power_state;
}
~~~

# See also

* [[/Module/Market]]
* [[/Module/Market/Auction]]
