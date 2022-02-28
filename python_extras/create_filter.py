"""Create filter tool

SYNOPSIS

Shell:

    bash$ gridlabd create_filter NAME NUMERATOR DENOMINATOR [OPTIONS ...]

GLM:

    #create_filter NAME NUMERATOR DENOMINATOR [OPTIONS ...]

Options:

    `[-c|--continuous]`: flag to indicate numerator and denominator are in continuous time

    `[-m|--minimum=MINIMUM]`: specify the minimum output value (default is none)

    `[-M|--maximum=MAXIMUM]`: specify the maximum output value (default is none)

    `[-o|--output=GLMFILE]`: specify the output GLM file name (default is `/dev/stdout`)

    `[-r|--resolution=RESOLUTION]`: specify the output resolution (default is none)

    `[-s|--skewtime=SKEW]`: specify the time skew (default is 0)

    `[-t|--timestep=TIMESTEP]`: specify the time step (default is 1s)

DESCRIPTION

The `create_filter` tool creates filter for use as inputs to GridLAB-D object
properties.

EXAMPLE

The following example creates a filter, from a second-order continuous-time
model, with 8 bits of resolution and a dynamic range of (0,1):

    bash% gridlabd create_filter test 1 1,0.2 -c -r=8 -m=0 -M=1
    TODO

SEE ALSO

* [[/Utilities/Fit_filter]]
"""