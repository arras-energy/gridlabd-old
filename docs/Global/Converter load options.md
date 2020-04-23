[[/Global/Converter load options]] -- Options for automatic file conversion modules

# Synopsis
GLM:
~~~
  #set <ext>_load_options=<options>
~~~
Command line:
~~~
  host% gridlabd -D <ext>_load_options=<options>
~~~

# Description

Automatic converters support extension using the converter naming convention `<input-extension>2<output-extension>.py`.  When the global variable `<output-extension>_load_options` is defined, the entire contents is passed to the converter whenever it is called.

# Example

The following example passes the command line option `-t cyme` to the `txt2glm.py` converter before the model is loaded.
~~~
  host% gridlabd -D png_load_options="-t cyme -o mymodel.glm" equipment.txt,load.txt,network.txt
~~~

# See also

* [[/Global/Converter save options]]
