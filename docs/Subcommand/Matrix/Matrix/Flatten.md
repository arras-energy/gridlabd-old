[[/Matrix/Flatten]]

~~~
Syntax
------

matrix flatten <matrix>


        Return a flattened copy of the matrix.

        All `N` elements of the matrix are placed into a single row.

        Parameters
        ----------
        order : {'C', 'F', 'A', 'K'}, optional
            'C' means to flatten in row-major (C-style) order. 'F' means to
            flatten in column-major (Fortran-style) order. 'A' means to
            flatten in column-major order if `m` is Fortran *contiguous* in
            memory, row-major order otherwise. 'K' means to flatten `m` in
            the order the elements occur in memory. The default is 'C'.

        Returns
        -------
        y : matrix
            A copy of the matrix, flattened to a `(1, N)` matrix where `N`
            is the number of elements in the original matrix.

        See Also
        --------
        ravel : Return a flattened array.
        flat : A 1-D flat iterator over the matrix.

        Examples
        --------
        >>> m = np.matrix([[1,2], [3,4]])
        >>> m.flatten()
        matrix([[1, 2, 3, 4]])
        >>> m.flatten('F')
        matrix([[1, 3, 2, 4]])

~~~
