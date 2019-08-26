python-install:
	@echo "python3 $(top_srcdir)/gldcore/link/python/setup.py install"
	@( export SRCDIR=$(top_srcdir) ; python3 $(top_srcdir)/gldcore/link/python/setup.py --quiet install )

python-clean:
	@echo "python3 $(top_srcdir)/gldcore/link/python/setup.py clean"
	@( export SRCDIR=$(top_srcdir) ; python3 $(top_srcdir)/gldcore/link/python/setup.py clean )
	-rm -rf $(SRCDIR)/build/lib.*
