[[/Transpose]]

~~~
Syntax
------

matrix transpose <matrix>


    Reverse or permute the axes of an array; returns the modified array.

    For an array a with two axes, transpose(a) gives the matrix transpose.

    Refer to `numpy.ndarray.transpose` for full documentation.

    Parameters
    ----------
    a : array_like
        Input array.
    axes : tuple or list of ints, optional
        If specified, it must be a tuple or list which contains a permutation of
        [0,1,..,N-1] where N is the number of axes of a.  The i'th axis of the
        returned array will correspond to the axis numbered ``axes[i]`` of the
        input.  If not specified, defaults to ``range(a.ndim)[::-1]``, which
        reverses the order of the axes.

    Returns
    -------
    p : ndarray
        `a` with its axes permuted.  A view is returned whenever
        possible.

    See Also
    --------
    ndarray.transpose : Equivalent method
    moveaxis
    argsort

    Notes
    -----
    Use `transpose(a, argsort(axes))` to invert the transposition of tensors
    when using the `axes` keyword argument.

    Transposing a 1-D array returns an unchanged view of the original array.

    Examples
    --------
    >>> x = np.arange(4).reshape((2,2))
    >>> x
    array([[0, 1],
           [2, 3]])

    >>> np.transpose(x)
    array([[0, 2],
           [1, 3]])

    >>> x = np.ones((1, 2, 3))
    >>> np.transpose(x, (1, 0, 2)).shape
    (2, 1, 3)

    >>> x = np.ones((2, 3, 4, 5))
    >>> np.transpose(x).shape
    (5, 4, 3, 2)

~~~
