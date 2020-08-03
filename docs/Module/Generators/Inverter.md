[[/Module/Generators/Inverter]] -- Connect a DC source to an AC node.

# Synopsis

~~~
  module generators {
    set {QUIET=65536, WARNING=131072, DEBUG=262144, VERBOSE=524288} message_flags;
  };
  class inverter {
    battery_soc "0.0";
    c_0 "0.0";
    c_1 "0.0";
    c_2 "0.0";
    c_3 "0.0";
    charge_lockout_time "0.0";
    charge_off_threshold "0.0";
    charge_on_threshold "0.0";
    charge_threshold "0.0";
    curr_VA_out_A "0+0i";
    curr_VA_out_B "0+0i";
    curr_VA_out_C "0+0i";
    delay_time "0.0";
    delta_epA "0.0";
    delta_epB "0.0";
    delta_epC "0.0";
    delta_eqA "0.0";
    delta_eqB "0.0";
    delta_eqC "0.0";
    delta_mdA "0.0";
    delta_mdB "0.0";
    delta_mdC "0.0";
    delta_mqA "0.0";
    delta_mqB "0.0";
    delta_mqC "0.0";
    disable_volt_var_if_no_input_power "FALSE";
    discharge_lockout_time "0.0";
    discharge_off_threshold "0.0";
    discharge_on_threshold "0.0";
    discharge_threshold "0.0";
    dynamic_model_mode "0";
    enable_1547_checks "FALSE";
    enable_ramp_rates_reactive "FALSE";
    enable_ramp_rates_real "FALSE";
    epA "0.0";
    epB "0.0";
    epC "0.0";
    eqA "0.0";
    eqB "0.0";
    eqC "0.0";
    excess_input_power "0.0";
    four_quadrant_control_mode "0";
    freq_pwr_sched "";
    generator_mode "0";
    generator_status "0";
    group_max_charge_rate "0.0";
    group_max_discharge_rate "0.0";
    group_rated_power "0.0";
    I_In "0+0i";
    IdqA "0+0i";
    IdqB "0+0i";
    IdqC "0+0i";
    IEEE_1547_trip_method "0";
    IEEE_1547_version "0";
    inverter_1547_status "FALSE";
    inverter_convergence_criterion "0.0";
    inverter_droop_fp "FALSE";
    inverter_droop_vq "FALSE";
    inverter_efficiency "0.0";
    inverter_manufacturer "0";
    inverter_type "0";
    islanded_state "FALSE";
    kdd "0.0";
    kdq "0.0";
    ki_Vterminal "0.0";
    kid "0.0";
    kiq "0.0";
    kp_Vterminal "0.0";
    kpd "0.0";
    kpq "0.0";
    max_charge_rate "0.0";
    max_discharge_rate "0.0";
    max_pwr_slew_rate "0.0";
    max_ramp_down_reactive "0.0";
    max_ramp_down_real "0.0";
    max_ramp_up_reactive "0.0";
    max_ramp_up_real "0.0";
    max_var_slew_rate "0.0";
    maximum_dc_power "0.0";
    maximum_dc_voltage "0.0";
    mdA "0.0";
    mdB "0.0";
    mdC "0.0";
    minimum_dc_power "0.0";
    mqA "0.0";
    mqB "0.0";
    mqC "0.0";
    nominal_frequency "0.0";
    over_freq_high_cutout "0.0";
    over_freq_high_disconenct_time "0.0";
    over_freq_low_cutout "0.0";
    over_freq_low_disconenct_time "0.0";
    over_voltage_high_cutout "0.0";
    over_voltage_high_disconnect_time "0.0";
    over_voltage_low_cutout "0.0";
    over_voltage_low_disconnect_time "0.0";
    P_Out "0.0";
    pf_reg "0";
    pf_reg_activate "0.0";
    pf_reg_activate_lockout_time "0.0";
    pf_reg_deactivate "0.0";
    pf_reg_high "0.0";
    pf_reg_low "0.0";
    pf_target "0.0";
    phaseA_I_Out "0+0i";
    phaseA_V_Out "0+0i";
    phaseB_I_Out "0+0i";
    phaseB_V_Out "0+0i";
    phaseC_I_Out "0+0i";
    phaseC_V_Out "0+0i";
    phases "0";
    power_A "0+0i";
    power_B "0+0i";
    power_C "0+0i";
    power_factor "0.0";
    power_in "0.0";
    Pref "0.0";
    prev_VA_out_A "0+0i";
    prev_VA_out_B "0+0i";
    prev_VA_out_C "0+0i";
    Q1 "0.0";
    Q2 "0.0";
    Q3 "0.0";
    Q4 "0.0";
    Q_Out "0.0";
    Qref "0.0";
    R_fp "0.0";
    R_vq "0.0";
    rated_battery_power "0.0";
    rated_power "0.0";
    reconnect_time "0.0";
    sense_object "";
    soc_reserve "0.0";
    Tfreq_delay "0.0";
    Tp_delay "0.0";
    Tq_delay "0.0";
    Tvol_delay "0.0";
    under_freq_high_cutout "0.0";
    under_freq_high_disconenct_time "0.0";
    under_freq_low_cutout "0.0";
    under_freq_low_disconenct_time "0.0";
    under_voltage_high_cutout "0.0";
    under_voltage_high_disconnect_time "0.0";
    under_voltage_low_cutout "0.0";
    under_voltage_low_disconnect_time "0.0";
    under_voltage_middle_cutout "0.0";
    under_voltage_middle_disconnect_time "0.0";
    use_multipoint_efficiency "FALSE";
    V1 "0.0";
    V2 "0.0";
    V3 "0.0";
    V4 "0.0";
    V_base "0.0";
    V_In "0+0i";
    VA_In "0+0i";
    VA_Out "0+0i";
    Vdc "0.0";
    volt_var_control_lockout "0.0";
    volt_var_sched "";
    VSI_freq "0.0";
    VSI_mode "0";
    VSI_Rfilter "0+0i";
    VSI_Xfilter "0+0i";
  }
