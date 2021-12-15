[[/Matrix/Argmax]]

~~~
Syntax
------

matrix argmax <matrix> axis=<int>


        Indexes of the maximum values along an axis.

        Return the indexes of the first occurrences of the maximum values
        along the specified axis.  If axis is None, the index is for the
        flattened matrix.

        Parameters
        ----------
        See `numpy.argmax` for complete descriptions

        See Also
        --------
        numpy.argmax

        Notes
        -----
        This is the same as `ndarray.argmax`, but returns a `matrix` object
        where `ndarray.argmax` would return an `ndarray`.

        Examples
        --------
        >>> x = np.matrix(np.arange(12).reshape((3,4))); x
        matrix([[ 0,  1,  2,  3],
                [ 4,  5,  6,  7],
                [ 8,  9, 10, 11]])
        >>> x.argmax()
        11
        >>> x.argmax(0)
        matrix([[2, 2, 2, 2]])
        >>> x.argmax(1)
        matrix([[3],
                [3],
                [3]])

~~~
