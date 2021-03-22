#!/bin/bash

while getopts "n:" flag
do
    case ${flag} in
        n)
            n=${OPTARG}
            shift
            ;;
    esac
    shift
done

AWS_ACCESS_KEY_ID=$ACCESS_ID AWS_SECRET_ACCESS_KEY=$SECRET_KEY aws sns publish --region $REGION --topic-arn $TOPIC --message $n
