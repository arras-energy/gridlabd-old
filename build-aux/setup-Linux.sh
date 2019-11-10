#!/bin/bash

if [ ! -f "/etc/os-release" ]; then
    echo "unknown linux release (/etc/os-release not found)"
    exit 2
fi
source "/etc/os-release"
SETUP="${0/.sh/-$ID-$(echo $VERSION_ID | cut -f1 -d.).sh}"
if [ ! -f "$SETUP" ]; then
	${0/Linux.sh/manual.sh}
    echo "ERROR: $SETUP not found: manual install required"
    exit 1
fi
if [ "${USER:-root}" == "root" ]; then
	function sudo ()
	{
		$*
	}
fi
sudo $SETUP $*