~~~

# Description

The minimum definition for an [[/inverter]] is 

~~~
  object inverter {
    generator_mode CONSTANT_PF;
    // implied child that provides a DC power input
 }
~~~

The `four_quadrant_control_mode=VOLT_VAR` mode is used to have the inverter generate/absorb VARs based off of the system voltage seen at the inverter terminals. The output on each phase determined on the system voltage measured on said phase. for example, in a three phase inverter, the output on phase A is based off of the phase A voltage measurement, the output on 
phase B is based off of the phase B voltage measurement, and the output on phase C is based off of the phase C voltage measurement. The inverter determines how much VARs are generated or 
absorbed by a volt/VAR curve that the user creates by setting the `V1`, `V2`, `V3`, `V4`, `Q1`, `Q2`, `Q3`, and `Q4` parameters. See the illustration below showing curve generated by these 
parameters.

[image:Inverter/volt_var_curve.jpg]

The values of these parameters are per unit. `V_base` is used to convert the voltage measurement to a per unit value. The `rated_power` parameter is used to convert the per unit value of VAR output into VARs. Note that `rated_power` is a per-phase limit, so for three-phase inverters, the overall limit will be three-times that value.  The inverter will try to output the amount of VARs determined from the curve without exceeding the power curve of the inverter.

~~~
object inverter {
    name "volt_var_inv";
    parent "inv_meter";
    inverter_type "FOUR_QUADRANT";
    four_quadrant_control_mode "VOLT_VAR";
    generator_status "ONLINE";
    generator_mode "SUPPLY_DRIVEN";
    phases "ABC";
    rated_power "70 kVA";
    inverter_efficiency "0.87";
    // Volt Var parameters;
    V_base "7200";
    V1 "0.90";
    Q1 "0.7";
    V2 "0.95";
    Q2 "0.0";
    V3 "1.05";
    Q3 "0.0";
    V4 "1.10";
    Q4 "-0.8";
}
~~~

## Properties

### `battery_soc[pu]`

~~~
  double battery_soc[pu];
~~~

FOUR QUADRANT MODEL: The state of charge of an attached battery

### `c_0`

~~~
  double c_0;
~~~

MULTIPOINT EFFICIENCY MODEL: the first coefficient in the efficienty curve

### `c_1`

~~~
  double c_1;
~~~

MULTIPOINT EFFICIENCY MODEL: the second coefficient in the efficienty curve

### `c_2`

~~~
  double c_2;
~~~

MULTIPOINT EFFICIENCY MODEL: the third coefficient in the efficienty curve

### `c_3`

~~~
  double c_3;
~~~

MULTIPOINT EFFICIENCY MODEL: the fourth coefficient in the efficienty curve

