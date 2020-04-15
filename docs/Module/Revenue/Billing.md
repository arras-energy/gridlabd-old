[[/Module/Revenue/Billing]] - Billing object implementation

# Synopsis

GLM:

~~~
class billing 
{
	tariff "";
	meter "";
	bill_day "0";
	bill_date "TS_ZERO";
	billing_days "0";
	total_bill "0.0";
	total_charges "0.0";
	energy_charges "0.0";
}
~~~

# Description

## Properties

### `bill_date`

~~~
TIMESTAMP bill_date; // (OUTPUT)
~~~

The `bill_date` property gives the date of the last bill generated.

### `bill_day`

~~~
int32 bill_day;
~~~

The bill day determine on which day of the month the bill is generated.  If `bill_day` is positive, the bill is computed on the corresponding day of each month.  

If `bill_day` is negative, the bill is computed on the corresponding day of the each month counted from the last day, i.e., -1 is the last day, -2 is the penultimate day, etc.  

If `bill_day` exceeds the number days in a month, then the bill day is moved of the last or first day of the month for positive or negative values, respectively. 

If `bill_day` is zero, then a random number between 1 and 31, inclusive, in chosen during initialization.

### `billing_days`

~~~
int32 billing_days; // (OUTPUT)
~~~

The `billing_days` property gives the number of days covered by the last bill generated.

### `meter`

~~~
object meter;
~~~

The `meter` property specifies the name of the meter object from which usage data is collected.  This object may be a [[/Module/Powerflow/Meter]] or [[/Module/Powerflow/Triplex_meter]] object.

### `tariff`

~~~
object tariff;
~~~

The `tariff` property specifies the name of the tariff object from which rate data is collected.  The object must be a [[/Module/Revenue/Tariff]] object.

### `total_bill`

~~~
double total_bill; // (OUTPUT)
~~~

The `total_bill` property gives the total of all the bills generated.

### `total_charges`

~~~
double total_charges; // (OUTPUT)
~~~

The `total_charges` property gives the total charges of all the bills generated.

# Example

~~~
#include "my-tariffs.glm"
object triplex_meter
{
	name "meter-name";
	phases AS;
	nominal_voltage 120 V;
	object house
	{
	};
}
object billing 
{
	meter "meter-name";
	tariff "tariff-name";
	baseline_demand 10.5 kWh/day;
}

~~~

# See also

* [[/Module/Powerflow/Meter]]
* [[/Module/Powerflow/Triplex_meter]]
* [[/Module/Revenue/Tariff]]
