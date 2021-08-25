[[/Subcommand/Plot]] -- GridLAB-D plot subcommand

# Synopsis

Shell:

~~~
$ gridlabd plot OPTIONS
~~~

GLM:

~~~
#plot OPTIONS
~~~

or

~~~
#on_exit CODE gridlabd plot OPTIONS
~~~

# Description

The 'gridlabd plot' command uses pandas and matplotlib to create PNG files from
    CSV files.

## Options

The following options are available:

### `-d|--debug`

Enable debug output (if any)

### `-e|--exception`

Enable raising of exception instead of printing exception message and exiting with error code 3.

### `--figure:OPTIONS[=VALUE]` 

Specify matplotlib pyplot figure option

### `-h|--help|help`

Display this help information

### `-i|--input=PATH`

Specify the input CSV pathname

### `-o|--output=PATH`

Specify the output PNG pathname (default is input with ".png")

### `-q|--quiet`

Disable error output (if any)

### `-s|--show`

Show the output

### `--plot:OPTIONS[=VALUE]`   

Specify pandas DataFrame plot option

### `-v|--verbose`

Enable verbose output (if any)

### `-w|--warning`

Disable warning output (if any)

# Example

The following example illustrates how to generate a plot of two fields generated
by a recorder.

~~~
module tape
{
    csv_header_type NAME;
}
clock
{
    timezone "PST+8PDT";
    starttime "2020-01-01 00:00:00 PST";
    stoptime "2020-01-02 00:00:00 PST";
}
class test {
    randomvar x[MW];
    randomvar y[MWh];
}
object test
{
    x "type:normal(100,10); min:0; max:200; refresh:1h";    
    y "type:normal(100,10); min:0; max:200; refresh:1h";
    object recorder
    {
        property "x,y";
        interval -1;
        file ${modelname/glm/csv};
    };
}
#on_exit 0 gridlabd plot -i=${modelname/glm/csv} --plot:x=timestamp --plot:y=x,y --plot:rot=90 --plot:grid --plot:legend --figure:tight_layout=True
~~~
