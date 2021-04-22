#!/bin/bash

echo "Checking savefile $1..."

CHECKLIST="house:0.latitude=37.415463 house:0.longitude=-122.201176 house:1.latitude=37.486807 house:1.longitude=-122.226587"
for CHECK in $CHECKLIST; do
    NAMEPROP=$(echo $CHECK | cut -f1 -d=)
    EXPECT=$(echo $CHECK | cut -f2 -d=)
    VALUE=$(gridlabd json-get objects ${NAMEPROP/./ } <$1)
    if [ "$VALUE" != "$EXPECT" ]; then
        echo "ERROR: house:0 latitude is incorrect, expected '$EXPECT' but got '$VALUE'"
    fi
done