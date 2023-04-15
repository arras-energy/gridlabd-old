[[/Module/Market]] -- Module market

# Synopsis

GLM:

~~~
  module market {
    message_flags "[QUIET|WARNING|DEBUG|VERBOSE]";
    bid_offset "<decimal> $";
  }
~~~

# Description

The market module is defined primarily by the bidding market capability it creates on the system. The actual market is created inside the auction object. The auction object is an object within the Market module . The auction object accepts bids from supply and demand devices within a system (sellers and buyers). After a set interval, the auction will resolve the supply and demand quantities and prices in a double-auction style to clear the market. This price will become the relevant cleared price after a specified latency interval.

The auction provides a means for different objects within the GridLAB-D program to base their supply or demand on a dynamic or real time price. The market implemented in the auction object is implemented as a double-auction market. A double-auction market is one where suppliers and demanders (sellers and bidders) submit their bids of desired price for a set quantity simultaneously. Bidding is handled in the manner outlined by Bid Acceptance below. Once the bidding submission period ends, the market "clears" by selecting the intersection point of the supply and demand curves. Six different clearing scenarios can occur. These different scenarios are outlined in the Market Clearing section below. After the market clears and the relevant latency interval expires, the market price becomes active. At this point, devices that bid into the market will respond appropriately based on internal logic comparing their bid price to the market clearing price. The auction object does not provide any book-keeping or enforcement of the market, it simply provides a central market for buyers and sellers to bid their respective prices and quantities.

At this time, the market only supports single-market bidding. For example, suppose the market is a day ahead energy market. Objects bid into this market one day in advance, but they must do this every hour. Rather than submitting all 24 bids into an hourly market for the next 24-hour period at once (block bidding), the market for 1 AM Tuesday will begin bidding at 1 AM Monday and end at 2 AM, the market for 2 AM Tuesday will begin bidding at 2 AM Monday and end at 3 AM, and so on and so forth.

## Market Objects

Current market objects (non-deprecated) include:

Auction objects (bid collection and market clearing):
- [[/Module/Market/Auction]]
- [[/Module/Market/Stubauction]]

Controller objects (modify behavior of controlled object and may bid into market):

- [[/Module/Market/Controller]]
- [[/Module/Market/Passive controller]]
- [[/Module/Market/Generator controller]]

Bidding objects (bids into a market, but does not control the response of any other object):

- [[/Module/Market/Stub bidder]]

Functional objects (contain functions for use by auction and controller objects - not described here)

- Bid
- Curve

## Auction Object

The auction provides a means for different objects within the GridLAB-D program to base their supply or demand on a dynamic or real time price. The market implemented in the auction object is implemented as a double-auction market. A double-auction market is one where suppliers and demanders (sellers and bidders) submit their bids of desired price for a set quantity simultaneously. Once the bidding submission period ends, the market "clears" by selecting the intersection point of the supply and demand curves. After the market clears and the relevant latency interval expires, the market price becomes active. At this point, devices that bid into the market will respond appropriately based on internal logic comparing their bid price to the market clearing price. The auction object does not provide any book-keeping or enforcement of the market, it simply provides a central market for buyers and sellers to bid their respective prices and quantities.

Further information describing clearing mechanisms and basic operation can be found [[/Module/Market]].

## Globals

### `message_flags`

~~~
  message_flags "[QUIET|WARNING|DEBUG|VERBOSE]";
~~~

Module message control flags

### `bid_offset`

~~~
  bid_offset "<decimal> $";
~~~

The bid offset value that prevents bids from being wrongly triggered

# See also

* [[/Module/Market/Auction]]
* [[/Module/Market/Controller]]
* [[/Module/Market/Stubauction]]
* [[/Module/Market/Passive_controller]]
* [[/Module/Market/Double_controller]]
* [[/Module/Market/Stub_bidder]]
* [[/Module/Market/Generator_controller]]
* [[/Module/Market/Supervisory_control]]

