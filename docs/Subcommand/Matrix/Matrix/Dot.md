[[/Matrix/Dot]]

~~~
Syntax
------

matrix dot <matrix> <matrix>

a.dot(b, out=None)

    Dot product of two arrays.

    Refer to `numpy.dot` for full documentation.

    See Also
    --------
    numpy.dot : equivalent function

    Examples
    --------
    >>> a = np.eye(2)
    >>> b = np.ones((2, 2)) * 2
    >>> a.dot(b)
    array([[2.,  2.],
           [2.,  2.]])

    This array method can be conveniently chained:

    >>> a.dot(b).dot(b)
    array([[8.,  8.],
~~~
