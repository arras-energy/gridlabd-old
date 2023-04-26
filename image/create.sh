set -x
if [ ! -f source/gridlabd.in ]; then
	echo "ERROR: you must be in the root folder of local git repository to create this image" >/dev/stderr
	exit 1
fi
case $(uname -s) in
	Darwin)
		VERSION=""
		;;
	Linux)
		. /etc/os-release
		IMAGEPATH=$(gridlabd --version=install)
		if [ ! -d $IMAGEPATH ]; then
			echo "ERROR: image path $IMAGEPATH not found" >/dev/stderr
			exit 1
		fi
		IMAGENAME=${IMAGEPATH##*/}
		IMAGETARZ=$PWD/${IMAGENAME}.tarz
		if ! ( cd ${IMAGEPATH%/*} ; tar cf $IMAGETARZ $IMAGENAME ) ; then
			echo "ERROR: unable to create $IMAGETARZ" >/dev/stderr
		else
			echo $IMAGETARZ
		fi
		;;
	*)
		;;
esac
