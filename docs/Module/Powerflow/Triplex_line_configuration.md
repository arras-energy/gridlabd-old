[[/Module/Powerflow/Triplex_line_configuration]] -- Class triplex_line_configuration

# Synopsis

GLM:

~~~
  object triplex_line_configuration {
    conductor_1 "<string>";
    conductor_2 "<string>";
    conductor_N "<string>";
    insulation_thickness "<decimal> in";
    diameter "<decimal> in";
    spacing "<string>";
    z11 "<string> Ohm/mile";
    z12 "<string> Ohm/mile";
    z21 "<string> Ohm/mile";
    z22 "<string> Ohm/mile";
    rating.summer.continuous "<decimal> A";
    rating.summer.emergency "<decimal> A";
    rating.winter.continuous "<decimal> A";
    rating.winter.emergency "<decimal> A";
  }
~~~

# Description

TODO

## Properties

### `conductor_1`

~~~
  object conductor_1;
~~~

Conductor type for phase 1

### `conductor_2`

~~~
  object conductor_2;
~~~

Conductor type for phase 2

### `conductor_N`

~~~
  object conductor_N;
~~~

Conductor type for phase N

### `insulation_thickness`

~~~
  double insulation_thickness[in];
~~~

Thickness of insulation around cabling

### `diameter`

~~~
  double diameter[in];
~~~

Total diameter of cable

### `spacing`

~~~
  object spacing;
~~~

Defines the line spacing configuration

### `z11`

~~~
  complex z11[Ohm/mile];
~~~

Phase 1 self-impedance, used for direct entry of impedance values

### `z12`

~~~
  complex z12[Ohm/mile];
~~~

Phase 1-2 induced impedance, used for direct entry of impedance values

### `z21`

~~~
  complex z21[Ohm/mile];
~~~

Phase 2-1 induced impedance, used for direct entry of impedance values

### `z22`

~~~
  complex z22[Ohm/mile];
~~~

Phase 2 self-impedance, used for direct entry of impedance values

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
  object triplex_line_configuration {
    insulation_thickness "0.0";
    diameter "0.0";
    z11 "0+0i";
    z12 "0+0i";
    z21 "0+0i";
    z22 "0+0i";
    rating.summer.continuous "0.0";
    rating.summer.emergency "0.0";
    rating.winter.continuous "0.0";
    rating.winter.emergency "0.0";
  }
~~~

# See also

* [[/Module/Powerflow]]

