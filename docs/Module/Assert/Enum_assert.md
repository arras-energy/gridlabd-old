[[/Module/Assert/Enum_assert]] -- Class enum_assert

# Synopsis

GLM:

~~~
  object enum_assert {
    status "{ASSERT_NONE,ASSERT_FALSE,ASSERT_TRUE}";
    value "<integer>";
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

### `value`
~~~
  int32 value;
~~~

Value to assert

### `target`
~~~
  char1024 target;
~~~

Property to perform the assert upon

# Example

~~~
  object enum_assert {
    status "0";
    value "0";
    target "";
  }
~~~

# See also
* [[/Module/Assert]]

