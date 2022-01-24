[[/Arcsinh]] -- Inverse hyperbolic sine element-wise.

~~~
Syntax
------

matrix arcsinh <matrix> [where=<boollist>] [axes=<tuplelist>] [axis=<inttuple>] [keepdims=<boolstr>] [casting=<str>] [order=<str>] [dtype=<str>] [subok=<boolstr>]

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
    Array of the same shape as `x`.
    This is a scalar if `x` is a scalar.

Notes
-----
`arcsinh` is a multivalued function: for each `x` there are infinitely
many numbers `z` such that `sinh(z) = x`. The convention is to return the
`z` whose imaginary part lies in `[-pi/2, pi/2]`.

For real-valued input data types, `arcsinh` always returns real output.
For each value that cannot be expressed as a real number or infinity, it
returns ``nan`` and sets the `invalid` floating point error flag.

For complex-valued input, `arccos` is a complex analytical function that
has branch cuts `[1j, infj]` and `[-1j, -infj]` and is continuous from
the right on the former and from the left on the latter.

The inverse hyperbolic sine is also known as `asinh` or ``sinh^-1``.

References
----------
.. [1] M. Abramowitz and I.A. Stegun, "Handbook of Mathematical Functions",
       10th printing, 1964, pp. 86. http://www.math.sfu.ca/~cbm/aands/
.. [2] Wikipedia, "Inverse hyperbolic function",
       https://en.wikipedia.org/wiki/Arcsinh

~~~
