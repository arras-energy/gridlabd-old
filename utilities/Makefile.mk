SUBLIME_SYNTAX = $(HOME)/Library/Application\ Support/Sublime\ Text\ 3/Packages/User/GLM.sublime-syntax
SUBLIME_SOURCE = utilities/sublime/GLM.sublime-syntax

sublime-syntax: $(SUBLIME_SYNTAX)

$(SUBLIME_SYNTAX): gldcore/load.cpp
	@echo "Updating sublime syntax"
	@gridlabd --sublime_syntax > "$@" || echo "ERROR: you need to install sublime package manager first"