### `charge_lockout_time[s]`

~~~
  double charge_lockout_time[s];
~~~

FOUR QUADRANT MODEL: Lockout time when a charging operation occurs before another LOAD_FOLLOWING dispatch operation can occur

### `charge_off_threshold[W]`

~~~
  double charge_off_threshold[W];
~~~

FOUR QUADRANT MODEL: power level at which the inverter should cease charging the battery in LOAD_FOLLOWING

### `charge_on_threshold[W]`

~~~
  double charge_on_threshold[W];
~~~

FOUR QUADRANT MODEL: power level at which the inverter should try charging the battery in LOAD_FOLLOWING

### `charge_threshold[W]`

~~~
  double charge_threshold[W];
~~~

FOUR QUADRANT MODEL: Level at which all inverters in the group will begin charging attached batteries. Regulated minimum load level.

### `curr_VA_out_A[VA];complex`

~~~
  AC power on A phase in three-phase system curr_VA_out_A[VA];complex;
~~~

240-V connection on a triplex system

### `curr_VA_out_B[VA]`

~~~
  complex curr_VA_out_B[VA];
~~~

AC power on B phase in three-phase system

### `curr_VA_out_C[VA]`

~~~
  complex curr_VA_out_C[VA];
~~~

AC power on C phase in three-phase system

### `delay_time[s]`

~~~
  double delay_time[s];
~~~

TODO

### `delta_epA`

~~~
  double delta_epA;
~~~

DELTAMODE: The change in real current error for phase A or triplex phase.

### `delta_epB`

~~~
  double delta_epB;
~~~

DELTAMODE: The change in real current error for phase B.

### `delta_epC`

~~~
  double delta_epC;
~~~

DELTAMODE: The change in real current error for phase C.

### `delta_eqA`

~~~
  double delta_eqA;
~~~

DELTAMODE: The change in reactive current error for phase A or triplex phase.

### `delta_eqB`

~~~
  double delta_eqB;
~~~

DELTAMODE: The change in reactive current error for phase B.

### `delta_eqC`

~~~
  double delta_eqC;
~~~

DELTAMODE: The change in reactive current error for phase C.

### `delta_mdA`

~~~
  double delta_mdA;
~~~

DELTAMODE: The change in d-axis current modulation for phase A or triplex phase.

### `delta_mdB`

~~~
  double delta_mdB;
~~~

DELTAMODE: The change in d-axis current modulation for phase B.

### `delta_mdC`

~~~
  double delta_mdC;
~~~

DELTAMODE: The change in d-axis current modulation for phase C.

### `delta_mqA`

~~~
  double delta_mqA;
~~~

DELTAMODE: The change in q-axis current modulation for phase A or triplex phase.

### `delta_mqB`

~~~
  double delta_mqB;
~~~

DELTAMODE: The change in q-axis current modulation for phase B.

### `delta_mqC`

~~~
  double delta_mqC;
~~~

DELTAMODE: The change in q-axis current modulation for phase C.

### `disable_volt_var_if_no_input_power`

~~~
  bool disable_volt_var_if_no_input_power;
~~~

TODO

### `discharge_lockout_time[s]`

~~~
  double discharge_lockout_time[s];
~~~

FOUR QUADRANT MODEL: Lockout time when a discharging operation occurs before another LOAD_FOLLOWING dispatch operation can occur

### `discharge_off_threshold[W]`

~~~
  double discharge_off_threshold[W];
~~~

FOUR QUADRANT MODEL: power level at which the inverter should cease discharging the battery in LOAD_FOLLOWING

### `discharge_on_threshold[W]`

~~~
  double discharge_on_threshold[W];
~~~

FOUR QUADRANT MODEL: power level at which the inverter should try discharging the battery in LOAD_FOLLOWING

### `discharge_threshold[W]`

~~~
  double discharge_threshold[W];
~~~

FOUR QUADRANT MODEL: Level at which all inverters in the group will begin discharging attached batteries. Regulated maximum load level.

double ;
### `dynamic_model_mode`

~~~
  enumeration {PI=1, PID=0} dynamic_model_mode;
~~~

DELTAMODE: Underlying model to use for deltamode control

### `enable_1547_checks`

~~~
  bool enable_1547_checks;
~~~

DELTAMODE: Enable IEEE 1547-2003 disconnect checking

### `enable_ramp_rates_reactive`

