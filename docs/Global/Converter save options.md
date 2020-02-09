[[/Global/Converter save options]] -- Options to automatic file conversion modules

# Synopsis

GLM:

~~~
  #set <ext>_save_options=<options>
~~~
Command line:
~~~
bash$ gridlabd -D <ext>_save_options=<options>
~~~

# Description

Automatic converters support extension using the converter naming convention `<input-extension>2<output-extension>.py`.  When the global variable `<output-extension>_save_options` is defined, the entire contents is passed to the converter whenever it is called.

# Example

The following example passes the command line option `-t profile` to the `json2png.py` converter after the simulation is completed.
~~~
bash$ gridlabd model.glm -o output.png -D png_save_options="-t profile"
~~~

# See also

* [[/Command/JSON Input]]
* [[/Command/JSON Output]]
