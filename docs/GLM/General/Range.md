[[GLM/General/Range]] -- Generate number series in a range

# Synopsis
GLM:
~~~
  ${RANGE[<delimiter>[<start>[,<stop>[,<step>]]]]}
~~~

# Description

The `RANGE` global generates a series of delimited values.  The following syntaxes are supported.

## `${RANGE}`

This syntax will generate the default range, which is `0 1`.

## `${RANGE<delim>}`

This syntax will generate the default range, which is `0<delim>1`.

## `${RANGE<delim><start>}`

This syntax will generate a range from `<start>` to `1` using `<delimiter>` between the values, and incrementing by `1.0` between values.

## `${RANGE<delim><start>,<stop>}`

This syntax will generate a range from `<start>` to `<stop>` using `<delimiter>` between the values, and incrementing by `1.0` between values.

## `${RANGE<delim><start>,<stop>,<step>}`

This syntax will generate a range from `<start>` to `<stop>` using `<delimiter>` between the values, and incrementing by `<step>` between values.

# Example

The following GLM file generates various ranges.

`range_example.glm`:
~~~
  #set suppress_repeat_messages=FALSE
  #print ${RANGE}
  #print ${RANGE,}
  #print ${RANGE -1}
  #print ${RANGE;1,10};
  #print ${RANGE,1,10,0.5}
~~~

The output is as follows:
~~~
range_example.glm(2): 0 1
range_example.glm(3): 0,1
range_example.glm(4): -1 0 1
range_example.glm(5): 1;2;3;4;5;6;7;8;9;10;
range_example.glm(6): 1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,8.5,9,9.5,10
~~~

# Caveats

If the last value generated is exactly equal to `<stop>`, then it is included in the range output.

If the range generated is too long for the available buffer space allocated to range generation, or the `<step>` is invalid for the `<start>` and `<stop>` values specified, then an error will be generated and processing will stop.

# See also
* [[GLM/Macro/For]]
