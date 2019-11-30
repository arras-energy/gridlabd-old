[[Module/Assert/Assert]] -- General purpose assert object

# Synopsis
~~~
module assert;
object assert {
	parent "<object-name>";
	status "INIT";
	target "<property-name>";
	part "<optional-part-name>";
	relation "==";
	value "<unary-comparison-value>";
	within "<comparison-limit-value";
	lower "<lower-binary-comparison-value>";
	upper "<upper-binary-comparison-value>";
	group "<optional-group-name>";
	start "INIT";
	stop "NEVER";
	hold "+0 s";
}
~~~

# Description

The general purpose `assert` object verifies that the properties of other objects meet specified characteristics.  
The `assert` object examines a single property of parent object and compares it to a value or pair of values.  If the assert fails, the simulation halts.

## Properties

The following properties are available in an `assert` object. 

### `group` 
~~~
char1024 group;
~~~

Allows a group specification.  See [[GLM/Finding_objects]] for details.

### `hold`
~~~
double hold[s];
~~~

Specifies how long the assert must be violated before the exception is thrown.

### `lower`
~~~
char1024 lower;
~~~

The `lower` property specifies the lower bound when using the `inside` or `outside` relations.

### `part`
~~~
char32 part;
~~~

The `part` property specifies the property part to use when comparing values.  All property parts are considered to have an underlying double value.  For information of valid property parts, see the documentation on the property in question.

### `relation`
~~~
enumeration {outside=7, inside=6, !==3, >==2, >=5, <==1, <=4, ===0} relation;
~~~

The `relation` operator specifies the relationship that must be satisfied.  Most relations are unary, in which case the `value` property is used for the comparison.  Some relations are binary, i.e., `inside` and `outside`, in which case the property is compared against the `lower` and `upper` values.

#### `inside`

Compares the target property to determine whether it is between the `lower` and `upper` values (inclusive).

#### `outside`

Compares the target property to determine whether it is outside the `lower` and `upper` values (exclusive).

#### `!=`

Compares the target property to determine whether it is different from the `value`.

#### `>=`

Compares the target property to determine whether it is greater than or equal to the `value`.

#### `>`

Compares the target property to determine whether it is greater than the `value`.

#### `<=`

Compares the target property to determine whether it is less than or equal to the `value`.

#### `>`

Compares the target property to determine whether it is less than the `value`.

#### `==`

Compares the target property to determine whether it is equal to the `value`.

### `start`
~~~
timestamp start;
~~~

Specifies when the assert is to start being applied.

### `status`
~~~
enumeration {NONE=3, FALSE=2, TRUE=1, INIT=0} status;
~~~

The `status` enumeration specifies the desired outcome of the test.  

#### `FALSE`

The `FALSE` keyword is used to specify that the assert test should fail.

#### `NONE`

The `NONE` keyword is used to specify that the assert test should be ignore.

#### `TRUE`

The `TRUE` keyword is used to specify that the assert test should succeed.

### `stop` 
~~~
timestamp stop;
~~~

Specifies when the assert is to stop being applied.

### `target`
~~~
char1024 target;
~~~

The `target` string specifies the name of the property to examine.

### `upper`
~~~
char1024 upper;
~~~

The `upper` property specifies the upper bound when using the `inside` or `outside` relation.

### `value`
~~~
char1024 value;
~~~

The `value` property specifies the value to compare to when using the `!=`, `>=`, `>`, `<=`, `<`, or `==` relations.

### `within`
~~~
char1024 within;
~~~

The `within` property specifies the accuracy to which `==` and `!=` comparisons are performed.

# Examples

The first example asserts that the temperature is `50 degF`.  The second example asserts that the temperature is between `0` and `40 degC`. The third example asserts that the temperature is within `1 degF` of `49.5 degF`.
~~~
module assert;
module climate;

object climate {
  temperature "50 degF";
  object assert { // example 1
    target "temperature";
    relation "==";
    value "50 degF";
  };
  object assert { // example 2
    target "temperature";
    relation "inside";
    lower "0 degC";
    upper "40 degC";
  };
  object assert { // example 3
    target "temperature";
    relation "==";
    value "49.5 degF";
    within "1 degF";
  };
}
~~~
The fourth example asserts that the real part of the voltage is `120`.  The fifth example asserts that the magnitude of the voltage exceeds `120`.  The sixth example asserts that the angle of the voltage angle is between `-45 deg` and `45 deg`.
~~~
module assert;
module powerflow;

object meter {
  nominal_voltage "120";
  phases "A";
  voltage_A "120+1j";
  object assert { // example 4
    target "voltage_A";
    part "real";
    relation "==";
    value "120.0";
  };
  object assert { // example 5
    target "voltage_A";
    part "mag";
    relation ">";
    value "120";
  };
  object assert (object)|assert { // example 6
    target "voltage_A";
    part "ang";
    relation "inside";
    lower "-45 deg";
    upper "45 deg";
  };
}
~~~
# Caveats

Some units have an absolute offset (e.g., `degC`, `degF`) that convert absolute values, not relative values.  Thus `within 0.01 degF` will not work as expected when compared to a property in `degC` because `0.01 degF` is about `-17 degC`. The only solution is to provide the value in the same units as the property, or to use units that don't have an absolute offset, e.g., `R` or `K`).

# See also
* [[Module/Assert]]
