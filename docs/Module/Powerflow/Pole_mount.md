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
parent of a pole mount must be the pole on which the equipment is mounted.

# Example

The following example illustrates attaching an overhead line to a pole.

~~~
module powerflow;
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
