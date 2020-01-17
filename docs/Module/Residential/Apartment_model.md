
This document provides the technical support manual for the multi-family apartment building model.

# Apartment Model

The apartment building model in GridLAB-D represents multi-storey, multi-tenant residential buildings.  The building must have more than 1 floor and more than 1 unit.  The building may have partial occupancy, and the vacant units are assumed to have not end-use loads, and only minimal space conditioning.  

Circulation may be outdoor, or indoor (with or without space conditioning).  If indoor, the circulation may be units on only one or on both sides. Circulation is assumed to be linear ending at the perimeter walls.

The building may be equipped with central heating and/or cooling, using a forced air, two-pipe, or four-pipe system. Apartment units may also have independent heating, cooling, and ventilation.

# Model Parameters

The building model parameters are grouped into the following categories.

- `building`: Parameters relating to the entire building.
- `core`: Parameters relating to the building core.
- `parking`: Parameters relating to the parking structure (if any).
- `power`: Parameters relating to the electric power systems.
- `system`: Parameters relating to the building systems.
- `unit`: Parameters relating to the apartment units.
- `vacant`: Paremeters relating to the vacant units.

## Building Parameters

- `building_floors`: (static integer) The number of floors in the building.  This value must be greater than 1. This value has no default and must be specified in the model.

- `building_floor_depth[ft]`: (static real) The ceiling-to-floor depth in the building (i.e., floor thickness). The default is `2.0 ft`.

- `building_floor_height[ft]`: (static real) The floor-to-ceiling height in the building (i.e., unit height). The default is `8.0 ft`.

- `building_occupancy_factor[pu]`: (static real) The fraction of building units that are occupied. The default is `0.9 pu`.

- `building_outdoor_temperature[degF]`: (input real) The temperature outside the building (driven by the weather model). The default is `59.0 degF`.

- `building_units`: (static integer) The number of units in the building. This value has no default and must be specified in the model.


## Core Parameters

- `core_configuration`: (static set) Indicates the configuration of the core.  The default is `NONE`. Options include:
  - `NONE`: The core is exterior, single-loaded, and unconditioned.
  - `INDOOR`: The core space is indoor. 
  - `DOUBLE`: The core is double-loaded. This parameter implies `INDOOR`.
  - `CONDITIONED`: The core space is conditioned. This parameter implies `INDOOR`.

- `core_cooling_setpoint[degF]`: (input real) The cooling setpoint in the core space. The value is only meaningful if the core is an indoor conditioned space. The default is `unit_cooling_setpoint+5.0`.

- `core_elevators`: (static integer) The number of elevators operating in the core space. The default is `0`.

- `core_heating_setpoint[degF]`: (input real) The heating setpoint in the core space. The value is only meaningful if the core is an indoor conditioned space. The default is `unit_heating_setpoint-5.0`.

- `core_laundry_units`: (static integer) The number of community/shared washer/dryers pairs installed in the core space.

- `core_mode`: (static enumeration) The operating mode of the core zone.  The default `OFF`. Valid values are:
  - `OFF`: The system is off.
  - `VENTILATING`: The system is ventilating.
  - `HEATING`: The system is heating.
  - `COOLING`: The system is cooling.
  
- `core_width[ft]`: (static real) The width of the core space. The default is `5 ft` for single-loaded corridors, and `6 ft` for double-loaded corridors.


## Parking Parameters

- `parking_capacity_chargers[kW]`: (static real) The installed vehicle charger capacity.  The default is `0.0 kW`.

- `parking_capacity_elevators[kW]`: (static real) The installed elevator capacity. The default is `0.0 kW`.

- `parking_capacity_lights[kW]`: (static real) The installed lighting capacity. The default is `0.0 kW`.

- `parking_capacity_ventilation[kW]`: (static real) The installed ventilation capacity. The default is `0.0 kW`.

- `parking_chargers_active`: (input integer) The number of EV chargers that are in use. The default is `0`.

- `parking_chargers_installed`: (static integer) The number of EV chargers installed. The default is `0`.

- `parking_configuration`: (static enumeration) The parking configuration. The default is `OUTDOOR`. The following options are recognized:
    - `OUTDOOR`: The parking is exterior (lighting is on only at night).
    - `INDOOR`: The parking in indoor (lighting and ventilation are always on).


- `parking_demand_chargers[pu]`: (output real) The parking EV charger demand as a fraction of installed capacity. The default is `0.0`.

- `parking_demand_elevators[pu]`: (output real) The parking elevator demand as a fraction of installed capacity. The default is `0.0`.

- `parking_demand_lights[pu]`: (output real) The parking lighting demand as a fraction of installed capacity. The default is `0.0`.

- `parking_demand_ventilation[pu]`: (output real) The parking ventilation demand as a fraction of installed capacity. The default is `0.0`.

- `parking_size`: (static integer) The total number of parking spots installed. 


## Power Parameters

- `power_core[kW]`: (output real) The power delivered to the core spaces in the building (including washing and drying).

- `power_parking[kW]`: (output real) The power delivered to the parking areas.

- `power_system[kW]`: (output real) The power delivered to the building systems (i.e., HVAC).

- `power_total[kW]`: (output real) The total power delivered to the building, i.e., core, parking, system, and units.

- `power_units[kW]`: (output real) The total power delivered to the apartment units (both occupied and vacant).
    

