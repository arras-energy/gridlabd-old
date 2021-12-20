[[/Matrix/Sort]]

~~~
Syntax
------

matrix sort <matrix> axis=<int> kind=<str> order=<strlist>

a.sort(axis=-1, kind=None, order=None)

    Sort an array in-place. Refer to `numpy.sort` for full documentation.

    Parameters
    ----------
    axis : int, optional
        Axis along which to sort. Default is -1, which means sort along the
        last axis.
    kind : {'quicksort', 'mergesort', 'heapsort', 'stable'}, optional
        Sorting algorithm. The default is 'quicksort'. Note that both 'stable'
        and 'mergesort' use timsort under the covers and, in general, the
        actual implementation will vary with datatype. The 'mergesort' option
        is retained for backwards compatibility.

        .. versionchanged:: 1.15.0
           The 'stable' option was added.

    order : str or list of str, optional
        When `a` is an array with fields defined, this argument specifies
        which fields to compare first, second, etc.  A single field can
        be specified as a string, and not all fields need be specified,
        but unspecified fields will still be used, in the order in which
        they come up in the dtype, to break ties.

    See Also
    --------
    numpy.sort : Return a sorted copy of an array.
    numpy.argsort : Indirect sort.
    numpy.lexsort : Indirect stable sort on multiple keys.
    numpy.searchsorted : Find elements in sorted array.
    numpy.partition: Partial sort.

    Notes
    -----
    See `numpy.sort` for notes on the different sorting algorithms.

    Examples
    --------
    >>> a = np.array([[1,4], [3,1]])
    >>> a.sort(axis=1)
    >>> a
    array([[1, 4],
           [1, 3]])
    >>> a.sort(axis=0)
    >>> a
    array([[1, 3],
           [1, 4]])

    Use the `order` keyword to specify a field to use when sorting a
    structured array:

    >>> a = np.array([('a', 2), ('c', 1)], dtype=[('x', 'S1'), ('y', int)])
    >>> a.sort(order='y')
    >>> a
    array([(b'c', 1), (b'a', 2)],
~~~
