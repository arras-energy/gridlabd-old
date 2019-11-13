The internal battery model in the generators module works as follows.

When the battery load is negative and the battery is not empty, then the battery load is 
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
  V_t = 0.5 \times ( V_{oc}^2 + (V_{oc}^2 + (4 \times P \times R_{in}))^{0.5}
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
