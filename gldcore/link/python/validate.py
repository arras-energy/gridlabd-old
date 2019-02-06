import sys
assert(sys.version_info.major>2)

import gridlabd
gridlabd.command('validate.glm')
gridlabd.start('wait')
gridlabd.save('done.json');
gridlabd.save('done.glm');


