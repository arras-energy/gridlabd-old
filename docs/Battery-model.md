The internal battery model in the generators module works as follows.

When the battery load is negative and the battery is not empty, then the battery load is 
$$
  P = - P_{max}.
$$
For Lithium-ion batteries, when $SOC > 0.1$ the open-circuit voltage is
$$
  V_{oc} = N_{series} \times ({4.1-3.6 \over 0.9} \times SOC + (4.1-{4.1-3.6 \over 0.9}))
$$
and when $SOC <= 0.1$
$$
  V_{oc} = N_{series} \times ({{3.6-3.2} \over {0.1}} \times SOC + 3.2)
$$

For all other battery types, *V_oc* = *V_max*.

The internal resistance is 

*R_in* = *V_oc*^2 x | *P_br* - *P_max* | / *P_br*^2

where 

*P_br* = *P_max* x *eta_rt*^0.5

The voltage at the terminals is

*V_t* = 0.5 x (*V_oc*^2 + (*V_oc*^2 + (4 x *P* x *R_in*))^0.5

The internal battery load is 

*P_in* = *V_oc* x *P* / *V_t*

The time to the next battery state change when discharging is

*t_next* = (*SOC_r*-*SOC*) x *E_max* x 3600 / *P_in*

where *SOC_r* is the desired SOC reserve. When charging the next time is

*t_next* = (1-*SOC*) x *E_max* x 3600 / *P_in*

with *E_max* defined as the battery's internal energy storage capacity.
