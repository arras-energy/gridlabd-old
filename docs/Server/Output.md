[[/Control/Output]] -- Server request to download simulation output files

# Synopsis
HTTP:
~~~
    GET /output/<filename.ext>
~~~

# Description

Output files may be requested using the `output` query. Only files in the working directory or subdirectories thereof may be accessed. The response header will specify the file type.

# Example

The following example downloads a CSV file produced by a recorder:
~~~
    bash$ curl http://localhost:6267/output/my_recorder_file.csv
~~~

# See also

* [[/Control/REST API]]
