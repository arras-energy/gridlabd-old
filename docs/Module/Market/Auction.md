[[/Module/Market/Auction]] -- Class auction

# Synopsis

GLM:

~~~
  object auction {
    unit "<string>";
    period "<decimal> s";
    latency "<decimal> s";
    market_id "<integer>";
    network "<string>";
    verbose "<string>";
    linkref "<string>";
    pricecap "<decimal>";
    price_cap "<decimal>";
    special_mode "{BUYERS_ONLY,SELLERS_ONLY,NONE}";
    statistic_mode "{OFF,ON}";
    fixed_price "<decimal>";
    fixed_quantity "<decimal>";
    capacity_reference_object "<string>";
    capacity_reference_property "<string>";
    capacity_reference_bid_price "<decimal>";
    max_capacity_reference_bid_quantity "<decimal>";
    capacity_reference_bid_quantity "<decimal>";
    init_price "<decimal>";
    init_stdev "<decimal>";
    future_mean_price "<decimal>";
    use_future_mean_price "<string>";
    current_market.start_time "<string>";
    current_market.end_time "<string>";
    current_market.clearing_price "<decimal> $";
    current_market.clearing_quantity "<decimal>";
    current_market.clearing_type "{NULL,FAILURE,EXACT,MARGINAL_PRICE,MARGINAL_BUYER,MARGINAL_SELLER}";
    current_market.marginal_quantity_load "<decimal>";
    current_market.marginal_quantity "<decimal>";
    current_market.marginal_quantity_bid "<decimal>";
    current_market.marginal_quantity_frac "<decimal>";
    current_market.seller_total_quantity "<decimal>";
    current_market.buyer_total_quantity "<decimal>";
    current_market.seller_min_price "<decimal>";
    current_market.buyer_total_unrep "<decimal>";
    current_market.cap_ref_unrep "<decimal>";
    next_market.start_time "<string>";
    next_market.end_time "<string>";
    next_market.clearing_price "<decimal> $";
    next_market.clearing_quantity "<decimal>";
    next_market.clearing_type "{NULL,FAILURE,EXACT,MARGINAL_PRICE,MARGINAL_BUYER,MARGINAL_SELLER}";
    next_market.marginal_quantity_load "<decimal>";
    next_market.marginal_quantity_bid "<decimal>";
    next_market.marginal_quantity_frac "<decimal>";
    next_market.seller_total_quantity "<decimal>";
    next_market.buyer_total_quantity "<decimal>";
    next_market.seller_min_price "<decimal>";
    next_market.cap_ref_unrep "<decimal>";
    past_market.start_time "<string>";
    past_market.end_time "<string>";
    past_market.clearing_price "<decimal> $";
    past_market.clearing_quantity "<decimal>";
    past_market.clearing_type "{NULL,FAILURE,EXACT,MARGINAL_PRICE,MARGINAL_BUYER,MARGINAL_SELLER}";
    past_market.marginal_quantity_load "<decimal>";
    past_market.marginal_quantity_bid "<decimal>";
    past_market.marginal_quantity_frac "<decimal>";
    past_market.seller_total_quantity "<decimal>";
    past_market.buyer_total_quantity "<decimal>";
    past_market.seller_min_price "<decimal>";
    past_market.cap_ref_unrep "<decimal>";
    margin_mode "{PROB,DENY,NORMAL}";
    warmup "<integer>";
    ignore_failed_market "{TRUE,FALSE}";
    ignore_pricecap "{TRUE,FALSE}";
    transaction_log_file "<string>";
    transaction_log_limit "<integer>";
    curve_log_file "<string>";
    curve_log_limit "<integer>";
    curve_log_info "{EXTRA,NORMAL}";
  }
~~~

# Description

The auction provides a means for different objects within the GridLAB-D program to base their supply or demand on a dynamic or real time price. The market implemented in the auction object is implemented as a double-auction market. A double-auction market is one where suppliers and demanders (sellers and bidders) submit their bids of desired price for a set quantity simultaneously. Once the bidding submission period ends, the market "clears" by selecting the intersection point of the supply and demand curves. After the market clears and the relevant latency interval expires, the market price becomes active. At this point, devices that bid into the market will respond appropriately based on internal logic comparing their bid price to the market clearing price. The auction object does not provide any book-keeping or enforcement of the market, it simply provides a central market for buyers and sellers to bid their respective prices and quantities.

