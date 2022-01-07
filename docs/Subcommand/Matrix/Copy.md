[[/Copy]]

~~~
Syntax
------

matrix copy <matrix>


    Return an array copy of the given object.

    Parameters
    ----------
    a : array_like
        Input data.
    order : {'C', 'F', 'A', 'K'}, optional
        Controls the memory layout of the copy. 'C' means C-order,
        'F' means F-order, 'A' means 'F' if `a` is Fortran contiguous,
        'C' otherwise. 'K' means match the layout of `a` as closely
        as possible. (Note that this function and :meth:`ndarray.copy` are very
        similar, but have different default values for their order=
        arguments.)
    subok : bool, optional
        If True, then sub-classes will be passed-through, otherwise the
        returned array will be forced to be a base-class array (defaults to False).

        .. versionadded:: 1.19.0

    Returns
    -------
    arr : ndarray
        Array interpretation of `a`.

    See Also
    --------
    ndarray.copy : Preferred method for creating an array copy

    Notes
    -----
    This is equivalent to:

    >>> np.array(a, copy=True)  #doctest: +SKIP

    Examples
    --------
    Create an array x, with a reference y and a copy z:

    >>> x = np.array([1, 2, 3])
    >>> y = x
    >>> z = np.copy(x)

    Note that, when we modify x, y changes, but not z:

    >>> x[0] = 10
    >>> x[0] == y[0]
    True
    >>> x[0] == z[0]
    False

    Note that np.copy is a shallow copy and will not copy object
    elements within arrays. This is mainly important for arrays
    containing Python objects. The new array will contain the
    same object which may lead to surprises if that object can
    be modified (is mutable):

    >>> a = np.array([1, 'm', [2, 3, 4]], dtype=object)
    >>> b = np.copy(a)
    >>> b[2][0] = 10
    >>> a
    array([1, 'm', list([10, 3, 4])], dtype=object)

    To ensure all elements within an ``object`` array are copied,
    use `copy.deepcopy`:

    >>> import copy
    >>> a = np.array([1, 'm', [2, 3, 4]], dtype=object)
    >>> c = copy.deepcopy(a)
    >>> c[2][0] = 10
    >>> c
    array([1, 'm', list([10, 3, 4])], dtype=object)
    >>> a
    array([1, 'm', list([2, 3, 4])], dtype=object)

~~~
