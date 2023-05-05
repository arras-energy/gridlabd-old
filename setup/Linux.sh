error () { echo "ERROR [Linux.sh]: $*" > /dev/stderr ; exit 1; }
. /etc/os-release
SOURCE=$GRIDLABD_ORIGIN/setup/darwin_${VERSION_ID%%.*}-$(uname -m).sh
if [ "$GRIDLABD_ORIGIN" = "." ]; then
	sh $SOURCE || error "unable to run $GRIDLABD_ORIGIN"
else
	( curl -H 'Cache-Control: no-cache' -fsL $SOURCE | sh ) || error "unable to find $SOURCE. Manual install required."=
fi
