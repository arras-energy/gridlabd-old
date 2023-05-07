[[/Module/Powerflow/Public_service]] -- Public service load

# Synopsis

~~~
class public_service 
{
    parent load;
    method schedule; // (REQUIRED) load schedule specification (<month>,<weekday>,<hour>-<hour>,<float>,...;...)
    double P0[W]; // (REQUIRED) public_service load nameplate power
    char256 threshold_source; // source for threshold value
    double threshold_value; // threshold value for turning on load
    bool threshold_invert; // invert effect of threshold, i.e., 'on' if less than
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

The `public_service` class support street lighting, water pumping, and other public utility services that are scheduled and/or event-driven.  The nominal load is `P0`. The load composition is specified using `Pz` and `Pi` for the constant impedance and constant power components. The constant power components are the sum of `Ma`, `Mb`, `Mc`, `Md`, and `Pe`.  The power factor is set using `Pf`.  

Loads can be scheduled using the `schedule` method. Load schedules can be aggregated per-unit using the syntax `M-M,D-D,H-H,Pt`, where `M-M` is the closed month range (1-12), `D-D` is the closed weekday range (0-6, with Monday=0), and `H-H` is the hour range (0-23).  The value of `Pt` is specified per-unit `P0` multiplied by the load components.  Extended syntaxes include the following:
* `M-M,D-D,H-H,Pt,Pf`, where `Pf` is the power factor;
* `M-M,D-D,H-H,Ma,Mb,Mc,Md,Pe,Z,I`, where the values are the load components; and
* `M-M,D-D,H-H,Ma,Mb,Mc,Md,Pe,Z,I,Pf`, where the values are the load components, including the power factor.

The load threshold can be used to specify a threshold condition for which the load is enabled.  When the source object and property is specified, then the `threshold_value` is compared to source value, and the load is enabled when the source value exceeds it.  The property `threshold_invert` reverses the logic so that the load is enabled when the source values is lesser.

# See also

* [[/Module/Powerflow/Load]]
* [[/Module/Powerflow/Meter]]
* [[/Module/Powerflow/Advanced_loads]]
