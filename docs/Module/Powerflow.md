[[/Module/Powerflow]] -- Module powerflow

# Synopsis

GLM:

~~~
module powerflow {
  all_powerflow_delta "<string>";
  convergence_error_handling {COLLAPSE,IGNORE,FATAL};
  current_frequency "<float> Hz";
  default_maximum_power_error <float>;
  default_maximum_voltage_error <float>;
  default_resistance <float>;
  deltamode_timestep <float> ns;
  enable_frequency_dependence "<string>";
  enable_inrush "<string>";
  enable_mesh_fault_current "<string>";
  enable_subsecond_models "<string>";
  fault_impedance "<string>";
  geographic_degree <float>;
  ground_impedance "<string>";
  line_capacitance "<string>";
  line_limits "<string>";
  low_voltage_impedance_level <float>;
  lu_solver "<string>";
  market_price_name "<string>";
  master_frequency_update "<string>";
  maximum_voltage_error <float>;
  message_flags [QUIET|WARNING|DEBUG|VERBOSE];
  nominal_frequency <float>;
  NR_admit_change "<string>";
  NR_deltamode_iteration_limit <integer>;
  NR_iteration_limit <integer>;
  NR_matrix_file "<string>";
  NR_matrix_output_interval {ALL,PER_CALL,ONCE,NEVER};
  NR_matrix_output_references "<string>";
  NR_superLU_procs <integer>;
  primary_voltage_ratio <float>;
  repair_time <float>;
  require_voltage_control "<string>";
  show_matrix_values "<string>";
  solver_headers "<string>";
  solver_method {NR,GS,FBS};
  solver_ml_config "<string>";
  solver_profile_enable "<string>";
  solver_profile_filename "<string>";
  solver_profile_headers_included "<string>";
  stop_on_pole_failure {TRUE,FALSE};
  warning_overfrequency <float>;
  warning_overvoltage <float>;
  warning_underfrequency <float>;
  warning_undervoltage <float>;
  warning_voltageangle <float>;
  wind_dir_name "<string>";
  wind_gust_name "<string>";
  wind_speed_name "<string>";
  voltage_fluctuation_threshold "<decimal> pu";
  default_voltage_violation_threshold "<decimal> pu";
  default_continuous_rating "<decimal> A";
  default_emergency_rating "<decimal> A";
}
~~~

# Description

The powerflow module calculates the voltages and currents on powerflow nodes and links, respectively. The choice of  `solver_method` depends on the network topology and the desired accuracy of the solution. The default solver is the Newton-Raphson solver, which can solve the most general network topology with the highest accuracy.

## Globals

### `message_flags`

~~~
  message_flags [QUIET|WARNING|DEBUG|VERBOSE];
~~~

Module message control flags

### `show_matrix_values`

~~~
  show_matrix_values "<string>";
~~~

TODO

### `primary_voltage_ratio`

~~~
  primary_voltage_ratio <float>;
~~~

TODO

### `nominal_frequency`

~~~
  nominal_frequency <float>;
~~~

TODO

### `require_voltage_control`

~~~
  require_voltage_control "<string>";
~~~

TODO

### `geographic_degree`

~~~
  geographic_degree <float>;
~~~

TODO

### `fault_impedance`

~~~
  fault_impedance "<string>";
~~~

TODO

### `ground_impedance`

~~~
  ground_impedance "<string>";
~~~

TODO

### `warning_underfrequency`

~~~
  warning_underfrequency <float>;
~~~

TODO

### `warning_overfrequency`

~~~
  warning_overfrequency <float>;
~~~

TODO

### `warning_undervoltage`

~~~
  warning_undervoltage <float>;
~~~

TODO

### `warning_overvoltage`

~~~
  warning_overvoltage <float>;
~~~

TODO

### `warning_voltageangle`

~~~
  warning_voltageangle <float>;
~~~

TODO

### `maximum_voltage_error`

