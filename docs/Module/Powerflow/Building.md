[[/Module/Powerflow/Building]] - General building load model

# Synopsis

GLM:

~~~
class building {
    parent load;
    // model parameters
    double dt[s]; // timestep to use when modeling building response to inputs
    // state variables
    double TA[degC]; // (OUTPUT) indoor air temperature
    double TM[degC]; // (OUTPUT) building mass temperature
    double M[pu]; // (OUTPUT) system mode per unit system capacity
    // thermal parameters
    double UA[W/K]; // (REQUIRED) conductance from interior air to outdoor air
    double CA[J/K]; // (REQUIRED) heat capacity of indoor air volume
    double UI[W/K]; // (REQUIRED) conductance from building mass to indoor air
    double CM[J/K]; // (REQUIRED) heat capacity of building mass
    double UM[W/K]; // (REQUIRED) conductance of building mass to outdoor air
    // design parameters
    double TH[degC]; // (REQUIRED) heating design temperature
    double TC[degC]; // (REQUIRED) cooling design temperature
    double DF[pu]; // system over-design factor
    double QH[W]; // (REQUIRED,OUTPUT) HVAC system capacity
    double QE[W/unit]; // (REQUIRED) nomimal enduse load capacity
    double QG[W/unit]; // (REQUIRED) natural gas heat per unit nominal enduse capacity
    double QO[W/unit]; // (REQUIRED) heat gain per occupant
    double QV[W/unit]; // (REQUIRED) ventilation gain per occupant
    double SA[m^2]; // (REQUIRED) building mass area exposed to solar radiation
    // control parameters
    double K[pu]; // HVAC mode proportional control gain w.r.t indoor temperature
    // input parameters
    double TO[degC]; // outdoor air temperature
    double EU[unit]; // enduse load fraction
    double NG[unit]; // natural gas demand
    double NH[unit]; // building occupants
    double QS[W/m^2]; // insolation
    double TS[degC]; // (REQUIRED) thermostat setpoint
    // output parameteres
    double PZM[pu]; // (OUTPUT) constant impedance HVAC real power per unit nominal capacity
    double PPM[pu]; // (OUTPUT) constant power HVAC real power per unit nominal capacity
    double QPM[pu]; // (OUTPUT) constant power HVAC reactive power per unit nominal capacity
    double PZE[W/unit]; // (OUTPUT) constant impedance end-use real power nominal capacity
    double PIE[W/unit]; // (OUTPUT) constant current end-use real power nominal capacity
    double PPE[W/unit]; // (OUTPUT) constant power end-use real power nominal capacity
    double QZE[VAr/unit]; // (OUTPUT) constant impedance end-use reactive power nominal capacity
    double QIE[VAr/unit]; // (OUTPUT) constant current end-use reactive power nominal capacity
    double QPE[VAr/unit]; // (OUTPUT) constant power end-use reactive power nominal capacity
    double PPH[pu]; // (OUTPUT) constant power ventilation real power per unit nominal capacity
    double QPH[pu]; // (OUTPUT) constant power ventilation reactive power per unit nominal capacity
    // metering parameters
    double measured_real_power[W]; // (OUTPUT) metered real power demand
    double measured_reactive_power[VAr]; // (OUTPUT) metered reactive power demand
    double measured_real_energy[Wh]; // (OUTPUT) cumulative metered real energy consumption
    double measured_real_energy_delta[Wh]; // (OUTPUT) cumulative metered real energy interval consumption
    double measured_reactive_energy[Wh]; // (OUTPUT) cumulative metered reactive energy consumption
    double measured_reactive_energy_delta[Wh]; // (OUTPUT) cumulative metered reactive energy interval consumption
    double measured_energy_delta_timestep[s]; // (OUTPUT) energy metering interval
    double measured_demand[W]; // (OUTPUT) maximum metered real power interval demand
    double measured_demand_timestep[s]; // (OUTPUT) maximum power metering interval
}
~~~

# Description

The `building` object implements a general building load model as transfer
functions from each input to the each of the output channels.  The model is
based on the pair of ordinary differential equations
$$
    C_A \dot T_A = U_A ( T_O - T_A ) + U_I ( T_M - T_A ) + Q_A \qquad (1)
