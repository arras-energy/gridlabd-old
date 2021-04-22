#!/bin/bash

while getopts ":v:n:b:" flag
do
  case "${flag}"
    in
      v) 
        RELEASE_VERSION=${OPTARG}
        ;;
      n)
        RELEASE_NAME=${OPTARG}
        ;;
      b)
        RELEASE_BODY=${OPTARG}
        ;;
    esac
done
shift $((OPTIND-1))

DRAFT="false"
PRE="false"
BRANCH="master"

API_JSON=$(printf '{"tag_name": "v%s","target_commitish": "%s","name": "%s","body": "%s","draft": %s,"prerelease": %s}' "$RELEASE_VERSION" "$BRANCH" "$RELEASE_NAME" "$RELEASE_BODY" "$DRAFT" "$PRE" )

#creates the release and publishes
API_RELEASE_RESPONSE=$(curl  -H "Authorization: token $GITHUB_TOKEN" --data "$API_JSON" -s -i https://api.github.com/repos/hipas/gridlabd/releases)

echo "$API_RELEASE_RESPONSE"
