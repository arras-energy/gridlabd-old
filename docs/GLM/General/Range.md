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

## `${RANGE<delim><start>}`

This syntax will generate a range from `<start>` to `1` using `<delimiter>` between the values, and incrementing by `1.0` between values.

## `${RANGE<delim><start>,<stop>}`

This syntax will generate a range from `<start>` to `<stop>` using `<delimiter>` between the values, and incrementing by `1.0` between values.

## `${RANGE<delim><start>,<stop>,<step>}`

This syntax will generate a range from `<start>` to `<stop>` using `<delimiter>` between the values, and incrementing by `<step>` between values.

# Caveats

If the last value generated is exactly equal to `<stop>`, then it is included in the range output.

If the range generated to too long for the available buffer space allocated to range generation, or the `<step>` is invalid for the `<start>` and `<stop>` values specified, then an error will be generated and processing will stop.

# See also
* [[GLM/Macro/For]]
