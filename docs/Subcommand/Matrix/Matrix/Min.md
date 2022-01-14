[[/Matrix/Min]]

~~~
Syntax
------

matrix min <matrix> axis=<int>


        Return the minimum value along an axis.

        Parameters
        ----------
        See `amin` for complete descriptions.

        See Also
        --------
        amin, ndarray.min

        Notes
        -----
        This is the same as `ndarray.min`, but returns a `matrix` object
        where `ndarray.min` would return an ndarray.

        Examples
        --------
        >>> x = -np.matrix(np.arange(12).reshape((3,4))); x
        matrix([[  0,  -1,  -2,  -3],
                [ -4,  -5,  -6,  -7],
                [ -8,  -9, -10, -11]])
        >>> x.min()
        -11
        >>> x.min(0)
        matrix([[ -8,  -9, -10, -11]])
        >>> x.min(1)
        matrix([[ -3],
                [ -7],
                [-11]])

~~~
