[[/Arctan2]] -- Element-wise arc tangent of ``x1/x2`` choosing the quadrant correctly.

~~~
Syntax
------

matrix arctan2 <matrix> <matrix> [where=<boollist>] [axes=<tuplelist>] [axis=<inttuple>] [keepdims=<boolstr>] [casting=<str>] [order=<str>] [dtype=<str>] [subok=<boolstr>]

Parameters
----------
x1 : array_like, real-valued
    `y`-coordinates.
x2 : array_like, real-valued
    `x`-coordinates.
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
angle : ndarray
    Array of angles in radians, in the range ``[-pi, pi]``.
    This is a scalar if both `x1` and `x2` are scalars.

See Also
--------
arctan, tan, angle

Notes
-----
*arctan2* is identical to the `atan2` function of the underlying
C library.  The following special values are defined in the C
standard: [1]_

====== ====== ================
`x1`   `x2`   `arctan2(x1,x2)`
====== ====== ================
+/- 0  +0     +/- 0
+/- 0  -0     +/- pi
 > 0   +/-inf +0 / +pi
 < 0   +/-inf -0 / -pi
+/-inf +inf   +/- (pi/4)
+/-inf -inf   +/- (3*pi/4)
====== ====== ================

Note that +0 and -0 are distinct floating point numbers, as are +inf
and -inf.

References
----------
.. [1] ISO/IEC standard 9899:1999, "Programming language C."

~~~
