[[/Module/Market/Passive_controller]] -- Class passive_controller

# Synopsis

GLM:

~~~
  object passive_controller {
    input_state "<integer>";
    input_setpoint "<decimal>";
    input_chained "<string>";
    observation "<decimal>";
    mean_observation "<decimal>";
    stdev_observation "<decimal>";
    expectation "<decimal>";
    sensitivity "<decimal>";
    period "<decimal> s";
    expectation_prop "<string>";
    expectation_obj "<string>";
    expectation_property "<string>";
    expectation_object "<string>";
    setpoint_prop "<string>";
    setpoint "<string>";
    state_prop "<string>";
    state_property "<string>";
    observation_obj "<string>";
    observation_prop "<string>";
    observation_object "<string>";
    observation_property "<string>";
    mean_observation_prop "<string>";
    stdev_observation_prop "<string>";
    stdev_observation_property "<string>";
    cycle_length "<integer>";
    base_setpoint "<decimal>";
    critical_day "<decimal>";
    two_tier_cpp "<string>";
    daily_elasticity "<decimal>";
    sub_elasticity_first_second "<decimal>";
    sub_elasticity_first_third "<decimal>";
    second_tier_hours "<integer>";
    third_tier_hours "<integer>";
    first_tier_hours "<integer>";
    first_tier_price "<decimal>";
    second_tier_price "<decimal>";
    third_tier_price "<decimal>";
    old_first_tier_price "<decimal>";
    old_second_tier_price "<decimal>";
    old_third_tier_price "<decimal>";
    Percent_change_in_price "<decimal>";
    Percent_change_in_peakoffpeak_ratio "<decimal>";
    Percent_change_in_Criticalpeakoffpeak_ratio "<decimal>";
    linearize_elasticity "<string>";
    price_offset "<decimal>";
    pool_pump_model "<string>";
    base_duty_cycle "<decimal>";
    trigger_time_under_frequency "<integer>";
    trigger_time_over_frequency "<integer>";
    release_time_under_frequency "<integer>";
    release_time_over_frequency "<integer>";
    release_point_under_frequency "<decimal>";
    release_point_over_frequency "<decimal>";
    trigger_point_under_frequency "<decimal>";
    trigger_point_over_frequency "<decimal>";
    frequency "<decimal>";
    PFC_mode "{OVER_UNDER_FREQUENCY,UNDER_FREQUENCY,OVER_FREQUENCY}";
    PFC_state "{RELEASED_ON,RELEASED_OFF,FORCED_ON,FORCED_OFF,TRIGGERED_ON,TRIGGERED_OFF,FREE}";
    state_observed "<string>";
    power_observed "<string>";
    output_observed "<integer>";
    bid_delay "<integer>";
    voltage_lockout "<decimal> %";
    voltage_lockout_time "<decimal> s";
    voltage_lockout_state "<integer>";
    distribution_type "{UNIFORM,EXPONENTIAL,NORMAL}";
    comfort_level "<decimal>";
    range_high "<decimal>";
    range_low "<decimal>";
    ramp_high "<decimal>";
    ramp_low "<decimal>";
    prob_off "<decimal>";
    output_state "<integer>";
    output_setpoint "<decimal>";
    control_mode "{PRIMARY_FREQUENCY_CONTROL,DIRECT_LOAD_CONTROL,ELASTICITY_MODEL,PROBABILITY_OFF,DUTYCYCLE,RAMP,NONE}";
    dlc_mode "{CYCLING,OFF}";
    cycle_length_off "<decimal> s";
    cycle_length_on "<decimal> s";
  }
~~~

# Description

This controller is similar to the [[/Module/Market/Controller]] object, except without the capability to bid back into an auction. It is designed as a passive demand response controller, which only receives price (or other) signals, generally from an auction or stubauction object, and responds accordingly. Additionally, it is used as a test bed for future transactive controller strategies, as it is easier to implement a passive response than an active bidding market.

## Properties

### `input_state`

~~~
  int32 input_state;
~~~

Not used at this time.

### `input_setpoint`

~~~
  double input_setpoint;
~~~

Not used at this time.

### `input_chained`

~~~
  bool input_chained;
~~~

Not used at this time.

