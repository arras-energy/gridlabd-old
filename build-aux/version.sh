#!/bin/bash
## Syntax: build-aux/version.sh [OPTION]
##
## Options:
##
##   --version       the version, e.g., 4.3.1
##   --number        the build number, e.g., 230701
##   --branch        the branch, e.g., master
##   --gitversion    the version of github, e.g., 2.34.1
##   --install       the install folder path name, e.g., /usr/local/opt/gridlabd/4.3.1-230426-master-ubuntu_22-x86_64
##   --name          the install folder base name, /usr/local/opt/gridlabd/4.3.1-230426-master-ubuntu_22-x86_64
##   --package       the package name, e.g., gridlabd
##   --package-name  the package full name, e.g., HiPAS GridLAB-D
##   --machine       the machine type, e.g., x86_64
##   --system        the system name, e.g., ubuntu_22
##   --sysspec       the system specification, e.g., ubuntu_22-x86_64
##   --bucket        the s3 bucket for the install file uploads
##   --python        the python version, e.g., 3.10
##   --parse INSTALL parses an install name into its constituents
##   --docker        the docker tag name, e.g., 4.3.1-230701-master-ubuntu_22-x86_64
##   --origin        the origin of the source code, e.g., slacgismo/gridlabd/master
##
## This file is used by autoconf to generate the version string.
## It is assumed that this script is run from the top-level srcdir.
#
EXE=$0
PYV=3.10
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
ORG=`git remote get-url origin | cut -f4- -d/`
if [ "$(uname -s)" == "Darwin" ]; then
    SYS="darwin_$(uname -r | cut -f1 -d.)"
elif [ -f "/etc/os-release" ]; then
    . /etc/os-release
    SYS="${ID}_${VERSION_ID%.*}"
else
    SYS="$(uname -s)_$(uname -r | cut -f1 -d.)"
fi
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
    --machine )
        echo "$HDW" ;;
    --system )
        echo "$SYS" ;;
    --sysspec )
        echo "$SYS-$HDW" ;;
    --python )
        echo $PYV ;;
    --parse )
        VER=$2
        VER=(${VER//-/ })
        echo "VERSION=${VER[0]}"
        echo "NUMBER=${VER[1]}"
        echo "BRANCH=${VER[2]}"
        echo "SYSTEM=${VER[3]}"
        echo "MACHINE=${VER[4]}"
        ;;
    --bucket )
        if [ "$BRA" = "master" ]; then
            echo "install.gridlabd.us"
        else
            echo "install-dev.gridlabd.us"
        fi
        ;;
    --origin )
        echo "$ORG/$(git rev-parse --abbrev-ref HEAD)"
        ;;
    --docker )
        echo "$ORG:$MAJ.$MIN.$PAT-$NUM-$BRA"
        ;;
    --help | -h)
        grep ^## $0 | cut -c4-
        ;;
esac
