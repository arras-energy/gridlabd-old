import sys
assert(sys.version_info.major>2)
import time
import gridlabd
import my_test

def showme(context):
	print("\n%s:\n\tglobal.clock.... %s\n\tmy_test.x....... %s" % (context,gridlabd.get_global('clock'),gridlabd.get_value('my_test','x')))

# def on_commit(t) :
# 	print('on_commit(%d)' % t)

# print("mod = %d"%gridlabd.module(my_test))
# print("mod = %d"%gridlabd.module(my_test))
# sys.stdout.flush()

#gridlabd.command('--debug')
gridlabd.command('validate.glm')
#gridlabd.command('-D')
#gridlabd.command('show_progress=FALSE')
gridlabd.start('pause')
gridlabd.set_global('glm_save_options','MINIMAL')
gridlabd.resume()
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


