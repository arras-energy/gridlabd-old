[[/Spacing]] -- Return the distance between x and the nearest adjacent number.

~~~
Syntax
------

matrix spacing <matrix> [where=<boollist>] [axes=<tuplelist>] [axis=<inttuple>] [keepdims=<boolstr>] [casting=<str>] [order=<str>] [dtype=<str>] [subok=<boolstr>]

Parameters
----------
x : array_like
    Values to find the spacing of.
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
    The spacing of values of `x`.
    This is a scalar if `x` is a scalar.

Notes
-----
It can be considered as a generalization of EPS:
``spacing(np.float64(1)) == np.finfo(np.float64).eps``, and there
should not be any representable number between ``x + spacing(x)`` and
x for any finite x.

Spacing of +- inf and NaN is NaN.

~~~
