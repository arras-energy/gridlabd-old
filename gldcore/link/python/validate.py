import sys
assert(sys.version_info.major>2)

import gridlabd
gridlabd.command('validate.glm')
gridlabd.command('-D')
gridlabd.command('suppress_repeat_messages=FALSE')
gridlabd.command('--warn')
gridlabd.start('pause')
gridlabd.save('done.json');
gridlabd.save('done.glm');


