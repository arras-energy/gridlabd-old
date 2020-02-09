[[/GLM/Property/Datetime]] -- Datetime property

# Synopsis

GLM:

~~~
  class <class-name> {
    datetime <property-name>;
  }
  object <class-name> {
    <property-name> "<date-value>";
  }
~~~
  
# Description

Datetime properties can be specified in US, EU, ISO, and ISO8601 formats.

## ISO8601 

The following ISO8601 date/time formats are now supported for all timestamp value inputs:

* `yyyy-mm-ddTHH:MM:SS[.SSSSSSSSS]Z` 
* `yyyy-mm-ddTHH:MM:SS[.SSSSSSSSS]{+,-}HH:MM`

GridLAB-D does not current support output in ISO 8601 format

# See also
