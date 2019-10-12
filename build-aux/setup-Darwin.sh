#!/bin/bash
RELEASE=$(uname -r | cut -f1 -d.)
SETUP="${0/.sh/-$RELEASE}"
if [ -f $SET ]; then
    source $SETUP
else
	error "not installer available for $(basename $SETUP)"
fi
