[[/Module/Residential/Clotheswasher]] -- Class clotheswasher

# Synopsis

GLM:

~~~
  object clotheswasher {
    motor_power "<decimal> kW";
    circuit_split "<decimal>";
    queue "<decimal> unit";
    demand "<decimal> unit/day";
    energy_meter "<string> kWh";
    stall_voltage "<decimal> V";
    start_voltage "<decimal> V";
    clothesWasherPower "<decimal>";
    stall_impedance "<string> Ohm";
    trip_delay "<decimal> s";
    reset_delay "<decimal> s";
    Is_on "<decimal>";
    normal_perc "<decimal>";
    perm_press_perc "<decimal>";
    NORMAL_PREWASH_POWER "<decimal>";
    NORMAL_WASH_POWER "<decimal>";
    NORMAL_SPIN_LOW_POWER "<decimal>";
    NORMAL_SPIN_MEDIUM_POWER "<decimal>";
    NORMAL_SPIN_HIGH_POWER "<decimal>";
    NORMAL_SMALLWASH_POWER "<decimal>";
    NORMAL_PREWASH_ENERGY "<decimal>";
    NORMAL_WASH_ENERGY "<decimal>";
    NORMAL_SPIN_LOW_ENERGY "<decimal>";
    NORMAL_SPIN_MEDIUM_ENERGY "<decimal>";
    NORMAL_SPIN_HIGH_ENERGY "<decimal>";
    NORMAL_SMALLWASH_ENERGY "<decimal>";
    PERMPRESS_PREWASH_POWER "<decimal>";
    PERMPRESS_WASH_POWER "<decimal>";
    PERMPRESS_SPIN_LOW_POWER "<decimal>";
    PERMPRESS_SPIN_MEDIUM_POWER "<decimal>";
    PERMPRESS_SPIN_HIGH_POWER "<decimal>";
    PERMPRESS_SMALLWASH_POWER "<decimal>";
    PERMPRESS_PREWASH_ENERGY "<decimal>";
    PERMPRESS_WASH_ENERGY "<decimal>";
    PERMPRESS_SPIN_LOW_ENERGY "<decimal>";
    PERMPRESS_SPIN_MEDIUM_ENERGY "<decimal>";
    PERMPRESS_SPIN_HIGH_ENERGY "<decimal>";
    PERMPRESS_SMALLWASH_ENERGY "<decimal>";
    GENTLE_PREWASH_POWER "<decimal>";
    GENTLE_WASH_POWER "<decimal>";
    GENTLE_SPIN_LOW_POWER "<decimal>";
    GENTLE_SPIN_MEDIUM_POWER "<decimal>";
    GENTLE_SPIN_HIGH_POWER "<decimal>";
    GENTLE_SMALLWASH_POWER "<decimal>";
    GENTLE_PREWASH_ENERGY "<decimal>";
    GENTLE_WASH_ENERGY "<decimal>";
    GENTLE_SPIN_LOW_ENERGY "<decimal>";
    GENTLE_SPIN_MEDIUM_ENERGY "<decimal>";
    GENTLE_SPIN_HIGH_ENERGY "<decimal>";
    GENTLE_SMALLWASH_ENERGY "<decimal>";
    queue_min "<decimal> unit";
    queue_max "<decimal> unit";
    clotheswasher_run_prob "<decimal>";
    state "{SPIN4,SPIN3,SPIN2,SPIN1,WASH,PREWASH,STOPPED}";
    spin_mode "{SMALLWASH,SPIN_WASH,SPIN_HIGH,SPIN_MEDIUM,SPIN_LOW}";
    wash_mode "{GENTLE,PERM_PRESS,NORMAL}";
    shape "<string>";
    load "<string>";
    energy "<string> kVAh";
    power "<string> kVA";
    peak_demand "<string> kVA";
    heatgain "<decimal> Btu/h";
    cumulative_heatgain "<decimal> Btu";
    heatgain_fraction "<decimal> pu";
    current_fraction "<decimal> pu";
    impedance_fraction "<decimal> pu";
    power_fraction "<decimal> pu";
    power_factor "<decimal>";
    constant_power "<string> kVA";
    constant_current "<string> kVA";
    constant_admittance "<string> kVA";
    voltage_factor "<decimal> pu";
    breaker_amps "<decimal> A";
    configuration "[IS220|IS110]";
    override "{OFF,ON,NORMAL}";
    power_state "{UNKNOWN,ON,OFF}";
  }
