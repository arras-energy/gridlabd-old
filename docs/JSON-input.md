[[JSON Input]] -- JSON input format

# Synopsis
~~~
  bash$ gridlabd filename.json
~~~

# Remarks
The loader does not directly support JSON input at this time. Instead a runtime converter is used, e.g.,
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

# See also
* [[JSON output]]
