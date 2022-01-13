[[/Fmax]] -- Element-wise maximum of array elements.

~~~
Syntax
------

matrix fmax <matrix> <matrix> [where=<boollist>] [axes=<tuplelist>] [axis=<inttuple>] [keepdims=<boolstr>] [casting=<str>] [order=<str>] [dtype=<str>] [subok=<boolstr>]

Parameters
----------
x1, x2 : array_like
    The arrays holding the elements to be compared.
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
y : ndarray or scalar
    The maximum of `x1` and `x2`, element-wise.
    This is a scalar if both `x1` and `x2` are scalars.

See Also
--------
fmin :
    Element-wise minimum of two arrays, ignores NaNs.
maximum :
    Element-wise maximum of two arrays, propagates NaNs.
amax :
    The maximum value of an array along a given axis, propagates NaNs.
nanmax :
    The maximum value of an array along a given axis, ignores NaNs.

minimum, amin, nanmin

Notes
-----
.. versionadded:: 1.3.0

The fmax is equivalent to ``np.where(x1 >= x2, x1, x2)`` when neither
x1 nor x2 are NaNs, but it is faster and does proper broadcasting.

~~~
