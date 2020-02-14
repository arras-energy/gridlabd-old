[[/Module/Generators/Battery]] -- Battery object

# Synopsis

GLM:

~~~
module generators;
object battery {
	generator_mode "UNKNOWN";
	additional_controls "NONE";
	generator_status "0";
	rfb_size "0";
	power_type "0";
	battery_state "WAITING";
	number_battery_state_changes "+0";
	monitored_power "+0 W";
	power_set_high "+0 W";
	power_set_low "+0 W";
	power_set_high_highT "+0 W";
	power_set_low_highT "+0 W";
	check_power_low "+0 W";
	check_power_high "+0 W";
	voltage_set_high "+0 V";
	voltage_set_low "+0 V";
	deadband "+0 V";
	sensitivity "+0";
	high_temperature "+0";
	midpoint_temperature "+0";
	low_temperature "+0";
	scheduled_power "+0 W";
	Rinternal "+0 Ohm";
	V_Max "+0 V";
	I_Max "+0+0i A";
	E_Max "+0 Wh";
	P_Max "+0 W";
	power_factor "+0";
	Energy "+0 Wh";
	efficiency "+0 unit";
	base_efficiency "+0 unit";
	parasitic_power_draw "+0 W";
	Rated_kVA "+0 kVA";
	V_Out "+0+0i V";
	I_Out "+0+0i A";
	VA_Out "+0+0i VA";
	V_In "+0+0i V";
	I_In "+0+0i A";
	V_Internal "+0+0i V";
	I_Internal "+0+0i A";
	I_Prev "+0+0i A";
	power_transferred "+0";
	use_internal_battery_model "FALSE";
	battery_type "UNKNOWON";
	nominal_voltage "+0 V";
	rated_power "+0 W";
	battery_capacity "+0 Wh";
	round_trip_efficiency "+0 pu";
	state_of_charge "+0 pu";
	battery_load "+0 W";
	reserve_state_of_charge "+0 pu";
}
~~~

# Description

The `battery` object implements a chemical energy storage system.  Batteries must have an `inverter` as the parent object.

There are two models available. The `LEGACY` model and the `INTERNAL` model.  By default the `LEGACY` model is used.  To select the `INTERNAL` model, set the `use_internal_battery_model` property to `TRUE`.

## Properties

### additional_controls

~~~
enumeration {LINEAR_TEMPERATURE=1, NONE=0} additional_controls; 
~~~

(LEGACY) In conjunction with `POWER_DRIVEN`, `VOLTAGE_CONTROLLED`, and `POWER_VOLTAGE_HYBRID`, this will activate control set points that adjust with temperature.

### base_efficiency

~~~
double base_efficiency[unit]; 
~~~

(LEGACY) The efficiency of the battery at rated voltaged and current.

### battery_capacity

~~~
double battery_capacity[Wh]; 
~~~

(INTERNAL) The rated battery capacity of the battery.

### battery_load

~~~
double battery_load[W]; 
~~~

(INTERNAL) The current power output of the battery.

### battery_state

~~~
enumeration {CONFLICTED=5, EMPTY=4, FULL=3, WAITING=0, DISCHARGING=2, CHARGING=1} battery_state; 
~~~

Describes the current state of the battery

### battery_type

~~~
enumeration {LEAD_ACID=2, LI_ION=1, UNKNOWON=0} battery_type; 
~~~

(INTERNAL) The type of the battery. Used to determine the soc vs voltage curve.

### check_power_low

~~~
double check_power_low[W]; 
~~~

(LEGACY) High set point of dead band for load following at lower temperatures (`POWER_DRIVEN` + `LINEAR_TEMPERATURE`).

### check_power_high

~~~
double check_power_high[W]; 
~~~

(LEGACY) Low set point of dead band for load following at lower temperatures (`POWER_DRIVEN` + `LINEAR_TEMPERATURE`).

### deadband

~~~
double deadband[V]; 
~~~

(LEGACY) Voltage deadband.

### E_Max

~~~
double E_Max[Wh]; 
~~~

(LEGACY) The maximum capacity of the battery.

### efficiency

~~~
double efficiency[unit]; 
~~~

(LEGACY) The efficiency of the battery.

### Energy

~~~
double Energy[Wh]; 
~~~

(LEGACY) The available capacity of the battery.

### generator_mode

~~~
enumeration {POWER_VOLTAGE_HYBRID=7, VOLTAGE_CONTROLLED=6, POWER_DRIVEN=5, SUPPLY_DRIVEN=4, CONSTANT_PF=3, CONSTANT_PQ=2, CONSTANT_V=1, UNKNOWN=0} generator_mode; 
~~~

(LEGACY) Selects generator control mode when using legacy model; in non-legacy models, this should be `SUPPLY_DRIVEN`.

### generator_status

~~~
enumeration {ONLINE=2, OFFLINE=1} generator_status; 
~~~

Describes whether the generator is online or offline.

### high_temperature

~~~
double high_temperature; 
~~~

(LEGACY) High temperature of linear control; defines slope.

### I_In

~~~
complex I_In[A]; 
~~~

(LEGACY) The current flowing into the battery of the battery.

### I_Internal

~~~
complex I_Internal[A]; 
~~~

(LEGACY) The internal current of the battery.

### I_Max

~~~
complex I_Max[A]; 
~~~

(LEGACY) The maximum current output of the battery.

### I_Out

~~~
complex I_Out[A]; 
~~~

(LEGACY) The AC current output of the battery.

### I_Prev

~~~
complex I_Prev[A]; 
~~~

(LEGACY) The previous current output of the battery.

### low_temperature

~~~
double low_temperature; 
~~~

(LEGACY) Low temperature of linear control; defines slope.

### midpoint_temperature