### `observation`

~~~
  double observation;
~~~

When the previous variables are used, this is where the observed value is assigned.

### `mean_observation`

~~~
  double mean_observation;
~~~

When the previous variables are used, this is where the mean value is assigned.

### `stdev_observation`

~~~
  double stdev_observation;
~~~

When the previous variables are used, this is where the standard deviation value is assigned.

### `expectation`

~~~
  double expectation;
~~~

When the previous variables are used, this is where the expected value is assigned.

### `sensitivity`

~~~
  double sensitivity;
~~~

Not used at this time.

### `period`

~~~
  double period[s];
~~~

The period of time for which the controller operates. This signals how often the controller will update the state of the set point and how often the controller will bid into the market. Ideally, this should be identical to, or a multiple of, the auction object’s time period. While this is not required, if the supply bid and demand bids do not coincide, odd behavior may occur. Must be a positive, non-zero value.

### `expectation_obj`, `expectation_object`, `expectation_prop`

~~~
  object expectation_obj;
  object expectation_object;
  char32 expectation_prop;
~~~

Observation property is the object's value to be compared against the expected value.

The object to watch for the expectation property

### `expectation_obj`, `expectation_object`, `expectation_property`, `expectation_prop`

~~~
  object expectation_obj;
  object expectation_object;
  char32 expectation_prop;
  char32 expectation_property;
~~~

The observed object requires that a current value and a standard deviation from the expected value be compared. This is the object where the observed value and the mean and standard deviation of the observed value are to be found. This is the property of the defined object that the observed property is compared against. In the transactive controller, this would be the average price of the market, while for a frequency control this might be 60 Hz.

### `setpoint_prop`, `setpoint`

~~~
  char32 setpoint_prop;
  char32 setpoint;
~~~

Defines the property to be modified by the controller.

### `state_prop`, `state_property`

~~~
  char32 state_property;
  char32 state_prop;
~~~

The property name within the parent object that specifies the current conditional state of the controllable object. For the HVAC system, this signifies on or off, however, future implementations may include multi-state objects.

The name of the actuator property in the parent object

### `observation_obj`, `observation_prop`, `observation_object`, `observation_property`

~~~
  object observation_obj;
  object observation_object;
  char32 observation_prop;
  char32 observation_property;
~~~

The observed object requires that a current value and a standard deviation from the expected value be compared. This is the object where the observed value and the mean and standard deviation of the observed value are to be found.

### `mean_observation_prop`

~~~
  char32 mean_observation_prop;
~~~

This is the name of the variable which contains the mean of the observed value.

### `stdev_observation_prop`, `stdev_observation_property`

~~~
  char32 stdev_observation_prop;
  char32 stdev_observation_property;
~~~

Standard deviation is the number of deviations away from the expectation property the observation property is currently.

### `cycle_length`

~~~
  int32 cycle_length;
~~~

Not used at this time.

### `base_setpoint`

~~~
  double base_setpoint;
~~~

This is the value of the set point were the controller not to exist, or the original set point prior to the controller's input. No limit to value.

### `critical_day`

~~~
  double critical_day;
~~~

Used to switch between TOU and CPP days, 1 is CPP, 0 is TOU. This is an integer flag. It needs to be set to 1 to specify a Critical (Event) Day and to 0 to specify a Non-Event day.

### `two_tier_cpp`

~~~
  bool two_tier_cpp;
~~~

This is a Boolean flag. It needs to be set to true if a two tier pricing needs to be specified for both Event and Non-Event Days. If using three-tier pricing for Event Days, this flag needs to be set to false.

### `daily_elasticity`

~~~
  double daily_elasticity;
~~~

This field can be used to specify the value of the Daily Elasticity coefficient. The Daily Elasticity coefficient specifies the factor by which the daily energy consumption changes given a change in the TOU pricing scheme.

### `sub_elasticity_first_second`

~~~
  double sub_elasticity_first_second;
~~~

This field can be used to specify the value of the Substitution Elasticity coefficient between the Peak pricing and the Off Peak pricing. The Substitution Elasticity coefficient specifies the factor by which the average Peak energy consumption is substituted to average off-peak energy consumption, given a change in the TOU pricing scheme. If using Two tier pricing schemes (two_tier_cpp is true), for CPP (Event) days (critical_day is 1), this value will be ignored for substitution calculation on CPP days.