~~~
  bool enable_ramp_rates_reactive;
~~~

DELTAMODE: Boolean used to indicate whether inverter ramp rate is enforced or not

### `enable_ramp_rates_real`

~~~
  bool enable_ramp_rates_real;
~~~

DELTAMODE: Boolean used to indicate whether inverter ramp rate is enforced or not

### `epA`

~~~
  double epA;
~~~

DELTAMODE: The real current error for phase A or triplex phase.

### `epB`

~~~
  double epB;
~~~

DELTAMODE: The real current error for phase B.

### `epC`

~~~
  double epC;
~~~

DELTAMODE: The real current error for phase C.

### `eqA`

~~~
  double eqA;
~~~

DELTAMODE: The reactive current error for phase A or triplex phase.

### `eqB`

~~~
  double eqB;
~~~

DELTAMODE: The reactive current error for phase B.

### `eqC`

~~~
  double eqC;
~~~

DELTAMODE: The reactive current error for phase C.

### `excess_input_power[W]`

~~~
  double excess_input_power[W];
~~~

FOUR QUADRANT MODEL: Excess power at the input of the inverter that is otherwise just lost, or could be shunted to a battery

### `four_quadrant_control_mode`

~~~
  enumeration {VOLTAGE_SOURCE=9, GROUP_LOAD_FOLLOWING=7, LOAD_FOLLOWING=5, VOLT_VAR_FREQ_PWR=8, VOLT_VAR=4, CONSTANT_PF=2, CONSTANT_PQ=1, NONE=0} four_quadrant_control_mode;
~~~

FOUR QUADRANT MODEL: Activates various control modes

### `freq_pwr_sched`

~~~
  char1024 freq_pwr_sched;
~~~

TODO

### `generator_mode;enumeration {SUPPLY_DRIVEN=5, CONSTANT_PF=4, CONSTANT_PQ=2, CONSTANT_V=1, UNKNOWN=0}`

~~~
  LEGACY MODEL: Selects generator control mode when using legacy model generator_mode;enumeration {SUPPLY_DRIVEN=5, CONSTANT_PF=4, CONSTANT_PQ=2, CONSTANT_V=1, UNKNOWN=0};
~~~

in non-legacy models, this should be SUPPLY_DRIVEN.

### `generator_status`

~~~
  enumeration {ONLINE=2, OFFLINE=1} generator_status;
~~~

describes whether the generator is online or offline

### `group_max_charge_rate[W]`

~~~
  double group_max_charge_rate[W];
~~~

FOUR QUADRANT MODEL: Sum of the charge rates of the batteries involved in the group load-following.

### `group_max_discharge_rate[W]`

~~~
  double group_max_discharge_rate[W];
~~~

FOUR QUADRANT MODEL: Sum of the discharge rates of the batteries involved in the group load-following.

### `group_rated_power[W]`

~~~
  double group_rated_power[W];
~~~

FOUR QUADRANT MODEL: Sum of the inverter power ratings of the inverters involved in the group power-factor regulation.

### `I_In[A]`

~~~
  complex I_In[A];
~~~

DC current

### `IdqA`

~~~
  complex IdqA;
~~~

DELTAMODE: The dq-axis current for phase A or triplex phase.

### `IdqB`

~~~
  complex IdqB;
~~~

DELTAMODE: The dq-axis current for phase B.

### `IdqC`

~~~
  complex IdqC;
~~~

DELTAMODE: The dq-axis current for phase C.

### `IEEE_1547_trip_method`

~~~
  enumeration {OVER_VOLTAGE_HIGH=9, OVER_VOLTAGE_LOW=8, UNDER_VOLTAGE_HIGH=7, UNDER_VOLTAGE_MID=6, UNDER_VOLTAGE_LOW=5, UNDER_FREQUENCY_LOW=4, UNDER_FREQUENCY_HIGH=3, OVER_FREQUENCY_LOW=2, OVER_FREQUENCY_HIGH=1, NONE=0} IEEE_1547_trip_method;
~~~

High over-voltage level trip

### `IEEE_1547_version`

~~~
  enumeration {IEEE1547A=2, IEEE1547=1, NONE=0} IEEE_1547_version;
~~~

DELTAMODE: Version of IEEE 1547 to use to populate defaults

### `inverter_1547_status`

~~~
  bool inverter_1547_status;
~~~

DELTAMODE: Indicator if the inverter is curtailed due to a 1547 violation or not

