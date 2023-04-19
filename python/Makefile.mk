PYTHONVERSION=$(shell python3 $(top_srcdir)/python/setup.py --version)

$(top_srcdir)/python/dist/gridlabd-$(PYTHONVERSION).tar.gz: $(top_srcdir)/source/build.h | $(PYENV)
	@echo "building $@"
	@rm -f $(top_srcdir)/python/dist/gridlabd-*.{whl,tar.gz}
	@$(PYACTIVATE);$(ENVPYTHON) -m pip install build 1>/dev/null
	@( export SRCDIR=$(realpath $(top_srcdir)) ; export BLDDIR=$(shell pwd); $(PYACTIVATE);$(ENVPYTHON) -m build $(top_srcdir)/python 1>/dev/null )

python-install: $(top_srcdir)/python/dist/gridlabd-$(PYTHONVERSION).tar.gz
	@echo "installing $@"
	@$(PYACTIVATE);$(ENVPYTHON) -m pip install --ignore-installed $(top_srcdir)/python/dist/gridlabd-*.whl

python-clean:
	@rm -f $(top_srcdir)/python/dist/*.{whl,tar.gz}

python-uninstall:
	@echo "uninstalling $(top_srcdir)/python"
	@rm -rf $(PYENV)

BUILT_SOURCES += python-install
