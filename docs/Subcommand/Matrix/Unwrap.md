[[/Unwrap]]

~~~
Syntax
------

matrix unwrap <matrix> discont=<float> axis=<int>


    Unwrap by changing deltas between values to 2*pi complement.

    Unwrap radian phase `p` by changing absolute jumps greater than
    `discont` to their 2*pi complement along the given axis.

    Parameters
    ----------
    p : array_like
        Input array.
    discont : float, optional
        Maximum discontinuity between values, default is ``pi``.
    axis : int, optional
        Axis along which unwrap will operate, default is the last axis.

    Returns
    -------
    out : ndarray
        Output array.

    See Also
    --------
    rad2deg, deg2rad

    Notes
    -----
    If the discontinuity in `p` is smaller than ``pi``, but larger than
    `discont`, no unwrapping is done because taking the 2*pi complement
    would only make the discontinuity larger.

    Examples
    --------
    >>> phase = np.linspace(0, np.pi, num=5)
    >>> phase[3:] += np.pi
    >>> phase
    array([ 0.        ,  0.78539816,  1.57079633,  5.49778714,  6.28318531]) # may vary
    >>> np.unwrap(phase)
    array([ 0.        ,  0.78539816,  1.57079633, -0.78539816,  0.        ]) # may vary

~~~
