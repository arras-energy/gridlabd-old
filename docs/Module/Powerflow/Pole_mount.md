[[/Module/Powerflow/Pole_mount]] -- Equipment mount on power pole

# Synopsis

GLM:

~~~
object pole_mount
{
    equipment link_id;
    height 0.0 ft;
    offset 0.0 ft;
    area 0.0 ft;
    direction 0.0 deg;
    weight 0.0 lbs;
    pole_spacing 0.0 ft;
    angle 0.0 deg;
    weather <climate-object>;
    wind_speed 0.0 m/s;
    wind_direction 0.0 deg;
}
~~~

# Description

The pole mount object attaches powerflow link objects to pole objects.  The
parent of a `pole_mount` must be the `pole` object on which the equipment is
mounted.

# Example

The following example illustrates mounting an overhead line on a pole.

~~~
module powerflow;
object overhead_line
{
    name "line123";
    phases ABC;
    nominal_voltage 12.47 kV;
    // etc...
}
object pole
{
    name "pole123";
    object pole_mount
    {
        equipment "line123";
        weather "weather";
    };
}
~~~

## Properties

### `direction`

~~~
    double direction;
~~~

Bearing of the mounted equipment, an angle measured clockwise from North in degrees with vertex at the pole centerline.

# See also

* [[/Module/Powerflow/Pole.md]]
