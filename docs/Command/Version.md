[[Command/Version]] -- Command line option to obtain the current version information

# Synopsis
~~~
bash$ gridlabd --version[=<format>]
~~~

# Description

In its simple format, the `--version` command line option output the basic version information for the active installation, e.g.,
~~~
bash$ gridlabd --version
GridLAB-D 4.2.0-191115
~~~

The `--version` option also supports formatting specifications to obtain additional version, build, and installation information. The format options are as follows:

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

# See also

* [[Subcommand/Version]]
* [[Command/Origin]]


Outputs `package-major.minor.patch-build-branch-system-hardware`.
