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

TODO

## Properties

### `input_state`
~~~
  int32 input_state;
~~~

TODO

### `input_setpoint`
~~~
  double input_setpoint;
~~~

TODO

### `input_chained`
~~~
  bool input_chained;
~~~

TODO

### `observation`
~~~
  double observation;
~~~

The observed value

### `mean_observation`
~~~
  double mean_observation;
~~~

The observed mean value

### `stdev_observation`
~~~
  double stdev_observation;
~~~

The observed standard deviation value

### `expectation`
~~~
  double expectation;
~~~

The observed expected value

### `sensitivity`
~~~
  double sensitivity;
~~~

The sensitivity of the control actuator to observation deviations

### `period`
~~~
  double period[s];
~~~

The cycle period for the controller logic

### `expectation_prop`
~~~
  char32 expectation_prop;
~~~

The name of the property to observe for the expected value

### `expectation_obj`
~~~
  object expectation_obj;
~~~

The object to watch for the expectation property

### `expectation_property`
~~~
  char32 expectation_property;
~~~

The name of the property to observe for the expected value

### `expectation_object`
~~~
  object expectation_object;
~~~

The object to watch for the expectation property

### `setpoint_prop`
~~~
  char32 setpoint_prop;
~~~

The name of the setpoint property in the parent object

### `setpoint`
~~~
  char32 setpoint;
~~~

The name of the setpoint property in the parent object

### `state_prop`
~~~
  char32 state_prop;
~~~

The name of the actuator property in the parent object

### `state_property`
~~~
  char32 state_property;
~~~

The name of the actuator property in the parent object

### `observation_obj`
~~~
  object observation_obj;
~~~

The object to observe

### `observation_prop`
~~~
  char32 observation_prop;
~~~

The name of the observation property

### `observation_object`
~~~
  object observation_object;
~~~

The object to observe

### `observation_property`
~~~
  char32 observation_property;
~~~

The name of the observation property

### `mean_observation_prop`
~~~
  char32 mean_observation_prop;
~~~

The name of the mean observation property

### `stdev_observation_prop`
~~~
  char32 stdev_observation_prop;
~~~

The name of the standard deviation observation property

### `stdev_observation_property`
~~~
  char32 stdev_observation_property;
~~~

The name of the standard deviation observation property

### `cycle_length`
~~~
  int32 cycle_length;
~~~

Length of time between processing cycles

### `base_setpoint`
~~~
  double base_setpoint;
~~~

The base setpoint to base control off of

### `critical_day`
~~~
  double critical_day;
~~~

Used to switch between TOU and CPP days, 1 is CPP, 0 is TOU

### `two_tier_cpp`
~~~
  bool two_tier_cpp;
~~~

TODO

### `daily_elasticity`
~~~
  double daily_elasticity;
~~~

TODO

### `sub_elasticity_first_second`
~~~
  double sub_elasticity_first_second;
~~~

TODO

### `sub_elasticity_first_third`
~~~
  double sub_elasticity_first_third;
~~~

TODO

### `second_tier_hours`
~~~
  int32 second_tier_hours;
~~~

TODO

### `third_tier_hours`
~~~
  int32 third_tier_hours;
~~~

TODO

### `first_tier_hours`
~~~
  int32 first_tier_hours;
~~~

TODO

### `first_tier_price`
~~~
  double first_tier_price;
~~~

TODO

### `second_tier_price`
~~~
  double second_tier_price;
~~~

TODO

### `third_tier_price`
~~~
  double third_tier_price;
~~~

TODO

### `old_first_tier_price`
~~~
  double old_first_tier_price;
~~~

TODO

### `old_second_tier_price`
~~~
  double old_second_tier_price;
~~~

TODO

### `old_third_tier_price`
~~~
  double old_third_tier_price;
~~~

TODO

### `Percent_change_in_price`
~~~
  double Percent_change_in_price;
~~~

TODO

