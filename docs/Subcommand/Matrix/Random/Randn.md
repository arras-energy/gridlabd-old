[[/Random/Randn]]

~~~
Syntax
------

matrix randn <intlist_args>


        randn(d0, d1, ..., dn)

        Return a sample (or samples) from the "standard normal" distribution.

        .. note::
            This is a convenience function for users porting code from Matlab,
            and wraps `standard_normal`. That function takes a
            tuple to specify the size of the output, which is consistent with
            other NumPy functions like `numpy.zeros` and `numpy.ones`.

        .. note::
            New code should use the ``standard_normal`` method of a ``default_rng()``
            instance instead; please see the :ref:`random-quick-start`.

        If positive int_like arguments are provided, `randn` generates an array
        of shape ``(d0, d1, ..., dn)``, filled
        with random floats sampled from a univariate "normal" (Gaussian)
        distribution of mean 0 and variance 1. A single float randomly sampled
        from the distribution is returned if no argument is provided.

        Parameters
        ----------
        d0, d1, ..., dn : int, optional
            The dimensions of the returned array, must be non-negative.
            If no argument is given a single Python float is returned.

        Returns
        -------
        Z : ndarray or float
            A ``(d0, d1, ..., dn)``-shaped array of floating-point samples from
            the standard normal distribution, or a single such float if
            no parameters were supplied.

        See Also
        --------
        standard_normal : Similar, but takes a tuple as its argument.
        normal : Also accepts mu and sigma arguments.
        Generator.standard_normal: which should be used for new code.

        Notes
        -----
        For random samples from :math:`N(\mu, \sigma^2)`, use:

        ``sigma * np.random.randn(...) + mu``

        Examples
        --------
        >>> np.random.randn()
        2.1923875335537315  # random

        Two-by-four array of samples from N(3, 6.25):

        >>> 3 + 2.5 * np.random.randn(2, 4)
        array([[-4.49401501,  4.00950034, -1.81814867,  7.29718677],   # random
               [ 0.39924804,  4.68456316,  4.99394529,  4.84057254]])  # random

~~~
