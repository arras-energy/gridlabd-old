[[/Matlib/Repmat]]

~~~
Syntax
------

matrix repmat <matrix> <int> <int>


    Repeat a 0-D to 2-D array or matrix MxN times.

    Parameters
    ----------
    a : array_like
        The array or matrix to be repeated.
    m, n : int
        The number of times `a` is repeated along the first and second axes.

    Returns
    -------
    out : ndarray
        The result of repeating `a`.

    Examples
    --------
    >>> import numpy.matlib
    >>> a0 = np.array(1)
    >>> np.matlib.repmat(a0, 2, 3)
    array([[1, 1, 1],
           [1, 1, 1]])

    >>> a1 = np.arange(4)
    >>> np.matlib.repmat(a1, 2, 2)
    array([[0, 1, 2, 3, 0, 1, 2, 3],
           [0, 1, 2, 3, 0, 1, 2, 3]])

    >>> a2 = np.asmatrix(np.arange(6).reshape(2, 3))
    >>> np.matlib.repmat(a2, 2, 3)
    matrix([[0, 1, 2, 0, 1, 2, 0, 1, 2],
            [3, 4, 5, 3, 4, 5, 3, 4, 5],
            [0, 1, 2, 0, 1, 2, 0, 1, 2],
            [3, 4, 5, 3, 4, 5, 3, 4, 5]])

~~~
