[[/Module/Powerflow]] -- Module powerflow

# Synopsis
GLM:
~~~
  module powerflow {
    message_flags "DEBUG|VERBOSE";
    show_matrix_values "FALSE";
    primary_voltage_ratio "+60";
    nominal_frequency "+60";
    require_voltage_control "FALSE";
    geographic_degree "+0";
    fault_impedance "+1e-06+0j";
    ground_impedance "+1e-06+0j";
    warning_underfrequency "+55";
    warning_overfrequency "+65";
    warning_undervoltage "+0.8";
    warning_overvoltage "+1.2";
    warning_voltageangle "+2";
    maximum_voltage_error "+1e-06";
    solver_method "FBS";
    NR_matrix_file """";
    NR_matrix_output_interval "NEVER";
    NR_matrix_output_references "FALSE";
    line_capacitance "FALSE";
    line_limits "TRUE";
    lu_solver """";
    NR_iteration_limit "500";
    NR_deltamode_iteration_limit "10";
    NR_superLU_procs "1";
    default_maximum_voltage_error "+1e-06";
    default_maximum_power_error "+0.0001";
    NR_admit_change "TRUE";
    enable_subsecond_models "FALSE";
    all_powerflow_delta "FALSE";
    deltamode_timestep "+1e+07 ns";
    current_frequency "+60 Hz";
    master_frequency_update "FALSE";
    enable_frequency_dependence "FALSE";
    default_resistance "+0.0001";
    enable_inrush "FALSE";
    low_voltage_impedance_level "+0.7";
    enable_mesh_fault_current "FALSE";
    convergence_error_handling "FATAL";
    solver_profile_enable "FALSE";
    solver_profile_filename "solver_nr_profile.csv";
    solver_profile_headers_included "TRUE";
    solver_headers "timestamp,duration[microsec],iteration,bus_count,branch_count,error";
    solver_ml_config "/usr/local/share/gridlabd/solver_ml.conf";
    market_price_name "current_market.clearing_price";
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

