[[/Module/Powerflow/Industrial]] -- Industrial load model

# Synopsis

~~~
module powerflow {
    char1024 NAICS_datafile;
}
class industrial {
    parent load;
 
    char32 industry_code; // (REQUIRED) industrial facility code (see naics_data_file.csv)
    method schedule; // (REQUIRED) load schedule specification (<month>,<weekday>,<hour>-<hour>,<float>;...)
    double P0[W]; // (REQUIRED) industrial load nameplate power
    double Pz[pu]; // constant impedance load fraction
    double Pi[pu]; // constant current load fraction
    double Ma[pu]; // motor type A load fraction
    double Mb[pu]; // motor type B load fraction
    double Mc[pu]; // motor type C load fraction
    double Md[pu]; // motor type D load fraction
    double Pe[pu]; // power electronics load fraction
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

The industrial load model uses the NERC composite load model based on the NAICS industry codes in the NAICS data file `naics_data_file.csv`.  The composite load model provides load fractions for the following load components:

* `Pz` - constant impedance loads
* `Pi` - constant current loads
* `Ma` - 3 phase motors with constant torque and low inertia
* `Mb` - 3 phase motors with speed-squared torque and high inertia
* `Mc` - 3 phase motors with speed-squared torque and low inertia
* `Md` - single phase induction motors driving compressors
* `Pe` - power electronic drives

These are the load fractions of the nameplate power `P0`, and should add add to 1.0.

## Load schedule

The load schedule specifies which load components are active at any given time. Schedule values are specified as fractions of the load component. Load schedule are specified by month, weekday, and hour of day, and optionally include a power factor.  There are four possible syntax allowed for a load schedule specification:

* **Total load only**: `M-M,D-D,H-H,L`, where `H-H` is the closed month of year range (Jan=1,Dec=12), `D-D` is the closed weekday range (Mon=0,Sun=6), `H-H` is the closed hour of day range (0-23), and `L` is the total fraction of nameplate load.
* **Total load with power factor**: `M-M,D-D,H-H,L,F`, where `H-H` is the closed month of year range (Jan=1,Dec=12), `D-D` is the closed weekday range (Mon=0,Sun=6), `H-H` is the closed hour of day range (0-23), `L` is the total fraction of nameplate load, and `F` is the power factor.
* **Composite load**: `M-M,D-D,H-H,A,B,C,D,E,Z,I`, where `H-H` is the closed month of year range (Jan=1,Dec=12), `D-D` is the closed weekday range (Mon=0,Sun=6), `H-H` is the closed hour of day range (0-23), and `A`, `B`, `C`, `D`, `E`, `Z`, `I` are the load component fractions of nameplate load.
* **Composite load with power factor**: `M-M,D-D,H-H,A,B,C,D,E,Z,I,F`, where `H-H` is the closed month of year range (Jan=1,Dec=12), `D-D` is the closed weekday range (Mon=0,Sun=6), `H-H` is the closed hour of day range (0-23), `A`, `B`, `C`, `D`, `E`, `Z`, `I` are the load component fractions of nameplate load, and `F` is the power factor.

## Load composition

The NERC composite load model data is read from the `NAICS_datafile`.  The data fields are illustrated by the following excerpt:

~~~
NAICS,Code,Industrial Load Type,IA,IB,IC,MD,PwrEl,Z,I
339,IND_OTH,Industrial - Other,0.1,0.3,0.3,0,0.2,0.05,0.05
211,IND_SHG,Shale Gas Extraction Plant,0,0.2,0.4,0,0.4,0,0
212,IND_MIN,Mining,0.25,0.25,0.3,0,0.2,0,0
...
~~~

When the load composition is loaded the sum is checked and a warning is displayed for any sum that is more 1e-3 away from 1.0. The load components are then normalized to ensure the sum is exactly 1.0.

## Meter

The industrial load is metered as a 3-phase load, with base power on each phase set to 1/3 of the total power.  The power factor is used to set the reactive power on each phase as well.  The four motor and power electronic loads comprise the constant power load, while the constant impedance and constant current loads are taken from the `Z` and `I` components directly.

# See also

* [[/Module/Powerflow/Load]]
* [[/Module/Industrial]]
