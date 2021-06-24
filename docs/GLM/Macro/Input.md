[[/GLM/Macro/Input]] -- Macro to input a non-GLM file

# Synopsis

GLM:

~~~~
#input "filename.ext" [options ...]
~~~~

# Description

The `#input` macro allows a non-GLM file to be input inline using [[/Command/Automatic import conversion]] with options.

## CSV converters 
Input: `csv`

Output: `glm`

From: `ami`, `scada`, `onpoint-weather`, `table`

Type: `ceus`, `rbsa`, `climate`, `object`

# Example 1

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

* [[/Command/Automatic import conversion]]
* [[/Converters/Import/Csv_files]]
* [[/GLM/Macro/Include]]
