set +x
DEFAULT_ORIGIN=slacgismo/gridlabd/master
if curl --version 1>/dev/null 2>&1 ; then
	export SYSTEMNAME=$(uname -s)
	if -z "$GRIDLABD_ORIGIN"; then 
		echo "WARNING: GRIDLABD_ORIGIN set to default $DEFAULT_ORIGIN"
		export GRIDLABD_ORIGIN=$DEFAULT_ORIGIN
	fi
	if ! curl -sL $GRIDLABD_ORIGIN/setup/$SYSTEMNAME.sh > /tmp/setup_$$.sh ; then
		sh /tmp/setup_$$.sh
		rm /tmp/setup_$$.sh
	else
		echo "ERROR: unable to find $GRIDLABD_ORIGIN/setup/$SYSTEMNAME.sh. Manual setup required." >/dev/stderr
	fi		
else
	echo "ERROR: you must install curl first" > /dev/stderr
fi