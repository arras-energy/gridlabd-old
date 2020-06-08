[[/Converters/README.md]] -- Import and export converters

# Synopsis

GLM:

~~~
#input "<basename>.<ext>" [<options> ...]
#output "<basename>.<ext>" [<options> ...]
~~~

Shell:

~~~
bash$ gridlabd [-D <ext>_load_options='<options> ...'] [-I] [-C] <basename>.<ext>
bash$ gridlabd -o <basename>.<ext> [-D <ext>_save_options='<options> ...']
~~~

# Description

HiPAS GridLAB-D automatically converts input and output files based on their extension and options provided, if any. In general each input and output extension has specific requirement for options to define how the file must be converted. Please see the documentation for the individual converters for details.

# See also

* [[/Converters/Input/Noaa_weather]]
* [[/Converters/Input/Onpoint_weather]]
* [[/Converters/Input/Visualcrossing_weather]]
