PYTHONVERSION=$(shell $(PYBIN)/python3 $(top_srcdir)/python/setup.py --version)

$(top_srcdir)/python/dist/gridlabd-$(PYTHONVERSION).tar.gz: $(top_srcdir)/source/build.h
	@echo "building $@"
	@rm -f $(top_srcdir)/python/dist/gridlabd-*.{whl,tar.gz}
	@$(PYBIN)/python3 -m pip install build 1>/dev/null
	@( export SRCDIR=$(realpath $(top_srcdir)) ; export BLDDIR=$(shell pwd); $(bindir)/pkgenv/bin/python3 -m build $(top_srcdir)/python 1>/dev/null )

python-install: $(top_srcdir)/python/dist/gridlabd-$(PYTHONVERSION).tar.gz
	@echo "installing $@"
	@$(PYBIN)/python3 -m pip install --ignore-installed $(top_srcdir)/python/dist/gridlabd-*.whl

python-clean:
	@rm -f $(top_srcdir)/python/dist/*.{whl,tar.gz}

python-uninstall:
	@echo "uninstalling $(top_srcdir)/python"
	@$(PYBIN)/python3 -m pip uninstall gridlabd -y || (echo "Use '. utilities/cleanwc' instead to clean this build."; exit 1)
	@rm -rf $(PYENV)

BUILT_SOURCES += python-install
