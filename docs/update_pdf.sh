#!/bin/bash

# main entry
if [ $# -eq 0 ]; then

	rm -rf /tmp/update_pdf-*
	TMPDIR=/tmp/update_pdf-$$

	# copy md files to a clean working folder
	rm -rf $TMPDIR
	mkdir $TMPDIR
	find * -type d -prune -exec $0 $TMPDIR/\{\} \; > gridlabd.md
	echo "Compiling gridlabd.pdf..." > /dev/stderr
	pandoc --toc --toc-depth=2 gridlabd.md -o pages.pdf
	gridlabd --version=all | cut -f3-4 -d' ' | sed 's/_/\\_/g' > version.tex
	pdflatex gridlabd
	exit 0
fi

# chapter
CHAPTER=$(basename "$*")
if [ ! -d "$*" -a -d "$CHAPTER" ]; then

	echo "Processing chapter $CHAPTER..." > /dev/stderr
	mkdir "$*"
	cp -r "$CHAPTER"/* "$*"
	find "$*" -name '[A-Z]*.md' -exec $0 \{\} \; 

elif [ -f "$*" ]; then

	FILE="$*"
	FULLNAME=$(echo "$*" | cut -f4- -d/ | sed 's/\.md$//')
	NAME=$(echo $FULLNAME | sed 's:.*/::g')
	pandoc "$FILE" -o /tmp/update_pdf-$$.pdf 1>/tmp/update_pdf-$$.out 2>&1 || echo "ERROR: $* cannot be processed" > /dev/stderr
	if [ -f /tmp/update_pdf-$$.pdf ]; then
		echo "
## $NAME
"
		sed -E 's/^#/###/g;s/\[\[(.+)\]\]/\1/g' < "$FILE"
		echo "

\\newpage

"
	else
		cat /tmp/update_pdf-$$.out > /dev/stderr
	fi
	rm -f /tmp/update_pdf-$$.{pdf,out}
	exit 0

else

	echo "ERROR: $* is invalid" > /dev/stderr
	exit 1

fi
