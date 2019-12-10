module generators {
	set {QUIET=65536, WARNING=131072, DEBUG=262144, VERBOSE=524288} message_flags; // module message control flags
	bool enable_subsecond_models; // Enable deltamode capabilities within the generators module
	double deltamode_timestep[ns]; // Desired minimum timestep for deltamode-related simulations
}
class solar {
	function interupdate_gen_object();
	enumeration {SUPPLY_DRIVEN=5, CONSTANT_PF=4, CONSTANT_PQ=2, CONSTANT_V=1, UNKNOWN=0} generator_mode;
	enumeration {ONLINE=2, OFFLINE=1} generator_status;
	enumeration {CONCENTRATOR=5, THIN_FILM_GA_AS=4, AMORPHOUS_SILICON=3, MULTI_CRYSTAL_SILICON=2, SINGLE_CRYSTAL_SILICON=1} panel_type;
	enumeration {DC=1, AC=2} power_type;
	enumeration {GROUND_MOUNTED=2, ROOF_MOUNTED=1} INSTALLATION_TYPE;
	enumeration {PLAYERVALUE=2, SOLPOS=1, DEFAULT=0} SOLAR_TILT_MODEL; // solar tilt model used to compute insolation values
	enumeration {FLATPLATE=1, DEFAULT=0} SOLAR_POWER_MODEL;
	double a_coeff; // a coefficient for module temperature correction formula
	double b_coeff[s/m]; // b coefficient for module temperature correction formula
	double dT_coeff[m*m*degC/kW]; // Temperature difference coefficient for module temperature correction formula
	double T_coeff[%/degC]; // Maximum power temperature coefficient for module temperature correction formula
	double NOCT[degF];
	double Tmodule[degF];
	double Tambient[degC];
	double wind_speed[mph];
	double ambient_temperature[degF]; // Current ambient temperature of air
	double Insolation[W/sf];
	double Rinternal[Ohm];
	double Rated_Insolation[W/sf];
	double Pmax_temp_coeff;
	double Voc_temp_coeff;
	complex V_Max[V];
	complex Voc_Max[V];
	complex Voc[V];
	double efficiency[unit];
	double area[sf];
	double soiling[pu]; // Soiling of array factor - representing dirt on array
	double derating[pu]; // Panel derating to account for manufacturing variances
	double Tcell[degC];
	double Rated_kVA[kVA]; // (DEPRECATED) This variable has issues with inconsistent handling in the code, so we will deprecate this in the future (VA maps to kVA, for example).
	double rated_power[W]; // Used to define the size of the solar panel in power rather than square footage.
	complex P_Out[kW];
	complex V_Out[V];
	complex I_Out[A];
	complex VA_Out[VA];
	object weather;
	double shading_factor[pu]; // Shading factor for scaling solar power to the array
	double tilt_angle[deg]; // Tilt angle of PV array
	double orientation_azimuth[deg]; // Facing direction of the PV array
	bool latitude_angle_fix; // Fix tilt angle to installation latitude value
	double latitude[deg]; // The location of the array in degrees latitude
	double longitude[deg]; // The location of the array in degrees longitude
	enumeration {FIXED_AXIS=1, DEFAULT=0} orientation;
	set {S=112, N=8, C=4, B=2, A=1} phases;
}

