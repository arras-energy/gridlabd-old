#!/bin/bash

REPO=${REPO:-https://github.com/hipas/gridlabd}
BRANCH=${BRANCH:-master}
echo "
#####################################
# DOCKER BUILD
#   gridlabd <- $REPO/$BRANCH
#####################################
"

# gridlabd source
cd /usr/local/src
git clone $REPO gridlabd -b $BRANCH
if [ ! -d /usr/local/src/gridlabd ]; then
	echo "ERROR: unable to download $REPO/$BRANCH"
	exit 1
fi

cd gridlabd 
autoreconf -isf 
./configure 
make -j30 system
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
	rm -rf /usr/local/src/gridlabd
fi
