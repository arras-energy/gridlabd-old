[[/Matrix/Resize]]

~~~
Syntax
------

matrix resize <matrix>

a.resize(new_shape, refcheck=True)

    Change shape and size of array in-place.

    Parameters
    ----------
    new_shape : tuple of ints, or `n` ints
        Shape of resized array.
    refcheck : bool, optional
        If False, reference count will not be checked. Default is True.

    Returns
    -------
    None

    Raises
    ------
    ValueError
        If `a` does not own its own data or references or views to it exist,
        and the data memory must be changed.
        PyPy only: will always raise if the data memory must be changed, since
        there is no reliable way to determine if references or views to it
        exist.

    SystemError
        If the `order` keyword argument is specified. This behaviour is a
        bug in NumPy.

    See Also
    --------
    resize : Return a new array with the specified shape.

    Notes
    -----
    This reallocates space for the data area if necessary.

    Only contiguous arrays (data elements consecutive in memory) can be
    resized.

    The purpose of the reference count check is to make sure you
    do not use this array as a buffer for another Python object and then
    reallocate the memory. However, reference counts can increase in
    other ways so if you are sure that you have not shared the memory
    for this array with another Python object, then you may safely set
    `refcheck` to False.

    Examples
    --------
    Shrinking an array: array is flattened (in the order that the data are
    stored in memory), resized, and reshaped:

    >>> a = np.array([[0, 1], [2, 3]], order='C')
    >>> a.resize((2, 1))
    >>> a
    array([[0],
           [1]])

    >>> a = np.array([[0, 1], [2, 3]], order='F')
    >>> a.resize((2, 1))
    >>> a
    array([[0],
           [2]])

    Enlarging an array: as above, but missing entries are filled with zeros:

    >>> b = np.array([[0, 1], [2, 3]])
    >>> b.resize(2, 3) # new_shape parameter doesn't have to be a tuple
    >>> b
    array([[0, 1, 2],
           [3, 0, 0]])

    Referencing an array prevents resizing...

    >>> c = a
    >>> a.resize((1, 1))
    Traceback (most recent call last):
    ...
    ValueError: cannot resize an array that references or is referenced ...

    Unless `refcheck` is False:

    >>> a.resize((1, 1), refcheck=False)
    >>> a
    array([[0]])
    >>> c
~~~
