error () { echo "ERROR [Linux.sh]: $*" > /dev/stderr ; exit 1; }
. /etc/os-release
COMMON=$GRIDLABD_ORIGIN/setup/${ID}_${VERSION_ID%%.*}.sh
SOURCE=$GRIDLABD_ORIGIN/setup/${ID}_${VERSION_ID%%.*}-$(uname -m).sh
if [ "$GRIDLABD_ORIGIN" = "." ]; then
	if [ -f $COMMON ]; then
		sh $COMMON || error "unable to setup from $GRIDLABD_ORIGIN"
	else
		sh $SOURCE || error "unable to setup from $GRIDLABD_ORIGIN"
	fi
elif ! (curl -H 'Cache-Control: no-cache' -fsL $COMMON | sh); then
	( curl -H 'Cache-Control: no-cache' -fsL $SOURCE | sh ) || error "unable to find $SOURCE. Manual install required."=
fi
