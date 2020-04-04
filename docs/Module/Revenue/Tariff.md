[[/Module/Tariff/Tariff]] - Tariff design implementation

# Synopsis

GLM:

~~~
class tariff 
{
	rate_design "";
	minimum_daily_charge "0.0";
	energy_charge_base "0.0";
	energy_charge_100 "0.0";
	energy_charge_400 "0.0";
	average_total_rate "0.0";
	minimum_average_rate_limit "0.0";
	discount "0.0";
}
~~~

# Description

The `tariff` class implement electricity tariffs.

## Properties

### `rate_design`

~~~
string rate_design;
~~~

The `rate_design` field specifies a rate design name for reference purposes.

### `minimum_daily_charge`

~~~
double minimum_daily_charge[$/day];
~~~

The `minimum_daily_charge` property specifies the minimum charge permitted per billing day.

### `energy_charge_base`

~~~
double energy_charge_base[$/kWh];
~~~

The `energy_charge_base` property specifies the Tier 1 energy price.

### `energy_charge_100`

~~~
double energy_charge_100[$/kWh];
~~~

The `energy_charge_100` property specifies the Tier 2 energy price.

### `energy_charge_400`

~~~
double energy_charge_400[$/kWh];
~~~

The `energy_charge_400` property specifies the Tier 3 energy price.

### `average_total_rate`

~~~
double average_total_rate[$/kWh];
~~~

The `average_total_rate` property specifies the blended energy price. This is not used to calculate bills, but can be used for estimation purposes.

### `minimum_average_rate_limit`

~~~
double minimum_average_rate_limit[$/kWh];
~~~

The `minimum_average_rate_limit` property specifies the minimum average rate. 

### `discount`

~~~
double discount[$/kWh];
~~~

The `discount` property specifies the low-income discount.

# Example

The following example defines a PG&E E-1 tariff.

~~~
module revenue
{
	lowincome_discount -34.8 %;
	program_credit 27.70 $;
	program_credit_months [4,10];
	summer_season_months [6,7,8,9];
	winter_season_months [1,2,3,4,5,10,11,12];
	billing_module "test_billing";
	billing_library "${TESTDIR:-.}";
};

object tariff {
	name "E1";
	rate_design "Tiered Energy Charges";
	minimum_daily_charge 0.32854 $/day;
	energy_charge_base 0.23522 $/kWh;
	energy_charge_100 0.29600 $/kWh;
	energy_charge_400 0.51860 $/kWh;
	average_total_rate 0.25659 $/kWh;
}
~~~

# See also

* [PG&E Tariffs](https://www.pge.com/tariffs/electric.shtml)
