[[/Use cases/Hosting capacity analysis]] - Hosting capacity analysis support

The `powerflow` module implements support for hosting capacity analysis in all objects derives from `link` and `node` classes.  

# Limit violations

When limit violations are detected, they are flagged by setting the `violation_active` property of the object whose limits are violated. The `violation_count` variable is also incremented.

## Current violations

Current limit violations are detected on all links when the `violation_rating[A]` is exceeded by the `current_in_[ABC]` on the link.  Current limits are only detected when the `violation_rating[A]` is positive. If the `violation_rating[A]` property of an object is zero, then the value of the module global `default_violation_rating[A]` is used. By default, this value is zero, which disables current violation detection.

## Voltage violations

Voltage limit violations are detected on all nodes when the `voltage_violation_threshold[pu]` is exceeded by the node voltages on any phases relative to the nominal voltage.  If the `voltage_violation_threshold[pu]` property is zero, then the value of the module global `default_voltage_violation_threshold[pu]` is used.  By default, this value is `0.05`, which enables voltage violation detection for all objects. Setting the global value to zero disables voltage violation checks.

If either the `undervoltage_violation_threshold[pu]` or `overvoltage_violation_threshold[pu]` are set and `undervoltage_violation_threshold[pu]` < `overvoltage_violation_threshold[pu]`, then these values are used instead.  Similarly, the corresponding global values are used if these values are zero. The default `undervoltage_violation_threshold[pu]` and `overvoltage_violation_threshold[pu]` values are zero, which disables them.

## Voltage fluctuations

Voltage fluctuations are detected on all nodes when both `voltage_fluctuation_threshold[pu]` and `DER_value[W]` are non-zero.  The solver compares the voltage of each phase of each object in the network with the DER on and off. If the difference in voltage exceeds `voltage_fluctuation_threshold[pu]`, then the violation is detected.  The default value of `voltage_fluctuation_threshold[pu]` is given by `default_voltage_fluctuation_threshold[pu]`, which is zero by default.

# Violation recording

When limit violations are enables they are recorded in the filename specified by the `powerflow` module global `violation_record`.  If this file is not specified, violations are not recorded when they are detected.

# Exhaustive violation checks

Normally, the voltage fluctuation tests stop checking if any violation is found, and no further tests are conducted.  If the module global `DER_violation_tests` is set to `ALL`, then all violations will be reported.
