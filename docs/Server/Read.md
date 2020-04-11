[[/Server/Read]] -- Server read command

# Synopsis

HTTP:

~~~
    GET /read/<object-name>.<property-name>;...
~~~

# Description

The query string is a list of object names and property names, where the semicolon separated tuples `<object-name>.<property-name>` may be repeated for multiple properties up to the length of a HTTP GET method request (which is  1024 characters in GridLAB-D).

The return value is a JSON list of the form

~~~~
[
    {   
        "object" : "<object-name>", 
        "property" : "<property-name>", 
        "value" : "<property-value>"
    },
    ...
]
~~~~

with one tuple for each entry of the GET request. 

# Example

The following server command is supported in this version of GridLAB-D:

~~~~
bash$ curl http://<hostname>:<portnum>/read/<object-name>.<property-name>;...
~~~~

# See also

* [[/Server/REST API]]
