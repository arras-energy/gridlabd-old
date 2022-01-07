[[/Matrix/Std]]

~~~
Syntax
------

matrix std <matrix> axis=<int> ddof=<int> [dtype=<str>]


        Return the standard deviation of the array elements along the given axis.

        Refer to `numpy.std` for full documentation.

        See Also
        --------
        numpy.std

        Notes
        -----
        This is the same as `ndarray.std`, except that where an `ndarray` would
        be returned, a `matrix` object is returned instead.

        Examples
        --------
        >>> x = np.matrix(np.arange(12).reshape((3, 4)))
        >>> x
        matrix([[ 0,  1,  2,  3],
                [ 4,  5,  6,  7],
                [ 8,  9, 10, 11]])
        >>> x.std()
        3.4520525295346629 # may vary
        >>> x.std(0)
        matrix([[ 3.26598632,  3.26598632,  3.26598632,  3.26598632]]) # may vary
        >>> x.std(1)
        matrix([[ 1.11803399],
                [ 1.11803399],
                [ 1.11803399]])

~~~
