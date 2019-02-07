install-exec-hook:
	( export SRCDIR=$(top_srcdir); python3 $(top_srcdir)/gldcore/link/python/setup.py install)
