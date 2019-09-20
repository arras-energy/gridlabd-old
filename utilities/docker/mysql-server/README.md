# Build docker image

  host% cd utilities/docker/mysql-server
  host% docker build -f Dockerfile-mysql -t gridlabd/mysql-base:latest .

# Create container

  host% docker run --name gridlabd-1 -it gridlabd/mysql-base

# Setup mysql database

Search output of docker run command for `GENERATED ROOT PASSWORD`. Run the command

  host% docker exec -it gridlabd-1 mysql -uroot -pGENERATED_ROOT_PASSWORD < /usr/local/src/gridlabd/utilities/docker/mysql-server/mysql-setup.sql

