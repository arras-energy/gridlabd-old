[[/GLM/Global/Geocode]] -- Geohash encoder

# Synopsis

GLM:

~~~
${GEOCODE <latitude>,<longitude>[#<resolution>]}
${GEOCODE <objname>[#<resolution>]}
~~~

# Description

Return the geohash code corresponding to the latitude/longitude given or the object name. This can be helpful is connecting object based on location, such as linking an object to weather.

# Example

The following example prints the geohash codes for a position and an object:

~~~
class test
{
    char32 geocode;
}
object test
{
    name "test";
    latitude 37.5;
    longitude -122.2;
}
#print ${GEOCODE 37.5,-122.2#6}
#print ${GEOCODE test#6}
~~~

# See also

* [[/GLM/Subcommand/Geodata]]
