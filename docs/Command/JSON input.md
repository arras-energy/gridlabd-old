[[/Command/JSON Input]] -- Command option for JSON input format

# Synopsis

~~~
bash$ gridlabd filename.json
~~~

# Description

The loader does not directly support JSON input at this time. Instead an runtime converter is used, which executes the following command automatically when needed:

~~~
bash$ python3 gridlabd_src/python/jsonglm/json2glm.py -i input.json -o output.glm
~~~

The converter is automatically called when a `.json` file is specified on the input, e.g.,

~~~
  bash% gridlabd filename.json
~~~

results in internal execution of command

~~~
  python3 ${GLD_ETC}/json2glm.py -i filename.name -o filename.glm
~~~

before `filename.glm` is loaded.

# Caveat

The converter generates a GLM file with the same basename. Be careful not to accidentially overwrite an existing GLM file implicitly when using this option.  In particularly, the following can be result in very unexpected behavior:

~~~
bash$ gridlabd -C filename.glm -o filename.json
bash$ python3 some-script-that-modifies-filename-json.py
bash$ gridlabd filename.json -o results.json
~~~

because the last command will overwrite the original input file with a GLM file modified by the python3 script run before it.

# See also

* [[/Command/JSON output]]
* [[/Global/json_save_options]]
