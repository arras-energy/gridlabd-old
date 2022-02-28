Create filter tool

# Synopsis

Shell:

    bash$ gridlabd create_filter NAME NUMERATOR DENOMINATOR [OPTIONS ...]

GLM:

    #create_filter NAME NUMERATOR DENOMINATOR [OPTIONS ...]

Options:

-  `-c|--continuous`: flag to indicate numerator and denominator are in continuous time

-  `-m|--minimum=MINIMUM`: specify the minimum output value (default is none)

-  `-M|--maximum=MAXIMUM`: specify the maximum output value (default is none)

-  `-o|--output=GLMFILE`: specify the output GLM file name (default is `/dev/stdout`)

-  `-r|--resolution=RESOLUTION`: specify the output resolution (default is none)

-  `-s|--skewtime=SKEW`: specify the time skew (default is 0)

-  `-t|--timestep=TIMESTEP`: specify the time step (default is 1s)

# Description

The `create_filter` tool creates filter for use as inputs to GridLAB-D object
properties.  The numerator and denominator must be specified as a comma-delimited
list of values in order of ascending order, e.g., `a_0,a_1,...,a_N` for an N-order
system. The order of numerator must be less than or equal to the order of the
denominator (i.e., the filter must be causal).

When RESOLUTION is specified, then both MINIMUM and MAXIMUM must also be specified.

# Example

The following example creates a filter, from a second-order continuous-time
model, with 8 bits of resolution and a dynamic range of (0,1):

    bash% gridlabd create_filter test 1,2,3 4,5,6 -r=8 -m=0 -M=1
    filter test(z,1s,0,resolution=8,minimum=0.0,maximum=1.0) = (+4+5z+6z^2) / (+1+2z+3z^2);

# See also

* [[/Utilities/Fit_filter]]
