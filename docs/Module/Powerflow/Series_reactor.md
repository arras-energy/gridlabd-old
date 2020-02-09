[[/Module/Powerflow/Series_reactor]] -- Class series_reactor

# Synopsis

GLM:

~~~
  object series_reactor {
    phase_A_impedance "<string> Ohm";
    phase_A_resistance "<decimal> Ohm";
    phase_A_reactance "<decimal> Ohm";
    phase_B_impedance "<string> Ohm";
    phase_B_resistance "<decimal> Ohm";
    phase_B_reactance "<decimal> Ohm";
    phase_C_impedance "<string> Ohm";
    phase_C_resistance "<decimal> Ohm";
    phase_C_reactance "<decimal> Ohm";
    rated_current_limit "<decimal> A";
    status "{OPEN,CLOSED}";
    from "<string>";
    to "<string>";
    power_in "<string> VA";
    power_out "<string> VA";
    power_out_real "<decimal> W";
    power_losses "<string> VA";
    power_in_A "<string> VA";
    power_in_B "<string> VA";
    power_in_C "<string> VA";
    power_out_A "<string> VA";
    power_out_B "<string> VA";
    power_out_C "<string> VA";
    power_losses_A "<string> VA";
    power_losses_B "<string> VA";
    power_losses_C "<string> VA";
    current_out_A "<string> A";
    current_out_B "<string> A";
    current_out_C "<string> A";
    current_in_A "<string> A";
    current_in_B "<string> A";
    current_in_C "<string> A";
    fault_current_in_A "<string> A";
    fault_current_in_B "<string> A";
    fault_current_in_C "<string> A";
    fault_current_out_A "<string> A";
    fault_current_out_B "<string> A";
    fault_current_out_C "<string> A";
    fault_voltage_A "<string> A";
    fault_voltage_B "<string> A";
    fault_voltage_C "<string> A";
    flow_direction "[CN|CR|CF|BN|BR|BF|AN|AR|AF|UNKNOWN]";
    mean_repair_time "<decimal> s";
    continuous_rating "<decimal> A";
    emergency_rating "<decimal> A";
    inrush_convergence_value "<decimal> V";
    phases "[ABCDNSG]";
    nominal_voltage "<decimal> V";
    supernode_name "<string>";
  }
~~~

# Description

TODO

## Properties

### `phase_A_impedance`
~~~
  complex phase_A_impedance[Ohm];
~~~

Series impedance of reactor on phase A

### `phase_A_resistance`
~~~
  double phase_A_resistance[Ohm];
~~~

Resistive portion of phase A's impedance

### `phase_A_reactance`
~~~
  double phase_A_reactance[Ohm];
~~~

Reactive portion of phase A's impedance

### `phase_B_impedance`
~~~
  complex phase_B_impedance[Ohm];
~~~

Series impedance of reactor on phase B

### `phase_B_resistance`
~~~
  double phase_B_resistance[Ohm];
~~~

Resistive portion of phase B's impedance

### `phase_B_reactance`
~~~
  double phase_B_reactance[Ohm];
~~~

Reactive portion of phase B's impedance

### `phase_C_impedance`
~~~
  complex phase_C_impedance[Ohm];
~~~

Series impedance of reactor on phase C

### `phase_C_resistance`
~~~
  double phase_C_resistance[Ohm];
~~~

Resistive portion of phase C's impedance

### `phase_C_reactance`
~~~
  double phase_C_reactance[Ohm];
~~~

Reactive portion of phase C's impedance

### `rated_current_limit`
~~~
  double rated_current_limit[A];
~~~

Rated current limit for the reactor

### `status`
~~~
  enumeration {OPEN, CLOSED} status;
~~~


### `from`
~~~
  object from;
~~~

From_node - source node

### `to`
~~~
  object to;
~~~

To_node - load node

### `power_in`
~~~
  complex power_in[VA];
~~~

Power flow in (w.r.t from node)

### `power_out`
~~~
  complex power_out[VA];
~~~

Power flow out (w.r.t to node)

### `power_out_real`
~~~
  double power_out_real[W];
~~~

Power flow out (w.r.t to node), real

### `power_losses`
~~~
  complex power_losses[VA];
~~~

Power losses

### `power_in_A`
~~~
  complex power_in_A[VA];
~~~

Power flow in (w.r.t from node), phase A

### `power_in_B`
~~~
  complex power_in_B[VA];
~~~

Power flow in (w.r.t from node), phase B

### `power_in_C`
~~~
  complex power_in_C[VA];
~~~

Power flow in (w.r.t from node), phase C

### `power_out_A`
~~~
  complex power_out_A[VA];
~~~

Power flow out (w.r.t to node), phase A

### `power_out_B`
~~~
  complex power_out_B[VA];
~~~

Power flow out (w.r.t to node), phase B

### `power_out_C`
~~~
  complex power_out_C[VA];
~~~

