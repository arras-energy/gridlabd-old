[[/Module/Powerflow/Transformer]] -- Class transformer

# Synopsis

GLM:

~~~
  object transformer {
    configuration "<string>";
    climate "<string>";
    ambient_temperature "<decimal> degC";
    top_oil_hot_spot_temperature "<decimal> degC";
    winding_hot_spot_temperature "<decimal> degC";
    percent_loss_of_life "<decimal>";
    aging_constant "<decimal>";
    use_thermal_model "<string>";
    transformer_replacement_count "<decimal>";
    aging_granularity "<decimal> s";
    phase_A_primary_flux_value "<decimal> Wb";
    phase_B_primary_flux_value "<decimal> Wb";
    phase_C_primary_flux_value "<decimal> Wb";
    phase_A_secondary_flux_value "<decimal> Wb";
    phase_B_secondary_flux_value "<decimal> Wb";
    phase_C_secondary_flux_value "<decimal> Wb";
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
    degradation_factor "<decimal> pu";
  }
~~~

# Description

TODO

## Properties

### `configuration`

~~~
  object configuration;
~~~

Configuration library used for transformer setup

### `climate`

~~~
  object climate;
~~~

Climate object used to describe thermal model ambient temperature

### `ambient_temperature`

~~~
  double ambient_temperature[degC];
~~~

Ambient temperature in degrees C

### `top_oil_hot_spot_temperature`

~~~
  double top_oil_hot_spot_temperature[degC];
~~~

Top-oil hottest-spot temperature, degrees C

### `winding_hot_spot_temperature`

~~~
  double winding_hot_spot_temperature[degC];
~~~

Winding hottest-spot temperature, degrees C

### `percent_loss_of_life`

~~~
  double percent_loss_of_life;
~~~

The percent loss of life

### `aging_constant`

~~~
  double aging_constant;
~~~

The aging rate slope for the transformer insulation

### `use_thermal_model`

~~~
  bool use_thermal_model;
~~~

Boolean to enable use of thermal model

### `transformer_replacement_count`

~~~
  double transformer_replacement_count;
~~~

Counter of the number times the transformer has been replaced due to lifetime failure

### `aging_granularity`

~~~
  double aging_granularity[s];
~~~

Maximum timestep before updating thermal and aging model in seconds

### `phase_A_primary_flux_value`

~~~
  double phase_A_primary_flux_value[Wb];
~~~

Instantaneous magnetic flux in phase A on the primary side of the transformer during saturation calculations

### `phase_B_primary_flux_value`

~~~
  double phase_B_primary_flux_value[Wb];
~~~

Instantaneous magnetic flux in phase B on the primary side of the transformer during saturation calculations

### `phase_C_primary_flux_value`

~~~
  double phase_C_primary_flux_value[Wb];
~~~

Instantaneous magnetic flux in phase C on the primary side of the transformer during saturation calculations

### `phase_A_secondary_flux_value`

~~~
  double phase_A_secondary_flux_value[Wb];
~~~

Instantaneous magnetic flux in phase A on the secondary side of the transformer during saturation calculations

### `phase_B_secondary_flux_value`

~~~
  double phase_B_secondary_flux_value[Wb];
~~~

Instantaneous magnetic flux in phase B on the secondary side of the transformer during saturation calculations

### `phase_C_secondary_flux_value`

~~~
  double phase_C_secondary_flux_value[Wb];
~~~

Instantaneous magnetic flux in phase C on the secondary side of the transformer during saturation calculations

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

### `degradation_factor`

Specifies the degradation of the transform turns ratio pu.  The final voltage ratio will be increased by `(1-degradation_factor)`.  Note that this value is only considered during initialization. Changes in the turns ratio degradation after the simulation starts are ignored.

# Example

~~~
  object transformer {
    ambient_temperature "0.0";
    top_oil_hot_spot_temperature "0.0";
    winding_hot_spot_temperature "0.0";
    percent_loss_of_life "0.0";
    aging_constant "0.0";
    use_thermal_model "FALSE";
    transformer_replacement_count "0.0";
    aging_granularity "0.0";
    phase_A_primary_flux_value "0.0";
    phase_B_primary_flux_value "0.0";
    phase_C_primary_flux_value "0.0";
    phase_A_secondary_flux_value "0.0";
    phase_B_secondary_flux_value "0.0";
    phase_C_secondary_flux_value "0.0";
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
    degradation_factor "0.0";
  }
~~~

# See also

* [[/Module/Powerflow]]

