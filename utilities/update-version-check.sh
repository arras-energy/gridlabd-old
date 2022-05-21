#!/bin/bash

VERSION=$(gridlabd --version=version)
COMMIT=$(gridlabd --version=commit)
BRANCH=$(gridlabd --version=branch)

mkdir -p build/aws
echo "$VERSION $COMMIT" > build/aws/$BRANCH

aws cp build/aws/$BRANCH s3://version.gridlabd.us/$BRANCH
