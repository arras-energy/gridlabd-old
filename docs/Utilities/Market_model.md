[[/Utilities/Market_model]] -- Create market model from ISO data

# Synopsis

Shell:

    sh$ gridlabd python market_model [-f|--file=DATACSV] [-i|--input=INPUTCOL(S)] [-o|--output=OUTPUTCOL] [
    -l|--lookahead=LOOKAHEAD] [-k|--order] [-g|--glm=GLMNAME] [-I|--init=ISODATETIME] [-M|--module=MODULENAME] [
    -C|--class=CLASSNAME] [-N|--name=OBJNAME] [-P|--player[=PLAYERCSV]] [-R|--recorder=RECORDERCSV] [--{by,with}_{hour,
    day,month,year}] [-v|--verbose] [-w|--warning] [-q|--quiet] [-d|--debug] [-h|--help|help]

GLM:

    #python -m market_model [-f|--file=DATACSV] [-i|--input=INPUTCOL(S)] [-o|--output=OUTPUTCOL] [
    -l|--lookahead=LOOKAHEAD] [-k|--order] [-g|--glm=GLMNAME] [-I|--init=ISODATETIME] [-M|--module=MODULENAME] [
    -C|--class=CLASSNAME] [-N|--name=OBJNAME] [-P|--player[=PLAYERCSV]] [-R|--recorder=RECORDERCSV] [--{by,with}_{hour,
    day,month,year}] [-v|--verbose] [-w|--warning] [-q|--quiet] [-d|--debug]


# Description

The `market_model` utility creates NxM filters from N input and M output
channels of data by fitting a Kth-order discrete-time transfer functions to
the data using a least-squares linear regressions of the equation:

    Y0 = -a1*Y1 - a2*Y2 - ... - aK*Yk + b0*U0 + b1*U1 + ... + bK*Uk

where

    Y0 is the output channel 0 at time t
    Y1 is the output channel 0 at time t-1
    ...
    YK is the output channel 0 at time t-K

    U0 is the input channel 0 at time t
    U1 is the output channel 0 at time t-1
    ...
    UK is the input channel 0 at time t-K

    a and b are the coefficients of the transfer functions

If a GLMNAME and CLASSNAME are specified, then an object of that class will be
created with the filter as inputs and object properties as outputs.  If the
module is not specified, the class is defined as a runtime class with all
inputs and outputs specified as doubles.  If OBJNAME is specified the object is 
given that name, otherwise a random object name is generated and appended to the 
OBJNAME global variable.

If PLAYERCSV is specified, then any created object will have the inputs 
assigned from that file.  If RECORDERCSV is specified, then any object created will
have the outputs recorded to that file.

If --with_{hour,day,month,year} is specified, then an hour, day, month, or
year column is added to the input data.  If --by_{hour,day,month,year} is
specified, then the input data is grouped by hour, day, month, or year. If
--with_time is specified, then the elapsed time in the specified units is
included in a column.

## Configuration

By default all the columns of the input and output files are used in the
filter model identification.  Only columns listed in the `converters` class will be used to generate filters.
The class `converters` must be defined, and contain the properties `cols`.  These must be
dictionaries where the keys are the column names, and values of the
converters to be used for those columns.  

For the example below, the following converter files can be used:

    from datetime import datetime
    class converters:
    def real(x):
        try:
            return float(x)
        except:
            return float('NAN')

    def datetime(x):
        return datetime.strptime(x, "%Y-%m-%d %H:%M:%S")

    cols = {"START_TIME_LOCAL": datetime, 'MW': real, 'LMP': real}

Note that using a converter often speeds up the CSV read process because the
date/time format is specified and the loader does not need to deduce the
format automatically, which can take significant additional time.

To limit which columns of the input or output files are used, it is sufficient
to omit them from the converter list.  For example, 

    cols = {"START_TIME_LOCAL":datetime, "MW":real}

omits the LMP data in the input file.

# Example

    $ python market_model -f=caiso_20220101-20220201.csv -i=LMP -o=MW -k=24 -l=3

