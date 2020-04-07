[[/Command/Automatic import conversion]] -- Automatic conversion of input files 

# Synopsis

~~~
bash$ gridlabd <filename>.<ext>
~~~

# Description

The GridLAB-D GLM loader will automatically convert to GLM format all input files that have "known" extensions.  A known extension is one for which a Python converter exists in GLPATH. Python converters use the naming convention `<ext>2glm.py` and are normally stored in the GridLAB-D `shared` folder.

## Support File Formats

### ZIP

When a ZIP file is specified as an input file, the contents of the archive are extracted into the same folder as the ZIP file, and the corresponding GLM file is run. If the GLM file is already present, it will be overwritten.

## Converter Development

Converters use the following calling syntax:

~~~
bash$ python3 <ext>2glm.py -i filename.<ext> -o filename.glm
~~~

where `<ext>` is the extension that is supported.

# Caveats

When a converter is run, the input file is converted in stored in a GLM file by the same name, but with the `.glm` extension instead.  If the GLM file already exists, **the existing GLM file may be overwritten**.

# See also

* [[/Command/JSON input]]
* [[/Command/JSON output]]
