[[/Module/Powerflow/Python_solver]] -- Python solver configuration

# Synopsis

`solver_py.conf`:

~~~
logfile <filename>
logfile <integer>
solver [enable|disable]
import_path <folder>
import <python-module-name>
busdump <filename>
branchdump <filename>
on_dump <method-name>
busdata <taglist>
branchdata <taglist>
learndata <taglist>
profiler <csv-filename>
option <name>=<value>
~~~

# Description

The Newton-Raphson (NR) powerflow solver can be configured to share solution data with a python module. The configuration file `solver_py.conf` may be changed in the shared folder, or place in the current folder, or added to folder specified in the configuration file.  When solver shares data with a Python module is uses
two methods.

The standard configuration parameters are documented in `/usr/local/share/gridlabd/solver_py.conf` where the default solver configuration is stored.

The `option` parameter may be added one or more times, and each `<name>=<value` tuple will be passed to `solve()` and `learn()` in `kwargs['options']` dictionary. These can be used to set custom options to a user-defined implementation of the python solver.  

## `solve(gridlabd,**kwargs)`

The `solve` method is called before a NR solution is attempted.  The bus and branch data are copied into `kwargs` and if a solution is found, a non-negative value should be returned.  If a negative value is returned, the NR solver is run to find the powerflow solution.

The `bustags` and `branchtags` may be used to identify the columns in `busdata` and `branchdata`.  The rows in `busdata` corresponding to bus ids, and the rows in `branchdata` correspond to branch ids.

## `learn(gridlabd,**kwargs)`

When an NR solution is found, the `learn` method is called.  The bus and branch data are are copied into `kwargs`, along with data about the solution.  If no `learndata` is specified, the only information given is the number of iterations and the type of solution (0 for a static powerflow solution, 1 for initial dynamic solution, and 2 for updated dynamic solution).

Additional learn data available includes the following

- `powerflow_values` - copies all the NR powerflow solution values. Specific member may be selected by using `powerflow.<member>` where `<member>` is one of the following
  - `deltaI_NR` 
  - `BA_diag`
  - `Y_offdiag_PQ`
  - `Y_diag_fixed`
  - `Y_diag_update`
  - `Y_Amatrix`

Please see `module/powerflow/solver_nr.h` in the code for details on the meaning and structure of this data.

# Profiler

When the `profiler` is specified in the configuration file, the solver records the elapsed time (in seconds) for each phase of the solution at each timestep in the simulation.

# Caveat

The standard output and error streams are not automatically redirected to GridLAB-D.  To redirect the standard output and error streams, you must explicitly change them *each time a method is called*.  For example:

~~~
def solve(gridlabd,**kwargs):
    print("solve called")
~~~

sends the message to `sys.stdout` regardless of whether GridLAB-D has redirected that output stream to a file.  In constrast:

~~~
def solver(gridlabd,**kwargs):
    sys.stdout = output_stream
    print("solve called")
~~~

sends the message to GridLAB-D's output stream, which would capture the output in a file if redirected by the GridLAB-D.
