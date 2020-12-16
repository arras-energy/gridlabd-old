[[/GLM/Global/Expansion]] -- Global variable expansions

# Synopsis

~~~
${name=value}
${name:-value}
${name:=value}
${name:+value}
${name:offset}
${name:offset:length}
${name/pattern/value}
${name//pattern/value}
${++value}
${--value}
${value++}
${value--}
${[~]name+[~]value}
${[~]name-[~]value}
${[~]name/[~]value}
${[~]name*[~]value}
${[~]name%[~]value}
${[~]name&[~]value}
${[~]name|[~]value}
${[~]name^[~]value}
${name+=value}
${name-=value}
${name/=value}
${name*=value}
${name%=value}
${name%&[~]value}
${name%|[~]value}
${name%^[~]value}
${name[!<=>&|~]value?yesvalue:novalue}
~~~

# Description

Global variable expansions perform string and integer operations on global variable of type `char1024` and `int32`, respectively. 

## `{name=value}`

Creates a new global `name` using `value`.  If the value can be interpreted as an integer, the new global will be defined as an `int32`.  Otherwise, the new value is defined as a `char1024`. Returns the value of the new global variable.

## `{name:-value}`

Returns the value assigned to `name` if it is defined.  If `name` is not defined, returns `value`.

## `{name:=value}`

Sets the value of the global variable if not defined. Returns the old value if defined or `value` if not defined.

## `{name:+value}`

Substitutes the `value` is `name` is defined, otherwise returns nothing.

## `{name:offset}`

Returns the value of the global `name` starting at the character at position `offset`.

## `{name:offset:length}`

Returns the value of the global `name` starting at the character at position `offset` and stopping after `length` characters.

## `{name/pattern/value}`

Returns the value of the global `name` substituting the first occurance of the substring `pattern`  with `value`.

## `{name//pattern/value}`

Returns the value of the global `name` substituting every occurance of the substring `pattern`  with `value`.

## `{++name}`

Returns the value of the global integer `name` after incrementing it by one.

## `{--name}`

Returns the value of the global integer `name` after decrementing it by one.

## `{name++}`

Returns the value of the global integer `name` before incrementing it by one.

## `{name--}`

Returns the value of the global integer `name` before decrementing it by one.

## `{[~]name+[~]value}`

Adds `value` to the value of the global integer `name`. The first `~` performs a bitwise `not` operation on the result. The second `~` performs a bitwise `not` operation on the value.

## `{[~]name-[~]value}`

Subtracts `value` from the value of the global integer `name`. The first `~` performs a bitwise `not` operation on the result. The second `~` performs a bitwise `not` operation on the value.

## `{[~]name/[~]value}`

Divides the value of the global integer `name` by `value`. The first `~` performs a bitwise `not` operation on the result. The second `~` performs a bitwise `not` operation on the value.

## `{[~]name*[~]value}`

Multiplies the value of the global integer `name` by `value`. The first `~` performs a bitwise `not` operation on the result. The second `~` performs a bitwise `not` operation on the value.

## `{[~]name%[~]value}`

Uses `value` to perform the modulo operation on the value of the global integer `name`. The first `~` performs a bitwise `not` operation on the result. The second `~` performs a bitwise `not` operation on the value.

## `{[~]name&[~]value}`

Performs the bitwise `and` operation of `value` on the value of the global integer `name`. The first `~` performs a bitwise `not` operation on the result. The second `~` performs a bitwise `not` operation on the value.

## `{[~]name|[~]value}`

Performs the bitwise `or` operation of `value` on the value of the global integer `name`. The first `~` performs a bitwise `not` operation on the result. The second `~` performs a bitwise `not` operation on the value.

## `{[~]name^[~]value}`

Performs the bitwise `xor` operation of `value` on the value of the global integer `name`. The first `~` performs a bitwise `not` operation on the result. The second `~` performs a bitwise `not` operation on the value.

## `{name+=value}`

Increments the value of the global integer `name` by `value` and returns the result.

## `{name-=value}`

Decrements the value of the global integer `name` by `value` and returns the result.

## `{name/=value}`

Divides the value of the global integer `name` by `value` and returns the result.

## `{name*=value}`

Multiplies the value of the global integer `name` by `value` and returns the result.

## `{name%=value}`

Performs the modulo operation on the value of the global integer `name` using `value` and returns the result.

## `{name%&[~]value}`

Performs the bitwise `and` operation on the value of the global integer `name` using `value` and returns the result.

## `{name%|[~]value}`

Performs the bitwise `or` operation on the value of the global integer `name` using `value` and returns the result.

## `{name%^[~]value}`

Performs the bitwise `xor` operation on the value of the global integer `name` using `value` and returns the result.

## `{name[!<=>&|~]value?yesvalue:novalue}`

Compares the global variable `name` to `value` and returns `yesvalue` if true and `novalue` if false.
