[[/Module/Powerflow/Vfd]] -- Class vfd

# Synopsis

GLM:

~~~
  object vfd {
    rated_motor_speed "<decimal> 1/min";
    desired_motor_speed "<decimal> 1/min";
    motor_poles "<decimal>";
    rated_output_voltage "<decimal> V";
    rated_horse_power "<decimal> hp";
    nominal_output_frequency "<decimal> Hz";
    desired_output_frequency "<decimal> Hz";
    current_output_frequency "<decimal> Hz";
    efficiency "<decimal> %";
    stable_time "<decimal> s";
    vfd_state "{STEADY_STATE,CHANGING,OFF}";
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

### `rated_motor_speed`
~~~
  double rated_motor_speed[1/min];
~~~

Rated speed of the VFD in RPM. Default = 1800 RPM

### `desired_motor_speed`
~~~
  double desired_motor_speed[1/min];
~~~

Desired speed of the VFD In ROM. Default = 1800 RPM (max)

### `motor_poles`
~~~
  double motor_poles;
~~~

Number of Motor Poles. Default = 4

### `rated_output_voltage`
~~~
  double rated_output_voltage[V];
~~~

Line to Line Voltage - VFD Rated voltage. Default to TO node nominal_voltage

### `rated_horse_power`
~~~
  double rated_horse_power[hp];
~~~

Rated Horse Power of the VFD. Default = 75 HP

### `nominal_output_frequency`
~~~
  double nominal_output_frequency[Hz];
~~~

Nominal VFD output frequency. Default = 60 Hz

### `desired_output_frequency`
~~~
  double desired_output_frequency[Hz];
~~~

VFD desired output frequency based on the desired RPM

### `current_output_frequency`
~~~
  double current_output_frequency[Hz];
~~~

VFD currently output frequency

### `efficiency`
~~~
  double efficiency[%];
~~~

Current VFD efficiency based on the load/VFD output Horsepower

### `stable_time`
~~~
  double stable_time[s];
~~~

Time taken by the VFD to reach desired frequency (based on RPM). Default = 1.45 seconds

### `vfd_state`
~~~
  enumeration {STEADY_STATE, CHANGING, OFF} vfd_state;
~~~

Current state of the VFD

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
  object vfd {
    rated_motor_speed "0.0";
    desired_motor_speed "0.0";
    motor_poles "0.0";
    rated_output_voltage "0.0";
    rated_horse_power "0.0";
    nominal_output_frequency "0.0";
    desired_output_frequency "0.0";
    current_output_frequency "0.0";
    efficiency "0.0";
    stable_time "0.0";
    vfd_state "0";
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

