The preferred method for running GridLAB-D is using [Docker](www.docker.org).  Once you have installed Docker, you can run GridLAB-D as follows.

~~~
bash% docker run -it slacgismo/gridlabd:latest gridlabd <filename>
~~~

where `<filename` is the name of the model file in the container that you wish to run.

If you wish to access the files in the current folder while running GridLAB-D in a container, then use the command

~~~
bash% docker run -vit $PWD:$PWD slacgismo/gridlabd:latest gridlabd -W $PWD <filename>
~~~

More information on using GridLAB-D docker containers can be found at [[/Install/Docker]].
