error () { echo "ERROR [Darwin.sh]: $*" > /dev/stderr ; exit 1; }
export VERSION_ID=$(uname -r | cut -f1 -d.)
COMMON=$GRIDLABD_ORIGIN/setup/darwin_${VERSION_ID%%.*}.sh
SOURCE=$GRIDLABD_ORIGIN/setup/darwin_${VERSION_ID%%.*}-$(uname -m).sh
if [ "$GRIDLABD_ORIGIN" = "." ]; then
	if [ -f $COMMON ]; then
		sh $COMMON || error "unable to setup from $GRIDLABD_ORIGIN"
	else
		sh $SOURCE || error "unable to setup from $GRIDLABD_ORIGIN"
	fi
elif ! (curl -H 'Cache-Control: no-cache' -fsL $COMMON | sh); then
	( curl -H 'Cache-Control: no-cache' -fsL $SOURCE | sh ) || error "unable to find $SOURCE. Manual install required."=
fi
