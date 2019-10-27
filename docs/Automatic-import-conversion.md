[[Automatic import conversion]] -- Automatic conversion of input files that have non-GLM extensions

# Synopsis
~~~
host% gridlabd <filename>.<ext>
~~~
# Description

The GridLAB-D GLM loader will automatically convert to GLM format all input files that have known extensions.  A "known" extension is one for which a Python converter exists in GLPATH. Python converters use the naming convention `<ext>2glm.py` and are normally stored in the `rt` folder in the GridLAB-D `shared` folder.

When the converter is run, the file will be converted in place to a GLM file by the same name, but with the `.glm` extension instead.  If the GLM file already exists, the old GLM file may be overwritten.

## Converter Development

Converters use the following calling syntax:
~~~
host% python3 ext2glm.py -i filename.ext -o filename.glm
~~~
# See also

* [[JSON output]]
