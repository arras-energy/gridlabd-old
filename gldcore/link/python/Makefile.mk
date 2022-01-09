$(top_srcdir)/gldcore/link/python/dist/gridlabd-$(notdir $(prefix))-.tar.gz: gldcore/build.h
	@echo "building $@"
	@python -m pip install -y 1>/dev/null
	@( export SRCDIR=$(realpath $(top_srcdir)) ; python3 -m build $(top_srcdir)/gldcore/link/python 1>/dev/null )

python-install: $(top_srcdir)/gldcore/link/python/dist/gridlabd-$(notdir $(prefix))-.tar.gz
	@echo "installing $@"
	@python3 -m pip install --ignore-installed $(top_srcdir)/gldcore/link/python/dist/gridlabd-*.whl

python-clean:
	@echo "uninstalling $(top_srcdir)/gldcore/link/python"
	@python3 -m pip uninstall gridlabd -y