$$
$$
    C_M \dot T_M = U_I (T_A - T_M ) + U_M ( T_O - T_M ) + Q_M \qquad (2)
$$
where
* $C_A$ is the heat capacity of the air volume;
* $C_M$ is the heat capacity of the building mass;
* $U_A$ is the conductance between the indoor to outdoor air;
* $U_I$ is the conductance between the indoor air and the building mass; and
* $U_M$ is the conductance between the building mass and the outdoor air.

The heat gains to the air come from the following sources:
* $Q_E E$, the internal enduse electric loads, where $Q_E$ is installed capacity of electric end-uses and $E$ is the fraction of electric end-uses active;
* $Q_G G$, the internal enduse gas loads, where $Q_G$ is the heat retained in the building for installed capacity of gas enduses and $G$ is the fraction of gas end-uses active;
* $Q_O H$, the occupancy heat gains where $Q_O$ is the heat gain for a single person, and $H$ is the number of people present;
* $Q_V H$, the ventilation heat gains, where $Q_V$ is the heat gain for fresh air per person; and
* $M Q_H$, the HVAC heat gains, where $M$ is the system mode, and $Q_H$ is the nameplate system capacity.
Thus
$$
    Q_A = E Q_E + G Q_G + H (Q_O+Q_V) + M Q_H
$$

The heat gains to the mass from the solar gains to building envelope and mass through windows, such that $Q_M = S_A Q_S$, where $S_A$ is the solar exposure area of the building mass, and $Q_S$ is the total insolation.

The HVAC system mode is included as a state variable such that the mode $M$ represents the state of the HVAC system given the indoor air temperature setpoint $T_S$ deviation from $T_A$ with a proportional gain $K$, where $M>0$ for heating, and $M<0$ for cooling. 

The state space model is thus
$$
    \left[ \begin{matrix} \dot T_A \\ \dot T_M \\ \dot M \end{matrix} \right]
    =
    \left[ \begin{matrix} 
        -{U_A+U_I \over C_A} & {U_I \over C_A} & {Q_H \over C_A}
    \\
        {U_I \over C_M} & -{U_M+U_I \over C_M} & 0
    \\
        K & 0 & 0
    \end{matrix} \right]
    \left[ \begin{matrix} T_A \\ T_M \\ M \end{matrix} \right]
    +
    \left[ \begin{matrix} 
        { U_A \over C_A } & {Q_E \over C_A} & {Q_G \over C_A} & {Q_O+Q_V \over C_A} & 0 & 0
    \\
        { U_M \over C_M } & 0 & 0 & 0 & {S_A \over C_M} & 0
    \\
        0 & 0 & 0 & 0 & 0 & -K
    \end{matrix} \right]
    \left[ \begin{matrix} T_O \\ E \\ G \\ H \\ Q_S \\ T_S \end{matrix} \right]
    \qquad(3)
$$

## Sizing

If the system size is not given, then $Q_H$ must be calculated for the design conditions.  The heating system capacity is found by solving Eq. (3) at steady-state for $Q_H$ given the $T_O=T_H$, $T_A=T_S$, $E=G=H=Q_S=0$, and $M=D_F$, the system over-design factor. This gives us the system
$$
    \left[ \begin{matrix} 0 \\ 0 \end{matrix} \right]
    =
    \left[ \begin{matrix} 
        {U_I \over C_A} & {D_F \over C_A} \\
        -{U_M+U_I \over C_M} & 0
    \end{matrix} \right]
    \left[ \begin{matrix} T_M \\ Q_H \end{matrix} \right]
    +
    \left[ \begin{matrix} 
        {U_A \over C_A} & -{U_A+U_I \over C_A} \\
        {U_M \over C_M} & {U_I \over C_M}
    \end{matrix} \right]
    \left[ \begin{matrix} T_H \\ T_S \end{matrix} \right]
$$

