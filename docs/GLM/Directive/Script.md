[[/GLM/Directive/Script]] - Directive to run external commands on simulation events

# Synopsis

~~~
script command;
script on_create command;
script on_init command;
script on_precommit command;
script on_presync command;
script on_sync command;
script on_postsync command;
script on_commit command;
script on_term command;
script export global-name;
~~~

# Description

The `script` directives cause external commands to be executed.  The simulation will wait for the command to exit.  If the exit code is non-zero, the simulation will immediately terminate with the exit code returned by the script.

Loader scripts (those without event specifications) are loaded one at a time in the order in which they are encountered by the loader.

Event scripts may be run in parallel if the `threadcount` is greater than 1.

## `export`

The variable listed is exported to the shell's environment before script are executed.

**Note**: The syntax for accessing a variable is dependent on the shell being used to interpret the script commands.  For example, DOS uses the `%name%` syntax whereas bash uses the `$name` syntax.

## `on_create`

The script is executed after all objects have been created and before the first object is initialized.

## `on_init`

The script is executed after all objects have been initialized and before the first sync event.

## `on_[pre]commit`

The script is executed before/after committing a clock step.

## `on_[pre|post]sync`

The script is executed after the specified clock synchronization pass has been completed.

## `on_term`

The script is executed when the simulation terminates.

# Caveat

* Platform independence
  - In the current implementation, scripts are interpreted by the platform's native shell command processor (e.g., DOS for MS Windows, bash for linux and Mac).  This means GLM files that use scripts are not normally portable from one platform to another. 
  - On most platforms you can specify the shell to use by preceding the command with the name of the shell, e.g. `script python my_script.py`.
  - Provided you include the shell command in the your PATH environment.  If platform independent scripts are desired, care should be take to only use those shell features that are platform independent.  See the shell's documentation for details.

* Asynchronous calls
  - On MS Windows platform the DOS shell interprets the `start` command as a request for asynchronous execution of the script.  On Linux/Mac platforms, a trailing `&` causes asynchronous execution of the script. If a script request is repeated before the previous copy if done (e.g., `on_sync`), this can result in many copies of the script running concurrently and the system becoming bogged down with multiple copies of the same process.

* Variable expansion
  - Variable names are interpreted when the script command is parsed by the GLM rather than when it is executed.  It is currently not possible to update the value of a variable when the script is executed.  As a result, the following directive will not work as expected `script on_sync echo ${clock}` because the value of the `clock` is interpreted when the directive is encountered (when `clock` contains the start time) and not when the script is executed.  You must use the `export` option to export variables to scripts.  The correct syntax for the above example is

~~~
script export clock;
script on_sync echo $clock; // linux/mac variable expansion syntax
~~~

# See also

* [[/Global/Threadcount]]
