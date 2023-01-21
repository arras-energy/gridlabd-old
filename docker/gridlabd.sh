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
cd $HOME
git clone $REPO gridlabd -b $BRANCH
if [ ! -d $HOME/gridlabd ]; then
	echo "ERROR: unable to download $REPO/$BRANCH"
	exit 1
fi

cd gridlabd 
./install.sh -t -p

# run validation
if [ "${RUN_VALIDATION:-no}" == "yes" ]; then
	gridlabd -T 0 --validate
fi

# cleanup source
if [ "${REMOVE_SOURCE:-yes}" == "yes" ]; then
	cd $HOME
	rm -rf gridlabd
fi
