[[/Random/Rand]]

~~~
Syntax
------

matrix rand <intlist_args>


        rand(d0, d1, ..., dn)

        Random values in a given shape.

        .. note::
            This is a convenience function for users porting code from Matlab,
            and wraps `random_sample`. That function takes a
            tuple to specify the size of the output, which is consistent with
            other NumPy functions like `numpy.zeros` and `numpy.ones`.

        Create an array of the given shape and populate it with
        random samples from a uniform distribution
        over ``[0, 1)``.

        Parameters
        ----------
        d0, d1, ..., dn : int, optional
            The dimensions of the returned array, must be non-negative.
            If no argument is given a single Python float is returned.

        Returns
        -------
        out : ndarray, shape ``(d0, d1, ..., dn)``
            Random values.

        See Also
        --------
        random

        Examples
        --------
        >>> np.random.rand(3,2)
        array([[ 0.14022471,  0.96360618],  #random
               [ 0.37601032,  0.25528411],  #random
               [ 0.49313049,  0.94909878]]) #random

~~~
