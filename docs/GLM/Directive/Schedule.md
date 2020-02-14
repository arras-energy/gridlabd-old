[[/GLM/Directive/Schedule]] -- define a value that changes over time in a pre-defined manner

# Synopsis

GLM:

~~~
schedule <schedule-name> 
{
    [normal;]
    [weighted;]
    [absolute;]
    [nonzero;]
    [positive;]
    [boolean;]
    [interpolate;]
    <minutes> <hours> <days> <months> <weekdays> <value>[;] [// <GLM comment>]
    <minutes> <hours> <days> <months> <weekdays> <value>[;] [# <schedule comment>]
    <minutes> <hours> <days> <months> <weekdays> <value>[;] [[/<minutes> <hours> <days> <months> <weekdays> <value>;] <...>]
    <...>
~~~

-or-

~~~
schedule <schedule-name> 
{
    [normal;]
    [weighted;]
    [absolute;]
    [nonzero;]
    [positive;]
    [boolean;]
    [interpolate;]
    <blockname> {
    <minutes> <hours> <days> <months> <weekdays> <value>[;] [// <GLM comment>]
    <minutes> <hours> <days> <months> <weekdays> <value>[;] [# <schedule comment>]
    <minutes> <hours> <days> <months> <weekdays> <value>[;] [[/<minutes> <hours> <days> <months> <weekdays> <value>;] <...>]
        <...>
    }
    <...>
}
~~~

# Description

Schedules are used to defined a value that changes over time in a pre-defined manner. All times in schedules are considered in local time, including timezone offset and daylight-saving/summer time offsets. Schedule are used by `loadshapes` properties and by transforms to apply the current value to other property types.

The general form of a simple schedule entry

~~~
schedule my_schedule 
{
    <minutes> <hours> <days> <months> <weekdays> <value>[;] [// <GLM comment>]
    <minutes> <hours> <days> <months> <weekdays> <value>[;] [# <schedule comment>]
    <minutes> <hours> <days> <months> <weekdays> <value>[;] [[/<minutes> <hours> <days> <months> <weekdays> <value>;] <...>]
}
~~~

The schedule directive can contain either a simple schedule, such as

~~~
schedule officehours 
{
    * 8-17 * * 1-5 # M-F 8a to 5p
}
~~~

or a complex schedule with multiple blocks, such as

~~~
schedule officehours 
{
    weekdays {
        * 8-16 * * 1-5 # Monday through Friday, 8am to 5pm
    }
    weekends {
        * 9-11,13-15 * * 6 # Saturdays, 9am-noon and 1pm to 4pm
    }
}
~~~

If you want to provide values for each time interval, they can be listed after the time specification, such as

~~~
schedule tou_price 
{
    * 21-8 * * 1-5 35 # weekdays 9pm-9am, $35
    * 9-20 * * 1-5 135 # weekdays 9am-9pm, $135
    * * * * 6-0 35 # weekends, $35
}
~~~

Omitted values on schedule items take on the default value of 1. Omitted times in the schedule take on the default value of 0.

## Options

### Normalization

Some schedules need to be normalized before they are used, depending on the application (e.g., loadshapes). When normalization takes place it is done separately over each block. The (optionally weighted) sum of the values given within the block is the normalization coefficient --- each value in the block is divided by the sum of all the values in the block. Some applications may need the signed sum and others may use the sum of the absolute values.

When weighting is used it is based on the fraction of minutes over which the value applies with respect to the total minutes over which the block applies. Only minutes that are explicitly listed in the block are count---omitted times (which are associated with the value 0.0) are ignored. If you wish to have the value 0 counted in the weighting, you must include the times for which it applies as well.

Normalization is controlled using the following options

- `normal` : enables normalization so that all values in each block are divided by the sum of the values in the block.
- `absolute` : normalization sums uses absolute values instead signed values. Its inclusion implies `normal`.
- `weighted` : normalization sums uses time-weighted values instead of simple values. Its inclusion implies `normal`.

Normalization options should be provided in-line, such as

~~~
schedule demand 
{
    weighted;
    * 21-8 * * 1-5 1.2 # weekdays 9pm-9am, weeknights
    * 9-20 * * 1-5 1.5 # weekdays 9am-9pm, weekdays
    * * * * 6-0    0.8 # weekends, holidays
}
~~~

which enables normalization using time-weighted values.

### Nonzero

The `nonzero` flag can be used to ensure that the schedule does not contain any undefined or zero values:

~~~
schedule demand 
{
    nonzero;
    * 21-8 * * 1-5 1.2 # weekdays 9pm-9am, weeknights
    * 9-20 * * 1-5 1.5 # weekdays 9am-9pm, weekdays
    * * * * 6-0    0.8 # weekends, holidays
}
~~~

### Positive

The `positive` flag can be used to ensure that the schedule does not contain any negative values:

~~~
schedule demand 
{
    positive;
    * 21-8 * * 1-5 1.2 # weekdays 9pm-9am, weeknights
    * 9-20 * * 1-5 1.5 # weekdays 9am-9pm, weekdays
    * * * * 6-0    0.8 # weekends, holidays
}
~~~

### Boolean

The `boolean` flag can be used to ensure that the schedule contains on 0 or 1 values:

~~~
schedule demand 
{
    boolean;
    * 21-8 * * 1-5 1 # weekdays 9pm-9am, weeknights
    * 9-20 * * 1-5 1 # weekdays 9am-9pm, weekdays
    * * * * 6-0    0 # weekends, holidays
}
~~~

### Absolute

The `absolute` flag can be used to ensure that the schedule normalization uses only positive magnitudes:

~~~
schedule demand 
{
    absolute;
    * 21-8 * * 1-5 1.2 # weekdays 9pm-9am, weeknights
    * 9-20 * * 1-5 1.5 # weekdays 9am-9pm, weekdays
    * * * * 6-0    0.8 # weekends, holidays
}
~~~

### Interpolation

The `interpolate` flag can be used to ensure that values used are
interpolated linearly for times between schedule changes. **Warning**:
use of this option may cause answers to vary depending on when objects
update.

# Caveats

There may be no more than 4 blocks, and each block may not contain more
than 63 *distinct* non-zero values. Although you may have more than 64
schedule entries in a block, the number of distinct values cannot exceed
64, and zero is always a value. If you have defined more than 63
distinct values, you can either limit the resolution over the dynamic
range, or you can use a orphaned player (i.e., having no parent) as a
source instead.

There are some notable differences from the *cron* syntax:

1.  The alternate use of *day* and *weekday* is not supported. If both
    day and weekday are not `*`, they are considered as *day* AND
    *weekday* rather than OR.
2.  The *step by* syntax (using `/`) is not supported.
3.  The special keywords (e.g., `\@hourly`, `\@daily`) are not supported.
4.  The weekday 7 refers to holidays, which can occur any day of the
    week. Holidays are not supported yet, but will be someday.

Because all times are considered in local time, there is a possibility
that scheduled changes during on the daylight-savings/summer time (DST)
shifts could result in a missing or duplicate value. For example,
scheduling an event at 2am the night DST ends may result in a duplicate
value. The solution is to schedule the event either before or after am
so the ambiguity is resolved internally as appropriate. Similarly,
scheduling an event at 2am the night DST starts could result in a gap in
the schedule, a problem also resolved by scheduling the event either
before or after 2am so the gap is automatically filled by the schedule
compiler.

# See also

*- [[/GLM/General/Transform]]
- [[/GLM/Property/Double]]
- [[/GLM/Property/Loadshape]]
