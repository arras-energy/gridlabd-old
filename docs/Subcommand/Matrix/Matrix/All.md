[[/Matrix/All]]

~~~
Syntax
------

matrix all <matrix> axis=<int>


        Test whether all matrix elements along a given axis evaluate to True.

        Parameters
        ----------
        See `numpy.all` for complete descriptions

        See Also
        --------
        numpy.all

        Notes
        -----
        This is the same as `ndarray.all`, but it returns a `matrix` object.

        Examples
        --------
        >>> x = np.matrix(np.arange(12).reshape((3,4))); x
        matrix([[ 0,  1,  2,  3],
                [ 4,  5,  6,  7],
                [ 8,  9, 10, 11]])
        >>> y = x[0]; y
        matrix([[0, 1, 2, 3]])
        >>> (x == y)
        matrix([[ True,  True,  True,  True],
                [False, False, False, False],
                [False, False, False, False]])
        >>> (x == y).all()
        False
        >>> (x == y).all(0)
        matrix([[False, False, False, False]])
        >>> (x == y).all(1)
        matrix([[ True],
                [False],
                [False]])

~~~
