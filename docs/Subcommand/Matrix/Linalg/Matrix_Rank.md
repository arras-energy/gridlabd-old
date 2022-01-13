[[/Linalg/Matrix_Rank]]

~~~
Syntax
------

matrix matrix_rank <matrix>


    Return matrix rank of array using SVD method

    Rank of the array is the number of singular values of the array that are
    greater than `tol`.

    .. versionchanged:: 1.14
       Can now operate on stacks of matrices

    Parameters
    ----------
    M : {(M,), (..., M, N)} array_like
        Input vector or stack of matrices.
    tol : (...) array_like, float, optional
        Threshold below which SVD values are considered zero. If `tol` is
        None, and ``S`` is an array with singular values for `M`, and
        ``eps`` is the epsilon value for datatype of ``S``, then `tol` is
        set to ``S.max() * max(M.shape) * eps``.

        .. versionchanged:: 1.14
           Broadcasted against the stack of matrices
    hermitian : bool, optional
        If True, `M` is assumed to be Hermitian (symmetric if real-valued),
        enabling a more efficient method for finding singular values.
        Defaults to False.

        .. versionadded:: 1.14

    Returns
    -------
    rank : (...) array_like
        Rank of M.

    Notes
    -----
    The default threshold to detect rank deficiency is a test on the magnitude
    of the singular values of `M`.  By default, we identify singular values less
    than ``S.max() * max(M.shape) * eps`` as indicating rank deficiency (with
    the symbols defined above). This is the algorithm MATLAB uses [1].  It also
    appears in *Numerical recipes* in the discussion of SVD solutions for linear
    least squares [2].

    This default threshold is designed to detect rank deficiency accounting for
    the numerical errors of the SVD computation.  Imagine that there is a column
    in `M` that is an exact (in floating point) linear combination of other
    columns in `M`. Computing the SVD on `M` will not produce a singular value
    exactly equal to 0 in general: any difference of the smallest SVD value from
    0 will be caused by numerical imprecision in the calculation of the SVD.
    Our threshold for small SVD values takes this numerical imprecision into
    account, and the default threshold will detect such numerical rank
    deficiency.  The threshold may declare a matrix `M` rank deficient even if
    the linear combination of some columns of `M` is not exactly equal to
    another column of `M` but only numerically very close to another column of
    `M`.

    We chose our default threshold because it is in wide use.  Other thresholds
    are possible.  For example, elsewhere in the 2007 edition of *Numerical
    recipes* there is an alternative threshold of ``S.max() *
    np.finfo(M.dtype).eps / 2. * np.sqrt(m + n + 1.)``. The authors describe
    this threshold as being based on "expected roundoff error" (p 71).

    The thresholds above deal with floating point roundoff error in the
    calculation of the SVD.  However, you may have more information about the
    sources of error in `M` that would make you consider other tolerance values
    to detect *effective* rank deficiency.  The most useful measure of the
    tolerance depends on the operations you intend to use on your matrix.  For
    example, if your data come from uncertain measurements with uncertainties
    greater than floating point epsilon, choosing a tolerance near that
    uncertainty may be preferable.  The tolerance may be absolute if the
    uncertainties are absolute rather than relative.

    References
    ----------
    .. [1] MATLAB reference documention, "Rank"
           https://www.mathworks.com/help/techdoc/ref/rank.html
    .. [2] W. H. Press, S. A. Teukolsky, W. T. Vetterling and B. P. Flannery,
           "Numerical Recipes (3rd edition)", Cambridge University Press, 2007,
           page 795.

    Examples
    --------
    >>> from numpy.linalg import matrix_rank
    >>> matrix_rank(np.eye(4)) # Full rank matrix
    4
    >>> I=np.eye(4); I[-1,-1] = 0. # rank deficient matrix
    >>> matrix_rank(I)
    3
    >>> matrix_rank(np.ones((4,))) # 1 dimension - rank 1 unless all 0
    1
    >>> matrix_rank(np.zeros((4,)))
    0
~~~
