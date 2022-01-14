[[/Angle]]

~~~
Syntax
------

matrix angle <matrix> deg=<boolstr>


    Return the angle of the complex argument.

    Parameters
    ----------
    z : array_like
        A complex number or sequence of complex numbers.
    deg : bool, optional
        Return angle in degrees if True, radians if False (default).

    Returns
    -------
    angle : ndarray or scalar
        The counterclockwise angle from the positive real axis on the complex
        plane in the range ``(-pi, pi]``, with dtype as numpy.float64.

        .. versionchanged:: 1.16.0
            This function works on subclasses of ndarray like `ma.array`.

    See Also
    --------
    arctan2
    absolute

    Notes
    -----
    Although the angle of the complex number 0 is undefined, ``numpy.angle(0)``
    returns the value 0.

    Examples
    --------
    >>> np.angle([1.0, 1.0j, 1+1j])               # in radians
    array([ 0.        ,  1.57079633,  0.78539816]) # may vary
    >>> np.angle(1+1j, deg=True)                  # in degrees
    45.0

~~~
