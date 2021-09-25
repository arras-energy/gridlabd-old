[[/CEUS]] -- Commercial End-Use Survey data-driven building model
# Synopsis

~~~
  module commercial {
    default_nominal_voltage "240";
    default_nominal_voltage_A "240+0d";
    default_nominal_voltage_B "240+120d";
    default_nominal_voltage_C "240-120d";
    default_weekday_code "WEEKDAY";
    default_saturday_code "SATURDAY";
    default_sunday_code "SUNDAY";
    default_holiday_code "HOLIDAY";
    default_month_heading "Month";
    default_daytype_heading "Daytype";
    default_hour_heading "Hour";
    default_temperature_heating_balance "55.0 degF";
    default_temperature_cooling_balance "70.0 degF";
    default_temperature_heating_base "55.0 degF";
    default_temperature_cooling_base "70.0 degF";
    default_solargain_base "0.0 W/m^2";
    default_price_base "0.0 $/MWh";
  }
  object ceus {
    filename "<file-name>";
    floor_area "<real-value> sf";
    composition "<enduse>:{<parameter>:<value>;...}";
    weather "<object-name>";
    total_power_A "<complex-value> VA";
    total_power_B "<complex-value> VA";
    total_power_C "<complex-value> VA";
    total_real_power "<real-value> [W]";
    total_reactive_power "<real-value> [VAR];
    weather "<object-name>";
    temperature_heating_balance "<real-value> degF";
    temperature_cooling_balance "<real-value> degF";
    temperature_heating_base "<real-value> degF"]";
    temperature_cooling_base "<real-value> degF";
    temperature_heating_sensitivity "<real-value> W/degF";
    solargain_base "<real-value> W/m^2";
    solargain_sensitivity "<real-value> m^2";
    tariff "<object-name>";
    price_base "<real-value> $/MWh";
    price_sensitivity "<real-value> W*MWh/$"; 
  }
~~~

# Description

The CEUS commercial building load model is based on the commercial energy use data collected by Itron for the California Energy Commission. The prepared data files for each California forecasting climate zone (FCZ) are available from the GitHub repository https://github.com/slacgismo/ceus_data in the [`loadshape`](https://github.com/slacgismo/ceus_data/tree/master/loadshape) folder.

The parent object of a building must be a `powerflow meter` object.  In the absence of a suitable parent object, the building will use the global variables `default_nominal_voltage_A`, `default_nominal_voltage_B`, `default_nominal_voltage_C` and `default_nominal_voltage` to determine the voltage.  The solver will update the meter's power demand value when the load changes.

## Properties

### `composition`

~~~
    method composition;
~~~

The load composition determines how much power is consumed by each end-use specified in the CEUS data file. The real and reactive fractions are given by the following terms
* `Zr`: the real constant impedance fraction
* `Ir`: the real constant current fraction
* `Pr`: the real constant power fraction
* `Zi`: the reactive constant impedance fraction
* `Ii`: the reactive constant current fraction
* `Pi`: the reactive constant power fraction

In general the sum the real power fractions should be 1.0 and the sum of the reactive power fractions should be less of 0.1.

The `Area` term of the fraction of the `floor_area` affected by the enduse specified. The default area fraction is `1.0`. The `Electric` term specifies that fraction of the end-use that is electrified. The default is `1.0`.

In addition, the composition can represent load sensitivities to temperature, price, and occupancy.  The composition parameters are
* `Th`, `Thb`, `Thc`, `Th0`, `Th1`: the enduse heating mode temperature sensitivity slope, base, intercept, domain min and max, respectively
* `Tc`, `Tcb`, `Tcc`, `Tc0`, `Tc1`: the enduse cooling mode sensitivity slope, base, intercept, domain min and max, respectively
* `S`, `Sb`, `Sc`, `S0`, `S1`: the enduse solar gain sensitivity slope, base, intercept, domain min and max, respectively
* `E`, `Eb`, `Ec`, `E0`, `E1`: the enduse price sensitivity slope, base, intercept, domain min and max, respectively
* `Oh`, `Ob`, `Oc`, `O0`, `O1`: the enduse occupancy sensitivity slope, base, intercept, domain min and max, respectively

### `filename`

~~~
    method filename;
~~~

