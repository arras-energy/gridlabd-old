[[/GLM/Property/Timestamp]] -- Timestamp property

# Synopsis

GLM:

~~~
  class <class-name> {
    Timestamp <property-name>;
  }
  object <class-name> {
    <property-name> "<date-value>";
  }
~~~
  
# Description

Timestamp properties can be specified in US, EU, ISO, and ISO 8601 formats, as indicated by the [[/Global/Dateformat]] variable.

## ISO8601 

The following ISO 8601 date/time formats are now supported for all timestamp value inputs:

* `yyyy-mm-ddTHH:MM:SS[.SSSSSSSSS]Z` 
* `yyyy-mm-ddTHH:MM:SS[.SSSSSSSSS]{+,-}HH:MM`

GridLAB-D does not currently support output in ISO 8601 format

# See also

* [[/Global/Dateformat]]
