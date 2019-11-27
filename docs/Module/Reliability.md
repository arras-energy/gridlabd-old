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

class metrics {
	char1024 report_file;
	char1024 customer_group;
	object module_metrics_object;
	char1024 metrics_of_interest;
	double metric_interval[s];
	double report_interval[s];
}

class metrics {
	char1024 report_file;
	char1024 customer_group;
	object module_metrics_object;
	char1024 metrics_of_interest;
	double metric_interval[s];
	double report_interval[s];
}