## System Parameters

- `system_cooling_air_temperature[degF]`: (static real) The system cooling supply air temperature. This is only meaningful when `system_type_central&COOL` and `system_type_ventilation==CENTRAL`. The default value is `50 degF`.

- `system_cooling_capacity[kBtu/h]`: (static real) The system cooling capacity. This is only meaningful when `system_type_central&COOL`. The default value is computed based on the heating and cooling design conditions (see below).

- `system_cooling_efficiency[kBtu/kWh]`: (static real) The system cooling efficiency. This is only meaning when `system_type_central&COOL`. The default value is "15.0 kBtu/kWh".

- `system_heating_air_temperature[degF]`: (static real) The system heating supply air temperature. This is only meaning when `system_type_central&HEAT` and `system_type_ventliation==CENTRAL`. The default is `110 degF`.

- `system_heating_capacity[kBtu/h]`: (static real) The system heating capacity. This is only meaningful when `system_type_central&HEAT`. The default value is computed based on the heating and cooling design conditions (see below).

- `system_heating_efficiency[kBtu/kWh]`: (static real) The system heating efficiency. This is only meaning when `system_type_central&HEAT`. The default value is `10 kBtu/kWh`.

- `system_mode`: (static enumeration) The operating mode of the central system.  The default `OFF`. Valid values are:
  - `OFF`: The system is off.
  - `VENTILATING`: The system is ventilating.
  - `HEATING`: The system is heating.
  - `COOLING`: The system is cooling.

- `system_type_central`: (static set) The central system type, if any. The default is `NONE`. Valid options are:
  - `NONE`: No central system is present.
  - `HEAT`: Only central heating is present.
  - `COOL`: Only central cooling is present.
  - `BOTH`: Both central heating and cooling are present. This is equivalent to `HEAT|COOL`.

- `system_type_economizer`: (static enumeration) The type of economizer used by the central cooling system, if any. The default is `NONE`. Valid options are:
  - `NONE`: No economizer present
  - `DRYBULB`: Economizer uses drybulb temperature sensor
  - `WEBBULB`: Economizer uses wetbulb temperature sensor
  - `DIFFERENTIAL`: Economizer uses differential of two wetbulb sensors.

- `system_type_ventilation`: (static enumeration) The type of central ventilation system, if any. The default is `NONE`. Valid values are:
  - `NONE`: No artificial ventilation is present
  - `LOCAL`: Local-only ventilation is present
  - `CENTRAL`: Central ventilation is present

## Unit Parameters

- `unit_appliance_types`: (static set) The types of appliances installed in the units. The default `REFRIGERATOR`. Valid values are:
  - `COOKING`: Units have cooking appliances.
  - `DISHWASHER`: Units have dishwashers.
  - `DRYER`: Units have dryers.
  - `REFRIGERATOR`: Units have refrigerators.
  - `NONE`: Units have no appliances.
  - `WASHER`: Units have washers.

- `unit_capacity_cooking[kW]`: (static double) The maximum power of the cooking appliances in units. The default is `3.0 kW`.

- `unit_capacity_dishwasher[kW]`: (static double) The maximum power of the dishwashing appliances in units. The default is `1.8 kW`.

- `unit_capacity_dryer[kW]`: (static double) The maximum power demand of dryers in units. The default is `3.0 kW`.

- `unit_capacity_lights[kW]`: (static double) The maximum lighting demand in units. The default is based on floor area, i.e., `0.6 W/sf`.

- `unit_capacity_plugs[kW]`: (static double) The maximum non-lighting plug load demand in units. The default is based on floor area, i.e., `0.5 W/sf`.

- `unit_capacity_refrigerator[kW]`: (static double) The maximum refrigeration demand in units. The default is `200 W`.

- `unit_capacity_washer[kW]`: (static double) The maximum washer power demand in units. The default is `1.0 kW`.

- `unit_cooling_capacity[kBtu/h]`: (static double) The cooling capacity of the unit systems, if any. The default is computed based on design conditions (see below).

- `unit_cooling_efficiency[kBtu/kW]`: (static double) The unit cooling system efficiency. The default is `15 kBtu/kWh`.

- `unit_cooling_setpoint[degF]`: (input double) The unit cooling system temperature setpoint. The default is `78 degF`.

- `unit_demand_cooking[pu]`: (dynamic double) The fraction of the cooking power capacity that is active. The default is set by the residential cooking schedule.

- `unit_demand_dishwasher[pu]`: (dynamic double) The fraction of the dishwasher power capacity that is active. The default is set by the residential dishwasher schedule.

- `unit_demand_dryer[pu]`: (dynamic double) The fraction of the dryer power capacity that is active. The default is set by the residential dryer schedule.

- `unit_demand_lights[pu]`: (dynamic double) The fraction of the lighting power capacity that is active. The default is set by the residential lighting schedule.

- `unit_demand_plugs[pu]`: (dynamic double) The fraction of the plugs power capacity that is active. The default is set by the residential plugs schedule.

- `unit_demand_refrigerator[pu]`: (dynamic double) The fraction of the refrigerator power capacity that is active. The default is set by the residential refrigerator schedule.

- `unit_demand_washer[pu]`: (dynamic double) The fraction of the washer power capacity that is active. The default is set by the residential washer schedule.

