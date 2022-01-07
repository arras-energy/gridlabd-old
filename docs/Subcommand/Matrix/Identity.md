[[/Identity]]

~~~
Syntax
------

matrix identity <int> dtype=<str>


    Return the identity array.

    The identity array is a square array with ones on
    the main diagonal.

    Parameters
    ----------
    n : int
        Number of rows (and columns) in `n` x `n` output.
    dtype : data-type, optional
        Data-type of the output.  Defaults to ``float``.
    like : array_like
        Reference object to allow the creation of arrays which are not
        NumPy arrays. If an array-like passed in as ``like`` supports
        the ``__array_function__`` protocol, the result will be defined
        by it. In this case, it ensures the creation of an array object
        compatible with that passed in via this argument.

        .. versionadded:: 1.20.0

    Returns
    -------
    out : ndarray
        `n` x `n` array with its main diagonal set to one,
        and all other elements 0.

    Examples
    --------
    >>> np.identity(3)
    array([[1.,  0.,  0.],
           [0.,  1.,  0.],
           [0.,  0.,  1.]])

~~~