~~~
  maximum_voltage_error <float>;
~~~

TODO

### `solver_method`

~~~
  solver_method "{NR,GS,FBS}";
~~~

TODO

### `NR_matrix_file`

~~~
  NR_matrix_file "<string>";
~~~

TODO

### `NR_matrix_output_interval`

~~~
  NR_matrix_output_interval {ALL,PER_CALL,ONCE,NEVER};
~~~

TODO

### `NR_matrix_output_references`

~~~
  NR_matrix_output_references "<string>";
~~~

TODO

### `line_capacitance`

~~~
  line_capacitance "<string>";
~~~

TODO

### `line_limits`

~~~
  line_limits "<string>";
~~~

TODO

### `lu_solver`

~~~
  lu_solver "<string>";
~~~

TODO

### `NR_iteration_limit`

~~~
  NR_iteration_limit <integer>;
~~~

TODO

### `NR_deltamode_iteration_limit`

~~~
  NR_deltamode_iteration_limit <integer>;
~~~

TODO

### `NR_superLU_procs`

~~~
  NR_superLU_procs <integer>;
~~~

TODO

### `default_maximum_voltage_error`

~~~
  default_maximum_voltage_error <float>;
~~~

TODO

### `default_maximum_power_error`

~~~
  default_maximum_power_error <float>;
~~~

TODO

### `NR_admit_change`

~~~
  NR_admit_change "<string>";
~~~

TODO

### `enable_subsecond_models`

~~~
  enable_subsecond_models "<string>";
~~~

Enable deltamode capabilities within the powerflow module

### `all_powerflow_delta`

~~~
  all_powerflow_delta "<string>";
~~~

Forces all powerflow objects that are capable to participate in deltamode

### `deltamode_timestep`

~~~
  deltamode_timestep "<float> ns";
~~~

Desired minimum timestep for deltamode-related simulations

### `current_frequency`

~~~
  current_frequency "<float> Hz";
~~~

Current system-level frequency of the powerflow system

### `master_frequency_update`

~~~
  master_frequency_update "<string>";
~~~

Tracking variable to see if an object has become the system frequency updater

### `enable_frequency_dependence`

~~~
  enable_frequency_dependence "<string>";
~~~

Flag to enable frequency-based variations in impedance values of lines and loads

### `default_resistance`

~~~
  default_resistance <float>;
~~~

TODO

### `enable_inrush`

~~~
  enable_inrush "<string>";
~~~

Flag to enable in-rush calculations for lines and transformers in deltamode

### `low_voltage_impedance_level`

~~~
  low_voltage_impedance_level <float>;
~~~

Lower limit of voltage (in per-unit) at which all load types are converted to impedance for in-rush calculations

### `enable_mesh_fault_current`

~~~
  enable_mesh_fault_current "<string>";
~~~

Flag to enable mesh-based fault current calculations

### `convergence_error_handling`

~~~
  convergence_error_handling {COLLAPSE,IGNORE,FATAL};
~~~

Flag to handle convergence error

### `solver_profile_enable`

~~~
  solver_profile_enable "<string>";
~~~

Flag to enable NR solver profiler

### `solver_profile_filename`

~~~
  solver_profile_filename "<string>";
~~~

Name of NR solver profile file

### `solver_profile_headers_included`

~~~
  solver_profile_headers_included "<string>";
~~~

Flag to include headers in NR solver profile file

### `solver_headers`

~~~
  solver_headers "<string>";
~~~

Headers in NR solver profile file

### `solver_ml_config`

~~~
  solver_ml_config "<string>";
~~~

ML solver configuration file location

### `market_price_name`

~~~
  market_price_name "<string>";
~~~

Sets the name of the market price property to use when access power markets.

### `repair_time`

~~~
  repair_time <float> h;
~~~

Specifies the number of hour required to repair a failed power pole. The default is 1 hour.

### `wind_speed_name`

~~~
  wind_speed_name "<string>";
~~~