- `unit_depth[ft]`: (static double) The unit depth is the average distance from the core wall to the perimeter meter. The default is `40 ft`

- `unit_heating_capacity[kBtu/h]`: (static double) The heating capacity of the unit systems, if any. The default is computed based on design conditions (see below).

- `unit_heating_efficiency[kBtu/kWh]`: (static double) The unit heating system efficiency. The default is `10 kBtu/kWh`. Note that the effectiveness of a heating decline with lower outdoor temperature, but the heating efficiency is never below `3.4 kBtu/kWh`.

- `unit_heating_setpoint[degF]`: (input double)  The unit heating system temperature setpoint. The default is `70 degF`.

- `unit_mode`: (dynamic enumeration) The local system mode. The default is `OFF`. Valid values are:
  - `OFF`: The system is off.
  - `VENTILATING`: The system is ventilating.
  - `HEATING`: The system is heating.
  - `COOLING`: The system is cooling.

- `unit_system_type`: (static set) The type of unit heating/cooling systems present. The default is `NONE`. Valid options are:
  - `NONE`: No local system is present.
  - `HEAT`: Only local heating is present.
  - `COOL`: Only local cooling is present.
  - `BOTH`: Both local heating and cooling are present. This is equivalent to `HEAT|COOL` and implies the unit is a heat-pump.

- `unit_width[ft]`: (static double) The unit width is the distance from one unit interior wall to the next. The default is `25` ft.

## Vacant Unit Parameters

- `vacant_cooling_setpoint[degF]`: The vacant unit cooling setpoint. The default is `120 degF`.

- `vacant_heating_setpoint[degF]`: The vacant unit heating setpoint. The default is `50 degF`.

- `vacant_mode`: The vacant unit system mode. The default `OFF`. Valid values are:
  - `OFF`: The system is off.
  - `VENTILATING`: The system is ventilating.
  - `HEATING`: The system is heating.
  - `COOLING`: The system is cooling.


## Heating/Cooling Design Conditions

The following parameters have defaults that are computed automatically if values are not provided in the model. The values for these calculations can be obtained from the "Thermal Model" section below.

### Unit Capacity

The unit heating/cooling capacity of local systems is based on the maximum heat/cooling gain/loss possible, including a safety margin of 50%.  

The maximum local unit heating capacity is

$$
    \overline{Q}_{H_A} = 1.5 [ U_{OA} ( \underline{T}_O - T_{H_A} ) + U_{AU} ( T_{H_U} - T_{H_A} ) + U_{AC} ( T_{H_C} - T_{H_A} ) ]
$$

where $\underline{T}_O$ is the minimum outdoor air temperature observed, and $T_{H_z}$ is the heating setpoint of the zone $z$.

The maximum local unit cooling capacity is 

$$
    \overline{Q}_{C_A} = 1.5 [ U_{OA} ( \overline{T}_O - T_{C_A} ) + U_{AU} ( T_{C_U} - T_{C_A} ) + U_{AC} ( T_{C_C} - T_{C_A} ) + Q_{A} ]
$$

where $\overline{T}_O$ is the maximum outdoor air temperature observed, and $T_{C_z}$ is the cooling setpoint of the zone $z$.