Additionally, the auction objects has published functions that can be used in runtime classes or other objects to get bid information into the auction object (submit_bid and submit_bid_state). The two are similar, except the latter has an additional input to define whether the state or if the load is currently ON or OFF (1 or 0) when the bid occurs. This is used for accounting in the capacity_reference_bid_quantity. The format is:

~~~
submit_bid( market object, bidding object, quantity, price, market id )
submit_bid_state( market object, bidding object, quantity, price, current state, market id )
~~~

## Properties

### `unit`

~~~
  char32 unit;
~~~

This describes the unit the auction is expecting to have information provided and delivered (input and output) for quantities. If a variable has units assigned to it, the auction will check to verify units are correct and convert where necessary (e.g. W->kW). If the variable does NOT have a unit assigned, such as a schedule or player file, the auction will assume the values are in this unit. Typical units are kW or MWh.

### `period`

~~~
  double period[s];
~~~

Defines the time between market clearings. This is also the valid bidding period for the market.

### `latency`

~~~
  double latency[s];
~~~

Defines the time between the market clearing and the price becoming active. For example, if the latency was set to 300 seconds (5 minutes), once the market clears, the current price would be "active" 5 minutes later.

### `market_id`

~~~
  int64 market_id;
~~~

This value is a unique identifier for each market frame, and is used to track bids across multiple time frames.

### `network`

~~~
  object network;
~~~

Future implementation and is not currently supported. Will be used to define the communications network to support market operations.

### `verbose`

~~~
  bool verbose;
~~~

Enables verbose output of the market. This will output all individual bid submissions, as well as information about the market clearing. Useful for debugging market interactions, or getting a more thorough view of the market proceedings.

### `linkref`

~~~
  object linkref;
~~~

This is a deprecated value that has similar properties to capacity_reference_object, but is no longer supported.

### `pricecap` or `price_cap`

~~~
  double pricecap;
  double price_cap;
~~~

Defines a maximum allowable bid price on the system. This bid effectively represents an infinite bid (this buyer must be satisfied first, or this seller must only be used as a last resort). Any bids above (or below the negative of) the amount will be truncated to this value and generate a warning message.

### `special_mode`

~~~
  enumeration {BUYERS_ONLY, SELLERS_ONLY, NONE} special_mode;
~~~

Enables different market type modes. The default `NONE` is the normal double-blind auction operation. With `SELLERS_ONLY` set, the market assumes no buyers will bid into the market and uses a fixed price or quantity (defined by fixed_price or fixed_quantity below) for the buyer's market. This is implemented as a single-blind auction scenario. `BUYERS_ONLY` is the converse scenario with the assumption that no sellers are on the system. The seller's market is then defined by the fixed_price or fixed_quantity inputs.

### `statistic_mode`

~~~
  enumeration {OFF, ON} statistic_mode;
~~~

By default, this is `ON`, and activates the calculation of market statistics a la Olympic Peninsula demonstration.

### `fixed_price`

~~~
  double fixed_price;
~~~

Defines the fixed price for special market bids. If `special_mode` is defined as something other than `NONE`, the market will use this price as the bidding price of the absent party (buyer or seller) for all market clearing scenarios.


### `fixed_quantity`

~~~
  double fixed_quantity;
~~~

Defines the fixed quantity for special market bids. If `special_mode` is defined as something other than `NONE`, the market will use this quantity as the bidding quantity of the absent party (buyer or seller) for all market clearing scenarios.


### `capacity_reference_object`

~~~
  object capacity_reference_object;
~~~

Defines an object in the current system that contains a cumulative `units` property. This property represents the total demand on the market and is to be used to help estimate unresponsive buyers on the system. The secondary variable, `capacity_reference_property` is the specific property of the object use. In a power system market, `capacity_reference_object` would be the feeder-level transformer object, and `capacity_reference_property` would be a property like `power_in`.

### `capacity_reference_property`

~~~
  char32 capacity_reference_property;
~~~

Defines the property of an object in the current system that contains a cumulative `units` value. This value represents the total demand on the market and is to be used to help estimate unresponsive buyers on the system. This property is read from the object specified in `capacity_reference_object`. In a power system market, `capacity_reference_object` would be the feeder-level transformer object, and `capacity_reference_property` would be a property like `power_in`.

