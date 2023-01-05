[[/Module/Powerflow/Public_service]] -- Public service load

# Synopsis

~~~
class public_service 
{
    parent load;
    method schedule; // (REQUIRED) load schedule specification (<month>,<weekday>,<hour>-<hour>,<float>;...)
    double P0[W]; // (REQUIRED) agricultural load nameplate power
    char256 sensitivity_source; // source for sensitivity value
    double sensitivity_base; // load sensitivity constant
    double sensitivity_value; // load sensitivity slope
    double Pz[pu]; // constant impedance load fraction
    double Pi[pu]; // constant current load fraction
    double Ma[pu]; // motor type A load fraction
    double Mb[pu]; // motor type B load fraction
    double Mc[pu]; // motor type C load fraction
    double Md[pu]; // motor type D load fraction
    double Pe[pu]; // power electronics load fraction
    double Pf[pu]; // power factor
    double measured_real_power[W]; // (OUTPUT) metered real power demand
    double measured_reactive_power[VAr]; // (OUTPUT) metered reactive power demand
    double measured_real_energy[Wh]; // (OUTPUT) cumulative metered real energy consumption
    double measured_real_energy_delta[Wh]; // (OUTPUT) metered real energy interval consumption
    double measured_reactive_energy[Wh]; // (OUTPUT) cumulative metered reactive energy consumption
    double measured_reactive_energy_delta[Wh]; // (OUTPUT) metered reactive energy interval consumption
    double measured_energy_delta_timestep[s]; // (OUTPUT) energy metering interval
    double measured_demand[W]; // (OUTPUT) maximum metered real power interval demand
    double measured_demand_timestep[s]; // (OUTPUT) maximum power metering interval
}
~~~

# Description

The `agricultural` class support street rural loads that are scheduled and/or event-driven.  The nominal load is `P0`. The load composition is specified using `Pz` and `Pi` for the constant impedance and constant power components. The constant power components are the sum of `Ma`, `Mb`, `Mc`, `Md`, and `Pe`.  The power factor is set using `Pf`.  

Loads can be scheduled using the `schedule` method. Load schedules can be aggregated per-unit using the syntax `M-M,D-D,H-H,Pt`, where `M-M` is the closed month range (1-12), `D-D` is the closed weekday range (0-6, with Monday=0), and `H-H` is the hour range (0-23).  The value of `Pt` is specified per-unit `P0` multiplied by the load components.  Extended syntaxes include the following:
* `M-M,D-D,H-H,Pt,Pf`, where `Pf` is the power factor;
* `M-M,D-D,H-H,Ma,Mb,Mc,Md,Pe,Z,I`, where the values are the load components; and
* `M-M,D-D,H-H,Ma,Mb,Mc,Md,Pe,Z,I,Pf`, where the values are the load components, including the power factor.

The load sensitivity can be used to specify a linear sensitivity function for the load.  When the source object and property is specified, then the input is scaled and offset by `sensitivity_value` and `sensitivity_base`. The result is then added to the total load, and the ZIP components are scaled by the ratio of the original load to the new scale, i.e.,
$$
    S = \frac{Z+I+P+ax+b}{Z+I+P}
$$
where $a$ is the load sensitivity, $x$ is the source value, and $b$ is the base sensitivity.

# See also

* [[/Module/Powerflow/Load]]
* [[/Module/Powerflow/Meter]]
* [[/Module/Powerflow/Advanced_loads]]