~~~

# Description

TODO

## Properties

### `motor_power`

~~~
  double motor_power[kW];
~~~

TODO

### `circuit_split`

~~~
  double circuit_split;
~~~

TODO

### `queue`

~~~
  double queue[unit];
~~~

The total laundry accumulated

### `demand`

~~~
  double demand[unit/day];
~~~

The amount of laundry accumulating daily

### `energy_meter`

~~~
  complex energy_meter[kWh];
~~~

TODO

### `stall_voltage`

~~~
  double stall_voltage[V];
~~~

TODO

### `start_voltage`

~~~
  double start_voltage[V];
~~~

TODO

### `clothesWasherPower`

~~~
  double clothesWasherPower;
~~~

TODO

### `stall_impedance`

~~~
  complex stall_impedance[Ohm];
~~~

TODO

### `trip_delay`

~~~
  double trip_delay[s];
~~~

TODO

### `reset_delay`

~~~
  double reset_delay[s];
~~~

TODO

### `Is_on`

~~~
  double Is_on;
~~~

TODO

### `normal_perc`

~~~
  double normal_perc;
~~~

TODO

### `perm_press_perc`

~~~
  double perm_press_perc;
~~~

TODO

### `NORMAL_PREWASH_POWER`

~~~
  double NORMAL_PREWASH_POWER;
~~~

TODO

### `NORMAL_WASH_POWER`

~~~
  double NORMAL_WASH_POWER;
~~~

TODO

### `NORMAL_SPIN_LOW_POWER`

~~~
  double NORMAL_SPIN_LOW_POWER;
~~~

TODO

### `NORMAL_SPIN_MEDIUM_POWER`

~~~
  double NORMAL_SPIN_MEDIUM_POWER;
~~~

TODO

### `NORMAL_SPIN_HIGH_POWER`

~~~
  double NORMAL_SPIN_HIGH_POWER;
~~~

TODO

### `NORMAL_SMALLWASH_POWER`

~~~
  double NORMAL_SMALLWASH_POWER;
~~~

TODO

### `NORMAL_PREWASH_ENERGY`

~~~
  double NORMAL_PREWASH_ENERGY;
~~~

TODO

### `NORMAL_WASH_ENERGY`

~~~
  double NORMAL_WASH_ENERGY;
~~~

TODO

### `NORMAL_SPIN_LOW_ENERGY`

~~~
  double NORMAL_SPIN_LOW_ENERGY;
~~~

TODO

### `NORMAL_SPIN_MEDIUM_ENERGY`

~~~
  double NORMAL_SPIN_MEDIUM_ENERGY;
~~~

TODO

### `NORMAL_SPIN_HIGH_ENERGY`

~~~
  double NORMAL_SPIN_HIGH_ENERGY;
~~~

TODO

### `NORMAL_SMALLWASH_ENERGY`

~~~
  double NORMAL_SMALLWASH_ENERGY;
~~~

TODO

### `PERMPRESS_PREWASH_POWER`

~~~
  double PERMPRESS_PREWASH_POWER;
~~~

TODO

### `PERMPRESS_WASH_POWER`

~~~
  double PERMPRESS_WASH_POWER;
~~~

TODO

### `PERMPRESS_SPIN_LOW_POWER`

~~~
  double PERMPRESS_SPIN_LOW_POWER;
~~~

TODO

### `PERMPRESS_SPIN_MEDIUM_POWER`

~~~
  double PERMPRESS_SPIN_MEDIUM_POWER;
~~~

TODO

### `PERMPRESS_SPIN_HIGH_POWER`

~~~
  double PERMPRESS_SPIN_HIGH_POWER;
~~~

TODO

### `PERMPRESS_SMALLWASH_POWER`

~~~
  double PERMPRESS_SMALLWASH_POWER;
~~~

TODO

### `PERMPRESS_PREWASH_ENERGY`

~~~
  double PERMPRESS_PREWASH_ENERGY;
~~~

TODO

