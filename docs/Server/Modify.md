[[/Server/Modify]] -- Server model modify command

# Synopsis
HTTP:
~~~~
    GET /modify/<object-name>.<property-name>=<value>;...
~~~~

# Description

The server allows direct modification of object properties using the `modify` query, where the semicolon separated tuples `<object-name>.<property-name>=<value>` can be repeated up to the maximum length of an HTTP `GET` request (1024 characters in the current version of GridLAB-D).

The response is simply `HTTP_OK` or `HTTP_FAILURE`.

# See also

* [[/Server/REST API]]
