[[/Command/README.md]] -- Command line options

GridLAB-D supports a large number of command line options.  The general syntax for a command line is

~~~
bash$ gridlabd [subcommand [options ...]] 
bash$ gridlabd [--pre-load-options ...] [files ...] [--post-load-options ...]
~~~

The command line is parsed in two passes. The first pass handles the subcommand, if any.  The second pass handles the input files options.  During the second pass, options are handled in the order in which they're received.

Note that most subcommands and some options halt processing of the command line when they are completed.

# Subcommands

Subcommands are used to access and manage most GridLAB-D resources and features.

# Pre-Load Options

Pre-load options are processed before the file load begins.  Pre-load options are processed in the order in which they are received.

# Files

Files are loaded in the order in which they are received.  If a file format is not GLM but is supported by the one the converters, the file is automatically converted to GLM format and the GLM is loaded. 

# Post-Load Options

Post-load options are processed after the file loads are completed.  At this point, the objects have been created, but they have not been initialized.
