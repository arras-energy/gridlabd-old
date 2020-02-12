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

### `latency`

~~~
  double latency[s];
~~~

Latency between market clearing and delivery

### `market_id`

~~~
  int64 market_id;
~~~

Unique identifier of market clearing

### `network`

~~~
  object network;
~~~

The comm network used by object to talk to the market (if any)

### `verbose`

~~~
  bool verbose;
~~~

Enable verbose auction operations

### `linkref`

~~~
  object linkref;
~~~

Reference to link object that has demand as power_out (only used when not all loads are bidding)

### `pricecap`

~~~
  double pricecap;
~~~

The maximum price (magnitude) allowed

### `price_cap`

~~~
  double price_cap;
~~~

The maximum price (magnitude) allowed

### `special_mode`

~~~
  enumeration {BUYERS_ONLY, SELLERS_ONLY, NONE} special_mode;
~~~

TODO

### `statistic_mode`

~~~
  enumeration {OFF, ON} statistic_mode;
~~~

TODO

### `fixed_price`

~~~
  double fixed_price;
~~~

TODO

### `fixed_quantity`

~~~
  double fixed_quantity;
~~~

TODO

### `capacity_reference_object`

~~~
  object capacity_reference_object;
~~~

TODO

### `capacity_reference_property`

~~~
  char32 capacity_reference_property;
~~~

TODO

### `capacity_reference_bid_price`

~~~
  double capacity_reference_bid_price;
~~~

TODO

### `max_capacity_reference_bid_quantity`

~~~
  double max_capacity_reference_bid_quantity;
~~~

TODO

### `capacity_reference_bid_quantity`

~~~
  double capacity_reference_bid_quantity;
~~~

TODO

### `init_price`

~~~
  double init_price;
~~~

TODO

### `init_stdev`

~~~
  double init_stdev;
~~~

TODO

### `future_mean_price`

~~~
  double future_mean_price;
~~~

TODO

### `use_future_mean_price`

~~~
  bool use_future_mean_price;
~~~

TODO

### `current_market.start_time`

~~~
  timestamp current_market.start_time;
~~~

TODO

### `current_market.end_time`

~~~
  timestamp current_market.end_time;
~~~

TODO

### `current_market.clearing_price`

~~~
  double current_market.clearing_price[$];
~~~

TODO

### `current_market.clearing_quantity`

~~~
  double current_market.clearing_quantity;
~~~

TODO

### `current_market.clearing_type`

~~~
  enumeration {NULL, FAILURE, EXACT, MARGINAL_PRICE, MARGINAL_BUYER, MARGINAL_SELLER} current_market.clearing_type;
~~~

TODO

### `current_market.marginal_quantity_load`

~~~
  double current_market.marginal_quantity_load;
~~~

TODO

### `current_market.marginal_quantity`

~~~
  double current_market.marginal_quantity;
~~~

TODO

### `current_market.marginal_quantity_bid`

~~~
  double current_market.marginal_quantity_bid;
~~~

TODO

### `current_market.marginal_quantity_frac`

~~~
  double current_market.marginal_quantity_frac;
~~~

TODO

### `current_market.seller_total_quantity`

~~~
  double current_market.seller_total_quantity;
~~~

TODO

### `current_market.buyer_total_quantity`

~~~
  double current_market.buyer_total_quantity;
~~~

TODO

### `current_market.seller_min_price`

~~~
  double current_market.seller_min_price;
~~~

TODO

### `current_market.buyer_total_unrep`

~~~
  double current_market.buyer_total_unrep;
~~~

TODO

### `current_market.cap_ref_unrep`

~~~
  double current_market.cap_ref_unrep;
~~~

TODO

### `next_market.start_time`

~~~
  timestamp next_market.start_time;
~~~

TODO

### `next_market.end_time`

~~~
  timestamp next_market.end_time;
~~~

TODO

### `next_market.clearing_price`

~~~
  double next_market.clearing_price[$];
~~~

TODO

### `next_market.clearing_quantity`

~~~
  double next_market.clearing_quantity;
~~~

TODO

### `next_market.clearing_type`

~~~
  enumeration {NULL, FAILURE, EXACT, MARGINAL_PRICE, MARGINAL_BUYER, MARGINAL_SELLER} next_market.clearing_type;
~~~

TODO

### `next_market.marginal_quantity_load`

~~~
  double next_market.marginal_quantity_load;
~~~

TODO

### `next_market.marginal_quantity_bid`

~~~
  double next_market.marginal_quantity_bid;
~~~

TODO

### `next_market.marginal_quantity_frac`

~~~
  double next_market.marginal_quantity_frac;
~~~

TODO

### `next_market.seller_total_quantity`

~~~
  double next_market.seller_total_quantity;
~~~

TODO

### `next_market.buyer_total_quantity`

~~~
  double next_market.buyer_total_quantity;
~~~

TODO

### `next_market.seller_min_price`

~~~
  double next_market.seller_min_price;
~~~

TODO

### `next_market.cap_ref_unrep`

~~~
  double next_market.cap_ref_unrep;
~~~

