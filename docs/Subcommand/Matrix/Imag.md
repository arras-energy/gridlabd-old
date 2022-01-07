[[/Imag]]

~~~
Syntax
------

matrix imag <matrix>


    Return the imaginary part of the complex argument.

    Parameters
    ----------
    val : array_like
        Input array.

    Returns
    -------
    out : ndarray or scalar
        The imaginary component of the complex argument. If `val` is real,
        the type of `val` is used for the output.  If `val` has complex
        elements, the returned type is float.

    See Also
    --------
    real, angle, real_if_close

    Examples
    --------
    >>> a = np.array([1+2j, 3+4j, 5+6j])
    >>> a.imag
    array([2.,  4.,  6.])
    >>> a.imag = np.array([8, 10, 12])
    >>> a
    array([1. +8.j,  3.+10.j,  5.+12.j])
    >>> np.imag(1 + 1j)
    1.0

~~~
