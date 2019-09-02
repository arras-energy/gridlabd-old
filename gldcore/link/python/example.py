import sys
assert(sys.version_info.major>2)

import gridlabd

# construct the command line (gridlabd hasn't started yet)
gridlabd.command('example.glm')
gridlabd.command('-D')
gridlabd.command('suppress_repeat_messages=FALSE')
#gridlabd.command('--debug')

# start gridlabd and wait for it to complete
gridlabd.start('wait')

# send the final model state to a file
gridlabd.save('done.json');


