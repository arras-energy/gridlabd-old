[[/Lcm]] -- Returns the lowest common multiple of ``|x1|`` and ``|x2|``

~~~
Syntax
------

matrix lcm <matrix> <matrix> [where=<boollist>] [axes=<tuplelist>] [axis=<inttuple>] [keepdims=<boolstr>] [casting=<str>] [order=<str>] [dtype=<str>] [subok=<boolstr>]

Parameters
----------
x1, x2 : array_like, int
    Arrays of values.
    If ``x1.shape != x2.shape``, they must be broadcastable to a common
    shape (which becomes the shape of the output).

Returns
-------
y : ndarray or scalar
    The lowest common multiple of the absolute value of the inputs
    This is a scalar if both `x1` and `x2` are scalars.

See Also
--------
gcd : The greatest common divisor

~~~
