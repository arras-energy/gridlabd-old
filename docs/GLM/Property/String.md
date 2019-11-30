[[GLM/Property/String]] - Property to contain general strings

# Synopsis
GLM:
~~~
  class class-name {
    string property-name;
  }
  object class-name {
    property-name "property-value";
  }
~~~
or
~~~
  object class-name {
    property-name """multiline
property-value""";
  }
~~~

# Description

The `string` property allows storage and import/export of strings of arbitrary length.
