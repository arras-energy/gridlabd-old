#!/bin/sh
# This file is used by autoconf to generate the version string.
# It is assumed that this script is run from the top-level srcdir.
FIL="gldcore/version.h"
MAJ=`cat $FIL | sed -rn 's/#define REV_MAJOR ([0-9]+).*/\1/p' | tr -d '\n'`
MIN=`cat $FIL | sed -rn 's/#define REV_MINOR ([0-9]+).*/\1/p' | tr -d '\n'`
PAT=`cat $FIL | sed -rn 's/#define REV_PATCH ([0-9]+).*/\1/p' | tr -d '\n'`
NUM=`git log --max-count=1 --pretty=format:"%ai" | cut -c 3,4,6,7,9,10`
BRA=`git branch | grep '^*' | cut -f2 -d' '`
SVN=`git --version | cut -f3 -d' '`
SYS=`uname`
HDW=`uname -m`
case $1 in
    -version | --version | --versio | --versi | --vers | --ver | --ver | --v)
        echo "$MAJ.$MIN.$PAT" ;;
    -branch | --branch | --branc | --bran | --bra | --br | --b)
        echo "$BRA" ;;
    -svnversion | --svnversion | --svnversio | --svnversi | --svnversi \
    | --svnvers | --svnver | --svnve | --svnv | --svn | --sv | --s)
        echo "$SVN" ;;
    --install )
        echo "gridlabd-$MAJ.$MIN.$PAT-$NUM-${BRA//-/_}-$SYS-$HDW" ;;
esac
