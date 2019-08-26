python-install:
	@( find /usr/include -name Python.h -print )
	@echo "python3 $(top_srcdir)/gldcore/link/python/setup.py install"
	@( export SRCDIR=$(top_srcdir) ; python3 $(top_srcdir)/gldcore/link/python/setup.py install )

python-clean:
	@echo "python3 $(top_srcdir)/gldcore/link/python/setup.py clean"
	@( export SRCDIR=$(top_srcdir) ; python3 $(top_srcdir)/gldcore/link/python/setup.py clean )
	-rm -rf $(SRCDIR)/build/lib.*