### `inverter_convergence_criterion`

~~~
  double inverter_convergence_criterion;
~~~

The maximum change in error threshold for exitting deltamode.

### `inverter_droop_fp`

~~~
  bool inverter_droop_fp;
~~~

DELTAMODE: Boolean used to indicate whether inverter f/p droop is included or not

### `inverter_droop_vq`

~~~
  bool inverter_droop_vq;
~~~

DELTAMODE: Boolean used to indicate whether inverter q/v droop is included or not

### `inverter_efficiency`

~~~
  double inverter_efficiency;
~~~

FOUR QUADRANT MODEL: The efficiency of the inverter

### `inverter_manufacturer`

~~~
  enumeration {XANTREX=3, SMA=2, FRONIUS=1, NONE=0} inverter_manufacturer;
~~~

MULTIPOINT EFFICIENCY MODEL: the manufacturer of the inverter to setup up pre-existing efficiency curves

### `inverter_type;enumeration {FOUR_QUADRANT=4, PWM=3, TWELVE_PULSE=2, SIX_PULSE=1, TWO_PULSE=0}`

~~~
  LEGACY MODEL: Sets efficiencies and other parameters inverter_type;enumeration {FOUR_QUADRANT=4, PWM=3, TWELVE_PULSE=2, SIX_PULSE=1, TWO_PULSE=0};
~~~

if using four_quadrant_control_mode, set this to FOUR_QUADRANT

### `islanded_state`

~~~
  bool islanded_state;
~~~

FOUR QUADRANT MODEL: Boolean used to let control modes to act under island conditions

### `kdd`

~~~
  double kdd;
~~~

DELTAMODE: The d-axis differentiator gain for the current modulation PID controller

### `kdq`

~~~
  double kdq;
~~~

DELTAMODE: The q-axis differentiator gain for the current modulation PID controller

### `ki_Vterminal`

~~~
  double ki_Vterminal;
~~~

DELTAMODE: The integrator gain for the VSI terminal voltage modulation

### `kid`

~~~
  double kid;
~~~

DELTAMODE: The d-axis proportional gain for the current modulation PI controller.

### `kiq`

~~~
  double kiq;
~~~

DELTAMODE: The q-axis proportional gain for the current modulation PI controller.

### `kp_Vterminal`

~~~
  double kp_Vterminal;
~~~

DELTAMODE: The proportional gain for the VSI terminal voltage modulation

### `kpd`

~~~
  double kpd;
~~~

DELTAMODE: The d-axis integration gain for the current modulation PI controller.

### `kpq`

~~~
  double kpq;
~~~

DELTAMODE: The q-axis integration gain for the current modulation PI controller.

### `max_charge_rate[W]`

~~~
  double max_charge_rate[W];
~~~

FOUR QUADRANT MODEL: maximum rate the battery can be charged in LOAD_FOLLOWING

### `max_discharge_rate[W]`

~~~
  double max_discharge_rate[W];
~~~

FOUR QUADRANT MODEL: maximum rate the battery can be discharged in LOAD_FOLLOWING

### `max_pwr_slew_rate[W/s]`

~~~
  double max_pwr_slew_rate[W/s];
~~~

TODO

### `max_ramp_down_reactive[VAr/s]`

~~~
  double max_ramp_down_reactive[VAr/s];
~~~

DELTAMODE: The reactive power ramp down rate limit

### `max_ramp_down_real[W/s]`

~~~
  double max_ramp_down_real[W/s];
~~~

DELTAMODE: The real power ramp down rate limit

### `max_ramp_up_reactive[VAr/s]`

~~~
  double max_ramp_up_reactive[VAr/s];
~~~

DELTAMODE: The reactive power ramp up rate limit

### `max_ramp_up_real[W/s]`

~~~
  double max_ramp_up_real[W/s];
~~~

DELTAMODE: The real power ramp up rate limit

### `maximum_dc_power`

~~~
  double maximum_dc_power;
~~~

MULTIPOINT EFFICIENCY MODEL: the maximum dc power point for the efficiency curve

### `maximum_dc_voltage`

~~~
  double maximum_dc_voltage;
~~~

MULTIPOINT EFFICIENCY MODEL: the maximum dc voltage point for the efficiency curve

### `mdA`

~~~
  double mdA;
~~~

DELTAMODE: The d-axis current modulation for phase A or triplex phase.

### `mdB`

