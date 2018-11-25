This folder contains the scripts necesary to manage a docker instance of MySQL for GridLAB-D on the local host.

# mysql.cnf

The configuration files is named `mysql.cnf` and contains the following parameter settings

* __REPO__: identifies the docker repo to use for the mysql image (default is `mysql/mysql-server`)
* __TAG__: identifies the docker image tag to use for the mysql image (default is `latest`)
* __NAME__: identifies the instance name to use for the mysql service (default is `mysql1`)
* __DATADIR__: identifies the folder in which the mysql data files are stored locally (default is `./data`)

# mysql-start

The `mysql-start` scripts creates a local instance of mysql that is ready for use by GridLAB-D.  The following options are supported

# mysql-status

# mysql-stop
