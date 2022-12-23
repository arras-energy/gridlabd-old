[[/Module/Market/Stubauction]] -- Class stubauction

# Synopsis

GLM:

~~~
  object stubauction {
    unit "<string>";
    period "<decimal> s";
    last.P "<decimal>";
    current_market.clearing_price "<decimal>";
    past_market.clearing_price "<decimal>";
    next.P "<decimal>";
    avg24 "<decimal>";
    std24 "<decimal>";
    avg72 "<decimal>";
    std72 "<decimal>";
    avg168 "<decimal>";
    std168 "<decimal>";
    market_id "<integer>";
    verbose "<string>";
    control_mode "{DISABLED,NORMAL}";
  }
~~~

# Description

This object performs in a similar manner to an auction object in the BUYERS_ONLY mode. This object will most likely be deprecated in versions 3.0 and greater.

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

### `last.P`, `past_market.clearing_price`

~~~
  double last.P;
  double past_market.clearing_price;
~~~

This is the previous market's clearing price, similar to auction.

### `next.P`, `current_market.clearing_price`

~~~
  double next.P;
  double current_market.clearing_price;
~~~

This is the current market's clearing price, similar to auction.

### `avg24`, `avg72`, `avg168`

~~~
  double avg24;
  double avg72;
  double avg168;
~~~

Unlike the auction object, statistics are not customizable in the stubauction. These values calculate the mean price over the previous day, 3-day, and 1-week periods.

### `std24`, `std72`, `std168`

~~~
  double std24;
  double std72;
  double std168;
~~~

Unlike the auction object, statistics are not customizable in the stubauction. These values calculate the standard deviation of price over the previous day, 3-day, and 1-week periods.

### `market_id`

~~~
  int64 market_id;
~~~

This value is a unique identifier for each market frame, and is used to track bids across multiple time frames.

### `verbose`

~~~
  bool verbose;
~~~

Enables verbose output of the market. This will output all individual bid submissions, as well as information about the market clearing. Useful for debugging market interactions, or getting a more thorough view of the market proceedings.

### `control_mode`

~~~
  enumeration {DISABLED, NORMAL} control_mode;
~~~

TTurns the statistic calculations on and off (`NORMAL` or on by default).

# Example

~~~
  object stubauction {
    unit "";
    period "0.0";
    last.P "0.0";
    current_market.clearing_price "0.0";
    past_market.clearing_price "0.0";
    next.P "0.0";
    avg24 "0.0";
    std24 "0.0";
    avg72 "0.0";
    std72 "0.0";
    avg168 "0.0";
    std168 "0.0";
    market_id "0";
    verbose "FALSE";
    control_mode "0";
  }
~~~

# See also

* [[/Module/Market]]

