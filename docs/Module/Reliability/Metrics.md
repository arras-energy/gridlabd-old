module reliability {
	set {QUIET=65536, WARNING=131072, DEBUG=262144, VERBOSE=524288} message_flags; // module message control flags
	bool enable_subsecond_models; // Flag to enable deltamode functionality in the reliability module
	double maximum_event_length[s]; // Maximum duration of any faulting event
	bool report_event_log; // Should the metrics object dump a logfile?
	int32 deltamode_timestep; // Default timestep for reliability deltamode operations
}
class metrics {
	char1024 report_file;
	char1024 customer_group;
	object module_metrics_object;
	char1024 metrics_of_interest;
	double metric_interval[s];
	double report_interval[s];
}

