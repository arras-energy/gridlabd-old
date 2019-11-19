[[Global/Glm_save_options]] -- Global variable to control when is included when a GLM file is output

# Synopsis

Command line usage:
~~~
  host% gridlabd [--define|-D] glm_save_options=LEGACY ...
  host% gridlabd [--define|-D] glm_save_options=MINIMAL ...
  host% gridlabd [--define|-D] glm_save_options=[NODEFAULTS|NOINTERNALS|NOGLOBALS|NOMACROS] ...
~~~
GLM usage:
~~~
  #set glm_save_options=LEGACY
  #set glm_save_options=MINIMAL
  #set glm_save_options=[NODEFAULTS|NOINTERNALS|NOGLOBALS|NOMACROS]
~~~

# Description

The `glm_save_options` global is used to control the contents of GLM output files.

## LEGACY

This causes GridLAB-D to output the GLM files using the legacy structure from older versions of the GridLAB-D.  This is current the default, but will be deprecated in favor of MINIMAL in a future version.

## NOINTERNALS

This causes the GLM to omit information only useful to the internal operations of GridLAB-D.  Some header information is omitted. Objects without proper names are given pseudonyms that conform to the internal object naming convention, i.e., `class:id`.

## NOGLOBALS

This causes the GLM file to omit all global variable values.

## NOMACROS

This causes the GLM file to avoid use of the macros.

## NODEFAULTS

This causes the GLM file to avoid use default values.

## MINIMAL

This is the minimum loadable GLM model. It is a combination of `NOINTERNALS`, `NOGLOBALS`, `NOMACROS`, and `NODEFAULTS`. When `LEGACY` is deprecated, it will become the default.

# Example
~~~
  host% gridlabd -D gld_save_options=MINIMAL model.glm -o model_single_file.glm
~~~

# See also

* [[Global/Json_save_options]]

