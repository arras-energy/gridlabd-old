[[/Module/Powerflow/Global/Solver_dump_enable]] -- Flag to enable solver bus/branch dumps on failure

# Synopsis

GLM:
~~~
module powerflow
{
    solver_dump_enable [FALSE|TRUE];
}
~~~

# Description

This flag enables dumps of the bus and branch data used by the Newton-Raphson powerflow solver.  The default value is `FALSE`.

# See also

* [[/Module/Powerflow]]
* [[/Module/Powerflow/Global/Solver_method]]