If the unit is a heat-pump (i.e., `unit_system_type==BOTH` then the unit capacity is the larger of the two values.

### System Capacity

The system capacity is computed based on the maximum heating/cooling gain/loss possible, including a safety margin of 50%, and not including the local unit capacity, if any.

The maximum system heating capacity is

$$
    N M ( \overline{Q}_{H^*_A} + \overline{Q}_{H_C} )
$$

where $\overline{Q}_{H^*_A}$ is the unit heating capacity not met by local unit systems, and 

$$
    \overline{Q}_{H_C} = U_{OC} ( \underline{T}_O - T_{H_C} ) + U_{UC} ( T_{H_U} - T_{H_C} )
$$

The maximum system cooling capacity is

$$
    N M ( \overline{Q}_{C^*_A} + \overline{Q}_{C_C} )
$$

where

$$
    \overline{Q}_{C_C} = U_{OC} ( \overline{T}_O - T_{C_C} ) + U_{UC} ( T_{C_U} - T_{C_C} )
$$

# Thermal Model

The thermal model is a based on a four-zone ETP model, where the following zone variables are defined:

Table 1: Zone variables

| Zone Name            | Temperature | Mass   | Heat   |
| :------------------- | :---------: | :----: | :----: |
| Occupied apartment   | $T_A$       | $C_A$  | $Q_A$  |
| Unoccupied apartment | $T_U$       | $C_U$  | $Q_U$  |
| Building core        | $T_C$       | $C_C$  | $Q_C$  |
| Building mass        | $T_M$       | $C_M$  |   -    |
| Outdoor air          | $T_O$       |   -    |   -    |

The model assumes that every zone is connected to every other zone with the following zone-zone conductance parameters defined

Table 2: Zone-zone conductance

| Conductance | Connected zones                       |
| ----------- | ------------------------------------- |
| $U_{OA}$    | Occupied apartment & outdoor air      |
| $U_{OU}$    | Unoccupied apartment & outdoor air    |
| $U_{OC}$    | Building core & outdoor air           |
| $U_{OM}$    | Building mass & outdoor air           |
| $U_{AU}$    | Occupied & unoccupied apartments      |
| $U_{AC}$    | Occupied apartments & building core   |
| $U_{AM}$    | Occupied apartments & building mass   |
| $U_{UC}$    | Uncccupied apartments & building core |
| $U_{UM}$    | Unoccupied apartments & building mass |
| $U_{CM}$    | Building core & building mass         |

The thermal dynamics are defined by Equation (1)

$$
    \begin{bmatrix} 
        \dot T_A 
    \\ 
        \dot T_U 
    \\
        \dot T_C
    \\
        \dot T_M
    \end{bmatrix}
    =
    \begin{bmatrix}
        -\frac{U_{OA}+U_{AU}+U_{AC}+U_{AM}}{C_A} & \frac{U_{AU}}{C_A} & \frac{U_{AC}}{C_A} & \frac{U_{AM}}{C_A}
    \\
        \frac{U_{AU}}{C_U} & -\frac{U_{OA}+U_{AU}+U_{UC}+U_{UM}}{C_U} & \frac{U_{UC}}{C_U} & \frac{U_{UM}}{C_U}
    \\
        \frac{U_{AC}}{C_C} & \frac{U_{UC}}{C_C} & -\frac{U_{OC}+U_{AC}+U_{UC}+U_{CM}}{C_C} & \frac{U_{CM}}{C_C}
    \\
        \frac{U_{AM}}{C_M} & \frac{U_{UM}}{C_M} & \frac{U_{CM}}{C_M} & -\frac{U_{OM}+U_{AM}+U_{UM}+U_{CM}}{C_M}
    \end{bmatrix}
    \begin{bmatrix}
        T_A
    \\
        T_U
    \\
        T_C
    \\
        T_M
    \end{bmatrix}
    +
    \begin{bmatrix}
        \frac{1}{C_A} & 0 & 0 & \frac{U_{OA}}{C_A}
    \\
        0 & \frac{1}{C_U} & 0 & \frac{U_{OU}}{C_U}
    \\
        0 & 0 & \frac{1}{C_C} & \frac{U_{OC}}{C_C}
    \\
        0 & 0 & 0 & \frac{U_{OM}}{C_M}
    \end{bmatrix}
    \begin{bmatrix}
        Q_A
    \\
        Q_U
    \\
        Q_C
    \\
        T_O
    \end{bmatrix}
    \qquad (1)
$$
where the heat gains are
- $Q_A = Q_{AH} + Q_{AS} + Q_{AV} + Q_{AE}$,
- $Q_U = Q_{UH} + Q_{US}$
- $Q_C = Q_{CH} + Q_{CS} + Q_{CV}$

The heat gains for each zones are defined as follows

Table 3: Occupied apartment heat gains

| Heat source      | Parameter |
| ---------------- | --------- |
| HVAC             | $Q_{AH}$  |
| Solar gain       | $Q_{AS}$  |
| Ventilation gain | $Q_{AV}$  |
| End-use gains    | $Q_{AE}$  |

Table 4: Unoccupied apartment heat gains 

| Heat source      | Parameter |
| ---------------- | --------- |
| HVAC             | $Q_{UH}$  |
| Solar gain       | $Q_{US}$  |

Table 5: Building core heat gains

| Heat source      | Parameter |
| ---------------- | --------- |
| HVAC             | $Q_{CH}$  |
| Solar gain       | $Q_{CS}$  |
| Ventilation gain | $Q_{CV}$  |


The following end-uses contribute to heat gain in building zones and are driven by schedules

Table 6: Zone end-use schedule source

| End-use     | $Q_{AE}$      | $Q_{CE}$     |
| ----------- | ------------- | ------------ |
| Lights      | `lighting`    | `hallway`    |
| Plugs       | `plugs`       |      -       |
| Cooking     | `cooking`     |      -       |
| Dishwashing | `dishwashing` |      -       |
| Washing     | `washing`     |      -       |
| Drying      | `drying`      |      -       |
| Hotwater    | `hotwater`    |      -       |


Equation (1) is represented canonically as

$$
    \boxed { \dot {\textbf T} = \textbf A \textbf T + \textbf B_1 \textbf q + \textbf B_2 \textbf u } \qquad (2)
$$

where $\textbf{T} = \begin{bmatrix} T_A & T_U & T_C & T_M \end{bmatrix}^\mathrm{T}$, $\textbf q$ are the ambient heat gains/losses, and $\textbf u$ is the controlled heat gains/losses, with

$$
    \textbf B_1 = \begin{bmatrix}
        {U_{OA}} \over {C_A} &   a \over {C_A} &               0 &               0   
    \\
        {U_{OU}} \over {C_U} &               0 &   a \over {C_U} &               0   
    \\
        {U_{OC}} \over {C_C} &               0 &               0 &   a \over {C_C} 
    \\
        {U_{OM}} \over {C_M} & {1-a}\over{C_M} & {1-a}\over{C_M} & {1-a}\over{C_M}   
    \end{bmatrix}
    \qquad \textrm{and} \qquad
    \textbf B_2 = \begin{bmatrix}
          a \over {C_A} &               0 &               0 &             0
    \\
                      0 &   a \over {C_U} &               0 &             0
    \\
                      0 &               0 &   a \over {C_C} &             0
    \\
        {1-a}\over{C_M} & {1-a}\over{C_M} & {1-a}\over{C_M} & 1 \over {C_M} 
    \end{bmatrix}
$$

and

$$
    \textbf q = \begin{bmatrix} T_O \\ Q_{AS} + Q_{AV} + Q_{AE} \\ Q_{US} \\ Q_{CS} + Q_{CV} + Q_{CE} \end{bmatrix}
    \qquad \textrm{and} \qquad
    \textbf u = \begin{bmatrix} Q_{AH} \\ Q_{UH} \\ Q_{CH} \\ Q_{MH} \end{bmatrix}.
$$

## HVAC Controls

The following thermostat setpoints are use to control HVAC

| Zone       | Heating  | Cooling  |
| ---------- | -------- | -------- |
| Occupied   | $T_{AH}$ | $T_{AC}$ |
| Unccupied  | $T_{UH}$ | $T_{UC}$ |
| Core       | $T_{CH}$ | $T_{CC}$ |

### System modes

The following types of systems can be modeled.

| Local | Central | Ventilation | Economizer | Description
| ----- | ------- | ----------- | ---------- |-----------------------------------
| Heat  | None    | None        | None       | Only local heating is available
| Cool  | None    | None        | None       | Only local cooling is available
| Both  | None    | None        | None       | Only local heating and cooling is available
| None  | Heat    | None        | None       | Only central heating is available using hotwater/steam pipes
| None  | Cool    | None        | None       | Only central cooling is available using chilled water pipes
| None  | Both    | None        | None       | Only central heating and cooling is available using four pipes
| Cool  | Heat    | None        | None       | Local cooling and central heating using hotwater/steam pipes
| Heat  | Cool    | None        | None       | Local heating and central cooling using chilled water pipes
| Heat  | None    | Central     | Optional   | Central ventilation with terminal heating
| Cool  | None    | Central     | Optional   | Central ventilation with terminal cooling
| Both  | None    | Central     | Optional   | Central ventilation with terminal heating and cooling
| None  | Both    | Central     | Optional   | Forced air central heating and cooling
| Heat  | Both    | Central     | Optional   | Forced air central heating and cooling with terminal heating
| Cool  | Both    | Central     | Optional   | Forced air central heating and cooling with terminal cooling

When a central system is present and only one local mode is allowed, the central will operate in the unavailable local mode if any zone requires it. The remaining zones will use the local mode to meet local conditioning requirement, if any.  If the ventilation is central, then the local mode will use the central system mode supply air temperature as the basis for the local mode (e.g., terminal reheat).

If an economizer is present and the system is operating in cooling mode, then the system load is reduced by the factor

$$
    E = {{T_O-T_D} \over {T_R-T_D}}
$$
where
- $T_D$ is the cooling air supply temperature (e.g., 50 degF)
- $T_R$ is the return air temperature, which may be computed from the floor-area-weighted indoor air temperatures of the conditioned spaced, e.g., $T_R = {{A_AT_A+A_CT_C}\over{A_A+A_C}}$, with $A_A$ and $A_C$ being the total occupied apartment and core space floor areas.

Otherwise, the parameter $E = 1$.

# Heating/Cooling Solution Method

The model assumes that the setpoints are maintained in the controlled zone, and computes the heating/cooling load required by setting Equation (2) equal to zero.  However, if the heating/cooling load exceeds the capacity of the central plant or the HVAC units, then the power will be saturated, and the Equation (2) will provide the temperature changes, if any, for each zone.

## Step 1 - Determine the system mode

Determine the zone balance temperatures by solving Equation (2) for $\textbf{T}$ given $\dot {\textbf T} = 0$ and $\textbf{u} = 0$ 

$$
    \textbf{T}^o = - \textbf{A}^{-1} \textbf{B}_1 \textbf{q}
$$

For each zone $n \in \{A,C,H\}$, the operating mode is

$$
    m_n = \left\{ \begin{matrix}
        +1 &:& T_n \lt T_n^o
    \\
        -1 &:& T_n \ge T_n^o
    \end{matrix} \right.
$$

## Step 2 - Compute the equilibrium load

Solve Equation (2) for the augmented $\textbf{u}$ at the temperature setpoint given $\dot {\textbf T}=0$, i.e.,

$$
    {\textbf u} = -{\textbf B}_2^{-1} ( {\textbf A} {\textbf T}^* + {\textbf B}_1 \textbf{q} )
$$

where the setpoint is

$$
    {\textbf T}^* = T_{set} - m \begin{bmatrix}  0 \\ T_{unoccupied} \\ T_{core} \\ 0 \end{bmatrix}
$$

where 

- $T_{set}$ is the general setpoint temperature for the building,
- $m$ is the building operating mode, i.e., `-1` for cooling and `+1` for heating,
- $T_{unoccupied}$ is the unoccupied temperature setpoint offset, e.g., `10 degF`, and
- $T_{core}$ is the core temperature setpoint offset, e.g., `5 degF`.

## Step 3 - Constrain the heat gain/loss

The value of $\textbf u$ obtain from Equation (2) may exceed the limits of the HVAC system, in which can they must be constrained such that element-wise

$$
    \textbf u^* = \textbf u \qquad \mathrm{s.t} \qquad \underline {\textbf u} \le {\textbf u} \le \overline {\textbf u}
$$

where $\underline {\textbf u}$ is the maximum cooling capacity and $\overline {\textbf u}$ is the maximum heating capacity of the HVAC system.  

## Step 4 - Compute the equilibrium temperature

Solve for the equilibrium temperature using Equation (2) given the control input $\textbf{u}$

$$
    \tilde{\textbf T} = -{\textbf A}^{-1}({\textbf B}_1 {\textbf q} + {\textbf B}_2 {\textbf u^*})
$$

## Step 5 - Compute the zone temperature changes

Given the constrained values $\textbf u^*$, we update the zone temperatures for the elapsed time $t$ using the solution Equation (2):

$$
    {\textbf T}(t) = \tilde{\textbf T} + e^{\textbf A t} {\textbf T}(0) + {\textbf A}^{-1} ( e^{\textbf A t} - {\textbf I} ) ( {\textbf B}_1 {\textbf q} + {\textbf B}_2 {\textbf u}^* )
$$

# Electric Power Demand

The power consumption is the sum of all the end-use loads and the HVAC load given the efficiency of the HVAC systems:

$$
    P = Q_{AE} + Q_{CE} + { E\over \eta_M } ||u||_1^*
$$

where $\eta_M$ is the efficiency of the HVAC system in the mode $M$.

# Thermal Parameters

The following parameters are used to compute the thermal parameters for a building.

Table 3: Building design parameters

| Parameter                  | Description
| -------------------------- | -------------------------------------------
| $N$                        | Number of floors (integer)
| $M$                        | Number of units per floor (integer)
| $K_X$                      | Exterior core (no=0, yes=1)
| $K_D$                      | Core loading (0=one side, 1=both sides)
| $F$                        | Floor thickness (ft)
| $X$                        | Unit width (ft)
| $Y$                        | Unit depth (ft)
| $Z$                        | Unit height (ft)
| $W$                        | Interior core width (ft)
| $A_W$                      | Unit window area (sf)
| $A_D$                      | Unit door area (sf)
| $C_{int}$                  | Unit interior mass (Btu/degF)
| $S_{floor}$                | Floor specific mass (Btu/degF.sf)

Table 4: Building thermal properties

| Parameter    | Description
| ------------ | ---------------------
| $R_{ext}$    | Exterior wall R-value
| $R_{int}$    | Interior wall R-value
| $R_{window}$ | Window R-value
| $R_{door}$   | Unit door R-value
| $R_{mass}$   | Mass R-value (i.e., floor, ceiling, furnishings)

Table 5: Derived building properties

| Parameter | Calculation &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;        | Description
| --------- | ------------------- | -------------------------------------------
| $A$       | $ X Y $             | Unit floor area (sf)
| $A_U$     | $ Y Z $             | Interior unit-unit wall area (sf)
| $A_O$     | $ X Z - A_W $       | Exterior unit wall area (sf)
| $A_C$     | $ Z X - A_D $       | Interior unit-core wall area (sf)
| $V$       | $ X Y Z $           | Unit volumne (cf)

To compute the thermal parameters we identify the expore areas for each zone. The occupancy factor $\beta$ is the ratio of occupied units to the total number of units.

Table 6: Conductance calculations

| Conductance | Parameter calculation &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
| ----------- | ---------------------------------------
| $U_{OA}$    | $ N M \beta (K_X+1) \left( { {A_O} \over {R_{ext}} } + { {A_W} \over {R_{window}} } \right) $
| $U_{OU}$    | $ N M (1-\beta) (K_X+1) \left( { {A_O} \over {R_{ext}} } + { {A_W} \over {R_{window}}} \right) $
| $U_{OC}$    | $ N (1-K_X) [ 2 W Z + (K_D+1) M X ] \left( { {A_O} \over {R_{ext}} } + { {A_W} \over {R_{window}} } \right) $
| $U_{OM}$    | $ N {{F (MX+2(K_D+1)Y+2W)} \over {R_{ext}}} $
| $U_{AU}$    | $ N M \beta (1-\beta) {A_U} \over {R_{int}} $
| $U_{AC}$    | $ N M \beta \left( { {A_W} \over {(1-K_X) R_{int} + K_X R_{ext}}} + { {A_D} \over {R_{door}} } \right) $
| $U_{AM}$    | $ N M \beta {A \over {R_{mass}}} $
| $U_{UC}$    | $ N M (1-\beta) \left( { {A_W} \over {(1-K_X) R_{int} + K_X R_{ext}}} + { {A_D} \over {R_{door}} } \right) $
| $U_{UM}$    | $ N M (1-\beta) { A \over {R_{mass}} } $
| $U_{CM}$    | $ N W M { {K_D+1} \over {R_{floor}}} $

Table 7: Capacitance calculation

| Heat capacity | Parameter calculation &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
| ------------- | -------------------------------------
| $C_A$         | $ N M \beta ( V S_{air} + C_{int} ) $
| $C_U$         | $ N M (1-\beta) ( V S_{air} + C_{int}) $
| $C_C$         | $ N M X W H C_{air} $
| $C_M$         | $ N M S_{floor} ( A + K X W )  $

# Example

The following example illustrates this method.  The method requires a linear algebra library:


```python
from numpy import *
```

The outdoor temperature is assumed to be 20F below the indoor setpoint.


```python
# set outdoor temperature (delta from T_A and T_M)
T_O = -20 # computed from weather data relative the apartment setpoint (e.g., T_A = 40 degF - T_O)
```

The building has the following characteristics


```python
# Table 3: Building design parameters
N = 10 # 10 floors
M = 20 # 20 units per floor
K_X = 0 # corridor is not exterior
K_D = 1 # corridor is double-sided
F = 2 # floor thickness (ft)
X = 30 # unit width (ft)
Y = 40 # unit depth (ft)
Z = 7.5 # unit height (ft)
W = 4.5 # corridor width (ft)
omega = 0.5 # exterior window/wall ratio
A_D = 3*6.6 # unit door area (sf)
C_int = 2000 # occupied unit interior thermal capacitance (Btu/degF)
S_floor = 150 # density of concrete floor (Btu/cf)
```


```python
# Table 4: Building thermal properties
R_ext = 19
R_int = 5
R_window = 2
R_door = 3
R_mass = 1
R_floor = 1
```


```python
# Table 5: Derived building properties
A_W = omega*X*Z # unit window area (sf)""
A_F = X*Y # unit floor area (ft)
A_U = Y*Z # unit-unit wall area (ft)
A_O = X*Z - A_W # unit-outdoor wall area (ft)
A_C = X*Z - A_D # unit-core wall area (ft)
V = X*Y*Z # unit volume (cf)
S_air = 0.24 * 0.074 # specific heat x density of air (Btu/degF.cf)
```


```python
# Building occupancy
beta = 0.9 # fraction of units that are occupied
```


```python
# set zone heat gains
Q_AS = 500000 # solar gains to occupied apartments (computed from weather data)
Q_AV = -10000 # ventilation gains to occupied apartments (computed from weather data)
Q_AE = 10000 # electric load gains to occupied aparments (computed from equipment data)
Q_US = (1-beta)*Q_AS # solar gains to unoccupied apartments (computed from weather data)
Q_CS = 1000 # solar gains to core spaces (computed from weather data)
Q_CV = -5000 # ventilation gains to core spaces (computed from weather data)
```


```python
# Building mode
m = -sign(T_O) # -1 for cooling, +1 for heating
```

The HVAC system capabilities are


```python
# set heat/cool constraints
u_min = matrix([[-500000],[-200000],[-1000000],[0]])
u_max = matrix([[500000],[200000],[1500000],[0]])
# plant efficiency
e_cool = 3.0
e_heat = 4.0
# temperature setpoints
T_unoccupied = 10 # unoccupied zone temperature setpoint offset (degF)
T_core = 5 # core zone temperature setpoint offset (degF)
```


```python
# Table 6: Conductance calculations
U_OA = N * M * beta * (K_X+1) * ( A_O/R_ext + A_W/R_window )
U_OU = U_OA * (1-beta) / beta
U_OC = N * (1-K_X) * (2*W*Z+(1-K_D)*M*X) * ( A_O/R_ext + A_W/R_window )
U_OM = N * F * (M*X+2*(K_D+1)*Y+2*W) / R_ext
U_AU = N * M * beta * (1-beta) * A_U/R_int
U_AC = N * M * beta * ( (A_W/((1-K_X)*R_int+K_X*R_ext)) + (A_D/R_door) ) 
U_AM = N * M * beta * A_F/R_mass
U_UC = N * M * (1-beta) * ( (A_W/((1-K_X)*R_int+K_X*R_ext)) + (A_D/R_door) )
U_UM = N * M * (1-beta) * A_F/R_mass
U_CM = N * M * W * (K_D+1)/R_floor
print("Table 6: Conductance calculations (kBtu/degF.h)\n\n" + 
      "U_OA U_OU U_OC U_OM U_AU U_AC U_AM U_UC U_UM U_CM\n" +
      "---- ---- ---- ---- ---- ---- ---- ---- ---- ----\n" +
      "%4.0f %4.0f %4.0f %4.0f %4.0f %4.0f %4.0f %4.0f %4.0f %4.0f" 
      % (U_OA/1000,U_OU/1000,U_OC/1000,U_OM/1000,U_AU/1000,
         U_AC/1000,U_AM/1000,U_UC/1000,U_UM/1000,U_CM/1000))
```

    Table 6: Conductance calculations (kBtu/degF.h)
    
    U_OA U_OU U_OC U_OM U_AU U_AC U_AM U_UC U_UM U_CM
    ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      11    1   42    1    1    5  216    1   24    2



```python
# Table 7: Capacitance calculations
C_A = N * M * beta * V * S_air
C_U = N * M * (1-beta) * V * S_air
C_C = N * M * X * W * Z * S_air
C_M = N * M * (C_int + (1 + X * W) * F * S_floor)
print("Table 7: Capacitance calculations (kBtu/degF)\n\n"
     + "C_A  C_U  C_C  C_M\n"
     + "---- ---- ---- ----\n"
     + "%4.0f %4.0f %4.0f %4.0f"
     % (C_A/1000,C_U/1000,C_C/1000,C_M/1000))
```

    Table 7: Capacitance calculations (kBtu/degF)
    
    C_A  C_U  C_C  C_M
    ---- ---- ---- ----
      29    3    4 8560



```python
# boundary condition
q = matrix([[T_O], [Q_AS+Q_AV+Q_AE], [Q_US+Q_CS], [Q_CV]])
print("Thermal conditions\n\n"
     + "T_O   Q_A  Q_U  Q_C\n"
     + "degF     kBtu/h    \n"
     + "---- ---- ---- ----")
print("%4.0f %4.0f %4.0f %4.0f"
     % (q[0],q[1]/1000,q[2]/1000,q[3]/1000))
```

    Thermal conditions
    
    T_O   Q_A  Q_U  Q_C
    degF     kBtu/h    
    ---- ---- ---- ----
     -20  500   51   -5



```python
from numpy.linalg import *
# compute the solution matrices
A = matrix([ 
      [-(U_OA+U_AU+U_AC+U_AM)/C_A,                   U_AU/C_A,                   U_AC/C_A,                   U_AM/C_A],
      [                  U_AU/C_U, -(U_OU+U_AU+U_UC+U_UM)/C_U,                   U_UC/C_U,                   U_UM/C_U],
      [                  U_AC/C_C,                   U_UC/C_C, -(U_OC+U_AC+U_UC+U_CM)/C_C,                   U_CM/C_C],
      [                  U_AM/C_M,                   U_UM/C_M,                   U_CM/C_M, -(U_OM+U_AM+U_UM+U_CM)/C_M]
    ])
a = 0.9 # fraction of heat gain that goes to zone instead of mass
b = 1-a # fraction of heat gain that is taken up directly by mass
B1 = matrix([ 
       [ U_OA/C_A, a/C_A,     0,     0],
       [ U_OU/C_U,     0, a/C_U,     0],
       [ U_OC/C_C,     0,     0, a/C_C],
       [ U_OM/C_M, b/C_M, b/C_M, b/C_M]
      ])
B2 = matrix([ 
       [ a/C_A,     0,     0,     0],
       [     0, a/C_U,     0,     0],
       [     0,     0, a/C_C,     0],
       [ b/C_M, b/C_M, b/C_M, 1/C_M]
      ])
ndim(B2)
```




    2




```python
T = matrix([[0],[0],[0],[0]])
Ainv = inv(A)
T_eq = -Ainv*(B1*q)
dT = A*T_eq + B1*q
T_set = m*matrix([[0],[T_unoccupied],[T_core],[0]])
print("Temperatures (degF)\n\n"
     + "Zone  dT/dt   Eq   Set \n"
     + "----- ----- ----- -----")
print("%-5.5s %5.1f %5.1f %5.1f" % ('T_A',dT[0],T_eq[0],T_set[0]))
print("%-5.5s %5.1f %5.1f %5.1f" % ('T_U',dT[1],T_eq[1],T_set[1]))
print("%-5.5s %5.1f %5.1f %5.1f" % ('T_C',dT[2],T_eq[2],T_set[2]))
print("%-5.5s %5.1f %5.1f   --" % ('T_M',dT[3],T_eq[3]))
```

    Temperatures (degF)
    
    Zone  dT/dt   Eq   Set 
    ----- ----- ----- -----
    T_A    -0.0   7.9   0.0
    T_U    -0.0   7.8  10.0
    T_C     0.0 -15.8   5.0
    T_M     0.0   7.9   --



```python
# initial temperature
T = matrix([[0], [0], [0], [0]])
# solve for the required control
u_req = -inv(B2) * (A*T_set+B1*q);
# constrain the solution
u = hstack((u_max,hstack((u_req,u_min)).max(axis=1))).min(axis=1)
# compute the temperature changes
dT = A*T + B1*q + B2*u
# compute time to change 0.1 degF
dt = 3600/max(abs(dT))[0,0]
# show me
print("shortfall = %.0f kBtu/h" % (sum(u_req - u)/1000))
print("dTA = %.2f degF/h" % (dT[0,0]))
print("dTU = %.2f degF/h" % (dT[1,0]))
print("dTC = %.2f degF/h" % (dT[2,0]))
print("dTM = %.2f degF/h" % (dT[3,0]))
print("dt = %.0f sec" % (dt))
```

    shortfall = -334 kBtu/h
    dTA = -1.29 degF/h
    dTU = 62.89 degF/h
    dTC = 67.32 degF/h
    dTM = 0.02 degF/h
    dt = 53 sec



```python
# power requirements
if m < 0: eta = e_heat 
else: eta = e_cool
P = m*sum(u)/eta
print("Mode Power\n"
      + "---- -----\n"
      + "%4.0f %5.0f"
      % (m,P/1000))
```

    Mode Power
    ---- -----
       1   371


---
Worksheets...


```python
Teq = -Ainv * (B1*q+B2*u)
Teq
```




    matrix([[17.01131703],
            [24.60200649],
            [ 7.63100952],
            [18.25342894]])




```python
t = 60/3600
Aeig=matrix(diag(exp(eig(A)[0]*t)))
Aeig
```




    matrix([[0.99996405, 0.        , 0.        , 0.        ],
            [0.        , 0.79402059, 0.        , 0.        ],
            [0.        , 0.        , 0.87439357, 0.        ],
            [0.        , 0.        , 0.        , 0.86857764]])




```python
Teq + Aeig*T + Ainv*(Aeig-eye(4))*(B1*q+B2*u)
```




    matrix([[20.19159879],
            [29.31449428],
            [ 8.75852011],
            [21.64060342]])




```python

```
