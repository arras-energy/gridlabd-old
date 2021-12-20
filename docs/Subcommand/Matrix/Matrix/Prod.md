[[/Matrix/Prod]]

~~~
Syntax
------

matrix prod <matrix> axis=<int> [dtype=<str>]


        Return the product of the array elements over the given axis.

        Refer to `prod` for full documentation.

        See Also
        --------
        prod, ndarray.prod

        Notes
        -----
        Same as `ndarray.prod`, except, where that returns an `ndarray`, this
        returns a `matrix` object instead.

        Examples
        --------
        >>> x = np.matrix(np.arange(12).reshape((3,4))); x
        matrix([[ 0,  1,  2,  3],
                [ 4,  5,  6,  7],
                [ 8,  9, 10, 11]])
        >>> x.prod()
        0
        >>> x.prod(0)
        matrix([[  0,  45, 120, 231]])
        >>> x.prod(1)
        matrix([[   0],
                [ 840],
                [7920]])

~~~