~~~
  double mdB;
~~~

DELTAMODE: The d-axis current modulation for phase B.

### `mdC`

~~~
  double mdC;
~~~

DELTAMODE: The d-axis current modulation for phase C.

### `minimum_dc_power`

~~~
  double minimum_dc_power;
~~~

MULTIPOINT EFFICIENCY MODEL: the minimum dc power point for the efficiency curve

### `mqA`

~~~
  double mqA;
~~~

DELTAMODE: The q-axis current modulation for phase A or triplex phase.

### `mqB`

~~~
  double mqB;
~~~

DELTAMODE: The q-axis current modulation for phase B.

### `mqC`

~~~
  double mqC;
~~~

DELTAMODE: The q-axis current modulation for phase C.

### `nominal_frequency[Hz]`

~~~
  double nominal_frequency[Hz];
~~~

TODO

### `over_freq_high_cutout[Hz]`

~~~
  double over_freq_high_cutout[Hz];
~~~

DELTAMODE: OF2 set point for IEEE 1547a

### `over_freq_high_disconenct_time[s]`

~~~
  double over_freq_high_disconenct_time[s];
~~~

DELTAMODE: OF2 clearing time for IEEE1547a

### `over_freq_low_cutout[Hz]`

~~~
  double over_freq_low_cutout[Hz];
~~~

DELTAMODE: OF1 set point for IEEE 1547a

### `over_freq_low_disconenct_time[s]`

~~~
  double over_freq_low_disconenct_time[s];
~~~

DELTAMODE: OF1 clearing time for IEEE 1547a

### `over_voltage_high_cutout[pu]`

~~~
  double over_voltage_high_cutout[pu];
~~~

High voltage value for overvoltage

### `over_voltage_high_disconnect_time[s]`

~~~
  double over_voltage_high_disconnect_time[s];
~~~

Highest voltage clearing time for overvoltage

### `over_voltage_low_cutout[pu]`

~~~
  double over_voltage_low_cutout[pu];
~~~

Lowest voltage value for overvoltage

### `over_voltage_low_disconnect_time[s]`

~~~
  double over_voltage_low_disconnect_time[s];
~~~

Lowest voltage clearing time for overvoltage

### `P_Out[VA]`

~~~
  double P_Out[VA];
~~~

FOUR QUADRANT MODEL: Scheduled real power out in CONSTANT_PQ control mode

### `pf_reg`

~~~
  enumeration {EXCLUDED=2, INCLUDED_ALT=3, INCLUDED=1} pf_reg;
~~~

Activate (or not) power factor regulation in four_quadrant_control_mode

### `pf_reg_activate`

~~~
  double pf_reg_activate;
~~~

FOUR QUADRANT MODEL: Lowest acceptable power-factor level below which power-factor regulation will activate.

### `pf_reg_activate_lockout_time[s]`

~~~
  double pf_reg_activate_lockout_time[s];
~~~

FOUR QUADRANT MODEL: Mandatory pause between the deactivation of power-factor regulation and it reactivation

### `pf_reg_deactivate`

~~~
  double pf_reg_deactivate;
~~~

FOUR QUADRANT MODEL: Lowest acceptable power-factor above which no power-factor regulation is needed.

### `pf_reg_high`

~~~
  double pf_reg_high;
~~~

FOUR QUADRANT MODEL: Upper limit for power-factor - if exceeds, go full reverse reactive

### `pf_reg_low`

~~~
  double pf_reg_low;
~~~

FOUR QUADRANT MODEL: Lower limit for power-factor - if exceeds, stop regulating - pf_target_var is below this

### `pf_target`

~~~
  double pf_target;
~~~

FOUR QUADRANT MODEL: Desired power-factor to maintain (signed) positive is inductive

### `phaseA_I_Out[V];complex`

~~~
  AC current on A phase in three-phase system phaseA_I_Out[V];complex;
~~~

240-V connection on a triplex system

### `phaseA_V_Out[V];complex`

~~~
  AC voltage on A phase in three-phase system phaseA_V_Out[V];complex;
~~~

240-V connection on a triplex system

### `phaseB_I_Out[V]`

~~~
  complex phaseB_I_Out[V];
~~~

AC current on B phase in three-phase system

### `phaseB_V_Out[V]`

~~~
  complex phaseB_V_Out[V];
~~~

AC voltage on B phase in three-phase system

### `phaseC_I_Out[V]`

