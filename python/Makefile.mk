PYTHONVERSION=$(shell python3 $(top_srcdir)/python/setup.py --version)
PYPKG=$(PYENV)/lib/python$(PYVER)/site-packages/gridlabd

$(top_srcdir)/python/dist/gridlabd-$(PYTHONVERSION).tar.gz: $(top_srcdir)/source/build.h | $(PYENV)
	@echo "building $@"
	@rm -f $(PYPKG)-$(PYTHONVERSION)*.{whl,tar.gz}
	@$(ENVPYTHON) -m pip install build 
	@export SRCDIR=$(realpath $(top_srcdir)) ; export BLDDIR=$(shell pwd); $(ENVPYTHON) -m build $(top_srcdir)/python

$(PYPKG)-$(PYTHONVERSION).tar.gz: $(top_srcdir)/python/dist/gridlabd-$(PYTHONVERSION).tar.gz
	@cp $(top_srcdir)/python/dist/gridlabd-$(PYTHONVERSION)*.{tar.gz,whl} $(PYENV)/lib/python$(PYVER)/site-packages

python-install: $(PYPKG)-$(PYTHONVERSION).tar.gz
	@echo "installing gridlabd-$(PYTHONVERSION)"
	$(ENVPYTHON) -m pip install --ignore-installed $(PYPKG)-$(PYTHONVERSION)-*.whl

python-clean:
	@echo "removing intermediate python module build artifacts"
	@rm -rf $(top_srcdir)/python/dist

python-uninstall:
	@echo "removing $(PYPKG) module files"
	@rm -rf $(PYPKG)*

BUILT_SOURCES += python-install
