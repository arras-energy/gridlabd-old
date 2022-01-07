[[/Module/Powerflow/Overhead_line_conductor]] -- Class overhead_line_conductor

# Synopsis

GLM:

~~~
  object overhead_line_conductor {
    geometric_mean_radius "<decimal> ft";
    resistance "<decimal> Ohm/mile";
    diameter "<decimal> in";
    rating.summer.continuous "<decimal> A";
    rating.summer.emergency "<decimal> A";
    rating.winter.continuous "<decimal> A";
    rating.winter.emergency "<decimal> A";
    weight "<decimal> lb/ft";
    strength "<decimal> lb";
  }
~~~

# Description

TODO

## Properties

### `geometric_mean_radius`

~~~
  double geometric_mean_radius[ft];
~~~

Radius of the conductor

### `resistance`

~~~
  double resistance[Ohm/mile];
~~~

Resistance in Ohms/mile of the conductor

### `diameter`

~~~
  double diameter[in];
~~~

Diameter of line for capacitance calculations

### `rating.summer.continuous`

~~~
  double rating.summer.continuous[A];
~~~

Continuous summer amp rating

### `rating.summer.emergency`

~~~
  double rating.summer.emergency[A];
~~~

Emergency summer amp rating

### `rating.winter.continuous`

~~~
  double rating.winter.continuous[A];
~~~

Continuous winter amp rating

### `rating.winter.emergency`

~~~
  double rating.winter.emergency[A];
~~~

Emergency winter amp rating

### `weight`

~~~
  double rating.winter.emergency[A];
~~~

Cable weight per lineal foot

### `strength`

~~~
  double rating.winter.emergency[A];
~~~

Cable strength

# Example

~~~
  object overhead_line_conductor {
    geometric_mean_radius "0.0";
    resistance "0.0";
    diameter "0.0";
    rating.summer.continuous "0.0";
    rating.summer.emergency "0.0";
    rating.winter.continuous "0.0";
    rating.winter.emergency "0.0";
  }
~~~

# See also

* [[/Module/Powerflow]]

