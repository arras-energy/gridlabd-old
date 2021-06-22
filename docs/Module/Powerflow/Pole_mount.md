[[/Module/Powerflow/Pole_mount]] -- Equipment mount on power pole

# Synopsis

GLM:

~~~
object pole_mount
{
    equipment link_id;
    height 0.0 ft;
    offset 0.0 ft;
    direction 0.0 deg;
    weight 0.0 lbs;
    area 0.0 ft;
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
    };
}
~~~

# See also

* [[/Module/Powerflow/Pole.md]]
