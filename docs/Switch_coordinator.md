[[Switch_coordinator]] -- Manage switches in powerflow module

# Synopsis
GLM:
~~~
  module gismo;
  object switch_coordinator
  {
     name "scheme-name";
     connect "switch_1-name";
     connect "switch_2-name";
     ...
     connect "switch_N-name";
     armed "switch_1-name|switch_2-name|...|switch_N-name";
     status "IDLE|ARMED|TOGGLE|DIRECT";
  }
~~~

# Description

The [[switch_coordinator]] object implements a switch coordination scheme for up to 32 switches in the powerflow module.  Switches are connected to the coordinator using the `connect` method by listing the name of the switch that is to be connected.

## connect

Use this property to connect a switch object to the switch coordinate.  This property may be called up to 32 times to connect at most 32 switches.

## armed 

The switch coordinator works by maintaining a list of `armed` switches which are subject to the next switch coordination status change. 

## status

The coordinator `status` is used to control the operating mode of the switch coordinator.

### IDLE

When the `status` is `IDLE` the coordinator is ready to receive arming commands, which are set by changing the switches listed in the `armed` property.  

After the switches are successfully operated, the coordinator's status will change to `IDLE` to indicate the operation has been complete.

### ARMED

When the status is changed to `TOGGLE` or `DIRECT` the switches listed in the `armed` property are operated as follows:

### DIRECT

The switches listed will be closed. All the switches not listed will be opened.

### TOGGLE

The switches listed will be toggled (i.e., open switches will close and closed switches will open).  
All the switches not listed will be unchanged.

# Example

The following example creates and switch coordinator, connects three switch, and sets the initial switch configuration to close switches 1 and 2.
~~~
  object switch_coordinator
  {
    name "example";
    connect "switch-1";
    connect "switch-2";
    connect "switch-3";
    armed "switch-1|switch-2";
    status "DIRECT";
  }
~~~

# Caveat

The `DIRECT` status does not control relative to `NORMAL` state, rather it controls as `OPEN=0` and `CLOSED=1` state. It would be nice to have a mode `NORMAL` that controls as `NORMAL=0` and `NOT_NORMAL=1` so that switches can be controlled relative to their `NORMAL` state.

# See also

* http://gridlab-d.shoutwiki.com/wiki/Powerflow_(module)