[[/Server/REST API]] -- GridLAB-D REST API

# Synopsis

Shell:

~~~
bash$ gridlabd --server <options> ...
~~~

GLM:

~~~
  #option server
  #set server_portnum=6267
~~~

HTTP:

~~~
  GET /<command>[/<options> ...]
~~~

# Description

The `--server` option enable the REST API while a simulation is running.  The REST API is used to access object properties, global variables, and many other parts of the simulation environment.

## Startup

To start GridLAB-D in `server` mode, simply include the command line argument `--server`, e.g.,

~~~
bash$ gridlabd --server modelname.glm
~~~

Typical the server is started on port 6267 of the localhost, with open access to the world.  These parameters of the server may be changed using the following server configuration globals:

* [[/Global/hostname]]
* [[/Global/server_portnum]]
* [[/Global/client_allowed]]

## Globals

To get the value of a global variable, use the following HTTP query

~~~
  GET /<format>/<variable-name>
~~~

where the method can be `raw`, or `json`, or `xml`. 

## Objects

To get the value of an object property, use the following HTTP query

~~~
  GET /<format>/<object-name>/<property-name>
~~~

For double and complex values, you may specify the unit, e.g.,

~~~
  GET /<format>/<object-name>/<property-name>[<unit>]
~~~

The `<unit>` must be compatible with the units of the value given.  

If you use the `xml` method, you can also specify formatting by appending the specifications to the property name to the unit seperated by a comma, e.g.,

~~~
  GET /xml/<object-name>/<property-name>[<spec>]
~~~

where the format `<spec>` is structured as follows for double values

~~~
  [<unit>,<precision><format>]
~~~

and for complex values

~~~
  [<unit>,<precision><format><part>]
~~~

The `<precision>` must be a single digit from 0 to 9.  The format must be one of `a`, `f`, `e`, `g`, `A`, `F`, `E`, `G`, as interpreted by `printf()`.  The part may `i` or `j` for complex rectangular, `d` for complex polar with angle in degrees, `r` for complex polar with angle in radians, `M` for magnitude only, `D` for angle only in degrees, `R` for angle only in radians.

To set the value of an [[/object (property)|object property]], use the following query

~~~
  GET /<format>/<object-name>/<property-name>=<value>
~~~

The value can include `[<units>]` and they will be converted automatically.  The value is read back after is set to confirm that it was accepted, including unit conversion, if appropriate.

## Formats

### `raw`

The raw response to a global variable or object property request will be in the form:

~~~
  <value>
~~~

### `xml`

The XML response to global variable requests will be in the form

~~~
  <globalvar>
    <name>variable-name</name>
    <value>value</value>
  </globalvar>
~~~

The response to object property requests will be in the form

~~~
  <property>
    <object>object-name</object>
    <name>property-name</name>
    <value>value</value>
  </property>
~~~

### 'json'

The JSON response to global variable requests will be in the form

~~~
  { "name" : "<variable-name>", "value" : "<value> <unit>" }
~~~

The response to object property requests will be in the form

~~~
  { 
    "object" : "<object-name>", 
    "name" : "<property-name>", 
    "type" : "<type-name>", 
    "value" : "<value>"
  }
~~~

If the property name is `*` or `*[tuple]`, then the response is of the form

~~~
  [ { "<property-name>" : "<value>" }, ... ]
~~~

where all the known properties of the object are output.
If the property name is `*[dict]`, then the response is of the form

~~~
  { "<property-name" : "<value>", ...}
~~~

In the event of an error, the JSON response is of the form

~~~
  { 
    "error" : "<description>", 
    "object" : "<object-name>", 
    "property" : "<property-name>"
  }
~~~

## Real-time models

Server mode is essential to the realtime mode. Together this allows a web-based application to access the global variable and properties of named objects of a real-time simulation to emulate a control room.  See the [IEEE-123 Model](https://github.com/slacgismo/gridlabd-models/tree/master/ieee123) for an example.

# See also

* [[/GLM/General/Real-time]]