Power flow out (w.r.t to node), phase C

### `power_losses_A`
~~~
  complex power_losses_A[VA];
~~~

Power losses, phase A

### `power_losses_B`
~~~
  complex power_losses_B[VA];
~~~

Power losses, phase B

### `power_losses_C`
~~~
  complex power_losses_C[VA];
~~~

Power losses, phase C

### `current_out_A`
~~~
  complex current_out_A[A];
~~~

Current flow out of link (w.r.t. to node), phase A

### `current_out_B`
~~~
  complex current_out_B[A];
~~~

Current flow out of link (w.r.t. to node), phase B

### `current_out_C`
~~~
  complex current_out_C[A];
~~~

Current flow out of link (w.r.t. to node), phase C

### `current_in_A`
~~~
  complex current_in_A[A];
~~~

Current flow to link (w.r.t from node), phase A

### `current_in_B`
~~~
  complex current_in_B[A];
~~~

Current flow to link (w.r.t from node), phase B

### `current_in_C`
~~~
  complex current_in_C[A];
~~~

Current flow to link (w.r.t from node), phase C

### `fault_current_in_A`
~~~
  complex fault_current_in_A[A];
~~~

Fault current flowing in, phase A

### `fault_current_in_B`
~~~
  complex fault_current_in_B[A];
~~~

Fault current flowing in, phase B

### `fault_current_in_C`
~~~
  complex fault_current_in_C[A];
~~~

Fault current flowing in, phase C

### `fault_current_out_A`
~~~
  complex fault_current_out_A[A];
~~~

Fault current flowing out, phase A

### `fault_current_out_B`
~~~
  complex fault_current_out_B[A];
~~~

Fault current flowing out, phase B

### `fault_current_out_C`
~~~
  complex fault_current_out_C[A];
~~~

Fault current flowing out, phase C

### `fault_voltage_A`
~~~
  complex fault_voltage_A[A];
~~~

Fault voltage, phase A

### `fault_voltage_B`
~~~
  complex fault_voltage_B[A];
~~~

Fault voltage, phase B

### `fault_voltage_C`
~~~
  complex fault_voltage_C[A];
~~~

Fault voltage, phase C

### `flow_direction`
~~~
  set {CN, CR, CF, BN, BR, BF, AN, AR, AF, UNKNOWN} flow_direction;
~~~

Flag used for describing direction of the flow of power

### `mean_repair_time`
~~~
  double mean_repair_time[s];
~~~

Time after a fault clears for the object to be back in service

### `continuous_rating`
~~~
  double continuous_rating[A];
~~~

Continuous rating for this link object (set individual line segments

### `emergency_rating`
~~~
  double emergency_rating[A];
~~~

Emergency rating for this link object (set individual line segments

### `inrush_convergence_value`
~~~
  double inrush_convergence_value[V];
~~~

Tolerance, as change in line voltage drop between iterations, for deltamode in-rush completion

### `phases`
~~~
  set {A, B, C, D, N, S, G} phases;
~~~

TODO

### `nominal_voltage`
~~~
  double nominal_voltage[V];
~~~

TODO

### `supernode_name`
~~~
  char1024 supernode_name;
~~~

TODO

# Example

~~~
  object series_reactor {
    phase_A_impedance "0+0i";
    phase_A_resistance "0.0";
    phase_A_reactance "0.0";
    phase_B_impedance "0+0i";
    phase_B_resistance "0.0";
    phase_B_reactance "0.0";
    phase_C_impedance "0+0i";
    phase_C_resistance "0.0";
    phase_C_reactance "0.0";
    rated_current_limit "0.0";
    status "0";
    power_in "0+0i";
    power_out "0+0i";
    power_out_real "0.0";
    power_losses "0+0i";
    power_in_A "0+0i";
    power_in_B "0+0i";
    power_in_C "0+0i";
    power_out_A "0+0i";
    power_out_B "0+0i";
    power_out_C "0+0i";
    power_losses_A "0+0i";
    power_losses_B "0+0i";
    power_losses_C "0+0i";
    current_out_A "0+0i";
    current_out_B "0+0i";
    current_out_C "0+0i";
    current_in_A "0+0i";
    current_in_B "0+0i";
    current_in_C "0+0i";
    fault_current_in_A "0+0i";
    fault_current_in_B "0+0i";
    fault_current_in_C "0+0i";
    fault_current_out_A "0+0i";
    fault_current_out_B "0+0i";
    fault_current_out_C "0+0i";
    fault_voltage_A "0+0i";
    fault_voltage_B "0+0i";
    fault_voltage_C "0+0i";
    flow_direction "0";
    mean_repair_time "0.0";
    continuous_rating "0.0";
    emergency_rating "0.0";
    inrush_convergence_value "0.0";
    phases "0";
    nominal_voltage "0.0";
    supernode_name "";
  }
~~~

# See also
* [[/Module/Powerflow]]

