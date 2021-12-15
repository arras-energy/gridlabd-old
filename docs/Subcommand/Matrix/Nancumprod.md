[[/Nancumprod]]

~~~
Syntax
------

matrix nancumprod <matrix> axis=<int> [dtype=<str>]


    Return the cumulative product of array elements over a given axis treating Not a
    Numbers (NaNs) as one.  The cumulative product does not change when NaNs are
    encountered and leading NaNs are replaced by ones.

    Ones are returned for slices that are all-NaN or empty.

    .. versionadded:: 1.12.0

    Parameters
    ----------
    a : array_like
        Input array.
    axis : int, optional
        Axis along which the cumulative product is computed.  By default
        the input is flattened.
    dtype : dtype, optional
        Type of the returned array, as well as of the accumulator in which
        the elements are multiplied.  If *dtype* is not specified, it
        defaults to the dtype of `a`, unless `a` has an integer dtype with
        a precision less than that of the default platform integer.  In
        that case, the default platform integer is used instead.
    out : ndarray, optional
        Alternative output array in which to place the result. It must
        have the same shape and buffer length as the expected output
        but the type of the resulting values will be cast if necessary.

    Returns
    -------
    nancumprod : ndarray
        A new array holding the result is returned unless `out` is
        specified, in which case it is returned.

    See Also
    --------
    numpy.cumprod : Cumulative product across array propagating NaNs.
    isnan : Show which elements are NaN.

    Examples
    --------
    >>> np.nancumprod(1)
    array([1])
    >>> np.nancumprod([1])
    array([1])
    >>> np.nancumprod([1, np.nan])
    array([1.,  1.])
    >>> a = np.array([[1, 2], [3, np.nan]])
    >>> np.nancumprod(a)
    array([1.,  2.,  6.,  6.])
    >>> np.nancumprod(a, axis=0)
    array([[1.,  2.],
           [3.,  2.]])
    >>> np.nancumprod(a, axis=1)
    array([[1.,  2.],
           [3.,  3.]])

~~~
