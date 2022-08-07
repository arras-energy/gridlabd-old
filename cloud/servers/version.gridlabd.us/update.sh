#!/bin/bash

cd /var/www/html
mkdir -p version
if [ ! -d gridlabd ]; then
    git clone https://source.gridlabd.us/ gridlabd
    cd gridlabd
else
    cd gridlabd
    git pull 2>/dev/null
fi
BRANCHES=$(git branch -a | grep remotes | grep -v HEAD | cut -f3- -d/)
for BRANCH in $BRANCHES; do
    git show remotes/origin/$BRANCH:source/version.h | grep '#define REV_' | cut -f3 -d' ' | tr '\n' '.' | cut -f-3 -d. > ../version/$BRANCH 2>/dev/null
done
