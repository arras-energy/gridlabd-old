[[/Command/Sublime_syntax]] -- Generates the SublimeText syntax for GLM files

# Synopsis

~~~
bash$ gridlabd --sublime_syntax                                            
~~~

# Description

The `--sublime_syntax` option generates the syntax file required to enable syntax highlighting for GLM files.

# Example

The following example generates the syntax file and stores in the current user's package folder for Sublime Text 3.

~~~
% gridlabd --sublime_syntax > $(HOME)/Library/Application\ Support/Sublime\ Text\ 3/Packages/User/GLM.sublime-syntax
~~~
