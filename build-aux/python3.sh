#!bin/sh
#
# This should redirect each gridlabd environment to its own correct python version and pass on all the arguments
echo "bin $GLD_BIN"
echo "at $@"
echo "star $*"
$GLD_BIN/python3 "$*" "$@"
