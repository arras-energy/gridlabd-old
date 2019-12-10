[[/Module/Powerflow/Line_configuration]] -- Class line_configuration

# Synopsis
GLM:
~~~
  object line_configuration {
    conductor_A "<string>";
    conductor_B "<string>";
    conductor_C "<string>";
    conductor_N "<string>";
    spacing "<string>";
    z11 "<string> Ohm/mile";
    z12 "<string> Ohm/mile";
    z13 "<string> Ohm/mile";
    z21 "<string> Ohm/mile";
    z22 "<string> Ohm/mile";
    z23 "<string> Ohm/mile";
    z31 "<string> Ohm/mile";
    z32 "<string> Ohm/mile";
    z33 "<string> Ohm/mile";
    c11 "<decimal> nF/mile";
    c12 "<decimal> nF/mile";
    c13 "<decimal> nF/mile";
    c21 "<decimal> nF/mile";
    c22 "<decimal> nF/mile";
    c23 "<decimal> nF/mile";
    c31 "<decimal> nF/mile";
    c32 "<decimal> nF/mile";
    c33 "<decimal> nF/mile";
    rating.summer.continuous "<decimal> A";
    rating.summer.emergency "<decimal> A";
    rating.winter.continuous "<decimal> A";
    rating.winter.emergency "<decimal> A";
  }
~~~

# Description

TODO

## Properties

### `conductor_A`
~~~
  object conductor_A;
~~~

TODO

### `conductor_B`
~~~
  object conductor_B;
~~~

TODO

### `conductor_C`
~~~
  object conductor_C;
~~~

TODO

### `conductor_N`
~~~
  object conductor_N;
~~~

TODO

### `spacing`
~~~
  object spacing;
~~~

TODO

### `z11`
~~~
  complex z11[Ohm/mile];
~~~

TODO

### `z12`
~~~
  complex z12[Ohm/mile];
~~~

TODO

### `z13`
~~~
  complex z13[Ohm/mile];
~~~

TODO

### `z21`
~~~
  complex z21[Ohm/mile];
~~~

TODO

### `z22`
~~~
  complex z22[Ohm/mile];
~~~

TODO

### `z23`
~~~
  complex z23[Ohm/mile];
~~~

TODO

### `z31`
~~~
  complex z31[Ohm/mile];
~~~

TODO

### `z32`
~~~
  complex z32[Ohm/mile];
~~~

TODO

### `z33`
~~~
  complex z33[Ohm/mile];
~~~

TODO

### `c11`
~~~
  double c11[nF/mile];
~~~

TODO

### `c12`
~~~
  double c12[nF/mile];
~~~

TODO

### `c13`
~~~
  double c13[nF/mile];
~~~

TODO

### `c21`
~~~
  double c21[nF/mile];
~~~

TODO

### `c22`
~~~
  double c22[nF/mile];
~~~

TODO

### `c23`
~~~
  double c23[nF/mile];
~~~

TODO

### `c31`
~~~
  double c31[nF/mile];
~~~

TODO

### `c32`
~~~
  double c32[nF/mile];
~~~

TODO

### `c33`
~~~
  double c33[nF/mile];
~~~

TODO

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
  object line_configuration {
    z11 "0+0i";
    z12 "0+0i";
    z13 "0+0i";
    z21 "0+0i";
    z22 "0+0i";
    z23 "0+0i";
    z31 "0+0i";
    z32 "0+0i";
    z33 "0+0i";
    c11 "0.0";
    c12 "0.0";
    c13 "0.0";
    c21 "0.0";
    c22 "0.0";
    c23 "0.0";
    c31 "0.0";
    c32 "0.0";
    c33 "0.0";
    rating.summer.continuous "0.0";
    rating.summer.emergency "0.0";
    rating.winter.continuous "0.0";
    rating.winter.emergency "0.0";
  }
~~~

# See also
* [[/Module/Powerflow]]

