[[/Module/Powerflow/Underground_line_conductor]] -- Class underground_line_conductor

# Synopsis

GLM:

~~~
  object underground_line_conductor {
    outer_diameter "<decimal> in";
    conductor_gmr "<decimal> ft";
    conductor_diameter "<decimal> in";
    conductor_resistance "<decimal> Ohm/mile";
    neutral_gmr "<decimal> ft";
    neutral_diameter "<decimal> in";
    neutral_resistance "<decimal> Ohm/mile";
    neutral_strands "<integer>";
    shield_thickness "<decimal> in";
    shield_diameter "<decimal> in";
    insulation_relative_permitivitty "<decimal> unit";
    shield_gmr "<decimal> ft";
    shield_resistance "<decimal> Ohm/mile";
    rating.summer.continuous "<decimal> A";
    rating.summer.emergency "<decimal> A";
    rating.winter.continuous "<decimal> A";
    rating.winter.emergency "<decimal> A";
    insulator_R "<R-value> K/W";
  }
~~~

# Description

TODO

## Properties

### `outer_diameter`

~~~
  double outer_diameter[in];
~~~

Outer diameter of conductor and sheath

### `conductor_gmr`

~~~
  double conductor_gmr[ft];
~~~

Geometric mean radius of the conductor

### `conductor_diameter`

~~~
  double conductor_diameter[in];
~~~

Diameter of conductor

### `conductor_resistance`

~~~
  double conductor_resistance[Ohm/mile];
~~~

Resistance of conductor in ohm/mile

### `neutral_gmr`

~~~
  double neutral_gmr[ft];
~~~

Geometric mean radius of an individual neutral conductor/strand

### `neutral_diameter`

~~~
  double neutral_diameter[in];
~~~

Diameter of individual neutral conductor/strand of concentric neutral

### `neutral_resistance`

~~~
  double neutral_resistance[Ohm/mile];
~~~

Resistance of an individual neutral conductor/strand in ohm/mile

### `neutral_strands`

~~~
  int16 neutral_strands;
~~~

Number of cable strands in neutral conductor

### `shield_thickness`

~~~
  double shield_thickness[in];
~~~

The thickness of Tape shield in inches

### `shield_diameter`

~~~
  double shield_diameter[in];
~~~

The outside diameter of Tape shield in inches

### `insulation_relative_permitivitty`

~~~
  double insulation_relative_permitivitty[unit];
~~~

Permitivitty of insulation, relative to air

### `shield_gmr`

~~~
  double shield_gmr[ft];
~~~

Geometric mean radius of shielding sheath

### `shield_resistance`

~~~
  double shield_resistance[Ohm/mile];
~~~

Resistance of shielding sheath in ohms/mile

### `rating.summer.continuous`

~~~
  double rating.summer.continuous[A];
~~~

Amp rating in summer, continuous

### `rating.summer.emergency`

~~~
  double rating.summer.emergency[A];
~~~

Amp rating in summer, short term

### `rating.winter.continuous`

~~~
  double rating.winter.continuous[A];
~~~

Amp rating in winter, continuous

### `rating.winter.emergency`

~~~
  double rating.winter.emergency[A];
~~~

Amp rating in winter, short term

# Example

~~~
  object underground_line_conductor {
    outer_diameter "0.0";
    conductor_gmr "0.0";
    conductor_diameter "0.0";
    conductor_resistance "0.0";
    neutral_gmr "0.0";
    neutral_diameter "0.0";
    neutral_resistance "0.0";
    neutral_strands "0";
    shield_thickness "0.0";
    shield_diameter "0.0";
    insulation_relative_permitivitty "0.0";
    shield_gmr "0.0";
    shield_resistance "0.0";
    rating.summer.continuous "0.0";
    rating.summer.emergency "0.0";
    rating.winter.continuous "0.0";
    rating.winter.emergency "0.0";
  }
~~~

# See also

* [[/Module/Powerflow]]

