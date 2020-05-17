[[/GLM/Macro/Input]] -- Macro to input a non-GLM file

# Synopsis

GLM:

~~~~
#input "filename.ext" [options ...]
~~~~

# Description

The `#input` macro allows a non-GLM file to be input inline using [[/GLM/Command/Automatic import conversion.md]] with options, e.g., 

# Example

The following example converts the file `data.csv` to `house.glm` using the converter `csv-ami2glm-house.py` with the `heating_setpoint` property set to `72 degF`:

~~~~
module powerflow;
object meter {
    name main;
    bustype SWING;
    nominal_voltage 120.0;
    phases ABCN;
#input "data.csv" -f ami -t house heating_setpoint=72degF
}
~~~~

# See also

* [[/Command/Automatic import conversion.md]]
* [[/GLM/Macro/Include]]