The CEUS data file is loaded when the `filename` is specified.  Each file is loaded only once and referenced by each building that uses it.  The loads in the data are used as the basis for the loadshapes that drive the final load composition.

### `floor_area`

~~~
    double floor_area[sf];
~~~

The floor area of the build.

### `price_base`

~~~
    double price_base[$/MWh];
~~~

The base price for the price sensitivity.

### `total_power_A`

~~~
    complex total_power_A[VA];
~~~

The total power on phase A.

### `total_power_B`

~~~
    complex total_power_B[VA];
~~~

The total power on phase B.

### `total_power_C`

~~~
    complex total_power_C[VA];
~~~

The total power on Phase C.

### `total_real_power`

~~~
    double total_real_power[W];
~~~

The total real power.

### `total_reactive_power`

~~~
    double total_reactive_power[VAR];
~~~

The total reactive power.

### `price_sensitivity`

~~~
    double price_sensitivity[W*MWh/$]; 
~~~

The price sensitivity.

### `solargain_base`

~~~
    double solargain_base[W/m^2];
~~~

The solar gain sensitivity base irradiance.

### `solargain_sensitivity`

~~~
    double solargain_sensitivity[m^2];
~~~

The solar gain sensitivity.

### `temperature_cooling_base`

~~~
    double temperature_cooling_base[degF];
~~~

The cooling sensitivity base temperature. 

### `temperature_cooling_balance`

~~~
    double temperature_cooling_balance[degF];
~~~

Cooling sensitivity is non-zero only for outdoor air temperatures above the cooling balance temperature.

### `temperature_heating_base`

~~~
    double temperature_heating_base[degF];
~~~

The heating sensitivity base temperature.

### `temperature_heating_balance`

~~~
    double temperature_heating_balance[degF];
~~~

Heating sensitivity is non-zero only for outdoor air temperatures below the heatin balance temperature.

### `temperature_heating_sensitivity`

~~~
    double temperature_heating_sensitivity[W/degF];
~~~

The heating temperature sensitivity.

### `tariff`

~~~
    object tariff;
~~~

The tariff object is used to establish now the pricing affects loads.

### `weather`

~~~
    object weather;
~~~

The `weather` object provides the current temperature, if given.  In the absence of a weather reference, no temperature sensitivity is applied to the load. 

# Sensitivity Model

The power impact of temperature changes are computed as follows. When $ OutdoorTemperature < TemperatureHeatingBalance $ then
$$ 
    Power = ( OutdoorTemperature - TemperatureHeatingBase ) \times TemperatureHeatingSensitivity. 
$$ 
When $ OutdoorTemperature > TemperatureCoolingBalance $ then 
$$ 
    Power = ( Temperature - TemperatureCoolingBase ) \times TemperatureCoolingSensitivity. 
$$

Similarly, the `weather` is used to obtain the solar irradiance sensitivity impact on power, which is computed as follows:
$$
  Power = ( Solargain - SolargainBase ) \times SolargainSensitivity.
$$

The `tariff` object provides the `price` values used to compute the load sensitivity to price. The power impact of price changes is computed as follows:
$$
  Power = ( Price - PriceBase ) \times PriceSensitivity.
$$

# Example

The following implements a `10,000 sf` office building in California forecasting climate zone 1 with 90% constant impedance at unity power factor, and 10% constant power at power factor `~0.89`:

~~~
  object ceus {
    filename "FCZ01_SOFF.csv";
    floor_area 10 ksf;
    composition "Heating:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Cooling:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Ventilation:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Water_Heating:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Refrigeration:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Exterior_Lighting:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Interior_Lighting:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Office_Equipment:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Miscellaneous:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Process:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Motors:{ZR:0.9;PR:0.1;PI:0.01}";
    composition "Air_Compressors:{ZR:0.9;PR:0.1;PI:0.01}";
  }
~~~

# Caveats

The temperature, price, and solar sensitivity are not implemented by enduse.  These sensitivities are applied at the whole building level only.

# See also

* 

* [[/Module/Commercial]]
* [California Commercial End-use Survey (CEUS) Website](https://www.energy.ca.gov/ceus)
* [SLAC GISMo CEUS Data Repository](https://github.com/slacgismo/ceus_data)
