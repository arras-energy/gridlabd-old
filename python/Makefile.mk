PYTHONVERSION=$(shell python3 $(top_srcdir)/python/setup.py --version)
PYPKG=$(PYENV)/lib/python$(PYVER)/site-packages/gridlabd

$(PYPKG)/gridlabd-$(PYTHONVERSION).tar.gz: $(top_srcdir)/source/build.h | $(PYENV)
	@echo "building $@"
	rm -f $(PYPKG)/gridlabd-$(PYTHONVERSION)*.{whl,tar.gz}
	$(ENVPYTHON) -m pip install build 
	export SRCDIR=$(realpath $(top_srcdir)) ; export BLDDIR=$(shell pwd); $(ENVPYTHON) -m build $(top_srcdir)/python
	mkdir -p $(PYPKG)
	cp $(top_srcdir)/python/dist/gridlabd-$(PYTHONVERSION)*.{tar.gz,whl} $(PYPKG)
	ls -lR $(PYPKG)

python-install: $(PYPKG)/gridlabd-$(PYTHONVERSION).tar.gz
	@echo "installing gridlabd-$(PYTHONVERSION)"
	$(ENVPYTHON) -m pip install --ignore-installed $(PYPKG)/gridlabd-$(PYTHONVERSION)-*.whl

python-clean:
	@rm -f $(PYPKG)/gridlabd-*.{whl,tar.gz}

python-uninstall:
	@echo "uninstalling $(top_srcdir)/python"
	@rm -rf $(PYENV)

BUILT_SOURCES += python-install
