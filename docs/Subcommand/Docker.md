[[/Subcommand/Docker]] -- Running GridLAB-D on Docker

# Synopsis

~~~
gridlabd docker help
gridlabd docker enable <image-name>
gridlabd docker disable
gridlabd docker status
~~~

# Description

The `docker` subcommand is used to manage the docker image used to run simulations. The active image is always tagged `gridlabd`.

## `enable` <image>

The `enable` subcommand tags the `<image>` as the active image.

## `disable`

The `disable` subcommand removes the image tagged `gridlabd`.

## `status`

The status command display all the available GridLAB-D image, including the active image.

# See also

* [[/Install/Docker]]

