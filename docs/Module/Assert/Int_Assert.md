[[/Module/Assert]] -- Module assert

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

TODO

## Properties

### `status`
~~~
  enumeration {ASSERT_NONE, ASSERT_FALSE, ASSERT_TRUE} status;
~~~

TODO

### `once`
~~~
  enumeration {ONCE_DONE, ONCE_TRUE, ONCE_FALSE} once;
~~~

TODO

### `within_mode`
~~~
  enumeration {WITHIN_RATIO, WITHIN_VALUE} within_mode;
~~~

TODO

### `value`
~~~
  int32 value;
~~~

TODO

### `within`
~~~
  int32 within;
~~~

TODO

### `target`
~~~
  char1024 target;
~~~

TODO

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