TODO

### `past_market.start_time`

~~~
  timestamp past_market.start_time;
~~~

TODO

### `past_market.end_time`

~~~
  timestamp past_market.end_time;
~~~

TODO

### `past_market.clearing_price`

~~~
  double past_market.clearing_price[$];
~~~

TODO

### `past_market.clearing_quantity`

~~~
  double past_market.clearing_quantity;
~~~

TODO

### `past_market.clearing_type`

~~~
  enumeration {NULL, FAILURE, EXACT, MARGINAL_PRICE, MARGINAL_BUYER, MARGINAL_SELLER} past_market.clearing_type;
~~~

TODO

### `past_market.marginal_quantity_load`

~~~
  double past_market.marginal_quantity_load;
~~~

TODO

### `past_market.marginal_quantity_bid`

~~~
  double past_market.marginal_quantity_bid;
~~~

TODO

### `past_market.marginal_quantity_frac`

~~~
  double past_market.marginal_quantity_frac;
~~~

TODO

### `past_market.seller_total_quantity`

~~~
  double past_market.seller_total_quantity;
~~~

TODO

### `past_market.buyer_total_quantity`

~~~
  double past_market.buyer_total_quantity;
~~~

TODO

### `past_market.seller_min_price`

~~~
  double past_market.seller_min_price;
~~~

TODO

### `past_market.cap_ref_unrep`

~~~
  double past_market.cap_ref_unrep;
~~~

TODO

### `margin_mode`

~~~
  enumeration {PROB, DENY, NORMAL} margin_mode;
~~~

TODO

### `warmup`

~~~
  int32 warmup;
~~~

TODO

### `ignore_failed_market`

~~~
  enumeration {TRUE, FALSE} ignore_failed_market;
~~~

TODO

### `ignore_pricecap`

~~~
  enumeration {TRUE, FALSE} ignore_pricecap;
~~~

TODO

### `transaction_log_file`

~~~
  char256 transaction_log_file;
~~~

TODO

### `transaction_log_limit`

~~~
  int32 transaction_log_limit;
~~~

TODO

### `curve_log_file`

~~~
  char256 curve_log_file;
~~~

TODO

### `curve_log_limit`

~~~
  int32 curve_log_limit;
~~~

TODO

### `curve_log_info`

~~~
  enumeration {EXTRA, NORMAL} curve_log_info;
~~~

TODO

# Example

~~~
  object auction {
    unit "";
    period "0.0";
    latency "0.0";
    market_id "0";
    verbose "FALSE";
    pricecap "0.0";
    price_cap "0.0";
    special_mode "0";
    statistic_mode "0";
    fixed_price "0.0";
    fixed_quantity "0.0";
    capacity_reference_property "";
    capacity_reference_bid_price "0.0";
    max_capacity_reference_bid_quantity "0.0";
    capacity_reference_bid_quantity "0.0";
    init_price "0.0";
    init_stdev "0.0";
    future_mean_price "0.0";
    use_future_mean_price "FALSE";
    current_market.start_time "TS_ZERO";
    current_market.end_time "TS_ZERO";
    current_market.clearing_price "0.0";
    current_market.clearing_quantity "0.0";
    current_market.clearing_type "0";
    current_market.marginal_quantity_load "0.0";
    current_market.marginal_quantity "0.0";
    current_market.marginal_quantity_bid "0.0";
    current_market.marginal_quantity_frac "0.0";
    current_market.seller_total_quantity "0.0";
    current_market.buyer_total_quantity "0.0";
    current_market.seller_min_price "0.0";
    current_market.buyer_total_unrep "0.0";
    current_market.cap_ref_unrep "0.0";
    next_market.start_time "TS_ZERO";
    next_market.end_time "TS_ZERO";
    next_market.clearing_price "0.0";
    next_market.clearing_quantity "0.0";
    next_market.clearing_type "0";
    next_market.marginal_quantity_load "0.0";
    next_market.marginal_quantity_bid "0.0";
    next_market.marginal_quantity_frac "0.0";
    next_market.seller_total_quantity "0.0";
    next_market.buyer_total_quantity "0.0";
    next_market.seller_min_price "0.0";
    next_market.cap_ref_unrep "0.0";
    past_market.start_time "TS_ZERO";
    past_market.end_time "TS_ZERO";
    past_market.clearing_price "0.0";
    past_market.clearing_quantity "0.0";
    past_market.clearing_type "0";
    past_market.marginal_quantity_load "0.0";
    past_market.marginal_quantity_bid "0.0";
    past_market.marginal_quantity_frac "0.0";
    past_market.seller_total_quantity "0.0";
    past_market.buyer_total_quantity "0.0";
    past_market.seller_min_price "0.0";
    past_market.cap_ref_unrep "0.0";
    margin_mode "0";
    warmup "0";
    ignore_failed_market "0";
    ignore_pricecap "0";
    transaction_log_file "";
    transaction_log_limit "0";
    curve_log_file "";
    curve_log_limit "0";
    curve_log_info "0";
  }
~~~

# See also

* [[/Module/Market]]