### `capacity_reference_bid_price`

~~~
  double capacity_reference_bid_price;
~~~

Defines the price that the capacity reference should be bid at (typically `price_cap` or wholesale market price).

### `max_capacity_reference_bid_quantity`

~~~
  double max_capacity_reference_bid_quantity;
~~~

Defines the maximum quantity that the capacity reference should be bid at (e.g. the maximum rated power at the sustation).

### `capacity_reference_bid_quantity`

~~~
  double capacity_reference_bid_quantity;
~~~

Not used at this time.

### `init_price`

~~~
  double init_price;
~~~

Defines the initial value to populate the market statistic buffer with. This will result in a mean of `init_price` for all starting intervals. For example, if `init_price` is `5.0`, then `current_mean_1d` would be `5.0`. The calculated means will immediately become "valid" and slowly update toward the actual mean as the initial buffer is fully populated with clearing prices. This is typically used in conjunction with `warmup 0` to avoid the startup transient.

### `init_stdev`

~~~
  double init_stdev;
~~~

Defines the initial value for populating any uninitialized standard deviations. If the standard deviation is not initialized on auction creation, it will revert to this value until the statistics interval is valid. For example, with a 1-hour market period, current_stdev_1d would remain at `init_stdev` until 24 market clearing prices are obtained. At this point, the current_stdev_1d value will represent the calculated standard deviation, not the value in `init_stdev`. This value must be specified, or the explicit `past_stdev_T` or `current_stdev_T` must be initialized to a user specified value. If unspecified, the auction will refuse to start and return an error.

### `future_mean_price`

~~~
  double future_mean_price;
~~~

Value of the mean, if representing a day-ahead market.

### `use_future_mean_price`

~~~
  bool use_future_mean_price;
~~~

By default, this is deactivated (0). If activated, ignores mean value calculations and uses future_mean_price as the mean to calculate the standard deviation around. This is used when day-ahead markets are considered.

### `current_market.start_time`

~~~
  timestamp current_market.start_time;
~~~

Represents the time this market becomes active.

### `current_market.end_time`

~~~
  timestamp current_market.end_time;
~~~

Represents the time this market becomes inactive.

### `current_market.clearing_price`

~~~
  double current_market.clearing_price[$];
~~~

Represents this market's clearing price.

### `current_market.clearing_quantity`

~~~
  double current_market.clearing_quantity;
~~~

Represents this market's clearing quantity.

### `current_market.clearing_type`

~~~
  enumeration {NULL, FAILURE, EXACT, MARGINAL_PRICE, MARGINAL_BUYER, MARGINAL_SELLER} current_market.clearing_type;
~~~

Represent the type of clearing situation that has occurred in the current market.

### `current_market.marginal_quantity_load` or `current_market.marginal_quantity`

~~~
  double current_market.marginal_quantity_load;
  double current_market.marginal_quantity;
~~~

Represents the marginal quantity of this market. Useful for debugging, as well as providing information for marginal buyers and sellers to handle proportional responses.

### `current_market.marginal_quantity_bid`

~~~
  double current_market.marginal_quantity_bid;
~~~

Represents the marginal bid of this market. Useful for debugging, as well as providing information for marginal buyers and sellers to handle proportional responses.

### `current_market.marginal_quantity_frac`

~~~
  double current_market.marginal_quantity_frac;
~~~

Represents the fraction of the bid quantity at the marginal quantity of this market. Useful for debugging, as well as providing information for marginal buyers and sellers to handle proportional responses.

### `current_market.seller_total_quantity`

~~~
  double current_market.seller_total_quantity;
~~~

Represents the cumulative quantity of all sellers in the seller curve, corresponding to the upper right corner.

### `current_market.buyer_total_quantity`

~~~
  double current_market.buyer_total_quantity;
~~~

Represents the cumulative quantity of all buyers in the buyer curve, corresponding to the lower right corner.

### `current_market.seller_min_price`

~~~
  double current_market.seller_min_price;
~~~

Represents the price of the lowest seller in the market.

### `current_market.buyer_total_unrep`

~~~
  double current_market.buyer_total_unrep;
~~~

Represents the total load of the unresponsive buyers in the current market, as defined by those who bid at the price cap.

### `current_market.cap_ref_unrep`

~~~
  double current_market.cap_ref_unrep;
~~~

