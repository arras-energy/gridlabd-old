#/bin/sh
function error () { echo "ERROR [setup.sh]: $*" > /dev/stderr ; exit 1; }
DEFAULT_ORIGIN=slacgismo/gridlabd/master
while [ $# -gt 0 ]; do
	case "$1" in
		-v|--verbose)
			set -x
			shift 1
			;;
		-h|--help)
			echo "Syntax: setup.sh [-v|--verbose] [ORG/REPO/BRANCH]" ; exit 0
			;;
		*)
			export GRIDLABD_ORIGIN=$1
			shift 1
			;;
	esac
done
if curl --version 1>/dev/null 2>&1 ; then
	export SYSTEMNAME=$(uname -s)
	if [ -z "$GRIDLABD_ORIGIN" ] ; then 
		export GRIDLABD_ORIGIN=$DEFAULT_ORIGIN
	fi
	test "$(echo $GRIDLABD_ORIGIN | cut -c-8)" != "https://" && GRIDLABD_ORIGIN=https://raw.githubusercontent.com/$GRIDLABD_ORIGIN
	echo GRIDLABD_ORIGIN=$GRIDLABD_ORIGIN
	if curl -H 'Cache-Control: no-cache' -fsL $GRIDLABD_ORIGIN/setup/$SYSTEMNAME.sh > /tmp/setup_$$.sh ; then
		trap "rm -f /tmp/setup_$$.sh" EXIT
		sh /tmp/setup_$$.sh || error "setup failed"
	else
		error "unable to find $GRIDLABD_ORIGIN/setup/$SYSTEMNAME.sh. Manual setup required."
	fi		
else
	error "you must install curl first"
fi
