[[/Utilities/Create_player]] - Player object creation tool

# Synopsis

Shell:

    bash$ gridlabd clean [-i|--input=INPUTCSV[#READOPTION[#...]][,...]]]
        [-o|--output=OUTPUTCSV[#WRITEOPTION[#...]]]
        [-p|--player=OUTPUTGLM] [-n|--name=OBJNAME] [-t|--target=TARGETNAME]
        [OPTIONS ...]

GLM:

    #clean -i|--input=INPUTCSV[#READOPTION[#...]][,...]]
        -o|--output=OUTPUTCSV [#WRITEOPTION[#...]]
        [-p|--player=OUTPUTGLM] [-n|--name=OBJNAME] [-t|--target=TARGETNAME]
        [OPTIONS ...]

Options:

    --collation=METHOD  input collation method. Valid methods are `merge`
                        or `concat`. If `merge` is specified, the method
                        of joining data can be specified using METHOD:JOIN.
                        Value join methods are `left`, `right`, `outer`,
                        `inner`, or `cross` (the default is "merge:inner").

    --resample[=SPEC]   resample the data with the specification SPEC.
                        SPEC=[METHOD]@[FREQ] where FREQ is of the form
                        [[[[[WEEKS:]DAYS:]HOURS:]MINUTES:]SECONDS]
                        ('mean@1:0:0' by default)

    --dropna[=AXIS]     remove rows (AXIS=0 by default) or columns
                        (AXIS=1) with NA values

    --fillna[=VALUE]    replace NA values with value (VALUE=0 by default)

# Description

The `create_player` tool collated CSV files and prepares data for use by
GridLAB-D player objects.

Each input file is loaded using the specified read options and concatenated
into a single dataframe.  Date/time values are normalized. The first column
is used as the primary index and the resulting data is sorted accordingly.
Resampling and NA handing are completed and the output is generated to the
specified output file, or to `/dev/stdout` by default.

# Input

The inputs are specified as comma delimited path names. Options are provided
using # separator in the format OPTION[:VALUE].  If VALUE is omitted, then
the `True` is used.  If the OPTION is already specified in the READOPTIONS
list then the type VALUE of much match the default value's type.  Otherwise
the type of VALUE is assumed to be `str`.

The following READOPTIONS are given by default (see Pandas read_csv for
details):

    delimiter : ','

    header : 0
    
    index_col : 0
    
    parse_dates : [0]

    infer_datetime_format : True
    
    dayfirst : False

The follow WRITEOPIONS are given by default (see Pandas to_csv for details):

    index_label : "datetime"

    date_format : "%Y-%m-%d %H:%M:%S"

    header : False

    float_format : "%.6g"
    
# Output

The output is always written as CSV data that is compatible with GridLAB-D
players, i.e., timestamps in column 0 formatted at `yyyy-mm-dd HH:MM:SS`,
with no header row.

If the PLAYERGLM option is specified, then a player GLM component is written
to the specified PLAYERGLM file.  When used, the name option may specify the
OBJNAME. By the default the OBJNAME is the base name of the first input file.
The output GLM file include a `module` directive to load the `tape` module,
as well as `#define` statement to indicate the start and end dates of the
primary index of the player's input data. Note that the target option must
specify the TARGETNAME (the player's parent object), and the target object
must have properties that match the output column names.

# Examples

The following example prepares the following input data to be used by a player
and generates a player object to read the resulting CSV file.

Input CSV:

    timestamp,real_power
    10/1/14 0:00,1798.85
    10/1/14 0:05,1782.63
    10/1/14 0:10,1766.58
    10/1/14 0:15,1768.91
    ...

Command:

    bash$ gridlabd python -m clean -i=example/weather.csv \
    -p=/tmp/test.glm -o=/tmp/test.csv -t=test 

CSV output:

    2014-10-01 00:00:00,1798.85
    2014-10-01 00:05:00,1782.63
    2014-10-01 00:10:00,1766.58
    2014-10-01 00:15:00,1768.91
    ...

GLM output:

    #define power_startdate "2014-10-01 00:00:00"
    #define power_stopdate "2017-09-30 23:55:00"
    module tape;
    object player {
        name "power";
        parent "test";
        property "real_power";
        file "/tmp/test.csv";
    }

The following example resamples the input data to obtain the maximum value
in each day

    bash$ gridlabd python -m clean -i=example/power.csv -o=/dev/stdout#header:1 --resample=max@1:0:0 

CSV output:

    datetime,real_power
    2014-10-01 00:00:00,1782.63
    2014-10-01 01:00:00,1701.61
    2014-10-01 02:00:00,1634.6
    2014-10-01 03:00:00,1640.41

# See Also

* [[/Module/Tape/Player]]
* [Pandas Documentation](https://pandas.pydata.org/docs/reference/)
