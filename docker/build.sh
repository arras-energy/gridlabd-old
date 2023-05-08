#!/bin/sh
## Syntax: docker/build.sh [--push] [--release]
##
## Build a docker image of the current repository. The default image name is
## the output of `build-aux/version.sh --docker`. The source image repo is
## the output of `build-aux/version.sh --origin`.
##
## Options:
##
##   --push     Pushes the resulting image to user's Dockerhub. The
##				
##
##   --release  Release the resulting image as the latest version of
##    			slacgismo/gridlabd. The master branch is released as 'latest'.
##    			Otherwise the branch name is used as the tag name.
## 
error () { echo "ERROR [docker/build.sh]: $*" ; exit 1; }

DOPUSH=no
DOLATEST=no
while [ $# -gt 0 ]; do
	case $1 in 
		-h|--help|help )
			grep '^##' $0 | cut -c3-
			exit 0
			;;
		--push )
			DOPUSH=yes
			;;
		--release )
			DOLATEST=yes
			;;
		* )
			error "option '$1' is invalid"
			;;
	esac
	shift 1
done

docker -v || error "you do not have docker installed and running"

ORIGIN=$(build-aux/version.sh --origin)
NAME=$(build-aux/version.sh --docker | cut -f1 -d:)
TAG=$(build-aux/version.sh --docker | cut -f2 -d:)
docker build docker --build-arg GRIDLABD_ORIGIN="$ORIGIN" -t "$NAME:$TAG" || error "build failed"
if [ "$DOPUSH" = "yes" ]; then
	docker push "$NAME:$TAG" || error "push image failed"
fi
if [ "$DOLATEST" = "yes" ]; then
	BRANCH=$(build-aux/version.sh --branch)
	if [ "$BRANCH" = "master" ]; then
		BRANCH="latest"
	fi
	IMAGE="slacgismo/gridlabd-$(build-aux/version.sh --machine)"
	docker tag "$NAME:$TAG" "$IMAGE:$BRANCH" || error "tag latest failed"
	if [ "$DOPUSH" = "yes" ]; then
		docker push "$IMAGE:$BRANCH" || error "push latest failed"
	fi
fi
