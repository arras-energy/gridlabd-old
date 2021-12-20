[[/Matrix/Setfield]]

~~~
Syntax
------

matrix setfield <matrix> <weakcomplex> <str> offset=<int>

a.setfield(val, dtype, offset=0)

    Put a value into a specified place in a field defined by a data-type.

    Place `val` into `a`'s field defined by `dtype` and beginning `offset`
    bytes into the field.

    Parameters
    ----------
    val : object
        Value to be placed in field.
    dtype : dtype object
        Data-type of the field in which to place `val`.
    offset : int, optional
        The number of bytes into the field at which to place `val`.

    Returns
    -------
    None

    See Also
    --------
    getfield

    Examples
    --------
    >>> x = np.eye(3)
    >>> x.getfield(np.float64)
    array([[1.,  0.,  0.],
           [0.,  1.,  0.],
           [0.,  0.,  1.]])
    >>> x.setfield(3, np.int32)
    >>> x.getfield(np.int32)
    array([[3, 3, 3],
           [3, 3, 3],
           [3, 3, 3]], dtype=int32)
    >>> x
    array([[1.0e+000, 1.5e-323, 1.5e-323],
           [1.5e-323, 1.0e+000, 1.5e-323],
           [1.5e-323, 1.5e-323, 1.0e+000]])
    >>> x.setfield(np.eye(3), np.int32)
    >>> x
    array([[1.,  0.,  0.],
           [0.,  1.,  0.],
~~~
