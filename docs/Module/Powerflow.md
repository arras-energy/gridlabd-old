[[/Module/Powerflow]] -- Module powerflow

# Synopsis
GLM:
~~~
  module powerflow {
    powerflow::message_flags "[QUIET|WARNING|DEBUG|VERBOSE]";
    powerflow::show_matrix_values "<string>";
    powerflow::primary_voltage_ratio "<decimal>";
    powerflow::nominal_frequency "<decimal>";
    powerflow::require_voltage_control "<string>";
    powerflow::geographic_degree "<decimal>";
    powerflow::fault_impedance "<string>";
    powerflow::ground_impedance "<string>";
    powerflow::warning_underfrequency "<decimal>";
    powerflow::warning_overfrequency "<decimal>";
    powerflow::warning_undervoltage "<decimal>";
    powerflow::warning_overvoltage "<decimal>";
    powerflow::warning_voltageangle "<decimal>";
    powerflow::maximum_voltage_error "<decimal>";
    powerflow::solver_method "{NR,GS,FBS}";
    powerflow::NR_matrix_file "<string>";
    powerflow::NR_matrix_output_interval "{ALL,PER_CALL,ONCE,NEVER}";
    powerflow::NR_matrix_output_references "<string>";
    powerflow::line_capacitance "<string>";
    powerflow::line_limits "<string>";
    powerflow::lu_solver "<string>";
    powerflow::NR_iteration_limit "<integer>";
    powerflow::NR_deltamode_iteration_limit "<integer>";
    powerflow::NR_superLU_procs "<integer>";
    powerflow::default_maximum_voltage_error "<decimal>";
    powerflow::default_maximum_power_error "<decimal>";
    powerflow::NR_admit_change "<string>";
    powerflow::enable_subsecond_models "<string>";
    powerflow::all_powerflow_delta "<string>";
    powerflow::deltamode_timestep "<decimal> ns";
    powerflow::current_frequency "<decimal> Hz";
    powerflow::master_frequency_update "<string>";
    powerflow::enable_frequency_dependence "<string>";
    powerflow::default_resistance "<decimal>";
    powerflow::enable_inrush "<string>";
    powerflow::low_voltage_impedance_level "<decimal>";
    powerflow::enable_mesh_fault_current "<string>";
    powerflow::convergence_error_handling "{COLLAPSE,IGNORE,FATAL}";
    powerflow::solver_profile_enable "<string>";
    powerflow::solver_profile_filename "<string>";
    powerflow::solver_profile_headers_included "<string>";
    powerflow::solver_headers "<string>";
    powerflow::solver_ml_config "<string>";
    powerflow::market_price_name "<string>";
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

