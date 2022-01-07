[[/Command/Version]] -- Command line option to obtain the current version information

# Synopsis

~~~
bash$ gridlabd --version[=<format>]
bash$ gridlabd --version[=-<requirement>]
~~~

# Description

In its simple format, the `--version` command line option output the basic version information for the active installation, e.g.,

~~~
bash$ gridlabd --version
GridLAB-D 4.2.0-191115
~~~

The `--version` option also supports formatting specifications to obtain additional version, build, and installation information. The format options are as follows:

In addition, the version command supports verifying that the version meets certain requirements.  See the [[/GLM/Macro/Version]] for details on the syntax for the requirements.

## Format

The format of the output can be specified using the `--version=<format>` option. The following formats are supported

### `all`

Outputs all version information, e.g., `package major.minor.patch-build (branch) platform`.

### `package` 

Outputs the package name, e.g., `GridLAB-D`.

### `number`

Outputs the version number, e.g., `major.minor.patch`.

### `build`

Outputs the build number, e.g., `YYMMDD`.

### `platform`

Outputs the platform name, e.g., `Darwin` or `Linux`.

### `branch`

Outputs the branch name, e.g., `master`.

### `install`

Outputs the installation path, e.g., `/usr/local/opt/gridlabd/4.2.0-191115-master`.

### `name`

Outputs the installation name, e.g., `gridlabd-4.2.0-191115-develop_fix_manual`.

### `git-repo`, `git-branch`, `git-commit`

Outputs the git repository URL, branch, and commit information, respectively.

# Example

The following command verifies that the version is at least 4.2:

~~~
bash$ gridlabd --version=-'ge 4.2'
~~~

# See also

* [[/Subcommand/Version]]
* [[/Command/Origin]]
* [[/GLM/Macro/Version]]
