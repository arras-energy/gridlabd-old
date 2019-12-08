#!/bin/sh
# This file is used by autoconf to generate the version string.
# It is assumed that this script is run from the top-level srcdir.
FIL="gldcore/version.h"
MAJ=`sed -En 's/#define REV_MAJOR ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
MIN=`sed -En 's/#define REV_MINOR ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
PAT=`sed -En 's/#define REV_PATCH ([0-9]+).*/\1/p' $FIL | tr -d '\n'`
PKG=`sed -En 's/#define PACKAGE "([a-z]+)".*/\1/p' $FIL | tr -d '\n'`
NUM=`git log --max-count=1 --pretty=format:"%ai" | cut -c 3,4,6,7,9,10`
BRA=`git branch | grep '^*' | cut -f2 -d' ' | tr '-' '_'`
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
        echo "/usr/local/opt/$PKG/$MAJ.$MIN.$PAT-$NUM-$BRA" ;;
    --name | -m)
        echo "$MAJ.$MIN.$PAT-$NUM-$BRA" ;;
    --help | -h)
        echo "Syntax: build-aux/version.sh <option>
Options:
  --version|-v    output version as REV_MAJOR.REV_MINOR.REV_PATCH
  --branch|-b     output branch name as BRANCH
  --gitversion|-g output git version
  --number|-n     output git last log entry YYMMDD
  --install|-i    output install path /usr/local/opt/PACKAGE/REV_MAJOR.REV_MINOR.REV_PATCH-NUMBER-BRANCH
  --name|-m       output install name REV_MAJOR.REV_MINOR.REV_PATCH-NUMBER-BRANCH
  --help|-h       output this help" ;;
esac