~~~
  complex phaseC_I_Out[V];
~~~

AC current on C phase in three-phase system

### `phaseC_V_Out[V]`

~~~
  complex phaseC_V_Out[V];
~~~

AC voltage on C phase in three-phase system

### `phases`

~~~
  set {S=112, N=8, C=4, B=2, A=1} phases;
~~~

The phases the inverter is attached to

### `power_A[VA];complex`

~~~
  AC power on A phase in three-phase system power_A[VA];complex;
~~~

240-V connection on a triplex system

### `power_B[VA]`

~~~
  complex power_B[VA];
~~~

AC power on B phase in three-phase system

### `power_C[VA]`

~~~
  complex power_C[VA];
~~~

AC power on C phase in three-phase system

### `power_factor[unit]`

~~~
  double power_factor[unit];
~~~

FOUR QUADRANT MODEL: The power factor used for CONSTANT_PF control mode

### `power_in[W]`

~~~
  double power_in[W];
~~~

LEGACY MODEL: No longer used

### `Pref`

~~~
  double Pref;
~~~

DELTAMODE: The real power reference.

### `prev_VA_out_A[VA];complex`

~~~
  AC power on A phase in three-phase system prev_VA_out_A[VA];complex;
~~~

240-V connection on a triplex system

### `prev_VA_out_B[VA]`

~~~
  complex prev_VA_out_B[VA];
~~~

AC power on B phase in three-phase system

### `prev_VA_out_C[VA]`

~~~
  complex prev_VA_out_C[VA];
~~~

AC power on C phase in three-phase system

### `Q1[pu]`

~~~
  double Q1[pu];
~~~

FOUR QUADRANT MODEL: VAR point 1 in volt/var curve. Used in VOLT_VAR control mode.

### `Q2[pu]`

~~~
  double Q2[pu];
~~~

FOUR QUADRANT MODEL: VAR point 2 in volt/var curve. Used in VOLT_VAR control mode.

### `Q3[pu]`

~~~
  double Q3[pu];
~~~

FOUR QUADRANT MODEL: VAR point 3 in volt/var curve. Used in VOLT_VAR control mode.

### `Q4[pu]`

~~~
  double Q4[pu];
~~~

FOUR QUADRANT MODEL: VAR point 4 in volt/var curve. Used in VOLT_VAR control mode.

### `Q_Out[VAr]`

~~~
  double Q_Out[VAr];
~~~

FOUR QUADRANT MODEL: Schedule reactive power out in CONSTANT_PQ control mode

### `Qref`

~~~
  double Qref;
~~~

DELTAMODE: The reactive power reference.

### `R_fp`

~~~
  double R_fp;
~~~

DELTAMODE: The droop parameter of the f/p droop

### `R_vq`

~~~
  double R_vq;
~~~

DELTAMODE: The droop parameter of the v/q droop

### `rated_battery_power[W]`

~~~
  double rated_battery_power[W];
~~~

FOUR QUADRANT MODEL: The rated power of battery when battery is attached

### `rated_power[VA]`

~~~
  double rated_power[VA];
~~~

FOUR QUADRANT MODEL: The rated power of the inverter

### `reconnect_time[s]`

~~~
  double reconnect_time[s];
~~~

DELTAMODE: Time delay after IEEE 1547-2003 violation clears before resuming generation

### `sense_object`

~~~
  object sense_object;
~~~

FOUR QUADRANT MODEL: name of the object the inverter is trying to mitigate the load on (node/link) in LOAD_FOLLOWING

### `soc_reserve[pu]`

~~~
  double soc_reserve[pu];
~~~

FOUR QUADRANT MODEL: The reserve state of charge of an attached battery for islanding cases

### `Tfreq_delay`

~~~
  double Tfreq_delay;
~~~

DELTAMODE: The time constant for delayed frequency seen by the inverter

### `Tp_delay`

~~~
  double Tp_delay;
~~~

DELTAMODE: The time constant for delayed real power seen by the VSI droop controller

### `Tq_delay`

~~~
  double Tq_delay;
~~~

DELTAMODE: The time constant for delayed reactive power seen by the VSI droop controller

### `Tvol_delay`

~~~
  double Tvol_delay;
~~~

DELTAMODE: The time constant for delayed voltage seen by the inverter

### `under_freq_high_cutout[Hz]`

~~~
  double under_freq_high_cutout[Hz];
