[[/Subcommand/Convert]] -- Subcommand to convert files from one format to another

# Synopsis

~~~
bash$ gridlabd convert [[-i|--input] INPUTFILE] [[-o|--output] OUTPUTFILE] [-v|--verbose] [-f|--from INPUTTYPE] [-t|--to OUTPUTTYPE] [OPTIONS]
~~~

# Description

The `convert` subcommand performs the file conversions used by the automatic input and output file converters in GridLAB-D. 

If `INPUTFILE` is not specified, then `/dev/stdin` is used.  If `OUTPUTFILE` is not specified, then `/dev/stdout` is used. 

If `INPUTTYPE` is not specified, then `INPUTFILE` must be specified and the `INPUTTYPE` is the extension of `INPUTFILE`.  The same applies to `OUTPUTTYPE`.  If `/dev/stdin` or `/dev/stdout` are used then the respective file type must be specified.  

All other arguments are passed through to the converter as follows using the `NAME[=VALUE]` syntax. If `NAME` is provided without a value, then the value `None` is provided to the converter.

# See also

* [[/Converters/README]]
