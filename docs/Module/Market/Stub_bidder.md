[[/Module/Market/Stub_bidder]] -- Class stub_bidder

# Synopsis

GLM:

~~~
  object stub_bidder {
    bid_period "<decimal> s";
    count "<integer>";
    market "<string>";
    role "{SELLER,BUYER}";
    price "<decimal>";
    quantity "<decimal>";
    bid_id "<integer>";
  }
~~~

# Description

TODO

## Properties

### `bid_period`

~~~
  double bid_period[s];
~~~

TODO

### `count`

~~~
  int16 count;
~~~

TODO

### `market`

~~~
  object market;
~~~

TODO

### `role`

~~~
  enumeration {SELLER, BUYER} role;
~~~

TODO

### `price`

~~~
  double price;
~~~

TODO

### `quantity`

~~~
  double quantity;
~~~

TODO

### `bid_id`

~~~
  int64 bid_id;
~~~

TODO

# Example

~~~
  object stub_bidder {
    bid_period "0.0";
    count "0";
    role "0";
    price "0.0";
    quantity "0.0";
    bid_id "0";
  }
~~~

# See also

* [[/Module/Market]]

