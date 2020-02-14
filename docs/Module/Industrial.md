[[/Module/Industrial]] -- Module industrial

# Synopsis

GLM:

~~~
module industrial 
{
	naics_data_file "<filename.csv>";
}
~~~

# Description

The `industrial` module implement industrial loads.

## Globals

### `naics_data_file`
~~~
naics_data_file "<filename.csv>";
~~~

The format of a NAICS data record is as follows:
~~~
<naics-code>,<description>,<lmtf-code>,<motor-a-fraction>,<motor-b-fraction>,<motor-c-fraction>,<motor-d-fraction>,<power-electronic-fraction>,<constant-current-fraction>,<constant-impedance-fraction>
~~~

If a NAICS code is listed more than once, only the first one is used.

# See also

* [[/Module/Industrial/Industrial]]
* [[/Module/Industrial/NAICS Data]]

