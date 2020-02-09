[[/GLM/Property/JSON]] -- JSON property format

# Synopsis

GLM:

~~~
object class {
  property {tag1:value1; tag2:value2; ...; tagN:valueN}; // DEPRECATED
  property {"tag1":"value1", "tag2":"value2", ..., "tagN":"valueN"};
}
~~~

# Description

Properties that have parts may be set or updated using JSON-formatted data.

# Example

The following examples set the property `z` to the value `-i`:

~~~
class example {
  complex z;
}
object example {
  z "0-1i";
}
object example {
  z {real:0.0; imag;-1.0};
}
object example {
  z {mag:1.0; ang:180;};
}
object example {
  z {mag:1.0; arg:3.1415926;};
}
object example {
  z {mag:-1.0; ang:0};
}
~~~

# See also
* [[/Command/JSON input]]
* [[/Command/JSON output]]

