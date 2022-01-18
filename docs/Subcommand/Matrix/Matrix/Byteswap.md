[[/Matrix/Byteswap]]

~~~
Syntax
------

matrix byteswap <matrix>

a.byteswap(inplace=False)

    Swap the bytes of the array elements

    Toggle between low-endian and big-endian data representation by
    returning a byteswapped array, optionally swapped in-place.
    Arrays of byte-strings are not swapped. The real and imaginary
    parts of a complex number are swapped individually.

    Parameters
    ----------
    inplace : bool, optional
        If ``True``, swap bytes in-place, default is ``False``.

    Returns
    -------
    out : ndarray
        The byteswapped array. If `inplace` is ``True``, this is
        a view to self.

    Examples
    --------
    >>> A = np.array([1, 256, 8755], dtype=np.int16)
    >>> list(map(hex, A))
    ['0x1', '0x100', '0x2233']
    >>> A.byteswap(inplace=True)
    array([  256,     1, 13090], dtype=int16)
    >>> list(map(hex, A))
    ['0x100', '0x1', '0x3322']

    Arrays of byte-strings are not swapped

    >>> A = np.array([b'ceg', b'fac'])
    >>> A.byteswap()
    array([b'ceg', b'fac'], dtype='|S3')

    ``A.newbyteorder().byteswap()`` produces an array with the same values
      but different representation in memory

    >>> A = np.array([1, 2, 3])
    >>> A.view(np.uint8)
    array([1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0,
           0, 0], dtype=uint8)
    >>> A.newbyteorder().byteswap(inplace=True)
    array([1, 2, 3])
    >>> A.view(np.uint8)
    array([0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
~~~