### `PERMPRESS_WASH_ENERGY`

~~~
  double PERMPRESS_WASH_ENERGY;
~~~

TODO

### `PERMPRESS_SPIN_LOW_ENERGY`

~~~
  double PERMPRESS_SPIN_LOW_ENERGY;
~~~

TODO

### `PERMPRESS_SPIN_MEDIUM_ENERGY`

~~~
  double PERMPRESS_SPIN_MEDIUM_ENERGY;
~~~

TODO

### `PERMPRESS_SPIN_HIGH_ENERGY`

~~~
  double PERMPRESS_SPIN_HIGH_ENERGY;
~~~

TODO

### `PERMPRESS_SMALLWASH_ENERGY`

~~~
  double PERMPRESS_SMALLWASH_ENERGY;
~~~

TODO

### `GENTLE_PREWASH_POWER`

~~~
  double GENTLE_PREWASH_POWER;
~~~

TODO

### `GENTLE_WASH_POWER`

~~~
  double GENTLE_WASH_POWER;
~~~

TODO

### `GENTLE_SPIN_LOW_POWER`

~~~
  double GENTLE_SPIN_LOW_POWER;
~~~

TODO

### `GENTLE_SPIN_MEDIUM_POWER`

~~~
  double GENTLE_SPIN_MEDIUM_POWER;
~~~

TODO

### `GENTLE_SPIN_HIGH_POWER`

~~~
  double GENTLE_SPIN_HIGH_POWER;
~~~

TODO

### `GENTLE_SMALLWASH_POWER`

~~~
  double GENTLE_SMALLWASH_POWER;
~~~

TODO

### `GENTLE_PREWASH_ENERGY`

~~~
  double GENTLE_PREWASH_ENERGY;
~~~

TODO

### `GENTLE_WASH_ENERGY`

~~~
  double GENTLE_WASH_ENERGY;
~~~

TODO

### `GENTLE_SPIN_LOW_ENERGY`

~~~
  double GENTLE_SPIN_LOW_ENERGY;
~~~

TODO

### `GENTLE_SPIN_MEDIUM_ENERGY`

~~~
  double GENTLE_SPIN_MEDIUM_ENERGY;
~~~

TODO

### `GENTLE_SPIN_HIGH_ENERGY`

~~~
  double GENTLE_SPIN_HIGH_ENERGY;
~~~

TODO

### `GENTLE_SMALLWASH_ENERGY`

~~~
  double GENTLE_SMALLWASH_ENERGY;
~~~

TODO

### `queue_min`

~~~
  double queue_min[unit];
~~~

TODO

### `queue_max`

~~~
  double queue_max[unit];
~~~

TODO

### `clotheswasher_run_prob`

~~~
  double clotheswasher_run_prob;
~~~

TODO

### `state`

~~~
  enumeration {SPIN4, SPIN3, SPIN2, SPIN1, WASH, PREWASH, STOPPED} state;
~~~

TODO

### `spin_mode`

~~~
  enumeration {SMALLWASH, SPIN_WASH, SPIN_HIGH, SPIN_MEDIUM, SPIN_LOW} spin_mode;
~~~

TODO

### `wash_mode`

~~~
  enumeration {GENTLE, PERM_PRESS, NORMAL} wash_mode;
~~~

TODO

### `shape`

~~~
  loadshape shape;
~~~

TODO

### `load`

~~~
  enduse load;
~~~

The enduse load description

### `energy`

~~~
  complex energy[kVAh];
~~~

The total energy consumed since the last meter reading

### `power`

~~~
  complex power[kVA];
~~~

The total power consumption of the load

### `peak_demand`

~~~
  complex peak_demand[kVA];
~~~

The peak power consumption since the last meter reading

### `heatgain`

~~~
  double heatgain[Btu/h];
~~~

The heat transferred from the enduse to the parent

### `cumulative_heatgain`

~~~
  double cumulative_heatgain[Btu];
~~~

The cumulative heatgain from the enduse to the parent

### `heatgain_fraction`

~~~
  double heatgain_fraction[pu];
~~~

The fraction of the heat that goes to the parent

### `current_fraction`

~~~
  double current_fraction[pu];
~~~

The fraction of total power that is constant current

