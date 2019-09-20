This page describes how to use the GridLAB-D docker image with a MySQL server built in. This is not the preferred way of using docker containers (as evidenced by the size of the image), but sometimes it is the only option available.

# Pull the docker image from DockerHub
Normally, you would simply pull the docker image from the hub:
~~~
host% docker pull gridlabd/mysql-server
~~~

# Build docker image locally
Sometimes, it's useful to build the image yourself, especially if you want to change options in the Dockerfile
~~~
host% cd utilities/docker/mysql-server
host% docker build -t gridlabd/mysql-base:latest .
~~~

# Create the container
Once the image is available on your system, you can create the container (which starts the mysql server):
~~~
host% docker run --name gridlabd-1 -it gridlabd/mysql-base | tee gridlabd-1.log
~~~

# Get the root password
Before going further, you're going to need the root password to your server:
~~~
host% export PASSWORD=$(grep GENERATED gridlabd-1.log | cut -f2 -d:)
host% echo $PASSWORD
<copy>
~~~
If you encounter `<paste>` below, this is the `<copy>` it refers to. 

# Configure the container for GridLAB-D
GridLAB-D needs admin, runtime, and user access to its data. This `mysql-setup.sql` script gets the MySQL server ready for GridLAB-D's use:
~~~
host% docker exec -it gridlabd-1 mysql -uroot -p < /tmp/mysql-setup.sql
Enter password: <paste><Enter>
~~~

# Validate the mysql server
Once the server is ready for GridLAB-D, you should validate that system using the `mysql` module validate tests:
~~~
host% docker exec -it gridlabd-1 rm -f /usr/local/src/gridlabd/mysql/validate.no
host% docker exec -it gridlabd-1 gridlabd -W /usr/local/src/gridlabd/mysql --validate
~~~

# Open a shell in the container
If you need to access the server host, you can open a shell:
~~~
host% docker exec -it gridlabd-1 bash
docker% exit
~~~

# Run a simulation using your local model
You can run simulation `model.glm` in the current folder:
~~~
host% docker exec -itv $(pwd):/gridlabd gridlabd-1 gridlabd -W /gridlabd model.glm 
~~~

# Dump the database
You should save database before you lose your data:
~~~
docker exec -it gridlabd-1 mysqldump -uroot -p<paste> gridlabd | tail +2 > dump.sql
~~~

# Shutdown the container
When you are done using the container, you can shut it down:
~~~
host% docker exec -it gridlabd-1 mysqladmin -p shutdown
Enter password: <paste><Enter>
~~~
Be advised: this destroys the database permanently. Don't forget to dump the database if you need it in the future.

# Kill the container
If shutdown doesn't work, you can kill it.
~~~
host% docker kill gridlabd-1
~~~
Be advised: this destroys the database permanently. Don't forget to dump the database if you need it in the future.

# Delete the container
If you don't intend to use the container in the future, or you want to pull/build it over, you should delete it:
~~~
host% docker rm gridlabd-1
~~~
