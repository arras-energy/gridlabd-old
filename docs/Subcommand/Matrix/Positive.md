[[/Positive]] -- Numerical positive, element-wise.

~~~
Syntax
------

matrix positive <matrix> [where=<boollist>] [axes=<tuplelist>] [axis=<intlist>] [keepdims=<bool>] [casting=<str>] [order=<str>] [dtype=<str>] [subok=<bool>]

Parameters
----------
x : array_like or scalar
    Input array.

Returns
-------
y : ndarray or scalar
    Returned array or scalar: `y = +x`.
    This is a scalar if `x` is a scalar.

Notes
-----
Equivalent to `x.copy()`, but only defined for types that support
~~~
