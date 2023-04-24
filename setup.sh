set +x

if curl --version 1>/dev/null 2>&1 ; then
	export SYSTEMNAME=$(uname -s)
	export GRIDLABD_SOURCE=https://raw.githubusercontent.com/${GRIDLABD_ORG:-slacgismo}/${GRIDLABD_REPO:-gridlabd}/${GRIDLABD_BRANCH:-master}
	if ! curl -sL $GRIDLABD_SOURCE/setup/$SYSTEMNAME.sh > /tmp/setup_$$.sh ; then
		sh /tmp/setup_$$.sh
		rm /tmp/setup_$$.sh
	else
		echo "ERROR: unable to find $GRIDLABD_SOURCE/setup/$SYSTEMNAME.sh. Manual setup required." >/dev/stderr
	fi		
else
	echo "ERROR: you must install curl first" > /dev/stderr
fi