[[/Matrix/Fill]]

~~~
Syntax
------

matrix fill <matrix> <weakcomplex>

a.fill(value)

    Fill the array with a scalar value.

    Parameters
    ----------
    value : scalar
        All elements of `a` will be assigned this value.

    Examples
    --------
    >>> a = np.array([1, 2])
    >>> a.fill(0)
    >>> a
    array([0, 0])
    >>> a = np.empty(2)
    >>> a.fill(1)
    >>> a
~~~
