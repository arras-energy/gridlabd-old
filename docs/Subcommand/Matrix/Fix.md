[[/Fix]]

~~~
Syntax
------

matrix fix <matrix>


    Round to nearest integer towards zero.

    Round an array of floats element-wise to nearest integer towards zero.
    The rounded values are returned as floats.

    Parameters
    ----------
    x : array_like
        An array of floats to be rounded
    out : ndarray, optional
        A location into which the result is stored. If provided, it must have
        a shape that the input broadcasts to. If not provided or None, a
        freshly-allocated array is returned.

    Returns
    -------
    out : ndarray of floats
        A float array with the same dimensions as the input.
        If second argument is not supplied then a float array is returned
        with the rounded values.

        If a second argument is supplied the result is stored there.
        The return value `out` is then a reference to that array.

    See Also
    --------
    rint, trunc, floor, ceil
    around : Round to given number of decimals

    Examples
    --------
    >>> np.fix(3.14)
    3.0
    >>> np.fix(3)
    3.0
    >>> np.fix([2.1, 2.9, -2.1, -2.9])
    array([ 2.,  2., -2., -2.])

~~~
