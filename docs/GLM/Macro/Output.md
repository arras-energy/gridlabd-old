[[/GLM/Macro/Output]] -- Setup output post-processing

# Synopsis

GLM:

~~~
#output "filename.ext" [options ...]
~~~

# Description

The `#output` macro sets up post-processing when a simulation is completed.  In general, output processing work similarly to input processing insofar as conversion to various file formats are automatically performed using the [[/Command/Automatic export conversion]] subsystem.  Any `options` provided are simply passed through to the file conversion routine.

# Example

The following example generates the default tag image for a GridLAB-D model:

~~~
#output "test_output_save.png"
~~~

# See also

* [[/GLM/Macro/Input]]
* [[/Command/Automatic export conversion]]
