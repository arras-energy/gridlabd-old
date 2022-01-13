[[/Matrix/Sum]]

~~~
Syntax
------

matrix sum <matrix> [axis=<inttuple>] [dtype=<str>]


        Returns the sum of the matrix elements, along the given axis.

        Refer to `numpy.sum` for full documentation.

        See Also
        --------
        numpy.sum

        Notes
        -----
        This is the same as `ndarray.sum`, except that where an `ndarray` would
        be returned, a `matrix` object is returned instead.

        Examples
        --------
        >>> x = np.matrix([[1, 2], [4, 3]])
        >>> x.sum()
        10
        >>> x.sum(axis=1)
        matrix([[3],
                [7]])
        >>> x.sum(axis=1, dtype='float')
        matrix([[3.],
                [7.]])
        >>> out = np.zeros((2, 1), dtype='float')
        >>> x.sum(axis=1, dtype='float', out=np.asmatrix(out))
        matrix([[3.],
                [7.]])

~~~
