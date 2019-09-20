# Build docker image
~~~
  host% cd utilities/docker/mysql-server
  host% docker build -f Dockerfile-mysql -t gridlabd/mysql-base:latest .
~~~

# Create the container
~~~
  host% docker run --name gridlabd-1 -it gridlabd/mysql-base | tee gridlabd-1.log
~~~

# Configure the container for GridLAB-D
~~~
  host% export PASSWORD=$(grep GENERATED gridlabd-1.log | cut -f2 -d:)
  host% docker exec -it gridlabd-1 mysql -uroot -p$PASSWORD < /tmp/mysql-setup.sql
~~~

# Open a shell in the container
~~~
  host% docker exec -it gridlabd-1 bash
~~~

# Run a simulation using your local model
~~~
  host% docker exec -itv $(pwd):/tmp
~~~

# Shutdown the container
~~~
  host% echo $PASSWORD 
  <Ctrl-C>
  host% docker exec -it gridlabd-1 mysqladmin -p shutdown
  Enter password: <Ctrl-V><Enter>
~~~

# Delete the container
~~~
  host% docker rm gridlabd-1
~~~
