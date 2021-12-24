[[/Rint]] -- Round elements of the array to the nearest integer.

~~~
Syntax
------

matrix rint <matrix> [where=<boollist>] [axes=<tuplelist>] [axis=<inttuple>] [keepdims=<boolstr>] [casting=<str>] [order=<str>] [dtype=<str>] [subok=<boolstr>]

Parameters
----------
x : array_like
    Input array.
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
out : ndarray or scalar
    Output array is same shape and type as `x`.
    This is a scalar if `x` is a scalar.

See Also
--------
fix, ceil, floor, trunc

Notes
-----
For values exactly halfway between rounded decimal values, NumPy
rounds to the nearest even value. Thus 1.5 and 2.5 round to 2.0,
-0.5 and 0.5 round to 0.0, etc.

~~~
