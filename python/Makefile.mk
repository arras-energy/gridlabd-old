PYTHONVERSION=$(shell python$(PYVER) $(top_srcdir)/python/setup.py --version)
PYPKG=$(PYENV)/lib/python$(PYVER)/site-packages/$(PACKAGE)
INSTALL_WHEEL = $(datadir)/$(PACKAGE)/$(PACKAGE)-$(PYTHONVERSION).whl

$(top_srcdir)/python/dist/$(PACKAGE)-$(PYTHONVERSION).tar.gz: $(top_srcdir)/source/build.h | $(PYENV)
	@echo "building $(PACKAGE)-$(PYTHONVERSION)..."
	@rm -f $(PYPKG)-$(PYTHONVERSION)*.{whl,tar.gz}
	@$(ENVPYTHON) -m pip install build 
	@export SRCDIR=$(realpath $(top_srcdir)) ; export BLDDIR=$(shell pwd); $(ENVPYTHON) -m build $(top_srcdir)/python

$(PYPKG)-$(PYTHONVERSION).tar.gz: $(top_srcdir)/python/dist/$(PACKAGE)-$(PYTHONVERSION).tar.gz
	@echo "copying $(top_srcdir)/python/dist to $(PYPKG)-$(PYTHONVERSION)..."
	@cp $(top_srcdir)/python/dist/$(PACKAGE)-$(PYTHONVERSION)*.{tar.gz,whl} $(PYENV)/lib/python$(PYVER)/site-packages


python-install: $(INSTALL_WHEEL)

$(INSTALL_WHEEL): $(PYPKG)-$(PYTHONVERSION).dist-info
	@echo "creating wheel '$(INSTALL_WHEEL)'..."
	@cp $(PYPKG)-$(PYTHONVERSION)-*.whl $(INSTALL_WHEEL)

$(PYPKG)-$(PYTHONVERSION).dist-info: $(PYPKG)-$(PYTHONVERSION).tar.gz
	@echo "installing $(PACKAGE)-$(PYTHONVERSION)..."
	$(ENVPYTHON) -m pip install --ignore-installed $(PYPKG)-$(PYTHONVERSION)-*.whl
	touch $@

python-clean:
	@echo "removing $(top_srcdir)/python/dist..."
	@rm -rf $(top_srcdir)/python/dist

python-uninstall:
	@echo "removing $(PYPKG)..."
	@rm -rf $(PYPKG)*

BUILT_SOURCES += python-install
