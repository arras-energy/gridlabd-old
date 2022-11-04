[[/Module/Market/Generator_controller]] -- Class generator_controller

# Synopsis

GLM:

~~~
  object generator_controller {
    generator_state "{STARTING,RUNNING,OFF}";
    amortization_type "{LINEAR_BID,LINEAR_COST,EXPONENTIAL}";
    generator_state_number "<integer>";
    market "<string>";
    bid_curve "<string>";
    bid_curve_file "<string>";
    bid_generator_rating "<decimal> VA";
    update_curve "<string>";
    is_marginal_gen "<string>";
    generator_rating "<decimal> VA";
    generator_output "<decimal>";
    input_unit_base "<decimal> MW";
    startup_cost "<decimal> $";
    shutdown_cost "<decimal> $";
    minimum_runtime "<decimal> s";
    minimum_downtime "<decimal> s";
    capacity_factor "<decimal>";
    amortization_factor "<decimal> 1/s";
    bid_delay "<decimal>";
    generator_attachment "{BUILDING,STANDALONE}";
    building_load_curr "<decimal>";
    building_load_bid "<decimal>";
    year_runtime_limit "<decimal> h";
    current_year_runtime "<decimal> h";
    runtime_year_end "<string>";
    scaling_factor "<decimal> unit";
    license_premium "<decimal>";
    hours_in_year "<decimal> h";
    op_and_maint_cost "<decimal> $";
    bid_id "<integer>";
  }
~~~

# Description

This controller is designed to serve as a bidding agent for distributed generation unit.

The controller is in development still.

## Properties

### `generator_state`

~~~
  enumeration {STARTING, RUNNING, OFF} generator_state;
~~~

Current generator state

### `amortization_type`

~~~
  enumeration {LINEAR_BID, LINEAR_COST, EXPONENTIAL} amortization_type;
~~~

Amortization compounding method

### `generator_state_number`

~~~
  int32 generator_state_number;
~~~

Current generator state as numeric value

### `market`

~~~
  object market;
~~~

Market the object will watch and bid into

### `bid_curve`

~~~
  char1024 bid_curve;
~~~

Bidding curve text format

### `bid_curve_file`

~~~
  char1024 bid_curve_file;
~~~

Bidding curve file name

### `bid_generator_rating`

~~~
  double bid_generator_rating[VA];
~~~

Size of generator in VA for the bid curve

### `update_curve`

~~~
  bool update_curve;
~~~

Flag to force updating of bidding curve parse

### `is_marginal_gen`

~~~
  bool is_marginal_gen;
~~~

Flag to indicate if the generator is a marginal generator

### `generator_rating`

~~~
  double generator_rating[VA];
~~~

Size of generator in VA for the active bid

### `generator_output`

~~~
  double generator_output;
~~~

Current real power output of generator

### `input_unit_base`

~~~
  double input_unit_base[MW];
~~~

Base multiplier for generator bid units

### `startup_cost`

~~~
  double startup_cost[$];
~~~

Cost to start the generator from OFF status

### `shutdown_cost`

~~~
  double shutdown_cost[$];
~~~

Cost to shut down the generator prematurely

### `minimum_runtime`

~~~
  double minimum_runtime[s];
~~~

Minimum time the generator should run to avoid shutdown cost

### `minimum_downtime`

~~~
  double minimum_downtime[s];
~~~

Minimum down time for the generator before it can bid again

### `capacity_factor`

~~~
  double capacity_factor;
~~~

Calculation of generator's current capacity factor based on the market period

### `amortization_factor`

~~~
  double amortization_factor[1/s];
~~~

Exponential decay factor in 1/s for shutdown cost repayment

### `bid_delay`

~~~
  double bid_delay;
~~~

Time before a market closes to bid

### `generator_attachment`

~~~
  enumeration {BUILDING, STANDALONE} generator_attachment;
~~~

Generator attachment type - determines interactions

### `building_load_curr`

~~~
  double building_load_curr;
~~~

Present building load value (if BUILDING attachment)

### `building_load_bid`

~~~
  double building_load_bid;
~~~

Expected building load value for currently bidding market period (if BUILDING attachment)

### `year_runtime_limit`

~~~
  double year_runtime_limit[h];
~~~

Total number of hours the generator can run in a year

### `current_year_runtime`

~~~
  double current_year_runtime[h];
~~~

Total number of hours generator has run this year

### `runtime_year_end`

~~~
  char1024 runtime_year_end;
~~~

Date and time the generator runtime year resets

### `scaling_factor`

~~~
  double scaling_factor[unit];
~~~

Scaling factor applied to license premium calculation

### `license_premium`

~~~
  double license_premium;
~~~

Current value of the generator license premium calculated

### `hours_in_year`

~~~
  double hours_in_year[h];
~~~

Number of hours assumed for the total year

### `op_and_maint_cost`

~~~
  double op_and_maint_cost[$];
~~~

Operation and maintenance cost per runtime year

### `bid_id`

~~~
  int64 bid_id;
~~~

TODO

# Example

~~~
  object generator_controller {
    generator_state "0";
    amortization_type "0";
    generator_state_number "0";
    bid_curve "";
    bid_curve_file "";
    bid_generator_rating "0.0";
    update_curve "FALSE";
    is_marginal_gen "FALSE";
    generator_rating "0.0";
    generator_output "0.0";
    input_unit_base "0.0";
    startup_cost "0.0";
    shutdown_cost "0.0";
    minimum_runtime "0.0";
    minimum_downtime "0.0";
    capacity_factor "0.0";
    amortization_factor "0.0";
    bid_delay "0.0";
    generator_attachment "0";
    building_load_curr "0.0";
    building_load_bid "0.0";
    year_runtime_limit "0.0";
    current_year_runtime "0.0";
    runtime_year_end "";
    scaling_factor "0.0";
    license_premium "0.0";
    hours_in_year "0.0";
    op_and_maint_cost "0.0";
    bid_id "0";
  }
~~~

# See also

* [[/Module/Market]]

