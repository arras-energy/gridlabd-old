#!/bin/bash
export PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

function error()
{
    echo "ERROR: $*"
    exit 1
}

function check()
{
    MIN=$1
    CMD=$2
    CHK=$3
    if [ "$(which $CMD)" == "" ]; then
        install-$CMD $MIN
    fi
    VER=$(eval "($CHK) 2>/dev/null")
    if [ -z "$VER" ]; then
        upgrade-$CMD $MIN
    else
        if [ "$VER" -lt "$MIN" ]; then
            upgrade-$CMD $MIN
        fi
    fi
}

function install-xcode-select ()
{
    error "xcode-select version $1 needs to be installed"
}

function upgrade-xcode-select ()
{
    error "xcode-select needs to be upgraded to version $1"
}

function install-git ()
{
    error "git version $1 needs to be installed"
}

function upgrade-git()
{
    error "git needs to be upgraded to version $1"
}

function install-brew()
{
    error "brew version $1 needs to be installed"
}

function upgrade-brew()
{
    error "brew needs to be upgraded to version $1"
}

function install-autoscan ()
{
    error "autotools version $1 needs to be installed"
}

function upgrade-autoscan ()
{
    error "autotools needs to be upgraded to version $1"
}

function install-autoreconf ()
{
    error "autotools version $1 needs to be installed"
}

function upgrade-autoreconf ()
{
    error "autotools needs to be upgraded to version $1"
}

function install-sed ()
{
    error "sed version $1 needs to be installed"
}

function upgrade-sed ()
{
    error "sed needs to be upgraded to version $1"
}

check 2354 xcode-select "xcode-select --version | cut -f3 -d' ' | cut -f1 -d."
check 2 git "git --version | cut -f3 -d' ' | cut -f1 -d."
check 2 brew "brew --version | sed -n 1p | cut -f2 -d' ' | cut -f1 -d."
check 2 autoscan "autoscan --version | sed -n 1p | cut -f4 -d' ' | cut -f1 -d."
check 2 autoreconf "autoreconf --version | sed -n 1p | cut -f4 -d' ' | cut -f1 -d."
check 4 sed "sed --version | sed -n 1p | cut -f4 -d' ' | cut -f1 -d."
check 10 g++ "g++ --version 2>/dev/null | sed -n 1p | cut -f4 -d' ' | cut -f1 -d."
