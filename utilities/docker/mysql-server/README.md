# Pull the docker image from DockerHub
~~~
host% docker 

# Build docker image locally
~~~
host% cd utilities/docker/mysql-server
host% docker build -f Dockerfile-mysql -t gridlabd/mysql-base:latest .
~~~

# Create the container
~~~
host% docker run --name gridlabd-1 -it gridlabd/mysql-base | tee gridlabd-1.log
~~~

# Get the root password
~~~
host% export PASSWORD=$(grep GENERATED gridlabd-1.log | cut -f2 -d:)
host% echo $PASSWORD
~~~

# Configure the container for GridLAB-D
~~~
<copy>
host% docker exec -it gridlabd-1 mysql -uroot -p < /tmp/mysql-setup.sql
Enter password: <paste><Enter>
~~~

# Validate the mysql server
~~~
host% docker exec -it gridlabd-1 rm -f /usr/local/src/gridlabd/mysql/validate.no
host% docker exec -it gridlabd-1 gridlabd -W /usr/local/src/gridlabd/mysql --validate
~~~

# Open a shell in the container
~~~
host% docker exec -it gridlabd-1 bash
docker% exit
~~~

# Run a simulation using your local model
~~~
host% docker exec -itv $(pwd):/tmp
~~~

# Shutdown the container
~~~
host% echo $PASSWORD 
<copy>
host% docker exec -it gridlabd-1 mysqladmin -p shutdown
Enter password: <paste><Enter>
~~~

# Kill the container
~~~
host% docker kill gridlabd-1
~~~

# Delete the container
~~~
host% docker rm gridlabd-1
~~~