### `Percent_change_in_peakoffpeak_ratio`
~~~
  double Percent_change_in_peakoffpeak_ratio;
~~~

TODO

### `Percent_change_in_Criticalpeakoffpeak_ratio`
~~~
  double Percent_change_in_Criticalpeakoffpeak_ratio;
~~~

TODO

### `linearize_elasticity`
~~~
  bool linearize_elasticity;
~~~

TODO

### `price_offset`
~~~
  double price_offset;
~~~

TODO

### `pool_pump_model`
~~~
  bool pool_pump_model;
~~~

Boolean flag for turning on the pool pump version of the DUTYCYCLE control

### `base_duty_cycle`
~~~
  double base_duty_cycle;
~~~

This is the duty cycle before modification due to the price signal

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

TODO

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

TODO

### `comfort_level`
~~~
  double comfort_level;
~~~

TODO

### `range_high`
~~~
  double range_high;
~~~

TODO

### `range_low`
~~~
  double range_low;
~~~

TODO

### `ramp_high`
~~~
  double ramp_high;
~~~

TODO

### `ramp_low`
~~~
  double ramp_low;
~~~

TODO

### `prob_off`
~~~
  double prob_off;
~~~

TODO

### `output_state`
~~~
  int32 output_state;
~~~

The target setpoint given the input observations

### `output_setpoint`
~~~
  double output_setpoint;
~~~

TODO

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

TODO

### `cycle_length_on`
~~~
  double cycle_length_on[s];
~~~

TODO

# Example

~~~
  object passive_controller {
    input_state "0";
    input_setpoint "0.0";
    input_chained "FALSE";
    observation "0.0";
    mean_observation "0.0";
    stdev_observation "0.0";
    expectation "0.0";
    sensitivity "0.0";
    period "0.0";
    expectation_prop "";
    expectation_property "";
    setpoint_prop "";
    setpoint "";
    state_prop "";
    state_property "";
    observation_prop "";
    observation_property "";
    mean_observation_prop "";
    stdev_observation_prop "";
    stdev_observation_property "";
    cycle_length "0";
    base_setpoint "0.0";
    critical_day "0.0";
    two_tier_cpp "FALSE";
    daily_elasticity "0.0";
    sub_elasticity_first_second "0.0";
    sub_elasticity_first_third "0.0";
    second_tier_hours "0";
    third_tier_hours "0";
    first_tier_hours "0";
    first_tier_price "0.0";
    second_tier_price "0.0";
    third_tier_price "0.0";
    old_first_tier_price "0.0";
    old_second_tier_price "0.0";
    old_third_tier_price "0.0";
    Percent_change_in_price "0.0";
    Percent_change_in_peakoffpeak_ratio "0.0";
    Percent_change_in_Criticalpeakoffpeak_ratio "0.0";
    linearize_elasticity "FALSE";
    price_offset "0.0";
    pool_pump_model "FALSE";
    base_duty_cycle "0.0";
    trigger_time_under_frequency "0";
    trigger_time_over_frequency "0";
    release_time_under_frequency "0";
    release_time_over_frequency "0";
    release_point_under_frequency "0.0";
    release_point_over_frequency "0.0";
    trigger_point_under_frequency "0.0";
    trigger_point_over_frequency "0.0";
    frequency "0.0";
    PFC_mode "0";
    PFC_state "0";
    state_observed "";
    power_observed "";
    output_observed "0";
    bid_delay "0";
    voltage_lockout "0.0";
    voltage_lockout_time "0.0";
    voltage_lockout_state "0";
    distribution_type "0";
    comfort_level "0.0";
    range_high "0.0";
    range_low "0.0";
    ramp_high "0.0";
    ramp_low "0.0";
    prob_off "0.0";
    output_state "0";
    output_setpoint "0.0";
    control_mode "0";
    dlc_mode "0";
    cycle_length_off "0.0";
    cycle_length_on "0.0";
  }
~~~

# See also
* [[/Module/Market]]

