[[/Module/Reliability/Cost]] -- Outage cost tracking

# Synopsis

GLM:

~~~
  module reliability
  {
    default_cost_type FOOD|HEATING|COOKING|LIGHTS;
  }
  object cost 
  {
    total <cost> $; // OUTPUT
    type ${reliability::default_cost_type}; 
  }
~~~

# Description

The `cost` object tracks the cost impact of outages on `triplex_meter` objects.  An outage is detected when the meter's `customer_interrupted` flag is set to `TRUE`, and the cost is calculated when the flag is reset to `FALSE`.

## Properties

### `total`

~~~
  double total[$];
~~~

The total cost incurred is updated whenever service is restored to a `triplex_meter`.

### `type`

~~~
  set {FOOD,WATER,SEPTIC,GEN_BASIC,GEN_COMFORT,GEN_ALL,LODGING,BUSINESS,ELDERCARE,HEATING,COOKING,LIGHTS,WAGES} type;
~~~

The type of costs incurred can be selected individually for each home. Omitted this property will cause the global `reliability::default_cost_type` to be used.

# Example

~~~
object triplex_meter
{
    phases "AS";
    nominal_voltage "120 V";
    object cost
    {
        object recorder
        {
            file "total_outage_cost.csv";
            property "total";
            interval -1;
        };
    };
}
~~~

# See also

* [[/Module/Reliability]]

