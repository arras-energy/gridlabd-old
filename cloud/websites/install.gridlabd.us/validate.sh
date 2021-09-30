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

        ;;

    linux)

        source "/etc/os-release"
        RELEASE=$ID$VERSION

        ;;

    *)

        error 3 "validation not available for $SYSTEM, try manual installation instead"

        ;;

esac

url_ok $SOURCE/latest-$SYSTEM-$RELEASE.txt || error 4 "unable to get $SOURCE/latest-$SYSTEM-$RELEASE.txt"
LATEST=$(curl -sL $SOURCE/latest-$SYSTEM-$RELEASE.txt)

[ ! -x /usr/local/bin/gridlabd ] && error 4 "gridlabd is not installed on this system"
TARGET=validate-$LATEST
mkdir -p $TARGET
cd $TARGET
url_tarxz "$SOURCE/validate-${LATEST%-*}.tarz" && /usr/local/bin/gridlabd -D keep_progress=TRUE --validate && error 5 "validation failed"
