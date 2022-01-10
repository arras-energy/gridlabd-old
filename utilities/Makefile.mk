SUBLIME_SYNTAX = $(HOME)/Library/Application\ Support/Sublime\ Text\ 3/Packages/User/GridLAB-D.sublime-build
SUBLIME_SOURCE = utilities/sublime/gridlabd.sublime-syntax

sublime-syntax: $(SUBLIME_SYNTAX)

$(SUBLIME_SYNTAX): $(SUBLIME_SOURCE)
	cp $< "$@" || echo "ERROR: you need to install sublime package manager first"
