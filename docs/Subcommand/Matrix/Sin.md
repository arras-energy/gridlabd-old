[[/Sin]] -- Trigonometric sine, element-wise.

~~~
Syntax
------

matrix sin <matrix> [where=<boollist>] [axes=<tuplelist>] [axis=<inttuple>] [keepdims=<boolstr>] [casting=<str>] [order=<str>] [dtype=<str>] [subok=<boolstr>]

Parameters
----------
x : array_like
    Angle, in radians (:math:`2 \pi` rad equals 360 degrees).
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
    The sine of each element of x.
    This is a scalar if `x` is a scalar.

See Also
--------
arcsin, sinh, cos

Notes
-----
The sine is one of the fundamental functions of trigonometry (the
mathematical study of triangles).  Consider a circle of radius 1
centered on the origin.  A ray comes in from the :math:`+x` axis, makes
an angle at the origin (measured counter-clockwise from that axis), and
departs from the origin.  The :math:`y` coordinate of the outgoing
ray's intersection with the unit circle is the sine of that angle.  It
ranges from -1 for :math:`x=3\pi / 2` to +1 for :math:`\pi / 2.`  The
function has zeroes where the angle is a multiple of :math:`\pi`.
Sines of angles between :math:`\pi` and :math:`2\pi` are negative.
The numerous properties of the sine and related functions are included
in any standard trigonometry text.

~~~
