[[/Command/Automatic export conversion]] -- Automatic conversion of output files 

# Synopsis

~~~
bash$ gridlabd <options> -o <filename>.<ext>
~~~

# Description

The GridLAB-D GLM save routines will automatically save output to files that have "known" extensions.  A known extension is one for which a Python converter exists in GLPATH. Python converters use the naming convention `json2<ext>.py` and are normally stored in the GridLAB-D `shared` folder.

## File save options

The data saved in the file may be controlled using [[/Global/Filesave_options]].

## Converter Implementation

Converters use the following calling syntax:

~~~
bash$ python3 json2<ext>.py -i filename.glm -o filename.<ext>
~~~

where `<ext>` is the extension that is supported.

# Caveats

When a converter is run, the output file is stored in JSON file by the same name, but with the `.json` extension instead.  If the JSON file already exists, **the existing JSON file may be overwritten**.

# See also

* [[/Command/JSON input]]
* [[/Command/JSON output]]
