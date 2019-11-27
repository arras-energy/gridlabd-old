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

TODO

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

