import sys
assert(sys.version_info.major>2)

import gridlabd
gridlabd.command('validate.glm')
gridlabd.command('-D')
gridlabd.command('show_progress=FALSE')
gridlabd.start('thread')
#gridlabd.start('wait')

import time
time.sleep(1)
clock = gridlabd.get_global('clock')
print("global.clock = '%s'\n" % clock)
value = gridlabd.get_value('my_test','x')
print("my_test.x = '%s'\n"%value)
gridlabd.save('running.json');

gridlabd.pause()

print("global.clock = %s\n" % gridlabd.get_global('clock'))
value = gridlabd.get_value('my_test','x')
print("my_test.x = '%s'\n"%value)

print("global.clock = %s\n" % gridlabd.get_global('clock'))
value = gridlabd.get_value('my_test','x')
print("my_test.x = '%s'\n"%value)

gridlabd.resume()

time.sleep(1)

print("global.clock = %s\n" % gridlabd.get_global('clock'))
value = gridlabd.get_value('my_test','x')
print("my_test.x = '%s'\n"%value)

gridlabd.wait()

print("global.clock = %s\n" % gridlabd.get_global('clock'))
value = gridlabd.get_value('my_test','x')
print("my_test.x = '%s'\n"%value)

gridlabd.save('done.json');
gridlabd.save('done.glm');


