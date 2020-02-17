[[/House]] -- Upgrade of residential house model

# Smart Fuse

A smart fuse controller has been added to the house distribution panel model.  In addition to providing overcurrent protection, smart fuses also provide voltage control to maintain constant power.  This capability is implemented using the `circuit` method in the house object.

## Initialization

When a house model is specified, the circuit specifications may be given using the <code>circuit</code> property.  The circuit specification is given as follows:

~~~
circuit "maxamps: <MAXAMPS>; vmin: <VMIN>; vmax: <VMAX>; control: <POWER>"
~~~

where `<MAXAMPS>` is the current trip setting, `<VMIN>>` and `<VMAX>` as the lower and upper voltage control limits, and `<POWER>` is the power control setting.  If `maxamps` is omitted, the enduse trip current is used as the default.  If `vmin` is omitted, 0.95 pu.V is used as the default.  If `vmax` is omitted, 1.05 puV is used as the default. If `control` is omitted, the power controller is disabled.

# Gas end-uses

The `gas_enduses` property can be set to a `|` separated list of end-uses that should be considered as supplied by natural gas instead of electricity.  These end-uses will still generate internal heat gains, but they will not present a load on the circuit to which they are assigned

# Messages

After initialization, the `circuit` property accepts messages of the form

~~~
<ENDUSE>&<COMMAND>
~~~

where `<STATUS>` is the enduse name of the circuit, and `<COMMAND>` is one of the commands described below.  

In all cases, the message reply is a JSON string of the form

~~~
{ "status" : "<STATUS>", "data" : "<DATA>" }
~~~

where `<STATUS>` is either `ok` for successful commands, or `error` for unsuccessful commands.

When the command is successful, the <code><<<DATA>>></code> block contains the following:

~~~
{ "breaker": "<STATE>", "maxamps": "<MAXAMPS>", "voltage": "<VOLTAGE>", "control": "<POWER>" }
~~~

where `<STATE>` is either `OPEN`, `CLOSED`, or `FAULT`.  The values of `<MAXAMPS>`, `<VOLTAGE>`, and `<POWER>` are given in units of Amps, Volts, and Watts, respectively.

When the command is unsuccessful, the `<DATA>` block contains the following:

~~~
{"info" : "<INFO>"}
~~~

## STATUS

When a `STATUS` message is received, no action is taken and the reply message contains current state of the circuit.

## RESET

When a `RESET` message is received, the circuit breaker is reset and the reply message contains current state of the circuit.

## OPEN

When a `OPEN` message is received, the circuit breaker is tripped and the reply message contains current state of the circuit.

## CONTROL

When a `CONTROL=value` message is received, the circuit power control reference is set to `value` and the reply message contains current state of the circuit.

# Controller Design

The voltage control logic is implemented as follows.  

* When the load is constant power, the circuit line voltage factor is set to `1.0`.  

* If the constant current load is 0.0, then the voltage is `Vf=(R-P)/I`.

* Otherwise the voltage is `Vf=(I^2-4(P-R)Z}-I)^(0.5)/(2Z)`, where `Z`, `I`, and `P` are the constant impedance, current, and power components of the loads, respectively, in Watts; `R` is the reference power in Watts, and `Vf` is the reference voltage in Volts.

