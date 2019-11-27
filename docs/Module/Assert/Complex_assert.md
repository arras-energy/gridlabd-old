[[/Module/Assert/Complex_assert]] -- Class complex_assert

# Synopsis
GLM:
~~~
  object complex_assert {
    status "{ASSERT_NONE,ASSERT_FALSE,ASSERT_TRUE}";
    once "{ONCE_DONE,ONCE_TRUE,ONCE_FALSE}";
    operation "{ANGLE,MAGNITUDE,IMAGINARY,REAL,FULL}";
    value "<string>";
    within "<decimal>";
    target "<string>";
  }
~~~

# Description

TODO

## Properties

### `status`
~~~
  enumeration {ASSERT_NONE, ASSERT_FALSE, ASSERT_TRUE} status;
~~~

Conditions for the assert checks

### `once`
~~~
  enumeration {ONCE_DONE, ONCE_TRUE, ONCE_FALSE} once;
~~~

Conditions for a single assert check

### `operation`
~~~
  enumeration {ANGLE, MAGNITUDE, IMAGINARY, REAL, FULL} operation;
~~~

Complex operation for the comparison

### `value`
~~~
  complex value;
~~~

Value to assert

### `within`
~~~
  double within;
~~~

Tolerance for a successful assert

### `target`
~~~
  char1024 target;
~~~

Property to perform the assert upon

# Example

~~~
  object complex_assert {
    status "0";
    once "0";
    operation "0";
    value "0+0i";
    within "0.0";
    target "";
  }
~~~

# See also
* [[/Module/Assert]]

