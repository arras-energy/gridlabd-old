[[/Command/Automatic export conversion]] -- Automatic conversion of output files 

# Synopsis

~~~
bash$ gridlabd <options> -o <filename>.<ext>
~~~

# Description

The GridLAB-D GLM save routines will automatically save output to files that have "known" extensions.  A known extension is one for which a Python converter exists in GLPATH. Python converters use the naming convention `json2<ext>.py` and are normally stored in the GridLAB-D `shared` folder.

## File save options

The data saved in the file may be controlled using [[/Global/Filesave_options]].

## Supported File Formats


### CSV

CSV files are exported using the `-o filename.csv -D csv_save_options='-t type'` command line option, or using the `#output "filename.csv" -t type` macro.

The only supported type at this time is `profile`, which generates a voltage profile for a each swing bus in a model.

### ZIP

ZIP files are exported using the `-o filename.zip` command line option.  When output to ZIP is used, all the files in the folder in which contains the JSON output file will be stored in the resulting ZIP file. If the ZIP file is already present, it will be overwritten.

## Converter Implementation

Converters use the following calling syntax:

~~~
bash$ python3 json2<ext>.py -i filename.glm -o filename.<ext>
~~~

where `<ext>` is the extension that is supported.

# Examples

Run `my-model.glm` and store all files in `my-model.zip`

~~~
bash$ gridlabd my-model.glm -o my-model.zip
~~~

# Caveats

When a converter is run, the output file is stored in JSON file by the same name, but with the `.json` extension instead.  If the JSON file already exists, **the existing JSON file may be overwritten**.

# See also

* [[/Command/JSON input]]
* [[/Command/JSON output]]
