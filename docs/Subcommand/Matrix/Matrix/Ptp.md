[[/Matrix/Ptp]]

~~~
Syntax
------

matrix ptp <matrix> axis=<int>


        Peak-to-peak (maximum - minimum) value along the given axis.

        Refer to `numpy.ptp` for full documentation.

        See Also
        --------
        numpy.ptp

        Notes
        -----
        Same as `ndarray.ptp`, except, where that would return an `ndarray` object,
        this returns a `matrix` object.

        Examples
        --------
        >>> x = np.matrix(np.arange(12).reshape((3,4))); x
        matrix([[ 0,  1,  2,  3],
                [ 4,  5,  6,  7],
                [ 8,  9, 10, 11]])
        >>> x.ptp()
        11
        >>> x.ptp(0)
        matrix([[8, 8, 8, 8]])
        >>> x.ptp(1)
        matrix([[3],
                [3],
                [3]])

~~~
