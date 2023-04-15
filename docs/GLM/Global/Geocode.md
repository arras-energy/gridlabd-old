[[/GLM/Global/Geocode]] -- Geohash encoder

# Synopsis

GLM:

~~~
${GEOCODE <latitude>,<longitude>[#<resolution>]}
${GEOCODE <objname>[#<resolution>]}
${GEOCODE <geohash>[.lat|.lon]}
~~~

# Description

Return the geohash code corresponding to the latitude/longitude given or the object name. This can be helpful is connecting object based on location, such as linking an object to weather.

The default resolution is 5. The resolution corresponds to the following distances:

    1   2500 km
    2   600 km
    3   80 km
    4   20 km
    5   2.5 km
    6   0.2 km
    7   0.08 km
    8   0.02 km
    9   0.0025 km
    10  0.0006 km
    11  0.000075 km

The reverse conversion transfer the geohash into a latitude/longitude pair.  If the `.lat` or `.lon` spec is include, then only the corresponding value is returned.

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
#print ${GEOCODE 9q9j76}
#print ${GEOCODE 9q9j76.lat}
#print ${GEOCODE 9q9j76.lon}
~~~

# See also

* [[/GLM/Subcommand/Geodata]]
