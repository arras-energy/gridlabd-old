#!/bin/bash

REPO=${REPO:-https://source.gridlabd.us/}
BRANCH=${BRANCH:-master}
echo "
#####################################
# DOCKER BUILD
#   gridlabd <- $REPO/$BRANCH
#####################################
"

# gridlabd source
cd /usr/local/opt/gridlabd/src
git clone $REPO gridlabd -b $BRANCH
if [ ! -d /usr/local/opt/gridlabd/src/gridlabd ]; then
	echo "ERROR: unable to download $REPO/$BRANCH"
	exit 1
fi

cd gridlabd 
autoreconf -isf 
./configure 
export MAKEFLAGS=-j$(($(nproc)*3))
export PYTHONSETUPFLAGS="-j $(($(nproc)*3))"
make system
export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH:-.}

# get weather
if [ "${GET_WEATHER:-yes}" == "yes" ]; then
	make index
fi

# run validation
if [ "${RUN_VALIDATION:-no}" == "yes" ]; then
	gridlabd -T 0 --validate
fi

# cleanup source
if [ "${REMOVE_SOURCE:-yes}" == "yes" ]; then
	rm -rf /usr/local/opt/gridlabd/src/gridlabd
fi
