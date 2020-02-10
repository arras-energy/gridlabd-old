[[/Install/Docker]] -- Running GridLAB-D on Docker

# Synopsis

~~~
bash$ docker run -it gridlabd/master <command>
~~~

# Description

To use the latest version of GridLAB-D with docker, install docker from see www.docker.com. Then run 

~~~~
bash$ docker pull gridlabd/slac-master:latest
bash$ docker tag gridlabd/slac-master:latest gridlabd
~~~~

To get more information about available docker images, see https://hub.docker.com/u/gridlabd/.

To run your GridLAB-D docker image version of GridLAB-D:

~~~~
bash$ docker run -vit $(pwd):/model gridlabd gridlabd -W /model my_model.glm
~~~~

If you are using a system where command aliases are allowed (e.g., linux):

~~~
bash$ alias gridlabd='docker run -vit $(pwd):/model gld gridlabd -W /model'
~~~

You can add this command to your shell profile, e.g., `$HOME/.bash_profile`.
After that you can use the simpler command

~~~
bash$ gridlabd my_model.glm
~~~

## Docker control

To enable a docker image:

~~~
bash$ gridlabd --docker enable <imagename>
~~~

Subsequent gridlabd commands will use the docker image `gridlabd` instead of the local installation.

To disable a docker image:

~~~
bash$ gridlabd --docker disable
~~~

Subsequent gridlabd commands will use the local installation instead of the docker image `gridlabd`.

To get a list of available and active docker images:

~~~
bash$ gridlabd --docker status
REPOSITORY             TAG                 IMAGE ID            CREATED             SIZE
gridlabd/slac-master   latest              398e452f9a01        2 days ago          1.56GB
gridlabd               latest              398e452f9a01        2 days ago          1.56GB
~~~

If the repository `gridlabd` is listed, then gridlabd will use the docker instance instead of the current installed version.

## Image clean-up 

To clean-up resources - images, containers, volumes, and networks - not associated with a container: 

~~~
bash$ docker system prune 
~~~

To remove any stopped containers and all unused images add a flag `-a` : 

~~~
bash$ docker system prune -a
~~~

## Creating an image locally from a custom branch

If you'd like to create an image locally with a specific branch (ex `test_branch`) instead of pulling from DockerHub (where the default branch is `master`). 

1. Clone the `slacgismo/gridlabd` repository locally and switch to the desired branch (`test_branch`). 

2. Navigate to `utilities/docker/centos` 

3. Edit `Makefile.conf` to define `GRIDLABD_IMG=gridlabd/test_branch`, change the tag to `GRIDLABD_TAG=test` and change any other options as desired. 

4. Build the image manually by running: 

~~~
bash$ make build-gridlabd
~~~

Note, you may first need to run 

~~~
bash$ make clean 
~~~

5. Run the image as described above 

# Caveat 

Docker has a known issue if your system goes to sleep while the daemon is running. After your system wakes up, containers may run a lot slower.  The only known solution at this time is to restart the docker daemon.

# See also

* [[/Subcommand/Docker]]