~~~

DELTAMODE: UF2 set point for IEEE 1547a

### `under_freq_high_disconenct_time[s]`

~~~
  double under_freq_high_disconenct_time[s];
~~~

DELTAMODE: UF2 clearing time for IEEE1547a

### `under_freq_low_cutout[Hz]`

~~~
  double under_freq_low_cutout[Hz];
~~~

DELTAMODE: UF1 set point for IEEE 1547a

### `under_freq_low_disconenct_time[s]`

~~~
  double under_freq_low_disconenct_time[s];
~~~

DELTAMODE: UF1 clearing time for IEEE 1547a

### `under_voltage_high_cutout[pu]`

~~~
  double under_voltage_high_cutout[pu];
~~~

High value of low voltage threshold for undervoltage

### `under_voltage_high_disconnect_time[s]`

~~~
  double under_voltage_high_disconnect_time[s];
~~~

Highest voltage clearing time for undervoltage

### `under_voltage_low_cutout[pu]`

~~~
  double under_voltage_low_cutout[pu];
~~~

Lowest voltage threshold for undervoltage

### `under_voltage_low_disconnect_time[s]`

~~~
  double under_voltage_low_disconnect_time[s];
~~~

Lowest voltage clearing time for undervoltage

### `under_voltage_middle_cutout[pu]`

~~~
  double under_voltage_middle_cutout[pu];
~~~

Middle-lowest voltage threshold for undervoltage

### `under_voltage_middle_disconnect_time[s]`

~~~
  double under_voltage_middle_disconnect_time[s];
~~~

Middle-lowest voltage clearing time for undervoltage

### `use_multipoint_efficiency`

~~~
  bool use_multipoint_efficiency;
~~~

FOUR QUADRANT MODEL: boolean to used the multipoint efficiency curve for the inverter when solar is attached

### `V1[pu]`

~~~
  double V1[pu];
~~~

FOUR QUADRANT MODEL: voltage point 1 in volt/var curve. Used in VOLT_VAR control mode.

### `V2[pu]`

~~~
  double V2[pu];
~~~

FOUR QUADRANT MODEL: voltage point 2 in volt/var curve. Used in VOLT_VAR control mode.

### `V3[pu]`

~~~
  double V3[pu];
~~~

FOUR QUADRANT MODEL: voltage point 3 in volt/var curve. Used in VOLT_VAR control mode.

### `V4[pu]`

~~~
  double V4[pu];
~~~

FOUR QUADRANT MODEL: voltage point 4 in volt/var curve. Used in VOLT_VAR control mode.

### `V_base[V]`

~~~
  double V_base[V];
~~~

FOUR QUADRANT MODEL: The base voltage on the grid side of the inverter. Used in VOLT_VAR control mode.

### `V_In[V]`

~~~
  complex V_In[V];
~~~

DC voltage

### `VA_In[VA]`

~~~
  complex VA_In[VA];
~~~

DC power

### `VA_Out[VA]`

~~~
  complex VA_Out[VA];
~~~

AC power

### `Vdc[V]`

~~~
  double Vdc[V];
~~~

LEGACY MODEL: DC voltage

### `volt_var_control_lockout[s]`

~~~
  double volt_var_control_lockout[s];
~~~

FOUR QUADRANT QUADRANT MODEL: the lockout time between volt/var actions.

### `volt_var_sched`

~~~
  char1024 volt_var_sched;
~~~

TODO

### `VSI_freq`

~~~
  double VSI_freq;
~~~

VSI frequency

### `VSI_mode`

~~~
  enumeration {VSI_DROOP=1, VSI_ISOCHRONOUS=0} VSI_mode;
~~~

VSI MODEL: Selects VSI mode for either isochronous or droop one

### `VSI_Rfilter[pu]`

~~~
  complex VSI_Rfilter[pu];
~~~

VSI filter resistance (p.u.)

### `VSI_Xfilter[pu]`

~~~
  complex VSI_Xfilter[pu];
~~~

VSI filter inductance (p.u.)


# Example

~~~
module generators;
object inverter {
    generator_mode "CONSTANT_PF";
    generator_status "ONLINE";
    inverter_type "PWM";
    power_factor "1.0;"
    parent "triplex_meter2";
    // child that provides a DC power input >>
}
~~~

# See Also

* [[/Modules/Generators/Battery]]
* [[/Modules/Generators/Solar]]
