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

