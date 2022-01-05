[[/Matlib/Rand]]

~~~
Syntax
------

matrix rand <intlist_args>


    Return a matrix of random values with given shape.

    Create a matrix of the given shape and propagate it with
    random samples from a uniform distribution over ``[0, 1)``.

    Parameters
    ----------
    \*args : Arguments
        Shape of the output.
        If given as N integers, each integer specifies the size of one
        dimension.
        If given as a tuple, this tuple gives the complete shape.

    Returns
    -------
    out : ndarray
        The matrix of random values with shape given by `\*args`.

    See Also
    --------
    randn, numpy.random.RandomState.rand

    Examples
    --------
    >>> np.random.seed(123)
    >>> import numpy.matlib
    >>> np.matlib.rand(2, 3)
    matrix([[0.69646919, 0.28613933, 0.22685145],
            [0.55131477, 0.71946897, 0.42310646]])
    >>> np.matlib.rand((2, 3))
    matrix([[0.9807642 , 0.68482974, 0.4809319 ],
            [0.39211752, 0.34317802, 0.72904971]])

    If the first argument is a tuple, other arguments are ignored:

    >>> np.matlib.rand((2, 3), 4)
    matrix([[0.43857224, 0.0596779 , 0.39804426],
            [0.73799541, 0.18249173, 0.17545176]])

~~~
