[[/Module/Powerflow/Triplex_line_conductor]] -- Class triplex_line_conductor

# Synopsis

GLM:

~~~
  object triplex_line_conductor {
    resistance "<decimal> Ohm/mile";
    geometric_mean_radius "<decimal> ft";
    rating.summer.continuous "<decimal> A";
    rating.summer.emergency "<decimal> A";
    rating.winter.continuous "<decimal> A";
    rating.winter.emergency "<decimal> A";
  }
~~~

# Description

TODO

## Properties

### `resistance`

~~~
  double resistance[Ohm/mile];
~~~

Resistance of cable in ohm/mile

### `geometric_mean_radius`

~~~
  double geometric_mean_radius[ft];
~~~

Geometric mean radius of the cable

### `rating.summer.continuous`

~~~
  double rating.summer.continuous[A];
~~~

Amp ratings for the cable during continuous operation in summer

### `rating.summer.emergency`

~~~
  double rating.summer.emergency[A];
~~~

Amp ratings for the cable during short term operation in summer

### `rating.winter.continuous`

~~~
  double rating.winter.continuous[A];
~~~

Amp ratings for the cable during continuous operation in winter

### `rating.winter.emergency`

~~~
  double rating.winter.emergency[A];
~~~

Amp ratings for the cable during short term operation in winter

# Example

~~~
  object triplex_line_conductor {
    resistance "0.0";
    geometric_mean_radius "0.0";
    rating.summer.continuous "0.0";
    rating.summer.emergency "0.0";
    rating.winter.continuous "0.0";
    rating.winter.emergency "0.0";
  }
~~~

# See also

* [[/Module/Powerflow]]

