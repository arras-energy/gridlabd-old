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
~~~

# Description

The Newton-Raphson (NR) powerflow solver can be configured to share solution data
with a python module. The configuration file `solver_py.conf` may be changed in the shared folder, or place in the current folder, or added to folder specified in the configuration file.  When solver shares data with a Python module is uses
two methods.

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

Please see `gldcore/powerflow/solver_nr.h` in the code for details on the meaning and structure of this data.
