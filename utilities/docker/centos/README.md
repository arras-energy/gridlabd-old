All the capabilities needed to install, build, update and manage the required docker images are handled by the makefile.  To obtain a list of the current targets, type
~~~
localhost% make
~~~
or
~~~
localhost% make help
~~~

Some oftens used targets are

* __install__: pulls and installs all the needed images to run the services
* __status__: lists the status of the services
* __start__: starts the services
* __stop__: stops the services
* __restart__: restarts the services
