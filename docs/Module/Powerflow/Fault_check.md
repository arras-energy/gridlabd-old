[[/Module/Powerflow/Fault_check]] -- Class fault_check

# Synopsis

GLM:

~~~
  object fault_check {
    check_mode "{ALL,ONCHANGE,SINGLE}";
    output_filename "<string>";
    reliability_mode "<string>";
    strictly_radial "<string>";
    full_output_file "<string>";
    grid_association "<string>";
    eventgen_object "<string>";
  }
~~~

# Description

TODO

## Properties

### `check_mode`
~~~
  enumeration {ALL, ONCHANGE, SINGLE} check_mode;
~~~

Frequency of fault checks

### `output_filename`
~~~
  char1024 output_filename;
~~~

Output filename for list of unsupported nodes

### `reliability_mode`
~~~
  bool reliability_mode;
~~~

General flag indicating if fault_check is operating under faulting or restoration mode -- reliability set this

### `strictly_radial`
~~~
  bool strictly_radial;
~~~

Flag to indicate if a system is known to be strictly radial -- uses radial assumptions for reliability alterations

### `full_output_file`
~~~
  bool full_output_file;
~~~

Flag to indicate if the output_filename report contains both supported and unsupported nodes -- if false, just does unsupported

### `grid_association`
~~~
  bool grid_association;
~~~

Flag to indicate if multiple, distinct grids are allowed in a GLM, or if anything not attached to the master swing is removed

### `eventgen_object`
~~~
  object eventgen_object;
~~~

Link to generic eventgen object to handle unexpected faults

# Example

~~~
  object fault_check {
    check_mode "0";
    output_filename "";
    reliability_mode "FALSE";
    strictly_radial "FALSE";
    full_output_file "FALSE";
    grid_association "FALSE";
  }
~~~

# See also
* [[/Module/Powerflow]]

