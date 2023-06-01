# Docker

To build the docker image for the current branch, run the following command:

~~~
docker/build.sh
~~~

# Update a docker image on Dockerhub

To push the docker image to Dockerhub, use the `--push` option

~~~
docker/build.sh --push
~~~

# Update the latest image on Dockerhub

The tag the docker image as the latest and push it, use the `--latest` option

~~~
docker/build.sh --latest --push
~~~

