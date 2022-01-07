[[/Matrix/Gett]]

~~~
Syntax
------

matrix getT <matrix>


        Returns the transpose of the matrix.

        Does *not* conjugate!  For the complex conjugate transpose, use ``.H``.

        Parameters
        ----------
        None

        Returns
        -------
        ret : matrix object
            The (non-conjugated) transpose of the matrix.

        See Also
        --------
        transpose, getH

        Examples
        --------
        >>> m = np.matrix('[1, 2; 3, 4]')
        >>> m
        matrix([[1, 2],
                [3, 4]])
        >>> m.getT()
        matrix([[1, 3],
                [2, 4]])

~~~
