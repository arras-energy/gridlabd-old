[[/Module/Industrial/Industrial]] -- General NAICS industrial loads

# Synopsis

GLM:

~~~
object industrial
{
	naics_code <naics-code>;
	total_power <power>;
	power_pf <power-factor>;
	impedance_pt <power-factor>
}
~~~

# Description

The `industrial` object implements a general industrial load model based on NAICS codes with load parameters developed by the NERC Load Modeling Task Force.

# Example

~~~
module powerflow;
module industrial
{
	naics_data_file "naics_data_file.csv";
}
object load
{
	phases ABCD;
	nominal_voltage 12.47 kV;
	object industrial
	{
		parent "test";
		naics_code 325; // petro-chemical plant
		total_power 1.0 MW;
	};
}
~~~

# See also

* (NERC Technical Reference Document: Dynamic Load Modeling (December 2016))[https://www.nerc.com/comm/PC/LoadModelingTaskForceDL/Dynamic%20Load%20Modeling%20Tech%20Ref%202016-11-14%20-%20FINAL.PDF]
* (NAICS Codes)[https://www.naics.com/code-search]
* [[/Module/Industrial]]