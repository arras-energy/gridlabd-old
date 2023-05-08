# Updating latest `master` docker image

To build the docker image for the current branch, run the following command:

~~~
docker/build.sh
~~~

To push the docker image to Dockerhub, use the `--push` option

~~~
docker/build.sh --push
~~~

The tag the docker image as the latest and push it, use the `--latest` option

~~~
docker/build.sh --latest --push
~~~

