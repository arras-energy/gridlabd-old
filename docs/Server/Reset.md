[[/Server/Reset]] -- Reset simulation to initial conditions

# Synopsis
HTTP:
~~~
GET /control/reset
~~~

# Description

The `/control/reset` command is used to reset the simulation to initial conditions.  After the command is completed, the simulation clock will be reset to `starttime` and the simulation will resume.

# See also
* [[/Global/Starttime]]
