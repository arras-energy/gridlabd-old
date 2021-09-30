#!/bin/bash

LATEST=$(curl -sL http://install.gridlabd.us/latest.txt)
SYSTEM=$(uname -s | tr A-Z a-z)

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
            echo "ERROR: URL '$1'; $TEXT (error $CODE)"
            return 1
        fi
    else
        echo "ERROR: 'curl -sI $1' failed"
        return -1
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

case $SYSTEM in

    darwin)
        
        
        RELEASE=$(uname -r | cut -f1 -d.)

        ;;

    linux)

        source "/etc/os-release"
        RELEASE=$ID$VERSION

        ;;

    *)

        echo Install not available for $SYSTEM.  Try manual installation instead.

        ;;

esac

TARGET=/usr/local/opt/gridlabd
mkdir -p $TARGET
cd $TARGET
url_run "http://install.gridlabd.us/install-$SYSTEM-$RELEASE.sh" || echo "WARNING: no setup found for $SYSTEM-$RELEASE"
url_tarxz "http://install.gridlabd.us/gridlabd-$SYSTEM-$RELEASE-$LATEST-master.tarz" && /usr/local/opt/gridlabd/$LATEST-master/bin/gridlabd version set

[ -z "$(which gridlabd 2>/dev/null)" ] && echo "gridlabd will be in your PATH the next time you log in"
