--[[version]] -- Version command line option

# Synopsis
~~~
bash$ gridlabd --version[=format]
~~~

# Description

The command line option --[[version]] now supports format options to obtain specific version/build information. When no format is specified the output is `package major.minor.path-build`. The format options are as follows:

## all

Outputs `package major.minor.patch-build (branch) platform`.

## package 

Outputs `package`.

## number

Outputs `major.minor.patch`.

## build

Outputs `build`.

## platform

Outputs `platform`.

## branch

Outputs `branch`.

## install

Outputs `package-major.minor.patch-build-branch-system-hardware`.