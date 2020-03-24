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
	capacity_charges "0.0";
	ramping_charges "0.0";
	fixed_charges "0.0";
	total_credits "0.0";
	energy_credits "0.0";
	capacity_credits "0.0";
	ramping_credits "0.0";
	fixed_credits "0.0";
}
~~~

# Description

## Properties

### `bill_day`

The bill day determine on which day of the month the bill is generated.  If `bill_day` is positive, the bill is computed on the corresponding day of each month.  

If `bill_day` is negative, the bill is computed on the corresponding day of the each month counted from the last day, i.e., -1 is the last day, -2 is the penultimate day, etc.  

If `bill_day` exceeds the number days in a month, then the bill day is moved of the last or first day of the month for positive or negative values, respectively. 

If `bill_day` is zero, then a random number between 1 and 31, inclusive, in chosen during initialization.

# Example

# See also