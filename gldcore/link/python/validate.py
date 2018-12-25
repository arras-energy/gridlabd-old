import sys
assert(sys.version_info.major>2)

import gridlabd
gridlabd.command('-W')
gridlabd.command('../../..')
gridlabd.command('--validate')
gridlabd.start('batch')
