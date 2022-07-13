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

This object is a "fake" bidder into the market. It can perform price and quantity bids (both buy and sell), but will not be directly reflected on the power system solution. Additionally, it is not able to control another device as a response to the price. This object is generally used for testing purposes, or to "fill out" a market that doesn't have enough buyers or sellers to be stable. According to its design, it is only able to use the submit_bid function, and does not support submit_bid_state.

## Properties

### `bid_period`

~~~
  double bid_period[s];
~~~

Describes how long between bids. Should generally align, or be a multiple of, with the auction market period.

### `count`

~~~
  int16 count;
~~~

Determines how many market periods the stub_bidder should bid. After count decrements to zero, the stub_bidder will no longer bid into the market.

### `market`

~~~
  object market;
~~~

This references the market that provides the price signal to the controller, and generates the rolling average and standard deviations seen by the object. This is also the object into which the controller will bid its price. It is typically specified as an auction or stubauction object, and is typically referenced by the name of the object.


### `role`

~~~
  enumeration {SELLER, BUYER} role;
~~~

Describes whether the device should be bidding into the market on the BUYER or SELLER curve.

### `price`

~~~
  double price;
~~~

This specifies the bidding price for the bidder at the given operating points. Must be between negative and positive price cap, or will be cut off by the auction.

### `quantity`

~~~
  double quantity;
~~~

This specifies the amount of power demanded by the object at the determined price. Must be a non-zero positive number.

### `bid_id`

~~~
  int64 bid_id;
~~~

Id of the last bid submitted.

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