The same method is used for cooling but with all internal gains on and maximum solar gain.
$$
    \left[ \begin{matrix} 0 \\ 0 \end{matrix} \right]
    =
    \left[ \begin{matrix} 
        {U_I \over C_A} & {D_F \over C_A} \\
        -{U_M+U_I \over C_M} & 0
    \end{matrix} \right]
    \left[ \begin{matrix} T_M \\ Q_H \end{matrix} \right]
    +
    \left[ \begin{matrix} 
        {U_A \over C_A} & -{U_A+U_I \over C_A} & 0 \\
        {U_M \over C_M} & {U_I \over C_M} & { QE+QG+QO+QV+1300S_A \over CA }
    \end{matrix} \right]
    \left[ \begin{matrix} T_H \\ T_S \\ 1 \end{matrix} \right]
$$

## Inputs

Weather inputs are the outdoor air temperature $T_O$ and insolation $Q_S$. The indoor temperature setpoint $T_S$ is also an input. Additionally, the fractional electric end-use load $E$, gas end-use load $G$, and occupant load $H$ are inputs per unit of nominal installed electric load, gas load, and occupancy, respectively.

## Outputs

The real and reactive ZIP components, $\mathbf P$ and $\mathbf Q$, respectively, are obtained from the enduse loads $Q_E$ and the state of the HVAC system $M$, including the system efficiency $\eta$, and the real and reactive power load compositions:
$$
    \left[ \begin{matrix} P_Z \\ P_I \\ P_P \\ Q_Z \\ Q_I \\ Q_P \end{matrix} \right]
    =
    \left[ \begin{matrix}
        0 & 0 & P_{Z_M} 
    \\
        0 & 0 & 0
    \\
        0 & 0 & P_{P_M} 
    \\
        0 & 0 & 0
    \\
        0 & 0 & 0 
    \\
        0 & 0 & Q_{P_M}
    \end{matrix} \right] 
    \left[ \begin{matrix} T_A \\ T_M \\ M \end{matrix} \right]
    +
    \left[ \begin{matrix}
        0 & P_{Z_E} & 0 & 0 & 0 & 0 
    \\
        0 & P_{I_E} & 0 & 0 & 0 & 0 
    \\
        0 & P_{P_E} & 0 & P_{P_H} & 0 & 0 
    \\
        0 & Q_{Z_E} & 0 & 0 & 0 & 0 
    \\
        0 & Q_{I_E} & 0 & 0 & 0 & 0 
    \\
        0 & Q_{P_E} & 0 & Q_{P_H} & 0 & 0 
    \end{matrix} \right] 
    \left[ \begin{matrix} T_O \\ E \\ G \\ H \\ Q_S \\ T_S \end{matrix} \right]
$$

# Example

~~~
object building
{
    phases ABC;
    nominal_voltage 480;
    dt 1h;

    // thermal parameters
    UA 300 W/K;
    CA 2e6 J/K;
    UI 6000 W/K;
    CM 8e6 J/K;
    UM 600 W/K;

    // design parameters
    TH -20 degC;
    TC 40 degC;
    QH 50000.0 W; // 0 to autosize
    QE 10000 W/unit;
    QG 1000 W/unit;
    QO 1200 W/unit;
    QV 400 W/unit;
    SA 10 m^2;

    // control parameters
    K 1.0;

    // inputs
    TO -20 degC;
    EU 0.1 unit;
    NG 0.1 unit;
    NH 2 unit;
    QS 1000 W/m^2;
    TS 20 degC;

    // outputs
    PZM 0 pu; // constant impedance HVAC real power (pu.W)
    PPM 0.3 pu; // constant power HVAC real power (pu.W)
    QPM 0.03 pu; // constant power HVAC reactive power (pu.VAr)
    PZE 500 W/unit; // constant impedance end-use real power (W)
    PIE 0 W/unit; // constant current end-use real power (W)
    PPE 500 W/unit; // constant power end-use real power (W)
    QZE 50 VAr/unit; // constant impedance end-use reactive power (VAr)
    QIE 0 VAr/unit; // constant current end-use reactive power (VAr)
    QPE 50 VAr/unit; // constant power end-use reactive power (VAr)
    PPH 0.06 pu; // constant power ventilation real power (pu.W/person)
    QPH 0.01 pu; // constant power ventilation reactive power (pu.VAr/person)
};
~~~

# See also

* [[/Module/Powerflow/Load]]
