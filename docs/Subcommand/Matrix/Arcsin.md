[[/Arcsin]] -- Inverse sine, element-wise.

~~~
Syntax
------

matrix arcsin <matrix> [where=<boollist>] [axes=<tuplelist>] [axis=<inttuple>] [keepdims=<boolstr>] [casting=<str>] [order=<str>] [dtype=<str>] [subok=<boolstr>]

Parameters
----------
x : array_like
    `y`-coordinate on the unit circle.
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
angle : ndarray
    The inverse sine of each element in `x`, in radians and in the
    closed interval ``[-pi/2, pi/2]``.
    This is a scalar if `x` is a scalar.

See Also
--------
sin, cos, arccos, tan, arctan, arctan2, emath.arcsin

Notes
-----
`arcsin` is a multivalued function: for each `x` there are infinitely
many numbers `z` such that :math:`sin(z) = x`.  The convention is to
return the angle `z` whose real part lies in [-pi/2, pi/2].

For real-valued input data types, *arcsin* always returns real output.
For each value that cannot be expressed as a real number or infinity,
it yields ``nan`` and sets the `invalid` floating point error flag.

For complex-valued input, `arcsin` is a complex analytic function that
has, by convention, the branch cuts [-inf, -1] and [1, inf]  and is
continuous from above on the former and from below on the latter.

The inverse sine is also known as `asin` or sin^{-1}.

References
----------
Abramowitz, M. and Stegun, I. A., *Handbook of Mathematical Functions*,
10th printing, New York: Dover, 1964, pp. 79ff.
http://www.math.sfu.ca/~cbm/aands/

~~~