### `sub_elasticity_first_third`

~~~
  double sub_elasticity_first_third;
~~~

This field can be used to specify the value of the Substitution Elasticity coefficient between the Critical pricing and the Off Peak pricing. The Substitution Elasticity coefficient specifies the factor by which the average Critical energy consumption is substituted to average off-peak energy consumption, given a change in the TOU pricing scheme. If using Two tier pricing schemes (two_tier_cpp is true), for non-CPP (non-Event) days (critical_day is 0), will be ignored for Substitution calculation on non-CPP days.

### `first_tier_hours`

~~~
  int32 first_tier_hours;
~~~

This field can be used to specify the duration of the off peak price (first tier) in hours. If not specified, the system will calculate it based on the number of hours given for the peak price hours and/or CPP hours.

### `second_tier_hours`

~~~
  int32 second_tier_hours;
~~~

This field can be used to specify the duration of the peak price (second tier) in hours. If using two tier pricing schemes (two_tier_cpp is true), this field should be used only to specify the duration of the peak price hours for non-CPP days (critical_day is 0). It should not be used to specify the CPP price hours for CPP (critical_day is 1) days.

### `third_tier_hours`

~~~
  int32 third_tier_hours;
~~~

This field can be used to specify the duration of the critical price (third tier) in hours. If using Two tier pricing schemes (two_tier_cpp is true), only this field should be used to specify the duration of the CPP price hours for CPP (critical_day is 1) days and second_tier_hours field should be used to specify the duration of the peak price hours for non-CPP days (critical_day is 0).

### `first_tier_price`

~~~
  double first_tier_price;
~~~

This field can be used to specify the off peak price in TOU/CPP pricing scheme.

### `second_tier_price`

~~~
  double second_tier_price;
~~~

This field can be used to specify the peak price in TOU/CPP pricing scheme. If using two tier pricing schemes (two_tier_cpp is true), this field should be used only to specify the peak price for non-CPP days (critical_day is 0). It should not be used to specify the CPP price for CPP (critical_day is 1) days.

### `third_tier_price`

~~~
  double third_tier_price;
~~~

This field can be used to specify the critical price in TOU/CPP pricing scheme. If using Two tier pricing schemes (two_tier_cpp is true), only this field should be used to specify the CPP price for CPP (critical_day is 1) days and second_tier_price field should be used to specify the peak price for non-CPP days (critical_day is 0).

### `old_first_tier_price`

~~~
  double old_first_tier_price;
~~~

This field describes the first tier price for the previous billing structure to estimate customer change in behavior.

### `old_second_tier_price`

~~~
  double old_second_tier_price;
~~~

This field describes the second tier price for the previous billing structure to estimate customer change in behavior.

### `old_third_tier_price`

~~~
  double old_third_tier_price;
~~~

This field describes the third tier price for the previous billing structure to estimate customer change in behavior.

### `Percent_change_in_price`

~~~
  double Percent_change_in_price;
~~~

This variable defines the ratio of the change in the daily average price between the old and new pricing schemes or rate structures. This is an output variable only, mainly used for diagnostics.

### `Percent_change_in_peakoffpeak_ratio`

~~~
  double Percent_change_in_peakoffpeak_ratio;
~~~

This variable defines the ratio of peak to off-peak prices between the old and new pricing schemes or rate structures. This is an output variable only, mainly used for diagnostics.

### `Percent_change_in_Criticalpeakoffpeak_ratio`

~~~
  double Percent_change_in_Criticalpeakoffpeak_ratio;
~~~

This variable defines the ratio of the critical peak to off-peak prices between the old and new pricing schemes or rate structures. This is an output variable only, mainly used for diagnostics.

### `linearize_elasticity`

~~~
  bool linearize_elasticity;
~~~

This option allows the user to activate the "linearized" version of the elasticity model. If TRUE, the model becomes linear and only examines a single data point. If FALSE (default), it assumes that the elasticity values are on a continuous curve with different prices for different price to load ratios.

### `price_offset`

~~~
  double price_offset;
~~~