Represents the total load of the unresponsive buyers in the current market as it was estimated within the capacity reference object.

### `next_market.*`

~~~
  timestamp next_market.*;
~~~

Represents the next market information, analogous to `current_market.*`.

### `past_market.*`

~~~
  timestamp past_market.*;
~~~

Represents the last market information, analogous to `current_market.*`.

### `margin_mode`

~~~
  enumeration {PROB, DENY, NORMAL} margin_mode;
~~~

Controls the way in which a market’s marginal devices behave, when `use_override` is `ON`. `NORMAL` indicates that marginal bidders will run normally. `DENY` will send a ‘turn off’ signal to marginal bidders. `PROB` indicates that the market has a chance of controlling the device to run, where the chance is the ratio of the marginal quantity to the bid quantity. (a 4.5kW HVAC has an 80% chance of running should the marginal quantity be 3.6kW).


### `warmup`

~~~
  int32 warmup;
~~~

Activates or de-activates bidding during the first 24 hours of the market to assist in "boot-strapping" of the market. If =1, bids will be ignored during first 24 hours of simulation.

### `ignore_failed_market`

~~~
  enumeration {TRUE, FALSE} ignore_failed_market;
~~~

TODO

### `ignore_pricecap`

~~~
  enumeration {TRUE, FALSE} ignore_pricecap;
~~~

Tells the auction that market cycles that clear at the price_cap shall not be used to calculate the mean price and its standard deviation. This prevents moments of market failure or similar price shocks from destabilizing the auction with wildly swinging price statistics.

### `transaction_log_file`

~~~
  char256 transaction_log_file;
~~~

By default, this is off. If given a file name, this log will record every bid (market id, time, price, quantity, state, and object) for both buyers and sellers up to a limit defined by `transaction_log_limit`.

### `transaction_log_limit`

~~~
  int32 transaction_log_limit;
~~~

Defines how many market cycles to capture in the transaction_log_file in terms of number of unique market ids. By default, this will capture every market from beginning to end of simulation.

### `curve_log_file`

~~~
  char256 curve_log_file;
~~~

By default, this is off. If given a file name, this log will record the final buyer and seller curve at the end of each market clearing.

### `curve_log_limit`

~~~
  int32 curve_log_limit;
~~~

Defines the number of market cycles to capture in the curve_log_file in terms of number of uniqure market ids. By default, this will capture every market from beginning to end of simulation.

### `curve_log_info`

~~~
  enumeration {EXTRA, NORMAL} curve_log_info;
~~~

Determines how much information to record in the curve_log_file. If `NORMAL`, this will only record the buyer and seller curves. If `EXTRA`, it will also record a number of additional values of interest, such as .clearing_type, .marginal_quantity, responsive and unresponsive loading, etc.

# Example

This is an auction setup for a single-sided market that allows controllers to respond to changes in price relative to the standard deviation and mean of the previous 24 hours:

~~~
class auction {
    double current_price_mean_24h;
    double current_price_stdev_24h;
}
object auction {
    name Market_1;
    period 900;
    special_mode BUYERS_ONLY;
    unit kW;
     object player {
         file price.player;
         loop 10;
         property current_market.clearing_price;
     };
}
~~~

You could also set up your own static values and not use the built-in statistics:

~~~
class auction {
    double my_avg;
    double my_std;
}
object auction {
    name Market_1;
    period 900;
    special_mode BUYERS_ONLY;
    unit kW;
    statistic_mode OFF;
    my_avg 0.110000;
    my_std 0.037953;
     object player {
         file price.player;
         loop 10;
         property current_market.clearing_price;
     };
}
~~~

To set up a congestion object with a full double-auction market, where the capacity object could bid in the LMP of the feeder, with a power limit of 1200 kW, and starts collecting bids immediately:

~~~
class auction {
    double current_price_mean_24h;
    double current_price_stdev_24h;
}
object auction {
    name Market_1;
    period 900;
    unit kW;
    capacity_reference_object Substation_Transformer;
    capacity_reference_property power_out_real;
    max_capacity_reference_bid_quantity 1200; //Defaults to 1200 kW
    init_price 0.10;
    init_stdev 0.03;
    warmup 0;
     object player {
         file price.player;
         loop 10;
         property capacity_reference_bid_price;
     };    
}
~~~

# See also

* [[/Module/Market]]

