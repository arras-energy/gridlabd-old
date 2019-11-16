[[Modify]] -- Server model modify command

The following server command is supported in this version:
~~~~
host% curl http://<hostname>:<portnum>/modify/<object-name>.<property-name>=<value>;...
~~~~
where the semicolon separated tuple `<object-name>.<property-name>=<value>` can be repeated up to the maximum length of an HTTP GET request (1024 characters in the current version of GridLAB-D).

The response is simply HTTP_OK or HTTP_FAILURE.
