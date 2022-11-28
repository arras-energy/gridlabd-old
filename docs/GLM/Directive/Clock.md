[[/GLM/Directive/Clock]] -- GLM clock control directive

# Synopsis

GLM:

~~~
clock
{
    timezone "<specification>";
    starttime "<datetime>";
    stoptime "<datetime>";
    runtime "<deltatime>";
}
~~~

# Description

The `clock` directive specifies how the internal clock will operate while a simulation runs.  The internal clock of a simulation runs separately from the host operating system clock, including separate handling of the timezone and daylight savings time.

## `timezone`

The `timezone` property sets the time zone for the simulation.  The time zone may be specified either as a ISO timezone, e.g., `PST+8PDT`, or a locale, e.g., `US/CA/San Francisco`.  If the time zone is not specified the simulation will use `UTC`.

## `runtime`

The `runtime` property sets the duration of the simulation run.  This will always be relative to the `starttime` and set the `stoptime` accordingly.  Valid units are `s`, `m`, `h`, `d`, and `w` for seconds, minutes, hours, days, and weeks, respectively.

Note that runtime units of months and years are not supported because they are not well-defined units.

## `starttime`

The `starttime` property specifies when the simulation start.  If no time zone is specified, the current time zone is used if it has been specified. Otherwise `UTC` is assumed. If no start time is specified, the current wall clock time is used. Note, `ISO8601` is supported.  

Note that the parameter `timestamp` is a synonym for `starttime`.

## `stoptime`

The `stoptime` property specifies when the simulation stops.  If no time zone is specified, the current time zone is used if it has been specified. Otherwise `UTC` is assumed. If no stop time is specified, `NEVER` is used, which means that the simulation will run until a steady state is achieved, if ever. Note, `ISO8601` is supported.

# Example

The following example sets up a simulation to start on 1 January 2020 at midnight Eastern Standard Time and run for one year.

~~~
clock {
    timezone "EST+5EDT";
    starttime "2020-01-01 00:00:00 EST";
    stoptime "2021-01-01 00:00:00 EST";
}
~~~

Example usage with ISO8601 format.
~~~
clock {
    timezone "EST+5EDT";
    starttime "2020-01-01T00:00:00-04:00";
    stoptime "2021-01-01T00:00:00-04:00";
}
~~~

# Caveat

Introducing more than one clock directive in a model is allowed. However, all elements of previous clock directives are reset when a new clock directive is encountered.  As a result, all previous parameters of previous settings are lost after the new directive is processed.

# See also

* [share/tzinfo.txt](https://source.gridlabd.us/blob/master/runtime/tzinfo.txt)
* [[/Subcommand/Timezone]]
