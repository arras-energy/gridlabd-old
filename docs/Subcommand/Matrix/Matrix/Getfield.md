[[/Matrix/Getfield]]

~~~
Syntax
------

matrix getfield <matrix> <str> offset=<int>

a.getfield(dtype, offset=0)

    Returns a field of the given array as a certain type.

    A field is a view of the array data with a given data-type. The values in
    the view are determined by the given type and the offset into the current
    array in bytes. The offset needs to be such that the view dtype fits in the
    array dtype; for example an array of dtype complex128 has 16-byte elements.
    If taking a view with a 32-bit integer (4 bytes), the offset needs to be
    between 0 and 12 bytes.

    Parameters
    ----------
    dtype : str or dtype
        The data type of the view. The dtype size of the view can not be larger
        than that of the array itself.
    offset : int
        Number of bytes to skip before beginning the element view.

    Examples
    --------
    >>> x = np.diag([1.+1.j]*2)
    >>> x[1, 1] = 2 + 4.j
    >>> x
    array([[1.+1.j,  0.+0.j],
           [0.+0.j,  2.+4.j]])
    >>> x.getfield(np.float64)
    array([[1.,  0.],
           [0.,  2.]])

    By choosing an offset of 8 bytes we can select the complex part of the
    array for our view:

    >>> x.getfield(np.float64, offset=8)
    array([[1.,  0.],
~~~
