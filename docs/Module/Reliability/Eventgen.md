module reliability {
	set {QUIET=65536, WARNING=131072, DEBUG=262144, VERBOSE=524288} message_flags; // module message control flags
	bool enable_subsecond_models; // Flag to enable deltamode functionality in the reliability module
	double maximum_event_length[s]; // Maximum duration of any faulting event
	bool report_event_log; // Should the metrics object dump a logfile?
	int32 deltamode_timestep; // Default timestep for reliability deltamode operations
}
class eventgen {
	function add_event();
	function interupdate_event_object();
	char1024 target_group;
	char256 fault_type;
	enumeration {TRIANGLE=10, BETA=9, GAMMA=8, WEIBULL=7, RAYLEIGH=6, EXPONENTIAL=5, PARETO=4, BERNOULLI=3, LOGNORMAL=2, NORMAL=1, UNIFORM=0} failure_dist;
	enumeration {TRIANGLE=10, BETA=9, GAMMA=8, WEIBULL=7, RAYLEIGH=6, EXPONENTIAL=5, PARETO=4, BERNOULLI=3, LOGNORMAL=2, NORMAL=1, UNIFORM=0} restoration_dist;
	double failure_dist_param_1;
	double failure_dist_param_2;
	double restoration_dist_param_1;
	double restoration_dist_param_2;
	char1024 manual_outages;
	double max_outage_length[s];
	int32 max_simultaneous_faults;
	char256 controlled_switch; // Name of a switch to manually fault/un-fault
	int32 switch_state; // Current state (1=closed, 0=open) for the controlled switch
}

