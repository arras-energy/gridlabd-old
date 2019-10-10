#!/bin/bash

export DOCS=documents/html

if [ ! -d $DOCS ]; then
	mkdir -p $DOCS
fi

cp utilities/troubleshooting.css $DOCS/
gawk -f ./utilities/troubleshooting.awk */*.{c,cpp} > $DOCS/troubleshooting.html
