[[Subcommand/Version]] -- System version manager

# Synopsis
~~~
bash$ gridlabd version 
bash$ gridlabd version help
bash$ gridlabd version check [-v|-q]
bash$ gridlabd version list <PATTERN>
bash$ gridlabd version show
bash$ gridlabd version set [<PATTERN>]
bash$ gridlabd version source
bash$ gridlabd version delete [-a|<PATTERN>]
~~~

# Description

When GridLAB-D is built it is installed into a folder named `/usr/local/opt/gridlabd/<MAJOR>.<MINOR>.<PATCH>-<BUILD>-<BRANCH>`.  Users may select which version they are using by modifying the `PATH` environment variable to include the desired version's `bin` folder, e.g.
~~~
bash$ export PATH=/usr/local/opt/gridlabd/<MAJOR>.<MINOR>.<PATCH>-<BUILD>-<BRANCH>:$PATH
~~~
However, this method does not set the version for all users on the system. The `gridlabd version` command manages the system version of GridLAB-D.  

With command options, the version subcommand displays the current system version, e.g.,
~~~
bash$ gridlabd version
4.2.0-191101-master
~~~
This is equivalent to `gridlabd version show`.

The follow command options are supported:

## `check [-v|-q]`

The `check` command determines whether the current version is up to date.

## `help`

The `help` command displays the available command options.

## `list [<PATTERN>]`

The `list` command displays the available versions that can be made available to all users on the system.

## `set [<PATTERN>]`

The `set` command changes the version that is made available to all users on the system.

## `show`

The `show` command display which version is current set for all users on the system.

## `source`

The `source` option opens the source code to the version.

## `delete`

The indicated version is deleted. If the version is a pattern, then multiple versions are deleted.  If `-a` is used, all versions but the current version are deleted.

# See also
* [[Install]]
* --[[version]] command line option
