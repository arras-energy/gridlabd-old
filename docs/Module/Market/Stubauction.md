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

### `last.P`

~~~
  double last.P;
~~~

Last cleared price

### `current_market.clearing_price`

~~~
  double current_market.clearing_price;
~~~

Next cleared price

### `past_market.clearing_price`

~~~
  double past_market.clearing_price;
~~~

Last cleared price

### `next.P`

~~~
  double next.P;
~~~

Next cleared price

### `avg24`

~~~
  double avg24;
~~~

Daily average of price

### `std24`

~~~
  double std24;
~~~

Daily stdev of price

### `avg72`

~~~
  double avg72;
~~~

Three day price average

### `std72`

~~~
  double std72;
~~~

Three day price stdev

### `avg168`

~~~
  double avg168;
~~~

Weekly average of price

### `std168`

~~~
  double std168;
~~~

Weekly stdev of price

### `market_id`

~~~
  int64 market_id;
~~~

Unique identifier of market clearing

### `verbose`

~~~
  bool verbose;
~~~

Enable verbose stubauction operations

### `control_mode`

~~~
  enumeration {DISABLED, NORMAL} control_mode;
~~~

The control mode to use for determining average and deviation calculations

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

