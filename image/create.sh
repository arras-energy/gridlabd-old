set -x
if [ ! -f source/gridlabd.in ]; then
	echo "ERROR: you must be in the root folder of local git repository to create this image" >/dev/stderr
else
	IMAGEPATH=$(gridlabd --version=install)
	if [ ! -d $IMAGEPATH ]; then
		echo "ERROR: image path $IMAGEPATH not found" >/dev/stderr
	else
		IMAGENAME=${IMAGEPATH##*/}
		IMAGETARZ=$PWD/image/${IMAGENAME}.tarz
		if ! ( cd ${IMAGEPATH%/*} ; tar cfz $IMAGETARZ $IMAGENAME ) ; then
			echo "ERROR: unable to create $IMAGETARZ" >/dev/stderr
		else
			echo $IMAGETARZ
		fi
	fi
fi