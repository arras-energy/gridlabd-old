# Docker

Docker Hub support automatic builds of GridLAB-D containers.  See [Docker cloud](https://cloud.docker.com/u/gridlabd/repository/list) for a current list of repositories.

The workflow for the master branch is as follows:
~~~
"{branch}" -> [Merge] -> "master"
"master" -> [Build] -> gridlabd/master:latest
"master" -> [Tag] -> gridlabd/master:[0-9]+
"master" -> [Tag] -> gridlabd/master:release-[0-9.]+
~~~

The workflow for a project master is as follows:
~~~
"master" -> [Merge] -> "master-{project}"
"master-{project}" -> [Build] -> gridlabd/{project}-master:latest
"master-{project}" -> [Tag] -> gridlabd/{project}-master:[0-9]+
"master-{project}" -> [Tag] -> gridlabd/{project}-master:release-[0-9.]+
~~~
