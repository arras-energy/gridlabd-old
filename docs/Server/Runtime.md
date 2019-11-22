[[/Control/Runtime]] -- Server query to download runtime files and data

# Synopsis
HTTP:
~~~
    GET /rb/<filename>
    GET /rt/<filename>[:<offset>]
~~~

# Description

Some client support functions require runtime files and data that are provided by the server. To read a runtime library use the query. These files may be downloaded as either binary images or text files, with parameter subsitution.

## Text files

Text files are downloaded using text access, i.e., `\n` is converted to `\r\n` is necessary. 

Global variable names surrounded by '<<<' '>>>' will be substituted on the server-side by the current value of the global.

## Binary files

Binary files are download using binary file access. No changes are made to the file when it is downloaded. 

If the `<offset>` is specified, the download will start at the byte offset specified. This can be used to obtain to resume of an incomplete download.

# Example
The following example downloads the javacsript file `gridlabd.js`:
~~~
  host% curl http://localhost:6267/rt/gridlabd.js
~~~

# See also

* [[/Control/REST API]]
