# GridLAB-D Subcommands

To add a new subcommand script you must do the following:

1. Create the subcommand script filename using the name `gridlabd-<subcommand>` where `<subcommand>` is an alphanumeric word or hyphenated words.

2. Add the full name subcommand script filename to `Makefile.mk`, e.g.,
~~~
bin_SCRIPTS += subcommands/gridlabd-<subcommand>
~~~

3. The subcommand will receive all the command line arguments given after the subcommand name. These may be use in any way you wish. However there are some conventions to observe.

  * The option `help` should always be supported, e.g., `gridlabd <subcommand> help` should display something like
  ~~~
  Syntax: gridlabd <subcommand> <options>
  Options:
    help    displays this help
    opt1    runs option 1
    opt2    runs option 2
  ~~~

  * Subcommand should exit with code 0 when they succeed, and different non-zero codes for each type of failure.  

  * The subcommand documentation in `docs` should describe the purpose, features, and exit codes for each command line option.

