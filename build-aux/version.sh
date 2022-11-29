#!/bin/bash
# This file is used by autoconf to generate the version string.
# It is assumed that this script is run from the top-level srcdir.
EXE=$0
TOP=$(cd ${EXE/version.sh/..}; pwd)
FIL="${TOP}/source/version.h"
MAJ=`sed -En 's/#define REV_MAJOR ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
MIN=`sed -En 's/#define REV_MINOR ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
PAT=`sed -En 's/#define REV_PATCH ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
PKG=`sed -En 's/#define PACKAGE "([a-z]+)".*/\1/p' $FIL | tr -d '\n'`
NAM=`sed -En 's/#define PACKAGE_NAME "([-A-Za-z ]+)".*/\1/p' $FIL | tr -d '\n'`
NUM=`git log --max-count=1 --pretty=format:"%ai" | cut -c 3,4,6,7,9,10`
BRA=`git rev-parse --abbrev-ref HEAD | git rev-parse --abbrev-ref HEAD | tr -c A-Za-z0-9 _ | sed 's/_+/_/g;s/_$//;s/^_//'`
GIT=`git --version | cut -f3 -d' '`
SYS=`uname`
HDW=`uname -m`
case $1 in
    --version | -v)
        echo "$MAJ.$MIN.$PAT" ;;
    --branch | -b)
        echo "$BRA" ;;
    --gitversion | -g)
        echo "$GIT" ;;
    --number | -n)
        echo "$NUM" ;;
    --install | -i)
        if [ -d /usr/local/opt/gridlabd -a -w /usr/local/opt/gridlabd ]; then
            echo "/usr/local/opt/$PKG/$MAJ.$MIN.$PAT-$NUM-$BRA-$SYS-$HDW"
        else
            echo "$HOME/opt/$PKG/$MAJ.$MIN.$PAT-$NUM-$BRA-$SYS-$HDW"
        fi
        ;;
    --name | -m)
        echo "$MAJ.$MIN.$PAT-$NUM-$BRA-$SYS-$HDW" ;;
    --package | -p)
        echo "$PKG" ;;
    --package-name | -P)
        echo "$NAM" ;;
    --help | -h)
        echo "Syntax: build-aux/version.sh <option>
Options:
  --version|-v      output version as REV_MAJOR.REV_MINOR.REV_PATCH
  --branch|-b       output branch name as BRANCH
  --gitversion|-g   output git version
  --number|-n       output git last log entry YYMMDD
  --install|-i      output install path /opt/gridlabd/opt/PACKAGE/REV_MAJOR.REV_MINOR.REV_PATCH-NUMBER-BRANCH-SYSTEM-ARCHITECTURE
  --package|-p      output package PACKAGE
  --package-name|-P output package name PACKAGE_NAME
  --name|-m         output install name REV_MAJOR.REV_MINOR.REV_PATCH-NUMBER-BRANCH
  --help|-h         output this help" ;;
esac
