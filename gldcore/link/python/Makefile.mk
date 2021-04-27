python-install: python-build
	@echo "python3 $(top_srcdir)/gldcore/link/python/setup.py --quiet install"
	@( export SRCDIR=$(top_srcdir) ; python3 $(top_srcdir)/gldcore/link/python/setup.py --quiet install)

python-build:
	@echo "python3 $(top_srcdir)/gldcore/link/python/setup.py --quiet build ${PYTHONSETUPFLAGS:--j 5}"
	@( export SRCDIR=$(top_srcdir) ; python3 $(top_srcdir)/gldcore/link/python/setup.py --quiet build ${PYTHONSETUPFLAGS:--j 5})

python-clean:
	@echo "python3 $(top_srcdir)/gldcore/link/python/setup.py clean"
	@( export SRCDIR=$(top_srcdir) ; python3 $(top_srcdir)/gldcore/link/python/setup.py clean )
	-rm -rf $(top_srcdir)/build/lib.*
