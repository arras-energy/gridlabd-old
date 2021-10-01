#!/bin/bash

[ -z "$SOURCE" ] && SOURCE=http://install.gridlabd.us

function error()
{
    RC=$1
    shift 1
    echo "ERROR [$SOURCE]: $*"
    exit $RC
}

function warning()
{
    echo "WARNING [$SOURCE]: $*"
}

function url_ok ()
{
    RESULT=$(curl -sI $1 | head -n 1)
    HTTPVER=$(echo "$RESULT" | cut -f1 -d' ')
    CODE=$(echo "$RESULT" | cut -f2 -d' ')
    if [ "$HTTPVER" == "HTTP/1.1" ]; then
        if [ "$CODE" -eq "200" ]; then
            return 0
        else
            return $CODE
        fi
    else
        return -1
    fi
}

function url_err ()
{
    TEXT=$(echo "$RESULT" | cut -f3- -d' ' | tr -dc A-Za-z\ )
    if [ "$HTTPVER" == "HTTP/1.1" ]; then
        if [ "$CODE" -ne "200" ]; then
            error 1 "URL '$1'; $TEXT (error $CODE)"
            return 1
        fi
    else
        error 2 "'curl -sI $1' failed"
    fi
}

function url_run ()
{
    url_ok "$1" && ( curl -sL "$1" | /bin/bash ) || return 1
}

function url_tarxz ()
{
    url_ok "$1" && ( curl -sL "$1" | tar xz ) || url_err "$1"
}

SYSTEM=$(uname -s | tr A-Z a-z)
case $SYSTEM in

    darwin)
        
        
        RELEASE=$(uname -r | cut -f1 -d.)
        alias nproc="sysctl -n hw.ncpu"

        ;;

    linux)

        source "/etc/os-release"
        RELEASE=$ID$VERSION_ID

        ;;

    *)

        error 3 "install not available for $SYSTEM, try manual installation instead"

        ;;

esac

url_ok $SOURCE/latest-$SYSTEM-$RELEASE.txt || error 4 "unable to get $SOURCE/latest-$SYSTEM-$RELEASE.txt"
LATEST=$(curl -sL $SOURCE/latest-$SYSTEM-$RELEASE.txt)

TARGET=/usr/local/opt/gridlabd
mkdir -p $TARGET
cd $TARGET

if ( url_run "$SOURCE/install-$SYSTEM-$RELEASE.sh" && url_tarxz "$SOURCE/gridlabd-$SYSTEM-$RELEASE-${LATEST%-*}-master.tarz" && /usr/local/opt/gridlabd/$LATEST-master/bin/gridlabd version set ); then
    echo -n ""
else
    echo "fast install failed, running full build"
    mkdir -p /usr/local/src
    cd /usr/local/src
    git clone https://github.com/slacgismo/gridlabd --depth 1 gridlabd
    cd gridlabd
    autoreconf -isf
    ./configure
    export MAKEFLAGS=-j$(($(nproc)*3))
    export PYTHONSETUPFLAGS="-j $(($(nproc)*3))"
    make system
fi

echo '
**************************************************
Welcome to the HiPAS GridLAB-D host.

Use 'gridlabd --help' for help on using GridLAB-D

Online documentation is available at

  https://docs.gridlabd.us/

**************************************************
' > /etc/motd
echo 'export PATH=/usr/local/bin:$PATH' >> /etc/bashrc

[ -z "$(which gridlabd 2>/dev/null)" ] && echo "gridlabd will be in your PATH the next time you log in"
