[[/Frexp]] -- Decompose the elements of x into mantissa and twos exponent.

~~~
Syntax
------

matrix frexp <matrix> [where=<boollist>] [axes=<tuplelist>] [axis=<inttuple>] [keepdims=<boolstr>] [casting=<str>] [order=<str>] [dtype=<str>] [subok=<boolstr>]

Parameters
----------
x : array_like
    Array of numbers to be decomposed.
out1 : ndarray, optional
    Output array for the mantissa. Must have the same shape as `x`.
out2 : ndarray, optional
    Output array for the exponent. Must have the same shape as `x`.
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
mantissa : ndarray
    Floating values between -1 and 1.
    This is a scalar if `x` is a scalar.
exponent : ndarray
    Integer exponents of 2.
    This is a scalar if `x` is a scalar.

See Also
--------
ldexp : Compute ``y = x1 * 2**x2``, the inverse of `frexp`.

Notes
-----
Complex dtypes are not supported, they will raise a TypeError.

~~~
