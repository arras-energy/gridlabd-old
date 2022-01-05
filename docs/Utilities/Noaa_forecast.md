[[/Utilities/Noaa_forecast]] -- NOAA weather forecast utility

# Synopsis

Shell:

~~~
    bash$ gridlabd noaa_forecast -p|-position=LAT,LON [-i|--interpolate=TIME|METHOD]
        [-g|--glm=GLMNAME] [-n|--name=OBJECTNAME] [-c|--csv=CSVNAME] [--test] [-h|--help|help]
~~~

GLM:

~~~
    #python -m noaa_forecast -p|-position=LAT,LON [-i|--interpolate=TIME|METHOD]
        [-g|--glm=GLMNAME] [-n|--name=OBJECTNAME] [-c|--csv=CSVNAME] [--test] [-h|--help|help]
    #include "GLMNAME"
~~~

Python:

~~~
    bash$ gridlabd python
    >>> import noaa_forecast as nf
    >>> ns.getforecast(37.5,-122.3)
                                   temperature[degF]  wind_speed[m/s]  wind_dir[deg]
    2021-10-21 14:00:00-07:00          68.000000         4.470400     202.500000
    2021-10-21 15:00:00-07:00          65.327601         4.275738     201.983153
    2021-10-21 16:00:00-07:00          62.990829         4.099336     201.673044
    ...                                      ...              ...            ...
    2021-10-28 06:00:00-07:00          57.284721         2.770949     315.450517
    2021-10-28 07:00:00-07:00          55.246212         2.946331     315.403079
    2021-10-28 08:00:00-07:00          53.000000         3.129280     315.000000
    [163 rows x 3 columns]
~~~

# Description

This module downloads weather forecasts from NOAA and writes GLM files.  This can be done from
the command line or using call the python API.

Interpolation is usually necessary because the data samples received from NOAA span several hours.
The default interval is 60 minutes, but can be set to any integer value in minutes. The sampling
method is by default `linear`.  Interpolation methods supported include 

  - `linear`

    Ignore the index and treat the values as equally spaced. This is the only method 
    supported on MultiIndexes.

  - `time`

    Works on daily and higher resolution data to interpolate given length of interval.

  - `index`, `values`

    Use the actual numerical values of the index.

  - `pad`

    Fill in NaNs using existing values.

  - `nearest`, `zero`, `slinear`, `quadratic`, `cubic`, `spline`, `barycentric`, `polynomial`

    Passed to `scipy.interpolate.interp1d`. These methods use the numerical values of the index. 
    Both `polynomial` and `spline` require that you also specify an order (`int`), e.g. 
    `df.interpolate(method='polynomial', order=5)`.

  - `krogh`, `piecewise_polynomial`, `spline`, `pchip`, `akima`, `cubicspline`

    Wrappers around the SciPy interpolation methods of similar names.

  - `from_derivatives`

    Refers to `scipy.interpolate.BPoly.from_derivatives` which replaces `piecewise_polynomial` 
    interpolation method in scipy 0.18.

## Parameters

The module uses several parameters to control its behavior. 

~~~
    server = "https://api.weather.gov/points/{latitude},{longitude}" # NOAA location server (provides forecast URL)
    user_agent = "(gridlabd.us, gridlabd@gmail.com)" # default user agent to report to NOAA
    date_format = "%Y-%m-%d %H:%M:%S"
    float_format="%.1f" # float format to use 
    interpolate_time = 60
    interpolate_method = 'quadratic'
~~~

The parameters can be changed before obtained the forecast.

~~~
    >>> import noaa_forecast as nf
    >>> nf.interpolate = 5
    >>> nf.getforecast(37.5,-122.3)
~~~

# Example

The following command downloads only the CSV data for a location:

~~~
    bash$ gridlabd noaa_forecast -p=45.62,-122.70 -c=test.csv
~~~

The following command downloads the CSV data and creates a GLM file with the data linked and weather object named:

~~~
    bash$ gridlabd noaa_forecast -p=45.62,-122.70 -c=test.csv -n=test -g=test.glm
~~~

# See also

* [https://www.weather.gov/documentation/services-web-api]
