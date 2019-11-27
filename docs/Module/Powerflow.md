[[/Module/Powerflow]] -- Module powerflow

# Synopsis
GLM:
~~~
  module powerflow {
    message_flags "[QUIET|WARNING|DEBUG|VERBOSE]";
    show_matrix_values "<string>";
    primary_voltage_ratio "<decimal>";
    nominal_frequency "<decimal>";
    require_voltage_control "<string>";
    geographic_degree "<decimal>";
    fault_impedance "<string>";
    ground_impedance "<string>";
    warning_underfrequency "<decimal>";
    warning_overfrequency "<decimal>";
    warning_undervoltage "<decimal>";
    warning_overvoltage "<decimal>";
    warning_voltageangle "<decimal>";
    maximum_voltage_error "<decimal>";
    solver_method "{NR,GS,FBS}";
    NR_matrix_file "<string>";
    NR_matrix_output_interval "{ALL,PER_CALL,ONCE,NEVER}";
    NR_matrix_output_references "<string>";
    line_capacitance "<string>";
    line_limits "<string>";
    lu_solver "<string>";
    NR_iteration_limit "<integer>";
    NR_deltamode_iteration_limit "<integer>";
    NR_superLU_procs "<integer>";
    default_maximum_voltage_error "<decimal>";
    default_maximum_power_error "<decimal>";
    NR_admit_change "<string>";
    enable_subsecond_models "<string>";
    all_powerflow_delta "<string>";
    deltamode_timestep "<decimal> ns";
    current_frequency "<decimal> Hz";
    master_frequency_update "<string>";
    enable_frequency_dependence "<string>";
    default_resistance "<decimal>";
    enable_inrush "<string>";
    low_voltage_impedance_level "<decimal>";
    enable_mesh_fault_current "<string>";
    convergence_error_handling "{COLLAPSE,IGNORE,FATAL}";
    solver_profile_enable "<string>";
    solver_profile_filename "<string>";
    solver_profile_headers_included "<string>";
    solver_headers "<string>";
    solver_ml_config "<string>";
    market_price_name "<string>";
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

### `show_matrix_values`
~~~
  show_matrix_values "<string>";
~~~

TODO

### `primary_voltage_ratio`
~~~
  primary_voltage_ratio "<decimal>";
~~~

TODO

### `nominal_frequency`
~~~
  nominal_frequency "<decimal>";
~~~

TODO

### `require_voltage_control`
~~~
  require_voltage_control "<string>";
~~~

TODO

### `geographic_degree`
~~~
  geographic_degree "<decimal>";
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
  warning_underfrequency "<decimal>";
~~~

TODO

### `warning_overfrequency`
~~~
  warning_overfrequency "<decimal>";
~~~

TODO

### `warning_undervoltage`
~~~
  warning_undervoltage "<decimal>";
~~~

TODO

### `warning_overvoltage`
~~~
  warning_overvoltage "<decimal>";
~~~

TODO

### `warning_voltageangle`
~~~
  warning_voltageangle "<decimal>";
~~~

TODO

### `maximum_voltage_error`
~~~
  maximum_voltage_error "<decimal>";
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
  NR_matrix_output_interval "{ALL,PER_CALL,ONCE,NEVER}";
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
  NR_iteration_limit "<integer>";
~~~

TODO

### `NR_deltamode_iteration_limit`
~~~
  NR_deltamode_iteration_limit "<integer>";
~~~

TODO

### `NR_superLU_procs`
~~~
  NR_superLU_procs "<integer>";
~~~

TODO

### `default_maximum_voltage_error`
~~~
  default_maximum_voltage_error "<decimal>";
~~~

TODO

### `default_maximum_power_error`
~~~
  default_maximum_power_error "<decimal>";
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
  deltamode_timestep "<decimal> ns";
~~~

Desired minimum timestep for deltamode-related simulations

### `current_frequency`
~~~
  current_frequency "<decimal> Hz";
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
  default_resistance "<decimal>";
~~~

TODO

### `enable_inrush`
~~~
  enable_inrush "<string>";
~~~

Flag to enable in-rush calculations for lines and transformers in deltamode

### `low_voltage_impedance_level`
~~~
  low_voltage_impedance_level "<decimal>";
~~~

Lower limit of voltage (in per-unit) at which all load types are converted to impedance for in-rush calculations

### `enable_mesh_fault_current`
~~~
  enable_mesh_fault_current "<string>";
~~~

Flag to enable mesh-based fault current calculations

### `convergence_error_handling`
~~~
  convergence_error_handling "{COLLAPSE,IGNORE,FATAL}";
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

TODO

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

