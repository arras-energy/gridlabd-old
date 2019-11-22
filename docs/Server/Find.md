[[/Server/Find]] -- Server object find query

# Synopsis
HTTP:
~~~
GET /find/<collection>
~~~

# Description

You may obtain collections of objects using the `find` method. The query element `<collection>` is the collection description, e.g., "class=name", "groupid=name".


# Example

The following obtains a list of all object of class `house` in `my-model.glm`:
~~~~
bash$ gridlabd --server my-model.glm &
bash$ curl http://localhost:6267/find/class=house
~~~~

# See also

* [[/GLM/General/Collection]]
* [[/Server/REST API]]
