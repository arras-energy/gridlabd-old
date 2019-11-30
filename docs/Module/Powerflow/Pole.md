[[Pole]] -- Power pole model

# Synopsis
~~~
 class pole {
 	parent node;
 	enumeration {FAILED=1, OK=0} pole_status; // pole status
 	double tilt_angle[rad]; // tilt angle of pole
 	double tilt_direction[deg]; // tilt direction of pole
 	object weather; // weather data
 	object configuration; // configuration data
 	double equipment_area[sf]; // equipment cross sectional area
 	double equipment_height[ft]; // equipment height on pole
 	int32 install_year; // year pole was installed 
        double repair_time[h]; // pole repair time
 }
~~~

# Description

The pole failure model is described in [Pole Loading Model](https://github.com/dchassin/gridlabd/raw/master/powerflow/docs/pole_loading.pdf). 

The pole reaches end of life status based on a degradation rate that is defined by minimum shell thickness of 2". See [Pole Degradation Model](https://www.sciencedirect.com/science/article/pii/S0167473005000457) details.

Generally, any node in a powerflow model can be upgraded to a pole by providing the [[weather]] and [[pole_configuration]] object references. 

Specifying `equipment_area` and `equipment_height` will cause the equipment wind load to be computed.

# See also
* [[Pole_configuration]]
* [Pole Loading Model](https://github.com/dchassin/gridlabd/raw/master/powerflow/docs/pole_loading.pdf)
* [Pole Degradation Model](https://www.sciencedirect.com/science/article/pii/S0167473005000457)
