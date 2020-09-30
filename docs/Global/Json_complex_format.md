[[/Global/Json_complex_format]] -- Control format of complex numbers written to JSON output

# Synopsis

GLM:

~~~
#set json_complex_format=[STRING|LIST|DICT|DEGREES|RADIANS]
~~~

# Description

Normally JSON output formats complex numbers according to the specifications for all complex numbers in GridLAB-D, i.e., following the standard conversion rules.  The `json_complex_format` overrides this standard formatting to allow JSON files to use special formats.

## `STRING`

The JSON output will of the form `<real>+<imag>j` or `<real>+<imag>j <unit>`.  If the `DEGREES` option is given, then the output is of the form `<mag>+<ang>d` or `<mag>+<ang>d <unit>`.  If the `RADIANS` option is given, then the output is of the form `<mag>+<arg>r` or `<mag>+<arg>r <unit>`.

## `LIST`

The JSON output will of the form `[<real>,<imag>]` or `[<real>,<imag>,"<unit>""]`.  If the `DEGREES` option is given, then the output is of the form `[<mag>,<ang>]` or `[<mag>,<ang>,"<unit>""]`.  If the `RADIANS` option is given, then the output is of the form `[<mag>,<arg>]` or `[<mag>,<arg>,"<unit>""]`.

## `DICT`

The JSON output will of the form `{"real":<real>,"imag":<imag>}` or `{"real":<real>,"imag":<imag>,"unit":<unit>}`.  If the `DEGREES` option is given, then the output is of the form `{"mag":<mag>,"ang":<ang>}` or `{"mag":<mag>,"ang":<ang>,"unit":<unit>}`.  If the `RADIANS` option is given, then the output is of the form `{"mag":<mag>,"arg":<arg>}` or `{"mag":<mag>,"arg":<arg>,"unit":<unit>}`.

## `DEGREES`

The `DEGREES` option changes the format from rectangle to polar degrees.

## `RADIANS`

The `DEGREES` option changes the format from rectangle to polar radians.

# Caveat

The behavior when using `LIST|DICT` or `DEGREES|RADIANS` is undefined.

# See also

* [[/Global/Complex_format]]