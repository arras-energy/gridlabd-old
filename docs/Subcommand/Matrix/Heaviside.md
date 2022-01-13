[[/Heaviside]] -- Compute the Heaviside step function.

~~~
Syntax
------

matrix heaviside <matrix> <matrix> [where=<boollist>] [axes=<tuplelist>] [axis=<inttuple>] [keepdims=<boolstr>] [casting=<str>] [order=<str>] [dtype=<str>] [subok=<boolstr>]

Parameters
----------
x1 : array_like
    Input values.
x2 : array_like
    The value of the function when x1 is 0.
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
out : ndarray or scalar
    The output array, element-wise Heaviside step function of `x1`.
    This is a scalar if both `x1` and `x2` are scalars.

Notes
-----
.. versionadded:: 1.13.0

References
----------
.. Wikipedia, "Heaviside step function",
   https://en.wikipedia.org/wiki/Heaviside_step_function

~~~