### `impedance_fraction`

~~~
  double impedance_fraction[pu];
~~~

The fraction of total power that is constant impedance

### `power_fraction`

~~~
  double power_fraction[pu];
~~~

The fraction of the total power that is constant power

### `power_factor`

~~~
  double power_factor;
~~~

The power factor of the load

### `constant_power`

~~~
  complex constant_power[kVA];
~~~

The constant power portion of the total load

### `constant_current`

~~~
  complex constant_current[kVA];
~~~

The constant current portion of the total load

### `constant_admittance`

~~~
  complex constant_admittance[kVA];
~~~

The constant admittance portion of the total load

### `voltage_factor`

~~~
  double voltage_factor[pu];
~~~

The voltage change factor

### `breaker_amps`

~~~
  double breaker_amps[A];
~~~

The rated breaker amperage

### `configuration`

~~~
  set {IS220, IS110} configuration;
~~~

The load configuration options

### `override`

~~~
  enumeration {OFF, ON, NORMAL} override;
~~~

TODO

### `power_state`

~~~
  enumeration {UNKNOWN, ON, OFF} power_state;
~~~

TODO

# Example

~~~
  object clotheswasher {
    motor_power "0.0";
    circuit_split "0.0";
    queue "0.0";
    demand "0.0";
    energy_meter "0+0i";
    stall_voltage "0.0";
    start_voltage "0.0";
    clothesWasherPower "0.0";
    stall_impedance "0+0i";
    trip_delay "0.0";
    reset_delay "0.0";
    Is_on "0.0";
    normal_perc "0.0";
    perm_press_perc "0.0";
    NORMAL_PREWASH_POWER "0.0";
    NORMAL_WASH_POWER "0.0";
    NORMAL_SPIN_LOW_POWER "0.0";
    NORMAL_SPIN_MEDIUM_POWER "0.0";
    NORMAL_SPIN_HIGH_POWER "0.0";
    NORMAL_SMALLWASH_POWER "0.0";
    NORMAL_PREWASH_ENERGY "0.0";
    NORMAL_WASH_ENERGY "0.0";
    NORMAL_SPIN_LOW_ENERGY "0.0";
    NORMAL_SPIN_MEDIUM_ENERGY "0.0";
    NORMAL_SPIN_HIGH_ENERGY "0.0";
    NORMAL_SMALLWASH_ENERGY "0.0";
    PERMPRESS_PREWASH_POWER "0.0";
    PERMPRESS_WASH_POWER "0.0";
    PERMPRESS_SPIN_LOW_POWER "0.0";
    PERMPRESS_SPIN_MEDIUM_POWER "0.0";
    PERMPRESS_SPIN_HIGH_POWER "0.0";
    PERMPRESS_SMALLWASH_POWER "0.0";
    PERMPRESS_PREWASH_ENERGY "0.0";
    PERMPRESS_WASH_ENERGY "0.0";
    PERMPRESS_SPIN_LOW_ENERGY "0.0";
    PERMPRESS_SPIN_MEDIUM_ENERGY "0.0";
    PERMPRESS_SPIN_HIGH_ENERGY "0.0";
    PERMPRESS_SMALLWASH_ENERGY "0.0";
    GENTLE_PREWASH_POWER "0.0";
    GENTLE_WASH_POWER "0.0";
    GENTLE_SPIN_LOW_POWER "0.0";
    GENTLE_SPIN_MEDIUM_POWER "0.0";
    GENTLE_SPIN_HIGH_POWER "0.0";
    GENTLE_SMALLWASH_POWER "0.0";
    GENTLE_PREWASH_ENERGY "0.0";
    GENTLE_WASH_ENERGY "0.0";
    GENTLE_SPIN_LOW_ENERGY "0.0";
    GENTLE_SPIN_MEDIUM_ENERGY "0.0";
    GENTLE_SPIN_HIGH_ENERGY "0.0";
    GENTLE_SMALLWASH_ENERGY "0.0";
    queue_min "0.0";
    queue_max "0.0";
    clotheswasher_run_prob "0.0";
    state "0";
    spin_mode "0";
    wash_mode "0";
    override "0";
    power_state "0";
  }
~~~

# See also

* [[/Module/Residential]]

