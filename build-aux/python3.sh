
#
# This should generally not be used and is only for the few C++ and header files making simple python calls.
exec $GLD_BIN/pkgenv/bin/python3 "$@"

# Python modules should start without a hashbang, and instead use the below code without the #s. See gridlabd-convert as an example. 
# ''':'
# exec "$GLD_BIN/pkgenv/bin/python3" "$0" "$@"
# :' '''