# Updating latest `master` docker image

To build and post the `master` docker image, run the following command:

~~~
    sh$ docker build docker -t slacgismo/gridlabd:latest
    sh$ docker push slacgismo/gridlabd:latest
~~~

# Updating `develop` docker image

To build and post the `develop` docker image, run the following command:

~~~
    sh$ docker build docker --build-arg BRANCH=develop -t slacgismo/gridlabd:develop
    sh$ docker push slacgismo/gridlabd:develop
~~~

# Updating a personal branch docker image

To build and post the `develop` docker image, run the following command:

~~~
    sh$ docker build docker --build-arg BRANCH=BRANCHNAME -t USERNAME/gridlabd:BUILDID
    sh$ docker push USERNAME/gridlabd:BUILDID
~~~

where
- `BRANCHNAME` is the branch you wish to build
- `USERNAME` is your Dockerhub user id
- `BUILDID` is a unique/sequential build identifier
