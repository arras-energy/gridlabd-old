[[/Install/Docker]] -- Running GridLAB-D on Docker

# Synopsis

~~~
bash$ docker pull slacgismo/gridlabd:latest
bash$ docker run -it -v $(pwd):$(pwd) slacgismo/gridlabd:latest gridlabd -W $(pwd) <...>
~~~

# Description

To use the latest version of GridLAB-D with docker, install docker from see www.docker.com. Then run 

~~~
bash$ docker pull slacgismo/gridlabd:latest
~~~

For convenience you may tag the image you wish to use by default:

~~~
bash$ docker tag slacgismo/gridlabd:latest gridlabd
~~~

To get more information about available docker images, see https://hub.docker.com/u/gridlabd/.

To run your GridLAB-D docker image version of GridLAB-D:

~~~~
bash$ docker run -it -v $(pwd):$(pwd) gridlabd gridlabd -W $(pwd) my_model.glm
~~~~

If you are using a system where command aliases are allowed (e.g., linux):

~~~
bash$ alias gridlabd='docker run -it -v $(pwd):$(pwd) gridlabd gridlabd -W $(pwd)'
~~~

You can add this command to your shell profile, e.g., `$HOME/.bash_profile`.

After that you can use the simpler command

~~~
bash$ gridlabd my_model.glm
~~~

## Using multiple local installations

If you have installed GridLAB-D locally, you can enable a docker image to run in its place:

~~~
bash$ gridlabd --docker enable <imagename>
~~~

Subsequent `gridlabd` commands will use the docker image `<imagename>` instead of the local installation.

To disable a docker image:

~~~
bash$ gridlabd --docker disable
~~~

Subsequent gridlabd commands will use the local installation instead of the docker image.

To get a list of available and active docker images:

~~~
bash$ gridlabd --docker status
REPOSITORY             TAG                 IMAGE ID            CREATED             SIZE
slacgismo/gridlabd     latest              398e452f9a01        2 days ago          1.56GB
gridlabd               latest              398e452f9a01        2 days ago          1.56GB
~~~

If the repository `gridlabd` is listed, then gridlabd will use the docker image instead of the current installed version.

Note: the `gridlabd --docker` command uses a the `docker tag` command to indicate which image is active.  If an tag image named `gridlabd` is present, that image will be used instead of the local installation. 

## Image clean-up 

To clean-up resources - images, containers, volumes, and networks - not associated with a container: 

~~~
bash$ docker system prune 
~~~

To remove any stopped containers and all unused images add a flag `-a` : 

~~~
bash$ docker system prune -a
~~~

## Building an image locally from local source

You can create an image locally with a specific branch (e.g., `develop`) instead of pulling from DockerHub (where the default branch is `master`). 

Firsrt, clone the `https://github.com/slacgismo/gridlabd` and checkout the desired branch (e.g., `develop`). 

~~~
bash$ git clone https://github.com/slacgismo/gridlabd -b develop /usr/local/opt/gridlabd/src/gridlabd
~~~

Then build the image locally: 

~~~
bash$ cd /usr/local/opt/gridlabd/src/gridlabd/docker
bash$ docker build --build-arg "BRANCH=develop" .
~~~

# Caveat 

Docker has a known issue on some versions of Linux running on Macs if your system goes to sleep while the daemon is running. After your system wakes up, containers may run a lot slower.  You can fix this problem by adding the following line to the file `/Applications/Docker.app/Contents/Resources/linuxkit/cmdline`:

~~~
tsc=reliable
~~~

See [SourceForge Forum](https://sourceforge.net/p/gridlab-d/discussion/842561/thread/ef4a34cb98) for details.

# See also

* [[/Subcommand/Docker]]

