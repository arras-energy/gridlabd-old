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

When enabled, the docker image is run instead of the local system installation.  To run a docker image directly from the command line without enabling it, use the command:

~~~
docker run -itv $PWD:$PWD <image-name> gridlabd <options>
~~~

If you need to enable debugging while running an image, use the command:

~~~
docker run -itv --cap-add=SYS_PTRACE --security-opt seccomp=unconfined $PWD:$PWD <image-name> gridlabd <options>
~~~

If you need to open a second terminal window on a running docker image, do the following:

~~~
docker ps
~~~

Copy the name at the end of the status line describing your gridlabd container. Then run the following command:

~~~
docker exec -it <container-name> bash
~~~

## `enable` <image>

The `enable` subcommand tags the `<image>` as the active image.

## `disable`

The `disable` subcommand removes the image tagged `gridlabd`.

## `status`

The status command display all the available GridLAB-D image, including the active image.

# See also

* [[/Install/Docker]]

