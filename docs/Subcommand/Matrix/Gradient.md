[[/Gradient]]

~~~
Syntax
------

matrix gradient <matrix> spacing=<matrix> edge_order=<int> [axis=<inttuple>]


    Return the gradient of an N-dimensional array.

    The gradient is computed using second order accurate central differences
    in the interior points and either first or second order accurate one-sides
    (forward or backwards) differences at the boundaries.
    The returned gradient hence has the same shape as the input array.

    Parameters
    ----------
    f : array_like
        An N-dimensional array containing samples of a scalar function.
    varargs : list of scalar or array, optional
        Spacing between f values. Default unitary spacing for all dimensions.
        Spacing can be specified using:

        1. single scalar to specify a sample distance for all dimensions.
        2. N scalars to specify a constant sample distance for each dimension.
           i.e. `dx`, `dy`, `dz`, ...
        3. N arrays to specify the coordinates of the values along each
           dimension of F. The length of the array must match the size of
           the corresponding dimension
        4. Any combination of N scalars/arrays with the meaning of 2. and 3.

        If `axis` is given, the number of varargs must equal the number of axes.
        Default: 1.

    edge_order : {1, 2}, optional
        Gradient is calculated using N-th order accurate differences
        at the boundaries. Default: 1.

        .. versionadded:: 1.9.1

    axis : None or int or tuple of ints, optional
        Gradient is calculated only along the given axis or axes
        The default (axis = None) is to calculate the gradient for all the axes
        of the input array. axis may be negative, in which case it counts from
        the last to the first axis.

        .. versionadded:: 1.11.0

    Returns
    -------
    gradient : ndarray or list of ndarray
        A list of ndarrays (or a single ndarray if there is only one dimension)
        corresponding to the derivatives of f with respect to each dimension.
        Each derivative has the same shape as f.

    Examples
    --------
    >>> f = np.array([1, 2, 4, 7, 11, 16], dtype=float)
    >>> np.gradient(f)
    array([1. , 1.5, 2.5, 3.5, 4.5, 5. ])
    >>> np.gradient(f, 2)
    array([0.5 ,  0.75,  1.25,  1.75,  2.25,  2.5 ])

    Spacing can be also specified with an array that represents the coordinates
    of the values F along the dimensions.
    For instance a uniform spacing:

    >>> x = np.arange(f.size)
    >>> np.gradient(f, x)
    array([1. ,  1.5,  2.5,  3.5,  4.5,  5. ])

    Or a non uniform one:

    >>> x = np.array([0., 1., 1.5, 3.5, 4., 6.], dtype=float)
    >>> np.gradient(f, x)
    array([1. ,  3. ,  3.5,  6.7,  6.9,  2.5])

    For two dimensional arrays, the return will be two arrays ordered by
    axis. In this example the first array stands for the gradient in
    rows and the second one in columns direction:

    >>> np.gradient(np.array([[1, 2, 6], [3, 4, 5]], dtype=float))
    [array([[ 2.,  2., -1.],
           [ 2.,  2., -1.]]), array([[1. , 2.5, 4. ],
           [1. , 1. , 1. ]])]

    In this example the spacing is also specified:
    uniform for axis=0 and non uniform for axis=1

    >>> dx = 2.
    >>> y = [1., 1.5, 3.5]
    >>> np.gradient(np.array([[1, 2, 6], [3, 4, 5]], dtype=float), dx, y)
    [array([[ 1. ,  1. , -0.5],
           [ 1. ,  1. , -0.5]]), array([[2. , 2. , 2. ],
           [2. , 1.7, 0.5]])]

    It is possible to specify how boundaries are treated using `edge_order`

    >>> x = np.array([0, 1, 2, 3, 4])
    >>> f = x**2
    >>> np.gradient(f, edge_order=1)
    array([1.,  2.,  4.,  6.,  7.])
    >>> np.gradient(f, edge_order=2)
    array([0., 2., 4., 6., 8.])

    The `axis` keyword can be used to specify a subset of axes of which the
    gradient is calculated

    >>> np.gradient(np.array([[1, 2, 6], [3, 4, 5]], dtype=float), axis=0)
    array([[ 2.,  2., -1.],
           [ 2.,  2., -1.]])

    Notes
    -----
    Assuming that :math:`f\in C^{3}` (i.e., :math:`f` has at least 3 continuous
    derivatives) and let :math:`h_{*}` be a non-homogeneous stepsize, we
    minimize the "consistency error" :math:`\eta_{i}` between the true gradient
    and its estimate from a linear combination of the neighboring grid-points:

    .. math::

        \eta_{i} = f_{i}^{\left(1\right)} -
                    \left[ \alpha f\left(x_{i}\right) +
                            \beta f\left(x_{i} + h_{d}\right) +
                            \gamma f\left(x_{i}-h_{s}\right)
                    \right]

    By substituting :math:`f(x_{i} + h_{d})` and :math:`f(x_{i} - h_{s})`
    with their Taylor series expansion, this translates into solving
    the following the linear system:

    .. math::

        \left\{
            \begin{array}{r}
                \alpha+\beta+\gamma=0 \\
                \beta h_{d}-\gamma h_{s}=1 \\
                \beta h_{d}^{2}+\gamma h_{s}^{2}=0
            \end{array}
        \right.

    The resulting approximation of :math:`f_{i}^{(1)}` is the following:

    .. math::

        \hat f_{i}^{(1)} =
            \frac{
                h_{s}^{2}f\left(x_{i} + h_{d}\right)
                + \left(h_{d}^{2} - h_{s}^{2}\right)f\left(x_{i}\right)
                - h_{d}^{2}f\left(x_{i}-h_{s}\right)}
                { h_{s}h_{d}\left(h_{d} + h_{s}\right)}
            + \mathcal{O}\left(\frac{h_{d}h_{s}^{2}
                                + h_{s}h_{d}^{2}}{h_{d}
                                + h_{s}}\right)

    It is worth noting that if :math:`h_{s}=h_{d}`
    (i.e., data are evenly spaced)
    we find the standard second order approximation:

    .. math::

        \hat f_{i}^{(1)}=
            \frac{f\left(x_{i+1}\right) - f\left(x_{i-1}\right)}{2h}
            + \mathcal{O}\left(h^{2}\right)

    With a similar procedure the forward/backward approximations used for
    boundaries can be derived.

    References
    ----------
    .. [1]  Quarteroni A., Sacco R., Saleri F. (2007) Numerical Mathematics
            (Texts in Applied Mathematics). New York: Springer.
    .. [2]  Durran D. R. (1999) Numerical Methods for Wave Equations
            in Geophysical Fluid Dynamics. New York: Springer.
    .. [3]  Fornberg B. (1988) Generation of Finite Difference Formulas on
            Arbitrarily Spaced Grids,
            Mathematics of Computation 51, no. 184 : 699-706.
            `PDF <http://www.ams.org/journals/mcom/1988-51-184/
            S0025-5718-1988-0935077-0/S0025-5718-1988-0935077-0.pdf>`_.
~~~
