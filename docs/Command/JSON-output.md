[[JSON output]] -- JSON output format

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

# See also
* [[JSON input]]
