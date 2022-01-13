[[/Matrix/Geth]]

~~~
Syntax
------

matrix getH <matrix>


        Returns the (complex) conjugate transpose of `self`.

        Equivalent to ``np.transpose(self)`` if `self` is real-valued.

        Parameters
        ----------
        None

        Returns
        -------
        ret : matrix object
            complex conjugate transpose of `self`

        Examples
        --------
        >>> x = np.matrix(np.arange(12).reshape((3,4)))
        >>> z = x - 1j*x; z
        matrix([[  0. +0.j,   1. -1.j,   2. -2.j,   3. -3.j],
                [  4. -4.j,   5. -5.j,   6. -6.j,   7. -7.j],
                [  8. -8.j,   9. -9.j,  10.-10.j,  11.-11.j]])
        >>> z.getH()
        matrix([[ 0. -0.j,  4. +4.j,  8. +8.j],
                [ 1. +1.j,  5. +5.j,  9. +9.j],
                [ 2. +2.j,  6. +6.j, 10.+10.j],
                [ 3. +3.j,  7. +7.j, 11.+11.j]])

~~~
