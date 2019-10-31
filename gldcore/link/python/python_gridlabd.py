"""GridLAB-D Python validation routine

This script runs a single gridlabd validate test.

Syntax: python3 python_gridlabd.py <gridlabd-command-arguments>

Example:

  host% python3 python_validate.py assert.glm

"""
import sys, gridlabd

# construct the command line (gridlabd hasn't started yet)
for cmd in sys.argv[1::]:
	gridlabd.command(cmd)

# start gridlabd and wait for it to complete
rc = gridlabd.start('wait')
quit(rc)