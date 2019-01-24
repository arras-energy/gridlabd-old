This folder contains the scripts necessary to manage a docker instance of MySQL for GridLAB-D on the local host.

# mysql-start [options]

The `mysql-start` script creates a local instance of mysql that is ready for use by GridLAB-D.  The following options are supported:

  * __-c|--clean__: starts the docker mysql service from a clean data. __WARNING: this deletes all existing data!__
  * __-h|--help__: produces help output
  * __-k|--keep__: keeps the instance when the start fail (allows inspection to help debug problems)
  * __-v|--verbose__: produces additional informational output

# mysql-status [options]

The `mysql-status` script inspects the status of the local instance of mysql for GridLAB-D.  The following options are supported:

  * __-h|--help__: outputs this help text
  * __-l|--logs__: displays the logs 

# mysql-stop [options]

The `mysql-stop` script stops and optionally deletes the local instance of mysql for GridLAB-D.  The following options are supported:

  *__-a|--all__: removes the instance after stopping it
  *__-c|--clean__: starts mysql from a clean database. __WARNING: this deletes all existing data!__
  *__-h|--help__: outputs help text
  *__-v|--verbose__: produces more informational output 
 
# mysql.cnf

The configuration files is named `mysql.cnf` and contains the following parameter settings

  * __REPO__: identifies the docker repo to use for the mysql image (default is `mysql/mysql-server`)
  * __TAG__: identifies the docker image tag to use for the mysql image (default is `latest`)
  * __NAME__: identifies the instance name to use for the mysql service (default is `mysql1`)
  * __DATADIR__: identifies the folder in which the mysql data files are stored locally (default is `./data`)

