[[/Log1P]] -- Return the natural logarithm of one plus the input array, element-wise.

~~~
Syntax
------

matrix log1p <matrix> [where=<boollist>] [axes=<tuplelist>] [axis=<inttuple>] [keepdims=<boolstr>] [casting=<str>] [order=<str>] [dtype=<str>] [subok=<boolstr>]

Parameters
----------
x : array_like
    Input values.
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
y : ndarray
    Natural logarithm of `1 + x`, element-wise.
    This is a scalar if `x` is a scalar.

See Also
--------
expm1 : ``exp(x) - 1``, the inverse of `log1p`.

Notes
-----
For real-valued input, `log1p` is accurate also for `x` so small
that `1 + x == 1` in floating-point accuracy.

Logarithm is a multivalued function: for each `x` there is an infinite
number of `z` such that `exp(z) = 1 + x`. The convention is to return
the `z` whose imaginary part lies in `[-pi, pi]`.

For real-valued input data types, `log1p` always returns real output.
For each value that cannot be expressed as a real number or infinity,
it yields ``nan`` and sets the `invalid` floating point error flag.

For complex-valued input, `log1p` is a complex analytical function that
has a branch cut `[-inf, -1]` and is continuous from above on it.
`log1p` handles the floating-point negative zero as an infinitesimal
negative number, conforming to the C99 standard.

References
----------
.. [1] M. Abramowitz and I.A. Stegun, "Handbook of Mathematical Functions",
       10th printing, 1964, pp. 67. http://www.math.sfu.ca/~cbm/aands/
.. [2] Wikipedia, "Logarithm". https://en.wikipedia.org/wiki/Logarithm

~~~