~~~
double midpoint_temperature; 
~~~

(LEGACY) Midpoint temperature of linear control; defines slope.

### monitored_power

~~~
double monitored_power[W]; 
~~~

(LEGACY) output only; power output value of parent meter when performing load following modes (`POWER_DRIVEN`).

### nominal_voltage

~~~
double nominal_voltage[V]; 
~~~

(INTERNAL) The rated DC voltage at the terminals of the battery.

### number_battery_state_changes

~~~
double number_battery_state_changes; 
~~~

(LEGACY) Number of times battery switches between charging and discharging.

### P_Max

~~~
double P_Max[W]; 
~~~

(LEGACY) The maximum power output of the battery.

### parasitic_power_draw

~~~
double parasitic_power_draw[W]; 
~~~

(LEGACY) The parasytic power draw of the battery when idle.

### power_factor

~~~
double power_factor; 
~~~

(LEGACY) The power factor output of the battery.

### power_set_high

~~~
double power_set_high[W]; 
~~~

(LEGACY) High set point of dead band for load following (`POWER_DRIVEN`).

### power_set_high_highT

~~~
double power_set_high_highT[W]; 
~~~

(LEGACY) High set point of dead band for load following at higher temperatures (`POWER_DRIVEN` + `LINEAR_TEMPERATURE`).

### power_set_low

~~~
double power_set_low[W]; 
~~~

### power_set_low_highT

~~~
double power_set_low_highT[W]; 
~~~

(LEGACY) Low set point of dead band for load following at higher temperatures (`POWER_DRIVEN` + `LINEAR_TEMPERATURE`).

### power_transferred

~~~
double power_transferred; 
~~~

(LEGACY) The power output of the battery.

### power_type

~~~
enumeration {DC=1, AC=2} power_type; 
~~~

(LEGACY) Not currently used.

### Rated_kVA

~~~
double Rated_kVA[kVA]; 
~~~

(LEGACY) The rated power of the battery.

### rated_power

~~~
double rated_power[W]; 
~~~

(INTERNAL) The rated power output of the battery.

### reserve_state_of_charge

~~~
double reserve_state_of_charge[pu]; 
~~~

(INTERNAL) The reserve state of charge the battery can reach.

### rfb_size

~~~
enumeration {LARGE=4, MED_HIGH_ENERGY=3, MED_COMMERCIAL=2, SMALL=1, HOUSEHOLD=5} rfb_size; 
~~~

Default settings for certain sizes of batteries.

LEGACY MODEL: Low set point of dead band for load following (`POWER_DRIVEN`).

### Rinternal

~~~
double Rinternal[Ohm]; 
~~~

(LEGACY) The internal resistance of the battery.

### scheduled_power

~~~
double scheduled_power[W]; 
~~~

(LEGACY) Real power output of battery/inverter system.

### sensitivity

~~~
double sensitivity; 
~~~

(LEGACY) Describes how sensitive the control is to temperature excursions; defines slope of linear control.

### state_of_charge

~~~
double state_of_charge[pu]; 
~~~

(INTERNAL) The current state of charge of the battery.

### V_Max

~~~
double V_Max[V]; 
~~~

(LEGACY) The maximum terminal voltage of the battery.

### voltage_set_high

~~~
double voltage_set_high[V]; 
~~~

(LEGACY) Ligh set point for voltage control.

### voltage_set_low

~~~
double voltage_set_low[V]; 
~~~

(LEGACY) Low set point for voltage control.

### V_In

~~~
complex V_In[V]; 
~~~

(LEGACY) The voltage at the terminals of the battery.

### V_Internal

~~~
complex V_Internal[V]; 
~~~

(LEGACY) The internal voltage of the battery.

### V_Out

~~~
complex V_Out[V]; 
~~~

(LEGACY) The AC voltage at the terminals of the battery.

### VA_Out

~~~
complex VA_Out[VA]; 
~~~

(LEGACY) The power output of the battery.

### use_internal_battery_model

~~~
bool use_internal_battery_model; 
~~~

Enables the internal battery model.

## Internal Model

The internal battery model in the generators module works as follows. When the battery load is negative and the battery is not empty, then the battery load is 
$$
  P = - P_{max}.
$$
For Lithium-ion batteries, when $SOC > 0.1$ the open-circuit voltage is
$$
  V_{oc} = N_{series} \times \left[ {4.1-3.6 \over 0.9} \times SOC + \left( 4.1 - { 4.1 - 3.6 \over 0.9 } \right) \right]
$$
and when $SOC <= 0.1$
$$
  V_{oc} = N_{series} \times \left( { 3.6 - 3.2 \over 0.1 } \times SOC + 3.2 \right)
$$
For all other battery types, $V_{oc} = V_{max}$.

The internal resistance is 
$$
  R_{in} = V_{oc}^2 \times { | P_{br} - P_{max} | \over P_{br}^2 }
$$
where 
$$
  P_{br} = P_{max} \times \eta_{rt}^{0.5}
$$

The voltage at the terminals is
$$
  V_t = 0.5 \times (V_{oc}^2 + (4 \times P \times R_{in}))^{0.5}
$$

The internal battery load is 
$$
  P_{in} = { V_{oc} \times P \over V_t }
$$
The time to the next battery state change when discharging is
$$
  t_{next} = ( SOC_r - SOC ) \times { E_{max} \times 3600 \over P_{in} }
$$
where $SOC_r$ is the desired SOC reserve. When charging the next time is
$$
  t_{next} = (1-SOC) { \times E_{max} \times 3600 \over P_{in} }
$$
with $E_{max}$ defined as the battery's internal energy storage capacity.

# See also

* [[/Module/Generators]]
* [[/Module/Generators/Inverter]]
