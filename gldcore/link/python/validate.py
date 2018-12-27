import sys
assert(sys.version_info.major>2)
import time
import gridlabd

def showme(context):
	print("%s: global.clock = '%s', my_test.x = '%s'" % (context,gridlabd.get_global('clock'),gridlabd.get_value('my_test','x')))


#gridlabd.command('--debug')
gridlabd.command('validate.glm')
gridlabd.command('-D')
gridlabd.command('show_progress=FALSE')
gridlabd.start('thread')

showme('started')

gridlabd.save('running.json');
gridlabd.pauseat('2018-02-01 00:00:00')

showme('pauseat(2018-02-01 00:00:00)')

gridlabd.resume()

time.sleep(1)

showme('sleep(1)')

gridlabd.wait()

showme('wait')

gridlabd.save('done.json');
gridlabd.save('done.glm');


