[[/Module/Tariff/Tariff]] - Tariff design implementation

# Synopsis

GLM:

~~~
class tariff 
{
	rate_design "";

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

# Example

The following example defines a PG&E E-1 tariff.

~~~
module revenue
{
	billing_module "test_billing";
	billing_library "${TESTDIR:-.}";
};
class pgande_residential_tariff {
	parent tariff;
	double minimum_daily_charge[$/day];
	double energy_charge_base[$/kWh];
	double energy_charge_100[$/kWh];
	double energy_charge_400[$/kWh];
	double average_total_rate[$/kWh];
	double minimum_average_rate_limit[$/kWh];
	double discount[$];
	char256 billing_function;
}
object pgande_residential_tariff {
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
