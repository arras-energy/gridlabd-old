[[/Command/JSON output]] -- JSON output format

# Synopsis
Command line:
~~~
  bash$ gridlabd [options] input.glm -o output.json
  bash$ curl http://localhost:6267/json/output.json
~~~
GLM:
~~~
  #set savefile=filename.json
  #set dumpfile=filename.json
~~~

# Description

The file extension `.json` is supported for output. The `-o filename.json` command line option and the server `/json/filename.json` HTTP query are supported. In addition, the save-file and dump-file may be specified using a `.json` extension.

# Caveats

Some advanced GLM features are not supported in JSON. For example, it is not possible to define implicit multi-object definitions using JSON as in GLM. In general, GLM *features* that are implicity often have to be implemented explicitly in JSON. That can lead to changes in the model behavior when performing "round-robin" conversions and the output GLM may not behave exactly the same as the input GLM.

# See also
* [[/Command/JSON input]]
* [[/Global/json_save_options]]
