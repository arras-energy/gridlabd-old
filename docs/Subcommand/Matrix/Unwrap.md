[[/Unwrap]]

~~~
Syntax
------

matrix unwrap <matrix> discont=<float> axis=<int> period=<float>


    Unwrap by taking the complement of large deltas with respect to the period.

    This unwraps a signal `p` by changing elements which have an absolute
    difference from their predecessor of more than ``max(discont, period/2)``
    to their `period`-complementary values.

    For the default case where `period` is :math:`2\pi` and is `discont` is
    :math:`\pi`, this unwraps a radian phase `p` such that adjacent differences
    are never greater than :math:`\pi` by adding :math:`2k\pi` for some
    integer :math:`k`.

    Parameters
    ----------
    p : array_like
        Input array.
    discont : float, optional
        Maximum discontinuity between values, default is ``period/2``. 
        Values below ``period/2`` are treated as if they were ``period/2``.
        To have an effect different from the default, `discont` should be
        larger than ``period/2``.
    axis : int, optional
        Axis along which unwrap will operate, default is the last axis.
    period: float, optional
        Size of the range over which the input wraps. By default, it is
        ``2 pi``.
        
        .. versionadded:: 1.21.0

    Returns
    -------
    out : ndarray
        Output array.

    See Also
    --------
    rad2deg, deg2rad

    Notes
    -----
    If the discontinuity in `p` is smaller than ``period/2``, 
    but larger than `discont`, no unwrapping is done because taking 
    the complement would only make the discontinuity larger.

    Examples
    --------
    >>> phase = np.linspace(0, np.pi, num=5)
    >>> phase[3:] += np.pi
    >>> phase
    array([ 0.        ,  0.78539816,  1.57079633,  5.49778714,  6.28318531]) # may vary
    >>> np.unwrap(phase)
    array([ 0.        ,  0.78539816,  1.57079633, -0.78539816,  0.        ]) # may vary
    >>> np.unwrap([0, 1, 2, -1, 0], period=4)
    array([0, 1, 2, 3, 4])
    >>> np.unwrap([ 1, 2, 3, 4, 5, 6, 1, 2, 3], period=6)
    array([1, 2, 3, 4, 5, 6, 7, 8, 9])
    >>> np.unwrap([2, 3, 4, 5, 2, 3, 4, 5], period=4)
    array([2, 3, 4, 5, 6, 7, 8, 9])
    >>> phase_deg = np.mod(np.linspace(0 ,720, 19), 360) - 180
    >>> np.unwrap(phase_deg, period=360)
    array([-180., -140., -100.,  -60.,  -20.,   20.,   60.,  100.,  140.,
            180.,  220.,  260.,  300.,  340.,  380.,  420.,  460.,  500.,
            540.])
~~~