This value is used as a floating point precision value. When the controller is comparing current price to the preset tier prices, this is the error allowed. Default is 10E-6.

### `pool_pump_model`

~~~
  bool pool_pump_model;
~~~

Activates the pool pump version of the DUTYCYCLE control mode, which has specific rules described in the FY2011 report to DOE on DR in SGIG.

### `base_duty_cycle`

~~~
  double base_duty_cycle;
~~~

Describes natural duty cycle of the controlled object in DUTYCYCLE mode.

### `trigger_time_under_frequency`

~~~
  int32 trigger_time_under_frequency;
~~~

Time to stay in triggered off state in seconds

### `trigger_time_over_frequency`

~~~
  int32 trigger_time_over_frequency;
~~~

Time to stay in triggered on state in seconds

### `release_time_under_frequency`

~~~
  int32 release_time_under_frequency;
~~~

Time to stay in released on state in seconds

### `release_time_over_frequency`

~~~
  int32 release_time_over_frequency;
~~~

Time to stay in released off state in seconds

### `release_point_under_frequency`

~~~
  double release_point_under_frequency;
~~~

Frequency value for releasing GFA in under frequency mode

### `release_point_over_frequency`

~~~
  double release_point_over_frequency;
~~~

Frequency value for releasing GFA in over frequency mode

### `trigger_point_under_frequency`

~~~
  double trigger_point_under_frequency;
~~~

Frequency value for triggereing GFA in under frequency mode

### `trigger_point_over_frequency`

~~~
  double trigger_point_over_frequency;
~~~

Frequency value for triggereing GFA in over frequency mode

### `frequency`

~~~
  double frequency;
~~~

Frequency value

### `PFC_mode`

~~~
  enumeration {OVER_UNDER_FREQUENCY, UNDER_FREQUENCY, OVER_FREQUENCY} PFC_mode;
~~~

Operation mode of the primary frequency controller

### `PFC_state`

~~~
  enumeration {RELEASED_ON, RELEASED_OFF, FORCED_ON, FORCED_OFF, TRIGGERED_ON, TRIGGERED_OFF, FREE} PFC_state;
~~~

State of the primary frequency controller

### `state_observed`

~~~
  char32 state_observed;
~~~

The name of the observed state property in the parent object

### `power_observed`

~~~
  char32 power_observed;
~~~

The name of the observed state property in the parent object

### `output_observed`

~~~
  int32 output_observed;
~~~

Not used at this time.

### `bid_delay`

~~~
  int32 bid_delay;
~~~

Time the controller will bid in advance before clearing

### `voltage_lockout`

~~~
  double voltage_lockout[%];
~~~

Lockout primary frequency control if voltage is deviating % from nominal

### `voltage_lockout_time`

~~~
  double voltage_lockout_time[s];
~~~

Voltage lockout period

### `voltage_lockout_state`

~~~
  int32 voltage_lockout_state;
~~~

Value to determine if water heater is in voltage lockout

### `distribution_type`

~~~
  enumeration {UNIFORM, EXPONENTIAL, NORMAL} distribution_type;
~~~

Not used at this time.

### `comfort_level`

~~~
  double comfort_level;
~~~

Not used at this time.

### `prob_off`

~~~
  double prob_off;
~~~

Not used at this time.

### `output_state`

~~~
  int32 output_state;
~~~

The target setpoint given the input observations

### `output_setpoint`

~~~
  double output_setpoint;
~~~

Not used at this time.

### `control_mode`

~~~
  enumeration {PRIMARY_FREQUENCY_CONTROL, DIRECT_LOAD_CONTROL, ELASTICITY_MODEL, PROBABILITY_OFF, DUTYCYCLE, RAMP, NONE} control_mode;
~~~

The control mode to use for determining controller action

### `dlc_mode`

~~~
  enumeration {CYCLING, OFF} dlc_mode;
~~~

This mode is roughly designed to force cycle an AC unit

### `cycle_length_off`

~~~
  double cycle_length_off[s];
~~~

Not used at this time.

### `cycle_length_on`

~~~
  double cycle_length_on[s];
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

To create an HVAC passive_controller, similar to a controller in RAMP mode that does not bid:

~~~
object passive_controller {
     period 300; 
     parent house1;
     control_mode RAMP;
     observation_object Market_1;
     observation_property current_market.clearing_price;
     stdev_observation_property current_price_stdev_24h;
     expectation_object Market_1;
     expectation_property current_price_mean_24h;
     range_low -0.005;
     range_high 3;
     ramp_low 2.4;
     ramp_high 2.4;
     base_setpoint 75;
     setpoint_property cooling_setpoint;
     state_property power_state;
};
~~~

A passive_controller, modifying the behavior of an analog ZIPload by using the ELASTICITY_MODEL with a 2-tier TOU and no CPP, would look like:

~~~
object passive_controller {
     period 300;
     parent ZIPload1;
     control_mode ELASTICITY_MODEL;
     two_tier_cpp false;
     observation_object Market_1;
     observation_property past_market.clearing_price;
     state_property multiplier;
     linearize_elasticity true;
     price_offset 0.01;
     critical_day 0;
     first_tier_hours 12;
     second_tier_hours 12;
     first_tier_price 0.076351;
     second_tier_price 0.152702;
     old_first_tier_price 0.124300;
     old_second_tier_price 0.124300;
     daily_elasticity -0.1305;
     sub_elasticity_first_second -0.0198;
     sub_elasticity_first_third -0.0290;
}
~~~

The same passive_controller, again in ELASTICITY_MODEL modifying a ZIPload, in a situation with TOU and CPP (price pattern shown in the following figure) would be:

Two-tier TOU and CPP, where 2nd tier TOU is replaced by CPP.

~~~
object passive_controller {
     period 300;
     parent ZIPload1;
     control_mode ELASTICITY_MODEL;
     two_tier_cpp true;
     observation_object Market_1;
     observation_property past_market.clearing_price;
     state_property multiplier;
     linearize_elasticity true;
     price_offset 0.01;
     critical_day critical_day_schedule.value; //schedule with a 1 on critical days, and 0 on normal days
     first_tier_hours 12;
     second_tier_hours 12;
     third_tier_hours 6;
     first_tier_price 0.076351;
     second_tier_price 0.152702;
     third_tier_price 0.76351;
     old_first_tier_price 0.124300;
     old_second_tier_price 0.124300;
     old_third_tier_price 0.124300;
     daily_elasticity -0.1305;
     sub_elasticity_first_second -0.0198;
     sub_elasticity_first_third -0.0290;
}
~~~

A passive_controller modifying the behavior of a waterheater in a manner similar to the Olympic Peninsula Demonstration project, using PROBABILITY_OFF, would look like:

~~~
object passive_controller {
     period 900; // Note period is a multiple of auction period.
     parent waterheater1;
     control_mode PROBABILITY_OFF;
     distribution_type NORMAL;
     observation_object Market_1;
     observation_property past_market.clearing_price;
     stdev_observation_property my_std;
     expectation_object Market_1;
     expectation_property my_avg;
     comfort_level 0.82;
     state_property override;
}
~~~

A passive_controller in DUTYCYCLE mode, modifying the behavior of a ZIPload (which has a duty_cycle defined), would look like:

~~~
object ZIPload {
     name pool_pump1;
     parent house1;
     // Representative of Pool Pump operation
     base_power 1400 W;
     duty_cycle 0.22;
     phase 0.26;
     period 4.96;
     heatgain_fraction 0.0;
     power_pf 1.0;
     current_pf 1.0;
     impedance_pf 1.0;
     impedance_fraction 0.2;
     current_fraction 0.4;
     power_fraction 0.4;
     is_240 TRUE;
     recovery_duty_cycle 0.27;
     object passive_controller {
          period 900;
          control_mode DUTYCYCLE;
          pool_pump_model true;
          observation_object Market_1;
          observation_property past_market.clearing_price;
          state_property override;
          base_duty_cycle 0.22;
          setpoint duty_cycle;
          first_tier_hours 12;
          second_tier_hours 12;
          third_tier_hours 6;
          first_tier_price 0.070489;
          second_tier_price 0.140979;
          third_tier_price 0.704894;
     };
}
~~~

# See also

* [[/Module/Market]]
* [[/Module/Market/Auction]]
* [[/Module/Market/Controller]]

