[[/Linalg/Qr]]

~~~
Syntax
------

matrix qr <matrix> mode=<str>


    Compute the qr factorization of a matrix.

    Factor the matrix `a` as *qr*, where `q` is orthonormal and `r` is
    upper-triangular.

    Parameters
    ----------
    a : array_like, shape (M, N)
        Matrix to be factored.
    mode : {'reduced', 'complete', 'r', 'raw'}, optional
        If K = min(M, N), then

        * 'reduced'  : returns q, r with dimensions (M, K), (K, N) (default)
        * 'complete' : returns q, r with dimensions (M, M), (M, N)
        * 'r'        : returns r only with dimensions (K, N)
        * 'raw'      : returns h, tau with dimensions (N, M), (K,)

        The options 'reduced', 'complete, and 'raw' are new in numpy 1.8,
        see the notes for more information. The default is 'reduced', and to
        maintain backward compatibility with earlier versions of numpy both
        it and the old default 'full' can be omitted. Note that array h
        returned in 'raw' mode is transposed for calling Fortran. The
        'economic' mode is deprecated.  The modes 'full' and 'economic' may
        be passed using only the first letter for backwards compatibility,
        but all others must be spelled out. See the Notes for more
        explanation.


    Returns
    -------
    q : ndarray of float or complex, optional
        A matrix with orthonormal columns. When mode = 'complete' the
        result is an orthogonal/unitary matrix depending on whether or not
        a is real/complex. The determinant may be either +/- 1 in that
        case.
    r : ndarray of float or complex, optional
        The upper-triangular matrix.
    (h, tau) : ndarrays of np.double or np.cdouble, optional
        The array h contains the Householder reflectors that generate q
        along with r. The tau array contains scaling factors for the
        reflectors. In the deprecated  'economic' mode only h is returned.

    Raises
    ------
    LinAlgError
        If factoring fails.

    See Also
    --------
    scipy.linalg.qr : Similar function in SciPy.
    scipy.linalg.rq : Compute RQ decomposition of a matrix.

    Notes
    -----
    This is an interface to the LAPACK routines ``dgeqrf``, ``zgeqrf``,
    ``dorgqr``, and ``zungqr``.

    For more information on the qr factorization, see for example:
    https://en.wikipedia.org/wiki/QR_factorization

    Subclasses of `ndarray` are preserved except for the 'raw' mode. So if
    `a` is of type `matrix`, all the return values will be matrices too.

    New 'reduced', 'complete', and 'raw' options for mode were added in
    NumPy 1.8.0 and the old option 'full' was made an alias of 'reduced'.  In
    addition the options 'full' and 'economic' were deprecated.  Because
    'full' was the previous default and 'reduced' is the new default,
    backward compatibility can be maintained by letting `mode` default.
    The 'raw' option was added so that LAPACK routines that can multiply
    arrays by q using the Householder reflectors can be used. Note that in
    this case the returned arrays are of type np.double or np.cdouble and
    the h array is transposed to be FORTRAN compatible.  No routines using
    the 'raw' return are currently exposed by numpy, but some are available
    in lapack_lite and just await the necessary work.

    Examples
    --------
    >>> a = np.random.randn(9, 6)
    >>> q, r = np.linalg.qr(a)
    >>> np.allclose(a, np.dot(q, r))  # a does equal qr
    True
    >>> r2 = np.linalg.qr(a, mode='r')
    >>> np.allclose(r, r2)  # mode='r' returns the same r as mode='full'
    True

    Example illustrating a common use of `qr`: solving of least squares
    problems

    What are the least-squares-best `m` and `y0` in ``y = y0 + mx`` for
    the following data: {(0,1), (1,0), (1,2), (2,1)}. (Graph the points
    and you'll see that it should be y0 = 0, m = 1.)  The answer is provided
    by solving the over-determined matrix equation ``Ax = b``, where::

      A = array([[0, 1], [1, 1], [1, 1], [2, 1]])
      x = array([[y0], [m]])
      b = array([[1], [0], [2], [1]])

    If A = qr such that q is orthonormal (which is always possible via
    Gram-Schmidt), then ``x = inv(r) * (q.T) * b``.  (In numpy practice,
    however, we simply use `lstsq`.)

    >>> A = np.array([[0, 1], [1, 1], [1, 1], [2, 1]])
    >>> A
    array([[0, 1],
           [1, 1],
           [1, 1],
           [2, 1]])
    >>> b = np.array([1, 0, 2, 1])
    >>> q, r = np.linalg.qr(A)
    >>> p = np.dot(q.T, b)
    >>> np.dot(np.linalg.inv(r), p)
    array([  1.1e-16,   1.0e+00])

~~~
