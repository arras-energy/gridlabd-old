#!/bin/bash

FAILED=0
for MODEL_NAME in $*; do
    diff --unified=0 -I '^\(|//\|.*guid.*\)' ../${MODEL_NAME}-meters.glm ${MODEL_NAME}-meters.glm || FAILED=$(($FAILED+1))
done
exit $FAILED
