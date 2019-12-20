[[/Module/Commercial/Parking]] -- Commercial parking structure

# Synopsis
GLM:
~~~
module commercial;
object parking {
  total_size 100;
  total_chargers 10;
  total_floors 1;
  total_elevetors 0;
  elevator_usage "0.2 pu/pu";
  charger_unit_power "6.7+0.0j kVA";
  elevator_unit_power "20+1j VAh/pu";
  charging_price "0.25 $/kWh";
  parking_fee "1.00 $/h";
  fee_waiver "0.0 pu";
  idle_penalty "2.0 pu";
  total_cars 0;
  ev_count 0;
  ev_active 0;
  total_power "0+0j kVA";
  lighting_power "0+0j kVA";
  elevator_power "0+0j kVA";
  fan_power "0+0j kVA";
  charger_power "0+0j kVA";
  total_revenue "0.00 $";
  parking_logfile "";
}
~~~

# Description

The `parking` object define a commercial parking structure equipped with electric vehicle chargers.  The number of spots `total_size` and number of chargers `total_chargers` determines the overall capacity of the parking facility.  

The unit power `charger_unit_power` set the power deliver capacity for each charging station.  The `charging_power` is determined by the number of EVs present.  Lighting and fan power are constant. Elevator power is determined by the number of arrivals and departures and the elevator usage, assuming that the parking lot fills from the ground floor in but empties randomly. EV chargers are assumed to be on the ground floor and therefore have no elevator load associated.

The `parking_fee` and `charging_price` are used to compute the `total_revenue`.  The `fee_waiver` is used to compute the fee for using a charger spot.  The `fee_waiver` is the factor applied to the `parking_fee` for occupying a charging spot.  The `idle_penalty` is the factor applied to the `parking_fee` for occupying a charging spot when not charging.

## Properties

### `total_size`
~~~
int32 total_size;
~~~

This parameter sets the total number of parking spots in the parking structure.

### `total_chargers`
~~~
int32 total_chargers;
~~~

This parameter sets the total number of electric vehicle chargers in the parking structure.

### `total_floors`
~~~
int32 total_floors;
~~~

This parameter sets the total number of floor in the parking structure. This parameter affects elevator power as the parking structure fills up.

### `total_elevators`
~~~
int32 total_elevators;
~~~

This parameter sets the total number of elevators available in the parking structure.  This parameter limits the maximum elevator power at any given time.

### `elevator_usage`
~~~
double elevator_usage[pu];
~~~

This parameter sets the fraction increase in the number of people who use the elevator per floor. Note that this quantity is truncated at 100% for floors greater than `1/elevator_usage` without warning, i.e., it is assumed that everyone uses the elevator beyond a certain floor.

### `charger_unit_power`
~~~
complex charger_unit_power[kVA];
~~~

This parameter set the power demand for a single charging station when active. When inactive, the power draw is assumed to be zero.

### `elevator_unit_power`
~~~
complex elevator_unit_power[VA/pu]
~~~

This parameter sets the energy required for an elevator to move one floor. When not running the elevator, the power draw is assumed to be zero.

### `charging_price`
~~~
double charging_price[$/kWh];
~~~

This parameter sets the energy price for charging an electric vehicle.

### `parking_fee`
~~~
double parking_fee[$/h];
~~~

This parameter sets the hourly price of parking without charging.

### `fee_waiver`
~~~
double fee_waiver[pu];
~~~

This parameter sets the fee waiver fraction for electric vehicle when charging. A value of `0.0` indicate that no waiver is available. A value of `1.0` indicates that the entire parking fee is waived.  A negative value indicates that a surcharged is applied when occupying a charging station.

### `idle_penalty`
~~~
double idle_penalty[pu];
~~~

This parameter sets the idle charger penalty applied to electric vehicles that occupy a charging station but are not charging.  The penalty is a fraction of the parking fee.  A value of `0.0` indicates that no penalty is applied.  A positive value indicates that fraction of the parking fee applied when not charging.  A negative value is not allowed. 

### `total_cars`
~~~
int32 total_cars;
~~~

This parameter indicates the total number of cars present in the parking structure, including EVs.

### `ev_count`
~~~
int32 ev_count;
~~~

This parameter indicates the total number of electric vehicles in the parking structure.

### `ev_active`
~~~
int32 ev_active;
~~~

This parameter indicates the total number of actively charging EVs in the parking structure.

### `total_power`
~~~
complex total_power[kVA];
~~~

This parameter indicates the total power demand of the parking structure. This quantity is updated automatically whenever the number of vehicles in parking structure changes or an EV charger changes state.

### `lighting_power`
~~~
complex lighting_power[kVA];
~~~

This parameter sets the total lighting power demand of the parking structure. This quantity is invariant.

### `elevator_power`
~~~
complex elevator_power[kVA];
~~~

This parameter indicates the total elevator load in the parking structure. This quantity changes twice whenever the number of vehicles in the parking structure changes.

### `fan_power`
~~~
complex fan_power[kVA];
~~~

This parameter sets the total fan power in the parking structure.  This quantity is invariant.

### `charger_power`
~~~
complex charger_power[kVA];
~~~

This parameter indicates the total charging power for the parking structure.  This quantity changes whenever the number of EVs or an EV charger state changes.

### `total_revenue`
~~~
double total_revenue[$];
~~~

This parameter indicates the total revenue generated by the parking structure. This value changes whenever a vehicle departs the parking structure or a vehicle charger turns off.

### `parking_logfile`
~~~
string parking_logfile;
~~~

This parameter sets the parking log file in which arrivals, departures, and charger events are logged. If the value is not set, no logfile is generated.

## Model

A commercial parking structure provides $ TotalSpaces > 0 $ general vehicle parking spaces, and $ TotalChargers \le TotalSpaces $ parking spaces equipped with EV chargers.  Of these $ TotalCars \le TotalSpaces $ spaces are occupied, and $ EvCount \le TotalChargers $ chargers are occupied.  Of the $EvCount$ occupied chargers, $ EvActive $ are charging.

The total power demand of the parking structure is
$$
  TotalPower = LightingPower + FanPower + ElevatorPower + ChargerPower
$$
where $LightingPower$ and $FanPower$ are static,
$$
  ChargerPower = ChargerUnitPower \times EvActive
$$
and
$$
  ElevatorPower = TODO
$$

The total revenue for the parking structure is computed as follows:
$$
  TotalRevenue = TODO
$$

# Example

~~~
object parking {
  total_spots 100;
  total_chargers 10;
}
~~~

# See also
* [[/Module/Commercial]]
