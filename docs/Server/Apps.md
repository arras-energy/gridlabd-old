[[/Server/Apps]] -- Server request to run applications

# Synopsis
HTTP:
~~~
    GET /control/<appname>/<option>
~~~

# Description

Server-side applications may be installed for a server and called by clients by using the app query.

# Example

The following example run the python script `hello.py`
~~~
  GET /python/hello.py
~~~

The following applications are currently supported if they are installed on the server:

Server applications
~~~
| Application | App Name | Script Extension |
| ----------- | -------- | ---------------- |
| Perl        | perl     | .pl              |
| GnuPlot     | gnuplot  | .plt             |
| Python      | python   | .py              |
| Java        | java     | .jar             |
| R           | r        | .r               |
| Scilab      | scilab   | .cse             |
| Octave      | octave   | .m               |
~~~

In all cases the output is copied to the client.  Typically, it is the HTML code, raw, XML, or JSON data needed to properly dispose of the result.

# See also

* [[/Server/REST API]]
