[[/Matrix/Partition]]

~~~
Syntax
------

matrix partition <matrix> <inttuple> axis=<int> kind=<str> order=<strlist>

a.partition(kth, axis=-1, kind='introselect', order=None)

    Rearranges the elements in the array in such a way that the value of the
    element in kth position is in the position it would be in a sorted array.
    All elements smaller than the kth element are moved before this element and
    all equal or greater are moved behind it. The ordering of the elements in
    the two partitions is undefined.

    .. versionadded:: 1.8.0

    Parameters
    ----------
    kth : int or sequence of ints
        Element index to partition by. The kth element value will be in its
        final sorted position and all smaller elements will be moved before it
        and all equal or greater elements behind it.
        The order of all elements in the partitions is undefined.
        If provided with a sequence of kth it will partition all elements
        indexed by kth of them into their sorted position at once.
    axis : int, optional
        Axis along which to sort. Default is -1, which means sort along the
        last axis.
    kind : {'introselect'}, optional
        Selection algorithm. Default is 'introselect'.
    order : str or list of str, optional
        When `a` is an array with fields defined, this argument specifies
        which fields to compare first, second, etc. A single field can
        be specified as a string, and not all fields need to be specified,
        but unspecified fields will still be used, in the order in which
        they come up in the dtype, to break ties.

    See Also
    --------
    numpy.partition : Return a parititioned copy of an array.
    argpartition : Indirect partition.
    sort : Full sort.

    Notes
    -----
    See ``np.partition`` for notes on the different algorithms.

    Examples
    --------
    >>> a = np.array([3, 4, 2, 1])
    >>> a.partition(3)
    >>> a
    array([2, 1, 3, 4])

    >>> a.partition((1, 3))
    >>> a
~~~
