[[/GLM/Macro/Input]] -- Macro to input a non-GLM file

# Synopsis

GLM:

~~~~
#input "filename.ext" [options ...]
~~~~

# Description

The `#input` macro allows a non-GLM file to be input inline using [[/GLM/Command/Automatic import conversion.md]] with options.

~~~~
#input "my-file.csv" -f from-option -t to-option
~~~~

# See also

* [[/GLM/Command/Automatic import conversion.md]]
* [[/GLM/Macro/Include]]
