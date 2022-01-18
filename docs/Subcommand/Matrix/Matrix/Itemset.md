[[/Matrix/Itemset]]

~~~
Syntax
------

matrix itemset <matrix> <inttuple> <weakcomplex>

a.itemset(*args)

    Insert scalar into an array (scalar is cast to array's dtype, if possible)

    There must be at least 1 argument, and define the last argument
    as *item*.  Then, ``a.itemset(*args)`` is equivalent to but faster
    than ``a[args] = item``.  The item should be a scalar value and `args`
    must select a single item in the array `a`.

    Parameters
    ----------
    \*args : Arguments
        If one argument: a scalar, only used in case `a` is of size 1.
        If two arguments: the last argument is the value to be set
        and must be a scalar, the first argument specifies a single array
        element location. It is either an int or a tuple.

    Notes
    -----
    Compared to indexing syntax, `itemset` provides some speed increase
    for placing a scalar into a particular location in an `ndarray`,
    if you must do this.  However, generally this is discouraged:
    among other problems, it complicates the appearance of the code.
    Also, when using `itemset` (and `item`) inside a loop, be sure
    to assign the methods to a local variable to avoid the attribute
    look-up at each loop iteration.

    Examples
    --------
    >>> np.random.seed(123)
    >>> x = np.random.randint(9, size=(3, 3))
    >>> x
    array([[2, 2, 6],
           [1, 3, 6],
           [1, 0, 1]])
    >>> x.itemset(4, 0)
    >>> x.itemset((2, 2), 9)
    >>> x
    array([[2, 2, 6],
           [1, 0, 6],
~~~
