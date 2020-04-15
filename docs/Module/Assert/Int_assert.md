[[/Module/Assert/Int_assert]] -- Class int_assert

# Synopsis

GLM:

~~~
  object int_assert {
    status "{ASSERT_NONE,ASSERT_FALSE,ASSERT_TRUE}";
    once "{ONCE_DONE,ONCE_TRUE,ONCE_FALSE}";
    within_mode "{WITHIN_RATIO,WITHIN_VALUE}";
    value "<integer>";
    within "<integer>";
    target "<string>";
  }
~~~

# Description

This class is used to assert integer values.

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

### `within_mode`

~~~
  enumeration {WITHIN_RATIO, WITHIN_VALUE} within_mode;
~~~

Method of applying tolerance

### `value`

~~~
  double value;
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
  object int_assert {
    status "0";
    once "0";
    within_mode "0";
    value "0";
    within "0";
    target "";
  }
~~~

# See also

* [[/Module/Assert]]

