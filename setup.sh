DEFAULT_ORIGIN=slacgismo/gridlabd/master
if curl --version 1>/dev/null 2>&1 ; then
	export SYSTEMNAME=$(uname -s)
	if [ -z "$GRIDLABD_ORIGIN" ] ; then 
		echo "WARNING: using default GRIDLABD_ORIGIN=$DEFAULT_ORIGIN" > /dev/stderr
		export GRIDLABD_ORIGIN=$DEFAULT_ORIGIN
	fi
	test $(echo $GRIDLABD_ORIGIN | cut -c-8) != "https://" && GRIDLABD_ORIGIN=https://raw.githubusercontent.com/$GRIDLABD_ORIGIN
	echo GRIDLABD_ORIGIN=$GRIDLABD_ORIGIN
	if curl -H 'Cache-Control: no-cache' -fsL $GRIDLABD_ORIGIN/setup/$SYSTEMNAME.sh > /tmp/setup_$$.sh ; then
		sh /tmp/setup_$$.sh
		rm /tmp/setup_$$.sh
	else
		echo "ERROR: unable to find $GRIDLABD_ORIGIN/setup/$SYSTEMNAME.sh. Manual setup required." >/dev/stderr
	fi		
else
	echo "ERROR: you must install curl first" > /dev/stderr
fi