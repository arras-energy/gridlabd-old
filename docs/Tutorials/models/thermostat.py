import gridlabd
class thermostat:
	ts = 120 # thermostat update rate	
	def __init__(self,obj): # this links the thermostat to the house
		self.Tair = gridlabd.property(obj,"air_temperature")
		self.Theat = gridlabd.property(obj,"heating_setpoint")
		self.Tcool = gridlabd.property(obj,"cooling_setpoint")
		self.mode = gridlabd.property(obj,"system_mode")
		self.deadband = gridlabd.property(obj,"thermostat_deadband").get_value()
	def update(self): # this is the thermostat control strategu
		T = self.Tair.get_value()
		T0 = self.Theat.get_value() - self.deadband/2
		T1 = self.Tcool.get_value() + self.deadband/2
		if T < T0:
			self.mode.set_value('HEAT')
		elif T > T1:
			self.mode.set_value('COOL')
		else:
			self.mode.set_value('OFF')
thermostats = {} # cache list thermostats
def on_precommit(t):
	return int((int(t/thermostat.ts)+1)*thermostat.ts)
def setup(obj,t): # called to set up a new thermostat in a house
	thermostats[obj] = thermostat(obj)
	gridlabd.set_value(obj,"thermostat_control",'NONE'); # disable the internal thermostat
	return 0 # setup ok
def update(obj,t):
	tstat = thermostats[obj] # get the cached thermostat class
	tstat.update() # update the thermostat
	return int((int(t/tstat.ts)+1)*tstat.ts) # return the time of the next thermostat update
	