Specifies the name of the climate or weather object property to use for wind speed.  The default is `wind_speed`.

### `wind_dir_name`

~~~
  wind_dir_name "<string>"l
~~~

Specifies the name of the climate or weather object property to use for wind direction.  The default is `wind_dir_name`.

### `wind_gust_name`

~~~
  wind_gust_name "<string>";
~~~

Specifies the name of the climate or weather object property to use for wind direction.  The default is `wind_dir_name`.

### `stop_on_pole_failure`

~~~
  stop_on_pole_failure <boolean>;
~~~

Specifies whether the simulation should stop when a pole fails.

### `voltage_fluctuation_threshold`

~~~
  voltage_fluctuation_threshold "<decimal> pu";
~~~

Specifies the value for the voltage fluctuations in nodes when `DER_value` is non-zero.  The default value is `0.03 pu`.

### `default_voltage_violation_threshold`

~~~
  default_voltage_violation_threshold "<decimal> pu";
~~~

Specifies the default value for the node `voltage_violation_threshold` property if it is not specified. The default value is `0.05 pu`.

### `default_continuous_rating`

~~~
  default_continuous_rating "<decimal> A";
~~~

Specifies the default value for the continuous current rating of a link. The default value is `1000 A`.

### `default_emergency_rating`

~~~
  default_emergency_rating "<decimal> A";
~~~

Specifies the default value for the emergency current rating of a link. The default value is `2000 A`.


# See also

* [[/Module/Powerflow/Powerflow_object]]
* [[/Module/Powerflow/Powerflow_library]]
* [[/Module/Powerflow/Node]]
* [[/Module/Powerflow/Link]]
* [[/Module/Powerflow/Capacitor]]
* [[/Module/Powerflow/Fuse]]
* [[/Module/Powerflow/Meter]]
* [[/Module/Powerflow/Line]]
* [[/Module/Powerflow/Line_spacing]]
* [[/Module/Powerflow/Overhead_line]]
* [[/Module/Powerflow/Underground_line]]
* [[/Module/Powerflow/Overhead_line_conductor]]
* [[/Module/Powerflow/Underground_line_conductor]]
* [[/Module/Powerflow/Line_configuration]]
* [[/Module/Powerflow/Transformer_configuration]]
* [[/Module/Powerflow/Transformer]]
* [[/Module/Powerflow/Load]]
* [[/Module/Powerflow/Regulator_configuration]]
* [[/Module/Powerflow/Regulator]]
* [[/Module/Powerflow/Triplex_node]]
* [[/Module/Powerflow/Triplex_meter]]
* [[/Module/Powerflow/Triplex_line]]
* [[/Module/Powerflow/Triplex_line_configuration]]
* [[/Module/Powerflow/Triplex_line_conductor]]
* [[/Module/Powerflow/Switch]]
* [[/Module/Powerflow/Substation]]
* [[/Module/Powerflow/Pqload]]
* [[/Module/Powerflow/Voltdump]]
* [[/Module/Powerflow/Series_reactor]]
* [[/Module/Powerflow/Restoration]]
* [[/Module/Powerflow/Frequency_gen]]
* [[/Module/Powerflow/Volt_var_control]]
* [[/Module/Powerflow/Fault_check]]
* [[/Module/Powerflow/Motor]]
* [[/Module/Powerflow/Billdump]]
* [[/Module/Powerflow/Power_metrics]]
* [[/Module/Powerflow/Currdump]]
* [[/Module/Powerflow/Recloser]]
* [[/Module/Powerflow/Sectionalizer]]
* [[/Module/Powerflow/Emissions]]
* [[/Module/Powerflow/Load_tracker]]
* [[/Module/Powerflow/Triplex_load]]
* [[/Module/Powerflow/Impedance_dump]]
* [[/Module/Powerflow/Vfd]]
* [[/Module/Powerflow/Pole]]
* [[/Module/Powerflow/Pole_configuration]]

