[[/Matrix/Geti]]

~~~
Syntax
------

matrix getI <matrix>


        Returns the (multiplicative) inverse of invertible `self`.

        Parameters
        ----------
        None

        Returns
        -------
        ret : matrix object
            If `self` is non-singular, `ret` is such that ``ret * self`` ==
            ``self * ret`` == ``np.matrix(np.eye(self[0,:].size))`` all return
            ``True``.

        Raises
        ------
        numpy.linalg.LinAlgError: Singular matrix
            If `self` is singular.

        See Also
        --------
        linalg.inv

        Examples
        --------
        >>> m = np.matrix('[1, 2; 3, 4]'); m
        matrix([[1, 2],
                [3, 4]])
        >>> m.getI()
        matrix([[-2. ,  1. ],
                [ 1.5, -0.5]])
        >>> m.getI() * m
        matrix([[ 1.,  0.], # may vary
                [ 0.,  1.]])

~~~
