[[/Fmod]] -- Return the element-wise remainder of division.

~~~
Syntax
------

matrix fmod <matrix> <matrix> [where=<boollist>] [axes=<tuplelist>] [axis=<inttuple>] [keepdims=<boolstr>] [casting=<str>] [order=<str>] [dtype=<str>] [subok=<boolstr>]

Parameters
----------
x1 : array_like
    Dividend.
x2 : array_like
    Divisor.
    If ``x1.shape != x2.shape``, they must be broadcastable to a common
    shape (which becomes the shape of the output).
out : ndarray, None, or tuple of ndarray and None, optional
    A location into which the result is stored. If provided, it must have
    a shape that the inputs broadcast to. If not provided or None,
    a freshly-allocated array is returned. A tuple (possible only as a
    keyword argument) must have length equal to the number of outputs.
where : array_like, optional
    This condition is broadcast over the input. At locations where the
    condition is True, the `out` array will be set to the ufunc result.
    Elsewhere, the `out` array will retain its original value.
    Note that if an uninitialized `out` array is created via the default
    ``out=None``, locations within it where the condition is False will
    remain uninitialized.
**kwargs
    For other keyword-only arguments, see the
    :ref:`ufunc docs <ufuncs.kwargs>`.

Returns
-------
y : array_like
    The remainder of the division of `x1` by `x2`.
    This is a scalar if both `x1` and `x2` are scalars.

See Also
--------
remainder : Equivalent to the Python ``%`` operator.
divide

Notes
-----
The result of the modulo operation for negative dividend and divisors
is bound by conventions. For `fmod`, the sign of result is the sign of
the dividend, while for `remainder` the sign of the result is the sign
of the divisor. The `fmod` function is equivalent to the Matlab(TM)
``rem`` function.

~~~
