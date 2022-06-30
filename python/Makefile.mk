PYTHONVERSION=$(shell python3 $(top_srcdir)/python/setup.py --version)

$(top_srcdir)/python/dist/gridlabd-$(PYTHONVERSION).tar.gz: $(top_srcdir)/source/build.h
	@echo "building $@"
	@rm -f $(top_srcdir)/python/dist/gridlabd-*.{whl,tar.gz}
	@python3 -m pip install build 1>/dev/null
	@( export SRCDIR=$(realpath $(top_srcdir)) ; export BLDDIR=$(shell pwd); python3 -m build $(top_srcdir)/python 1>/dev/null )

python-install: $(top_srcdir)/python/dist/gridlabd-$(PYTHONVERSION).tar.gz

	@echo "installing $@"
	@python3 -m pip install --ignore-installed $(top_srcdir)/python/dist/gridlabd-*.whl

python-clean:
	@rm $(top_srcdir)/python/dist/*.{whl,tar.gz}
	@echo "uninstalling $(top_srcdir)/python"
	@python3 -m pip uninstall gridlabd -y || (echo "Use '. utilities/cleanwc' instead to clean this build."; exit 1)
